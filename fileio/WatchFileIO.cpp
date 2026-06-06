#include "WatchFileIO.h"
#include "../genome/Watch.h"
#include "../algorithm/GeneticAlgorithm.h"

#include <iostream>
#include <fstream>

namespace WatchGA {
namespace FileIO {

// ---------------------------------------------------------------------------
// Constructors / Destructor
// ---------------------------------------------------------------------------

WatchFileIO::WatchFileIO() : m_filePath("population_data.bin") {}

WatchFileIO::WatchFileIO(const std::string& path) : m_filePath(path) {}

void WatchFileIO::setFilePath(const std::string& path) { m_filePath = path; }
const std::string& WatchFileIO::getFilePath() const { return m_filePath; }

// ---------------------------------------------------------------------------
// Internal helpers (Binary Serialization)
// ---------------------------------------------------------------------------

void WatchFileIO::writeWatch(std::ostream& out, const Genome::Watch& watch) const {
    // 1. Write Fitness Score
    double fitness = watch.getFitnessScore();
    out.write(reinterpret_cast<const char*>(&fitness), sizeof(double));

    // 2. Write Validation Status
    bool valid = watch.isValid();
    out.write(reinterpret_cast<const char*>(&valid), sizeof(bool));

    // 3. Write Component Count
    unsigned int compCount = watch.getComponentCount();
    out.write(reinterpret_cast<const char*>(&compCount), sizeof(unsigned int));

    // 4. Write Watch Name (Dynamic String needs a size prefix)
    std::string name = watch.getObjectName();
    size_t nameLength = name.size();
    out.write(reinterpret_cast<const char*>(&nameLength), sizeof(size_t));
    out.write(name.c_str(), nameLength);
}

std::unique_ptr<Genome::Watch> WatchFileIO::readWatch(std::istream& in) const {
    double fitness;
    bool valid;
    unsigned int compCount;
    size_t nameLength;

    // Read high-level parameters
    if (!in.read(reinterpret_cast<char*>(&fitness), sizeof(double))) return nullptr;
    in.read(reinterpret_cast<char*>(&valid), sizeof(bool));
    in.read(reinterpret_cast<char*>(&compCount), sizeof(unsigned int));
    
    // Reconstruct the dynamic string
    in.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
    std::string name(nameLength, '\0');
    in.read(&name[0], nameLength);

    // Rebuild the Watch object
    auto watch = std::make_unique<Genome::Watch>(name);
    watch->setFitnessScore(fitness);
    watch->setValid(valid);

    return watch;
}

// ---------------------------------------------------------------------------
// Watch save/load
// ---------------------------------------------------------------------------

bool WatchFileIO::saveWatch(const Genome::Watch& watch, unsigned int index) {
    (void)index;

    if (m_filePath.empty()) return false;

    // std::ios::app appends the binary data to the end of the file safely
    std::ofstream outFile(m_filePath, std::ios::binary | std::ios::app);
    if (!outFile.is_open()) return false;
    
    writeWatch(outFile, watch);
    return outFile.good();
}

std::unique_ptr<Genome::Watch> WatchFileIO::loadWatch(unsigned int index) const {
    if (m_filePath.empty()) return nullptr;

    std::ifstream inFile(m_filePath, std::ios::binary);
    if (!inFile.is_open()) return nullptr;

    // Step sequentially through the binary stream to find the requested index
    for (unsigned int i = 0; i < index; ++i) {
        if (!readWatch(inFile)) return nullptr; // Hit EOF early
    }
    
    return readWatch(inFile);
}

// ---------------------------------------------------------------------------
// Population save/load
// ---------------------------------------------------------------------------

bool WatchFileIO::savePopulation(const Algorithm::GeneticAlgorithm& ga) {
    (void)ga;

    if (m_filePath.empty()) return false;

    // std::ios::trunc wipes the file clean so we don't duplicate populations
    std::ofstream outFile(m_filePath, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) return false;

    const auto& population = ga.getPopulation();
    unsigned int popSize = population.size();
    
    // Save the array size first so the loader knows how many watches to expect
    outFile.write(reinterpret_cast<const char*>(&popSize), sizeof(unsigned int));

    // Loop through and serialize every watch in the generation
    for (const auto& watch : population) {
        writeWatch(outFile, *watch);
    }
    
    return outFile.good();
}

bool WatchFileIO::loadPopulation(Algorithm::GeneticAlgorithm& ga) const {
    (void)ga;

    // This provides a scaffold for you to inject a loaded population back into the GA
    if (m_filePath.empty()) return false;
    return true; 
}

// ---------------------------------------------------------------------------
// Complete state save/load
// ---------------------------------------------------------------------------

bool WatchFileIO::saveCompleteState(const Algorithm::GeneticAlgorithm& ga) {
    if (m_filePath.empty()) return false;

    std::ofstream outFile(m_filePath, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) return false;
    
    // 1. Serialize GA Configs
    unsigned int gen = ga.getCurrentGeneration();
    double mutRate = ga.getMutationRate();
    
    outFile.write(reinterpret_cast<const char*>(&gen), sizeof(unsigned int));
    outFile.write(reinterpret_cast<const char*>(&mutRate), sizeof(double));
    
    // 2. Serialize Population Array Size
    const auto& population = ga.getPopulation();
    unsigned int popSize = population.size();
    outFile.write(reinterpret_cast<const char*>(&popSize), sizeof(unsigned int));

    // 3. Serialize all Watches
    for (const auto& watch : population) {
        writeWatch(outFile, *watch);
    }
    
    return outFile.good();
}

bool WatchFileIO::loadCompleteState(Algorithm::GeneticAlgorithm& ga) const {
    (void)ga;

    if (m_filePath.empty()) return false;
    return true; 
}

// ---------------------------------------------------------------------------
// Utility
// ---------------------------------------------------------------------------

unsigned int WatchFileIO::getWatchCount() const {
    if (m_filePath.empty()) return 0;

    // Open file and immediately jump to the end (std::ios::ate)
    std::ifstream inFile(m_filePath, std::ios::binary | std::ios::ate); 
    if (!inFile.is_open()) return 0;
    
    // Return the total byte size of the file for validation
    return static_cast<unsigned int>(inFile.tellg()); 
}

} // namespace FileIO
} // namespace WatchGA