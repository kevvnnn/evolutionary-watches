#include "WatchFileIO.h"
#include "../genome/Watch.h"
#include "../algorithm/GeneticAlgorithm.h"
#include "../genome/WatchComponent.h"
#include "../genome/components/Gear.h"
#include "../genome/components/BalanceWheel.h"
#include "../genome/components/Spring.h"
#include "../genome/components/Hand.h"
#include "../genome/components/Jewel.h"

#include <iostream>
#include <fstream>

namespace WatchGA {
namespace FileIO {

// ---------------------------------------------------------------------------
// Constructors & File Path Handlers
// ---------------------------------------------------------------------------
WatchFileIO::WatchFileIO() : m_filePath("population_data.bin") {}
WatchFileIO::WatchFileIO(const std::string& path) : m_filePath(path) {}
void WatchFileIO::setFilePath(const std::string& path) { m_filePath = path; }
const std::string& WatchFileIO::getFilePath() const { return m_filePath; }

// ---------------------------------------------------------------------------
// String Helpers for Binary Data
// ---------------------------------------------------------------------------
static void writeString(std::ostream& out, const std::string& str) {
    size_t len = str.size();
    out.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
    out.write(str.c_str(), len);
}

static std::string readString(std::istream& in) {
    size_t len = 0;
    if (!in.read(reinterpret_cast<char*>(&len), sizeof(size_t))) return "";
    std::string str(len, '\0');
    in.read(&str[0], len);
    return str;
}

// ---------------------------------------------------------------------------
// Deep Binary Serialization
// ---------------------------------------------------------------------------
void WatchFileIO::writeWatch(std::ostream& out, const Genome::Watch& watch) const {
    double fitness = watch.getFitnessScore();
    out.write(reinterpret_cast<const char*>(&fitness), sizeof(double));

    bool valid = watch.isValid();
    out.write(reinterpret_cast<const char*>(&valid), sizeof(bool));

    unsigned int compCount = watch.getComponentCount();
    out.write(reinterpret_cast<const char*>(&compCount), sizeof(unsigned int));

    writeString(out, watch.getObjectName());

    // --- DEEP POLYMORPHIC SERIALIZATION ---
    using namespace Genome::Components;
    for (const auto& compPtr : watch.getAllComponents()) {
        const Genome::WatchComponent* comp = compPtr.get();

        writeString(out, comp->getName());
        double w = comp->getWeight(); out.write(reinterpret_cast<const char*>(&w), sizeof(double));
        double f = comp->getFriction(); out.write(reinterpret_cast<const char*>(&f), sizeof(double));

        // Downcast to find exactly what component this is and save its unique DNA
        if (auto* gear = dynamic_cast<const Gear*>(comp)) {
            int type = 1; out.write(reinterpret_cast<const char*>(&type), sizeof(int));
            unsigned int tc = gear->getToothCount(); out.write(reinterpret_cast<const char*>(&tc), sizeof(unsigned int));
            double dia = gear->getDiameter(); out.write(reinterpret_cast<const char*>(&dia), sizeof(double));
            double mq = gear->getMeshingQuality(); out.write(reinterpret_cast<const char*>(&mq), sizeof(double));
            
            bool hj = gear->hasJewel(); out.write(reinterpret_cast<const char*>(&hj), sizeof(bool));
            if (hj) {
                double jh = gear->getJewel()->getHardness(); out.write(reinterpret_cast<const char*>(&jh), sizeof(double));
                bool jc = gear->getJewel()->isCapJewel(); out.write(reinterpret_cast<const char*>(&jc), sizeof(bool));
            }
        }
        else if (auto* bw = dynamic_cast<const BalanceWheel*>(comp)) {
            int type = 2; out.write(reinterpret_cast<const char*>(&type), sizeof(int));
            double moi = bw->getMomentOfInertia(); out.write(reinterpret_cast<const char*>(&moi), sizeof(double));
            double iso = bw->getIsochronism(); out.write(reinterpret_cast<const char*>(&iso), sizeof(double));
            double amp = bw->getAmplitude(); out.write(reinterpret_cast<const char*>(&amp), sizeof(double));
        }
        else if (auto* spring = dynamic_cast<const Spring*>(comp)) {
            int type = 3; out.write(reinterpret_cast<const char*>(&type), sizeof(int));
            int stype = static_cast<int>(spring->getType()); out.write(reinterpret_cast<const char*>(&stype), sizeof(int));
            double el = spring->getElasticity(); out.write(reinterpret_cast<const char*>(&el), sizeof(double));
            double fr = spring->getFatigueResistance(); out.write(reinterpret_cast<const char*>(&fr), sizeof(double));
            double len = spring->getLength(); out.write(reinterpret_cast<const char*>(&len), sizeof(double));
        }
        else if (auto* hand = dynamic_cast<const Hand*>(comp)) {
            int type = 4; out.write(reinterpret_cast<const char*>(&type), sizeof(int));
            int htype = static_cast<int>(hand->getType()); out.write(reinterpret_cast<const char*>(&htype), sizeof(int));
            double len = hand->getLength(); out.write(reinterpret_cast<const char*>(&len), sizeof(double));
            double bal = hand->getBalance(); out.write(reinterpret_cast<const char*>(&bal), sizeof(double));
        }
        else if (auto* jewel = dynamic_cast<const Jewel*>(comp)) {
            int type = 5; out.write(reinterpret_cast<const char*>(&type), sizeof(int));
            double hard = jewel->getHardness(); out.write(reinterpret_cast<const char*>(&hard), sizeof(double));
            bool cap = jewel->isCapJewel(); out.write(reinterpret_cast<const char*>(&cap), sizeof(bool));
        }
        else {
            int type = 0; out.write(reinterpret_cast<const char*>(&type), sizeof(int)); // Unknown generic
        }
    }
}

std::unique_ptr<Genome::Watch> WatchFileIO::readWatch(std::istream& in) const {
    double fitness; bool valid; unsigned int compCount;
    
    if (!in.read(reinterpret_cast<char*>(&fitness), sizeof(double))) return nullptr;
    in.read(reinterpret_cast<char*>(&valid), sizeof(bool));
    in.read(reinterpret_cast<char*>(&compCount), sizeof(unsigned int));
    
    std::string name = readString(in);
    auto watch = std::make_unique<Genome::Watch>(name);
    watch->setFitnessScore(fitness);
    watch->setValid(valid);

    using namespace Genome::Components;
    for (unsigned int i = 0; i < compCount; ++i) {
        std::string cname = readString(in);
        double w, f;
        in.read(reinterpret_cast<char*>(&w), sizeof(double));
        in.read(reinterpret_cast<char*>(&f), sizeof(double));

        int type; in.read(reinterpret_cast<char*>(&type), sizeof(int));
        
        if (type == 1) { // Gear
            unsigned int tc; double dia, mq; bool hj;
            in.read(reinterpret_cast<char*>(&tc), sizeof(unsigned int));
            in.read(reinterpret_cast<char*>(&dia), sizeof(double));
            in.read(reinterpret_cast<char*>(&mq), sizeof(double));
            in.read(reinterpret_cast<char*>(&hj), sizeof(bool));
            
            auto gear = std::make_unique<Gear>(cname, w, f, tc, dia, mq);
            if (hj) {
                double jh; bool jc;
                in.read(reinterpret_cast<char*>(&jh), sizeof(double));
                in.read(reinterpret_cast<char*>(&jc), sizeof(bool));
                gear->setJewel(std::make_unique<Jewel>("Jewel_For_"+cname, 0.01, 0.005, jh, jc));
            }
            watch->addComponent(std::move(gear));
        } 
        else if (type == 2) { // BalanceWheel
            double moi, iso, amp;
            in.read(reinterpret_cast<char*>(&moi), sizeof(double));
            in.read(reinterpret_cast<char*>(&iso), sizeof(double));
            in.read(reinterpret_cast<char*>(&amp), sizeof(double));
            watch->addComponent(std::make_unique<BalanceWheel>(cname, w, f, moi, iso, amp));
        } 
        else if (type == 3) { // Spring
            int stype; double el, fr, len;
            in.read(reinterpret_cast<char*>(&stype), sizeof(int));
            in.read(reinterpret_cast<char*>(&el), sizeof(double));
            in.read(reinterpret_cast<char*>(&fr), sizeof(double));
            in.read(reinterpret_cast<char*>(&len), sizeof(double));
            watch->addComponent(std::make_unique<Spring>(cname, w, f, static_cast<Spring::SpringType>(stype), el, fr, len));
        } 
        else if (type == 4) { // Hand
            int htype; double len, bal;
            in.read(reinterpret_cast<char*>(&htype), sizeof(int));
            in.read(reinterpret_cast<char*>(&len), sizeof(double));
            in.read(reinterpret_cast<char*>(&bal), sizeof(double));
            watch->addComponent(std::make_unique<Hand>(cname, w, f, static_cast<Hand::HandType>(htype), len, bal));
        } 
        else if (type == 5) { // Jewel
            double hard; bool cap;
            in.read(reinterpret_cast<char*>(&hard), sizeof(double));
            in.read(reinterpret_cast<char*>(&cap), sizeof(bool));
            watch->addComponent(std::make_unique<Jewel>(cname, w, f, hard, cap));
        }
    }
    return watch;
}

// ---------------------------------------------------------------------------
// Watch / Population save/load overrides
// ---------------------------------------------------------------------------

bool WatchFileIO::saveWatch(const Genome::Watch& watch, unsigned int index) {
    (void)index;
    if (m_filePath.empty()) return false;
    std::ofstream outFile(m_filePath, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) return false;
    writeWatch(outFile, watch);
    return outFile.good();
}

std::unique_ptr<Genome::Watch> WatchFileIO::loadWatch(unsigned int index) const {
    if (m_filePath.empty()) return nullptr;
    std::ifstream inFile(m_filePath, std::ios::binary);
    if (!inFile.is_open()) return nullptr;
    for (unsigned int i = 0; i < index; ++i) {
        if (!readWatch(inFile)) return nullptr; 
    }
    return readWatch(inFile);
}

bool WatchFileIO::savePopulation(const Algorithm::GeneticAlgorithm& ga) {
    (void)ga;
    if (m_filePath.empty()) return false;
    std::ofstream outFile(m_filePath, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) return false;
    unsigned int popSize = ga.getPopulation().size();
    outFile.write(reinterpret_cast<const char*>(&popSize), sizeof(unsigned int));
    for (const auto& watch : ga.getPopulation()) writeWatch(outFile, *watch);
    return outFile.good();
}

bool WatchFileIO::loadPopulation(Algorithm::GeneticAlgorithm& ga) const {
    (void)ga;
    if (m_filePath.empty()) return false;
    return true; 
}

bool WatchFileIO::saveCompleteState(const Algorithm::GeneticAlgorithm& ga) {
    if (m_filePath.empty()) return false;
    std::ofstream outFile(m_filePath, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) return false;
    unsigned int gen = ga.getCurrentGeneration();
    double mutRate = ga.getMutationRate();
    outFile.write(reinterpret_cast<const char*>(&gen), sizeof(unsigned int));
    outFile.write(reinterpret_cast<const char*>(&mutRate), sizeof(double));
    unsigned int popSize = ga.getPopulation().size();
    outFile.write(reinterpret_cast<const char*>(&popSize), sizeof(unsigned int));
    for (const auto& watch : ga.getPopulation()) writeWatch(outFile, *watch);
    return outFile.good();
}

bool WatchFileIO::loadCompleteState(Algorithm::GeneticAlgorithm& ga) const {
    (void)ga;
    if (m_filePath.empty()) return false;
    return true; 
}

unsigned int WatchFileIO::getWatchCount() const {
    if (m_filePath.empty()) return 0;
    std::ifstream inFile(m_filePath, std::ios::binary | std::ios::ate); 
    if (!inFile.is_open()) return 0;
    return static_cast<unsigned int>(inFile.tellg()); 
}

} // namespace FileIO
} // namespace WatchGA