#include "EvolutionHistory.h"
#include "../genome/Watch.h"
#include <fstream>
#include <stdexcept>

namespace WatchGA {
namespace FileIO {

// ------------------------------------------------------------
// ✅ MISSING CONSTRUCTOR FIX (THIS WAS YOUR ERROR)
// ------------------------------------------------------------
EvolutionHistory::EvolutionHistory()
    : m_experimentName("")
{
}

// Existing constructor
EvolutionHistory::EvolutionHistory(const std::string& name)
    : m_experimentName(name)
{
}

// ------------------------------------------------------------
// Core functions
// ------------------------------------------------------------

void EvolutionHistory::addRecord(const GenerationRecord& record)
{
    m_records.push_back(record);
}

const std::vector<EvolutionHistory::GenerationRecord>&
EvolutionHistory::getAllRecords() const
{
    return m_records;
}

const EvolutionHistory::GenerationRecord&
EvolutionHistory::getRecord(unsigned int gen) const
{
    if (gen >= m_records.size()) {
        throw std::out_of_range("Generation index out of range");
    }
    return m_records[gen];
}

unsigned int EvolutionHistory::getRecordCount() const
{
    return static_cast<unsigned int>(m_records.size());
}

const std::string& EvolutionHistory::getExperimentName() const
{
    return m_experimentName;
}

void EvolutionHistory::setExperimentName(const std::string& name)
{
    m_experimentName = name;
}

void EvolutionHistory::clear()
{
    m_records.clear();
    m_experimentName.clear();
}

// ------------------------------------------------------------
// Save
// ------------------------------------------------------------

bool EvolutionHistory::saveToFile(const std::string& path) const
{
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    size_t nameLength = m_experimentName.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(m_experimentName.data(), nameLength);

    unsigned int recordCount = getRecordCount();
    file.write(reinterpret_cast<const char*>(&recordCount), sizeof(recordCount));

    for (const auto& record : m_records) {

        file.write(reinterpret_cast<const char*>(&record.generationNumber),
                   sizeof(record.generationNumber));

        file.write(reinterpret_cast<const char*>(&record.bestFitness),
                   sizeof(record.bestFitness));

        file.write(reinterpret_cast<const char*>(&record.averageFitness),
                   sizeof(record.averageFitness));

        file.write(reinterpret_cast<const char*>(&record.worstFitness),
                   sizeof(record.worstFitness));

        if (record.bestWatch) {

            int watchId = record.bestWatch->getId();
            double watchFitness = record.bestWatch->getFitness();
            std::string watchName = record.bestWatch->getName();

            file.write(reinterpret_cast<const char*>(&watchId), sizeof(watchId));
            file.write(reinterpret_cast<const char*>(&watchFitness), sizeof(watchFitness));

            size_t watchNameLen = watchName.size();
            file.write(reinterpret_cast<const char*>(&watchNameLen), sizeof(watchNameLen));
            file.write(watchName.data(), watchNameLen);
        }
    }

    return true;
}

// ------------------------------------------------------------
// Load
// ------------------------------------------------------------

bool EvolutionHistory::loadFromFile(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    clear();

    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

    m_experimentName.resize(nameLength);
    file.read(&m_experimentName[0], nameLength);

    unsigned int recordCount;
    file.read(reinterpret_cast<char*>(&recordCount), sizeof(recordCount));

    for (unsigned int i = 0; i < recordCount; ++i) {

        GenerationRecord record;

        file.read(reinterpret_cast<char*>(&record.generationNumber),
                  sizeof(record.generationNumber));

        file.read(reinterpret_cast<char*>(&record.bestFitness),
                  sizeof(record.bestFitness));

        file.read(reinterpret_cast<char*>(&record.averageFitness),
                  sizeof(record.averageFitness));

        file.read(reinterpret_cast<char*>(&record.worstFitness),
                  sizeof(record.worstFitness));

        int watchId;
        double watchFitness;
        size_t watchNameLen;

        file.read(reinterpret_cast<char*>(&watchId), sizeof(watchId));
        file.read(reinterpret_cast<char*>(&watchFitness), sizeof(watchFitness));
        file.read(reinterpret_cast<char*>(&watchNameLen), sizeof(watchNameLen));

        std::string watchName;
        watchName.resize(watchNameLen);
        file.read(&watchName[0], watchNameLen);

        auto watch = std::make_shared<Genome::Watch>();
        watch->setId(watchId);
        watch->setFitness(watchFitness);
        watch->setName(watchName);

        record.bestWatch = watch;

        m_records.push_back(record);
    }

    return true;
}

} // namespace FileIO
} // namespace WatchGA