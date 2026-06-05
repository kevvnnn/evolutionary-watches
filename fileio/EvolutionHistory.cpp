/**
 * @file EvolutionHistory.cpp
 * @brief Implementation for EvolutionHistory class
 * @ownership Person C
 */

#include "EvolutionHistory.h"
#include <fstream>
#include <stdexcept>

namespace WatchGA {
namespace FileIO {

/**
 * @brief Default constructor
 * Initializes an empty evolution history with no experiment name
 */
EvolutionHistory::EvolutionHistory()
    : m_experimentName("")
{
}

/**
 * @brief Constructor with experiment name
 * @param name Name of the evolutionary experiment
 */
EvolutionHistory::EvolutionHistory(const std::string& name)
    : m_experimentName(name)
{
}

/**
 * @brief Add a new generation record to the history
 * @param record Generation data to store
 */
void EvolutionHistory::addRecord(const GenerationRecord& record)
{
    m_records.push_back(record);
}

void EvolutionHistory::generateDummyData(unsigned int numGenerations)
{
    clear();
    for (unsigned int gen = 0; gen < numGenerations; ++gen)
    {
        GenerationRecord rec;
        rec.generationNumber = gen;
        // Dummy fitness trend: increasing average with some noise
        rec.bestFitness = 0.2 + (gen * 0.015) + (rand() % 100) / 500.0;
        rec.averageFitness = 0.1 + (gen * 0.01) + (rand() % 100) / 800.0;
        rec.worstFitness = 0.05 + (gen * 0.005) + (rand() % 100) / 1000.0;
        rec.bestWatch = nullptr;
        m_records.push_back(rec);
    }
}

/**
 * @brief Get all stored generation records (read-only)
 * @return Constant reference to the record vector
 */
const std::vector<EvolutionHistory::GenerationRecord>& EvolutionHistory::getAllRecords() const
{
    return m_records;
}

/**
 * @brief Get a single record by generation number
 * @param gen Target generation number
 * @return Matching generation record
 * @throws std::out_of_range if generation not found
 */
const EvolutionHistory::GenerationRecord& EvolutionHistory::getRecord(unsigned int gen) const
{
    for (const auto& record : m_records)
    {
        if (record.generationNumber == gen)
        {
            return record;
        }
    }

    throw std::out_of_range("Generation not found in evolution history.");
}

/**
 * @brief Get total number of stored records
 * @return Count of generation records
 */
unsigned int EvolutionHistory::getRecordCount() const
{
    return static_cast<unsigned int>(m_records.size());
}

/**
 * @brief Get the experiment name
 * @return Constant reference to experiment name
 */
const std::string& EvolutionHistory::getExperimentName() const
{
    return m_experimentName;
}

/**
 * @brief Set the experiment name
 * @param name New experiment name
 */
void EvolutionHistory::setExperimentName(const std::string& name)
{
    m_experimentName = name;
}

/**
 * @brief Clear all stored generation records
 */
void EvolutionHistory::clear()
{
    m_records.clear();
}

/**
 * @brief Save evolution history to a file
 * @param path File path to save
 * @return True on success, false on failure
 */
bool EvolutionHistory::saveToFile(const std::string& path) const
{
    std::ofstream file(path);
    if (!file.is_open())
        return false;

    // Save experiment metadata
    file << m_experimentName << '\n';
    file << m_records.size() << '\n';

    // Save each generation's statistics
    for (const auto& rec : m_records)
    {
        file << rec.generationNumber << ' '
             << rec.bestFitness << ' '
             << rec.averageFitness << ' '
             << rec.worstFitness << '\n';
    }

    file.close();
    return true;
}

/**
 * @brief Load evolution history from a file
 * @param path File path to load
 * @return True on success, false on failure
 */
bool EvolutionHistory::loadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    clear();

    // Load metadata
    size_t recordCount = 0;
    std::getline(file, m_experimentName);
    file >> recordCount;

    // Load generation records
    GenerationRecord rec;
    for (size_t i = 0; i < recordCount; ++i)
    {
        file >> rec.generationNumber
             >> rec.bestFitness
             >> rec.averageFitness
             >> rec.worstFitness;
        rec.bestWatch = nullptr; // Watch object not serialized in basic implementation
        m_records.push_back(rec);
    }

    file.close();
    return true;
}

} // namespace FileIO
} // namespace WatchGA