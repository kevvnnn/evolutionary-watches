#include "EvolutionHistory.h"

#include <fstream>
#include <stdexcept>

namespace WatchGA {
namespace FileIO {

/// <summary>
/// Default constructor.
/// Creates an empty evolution history with no experiment name.
/// </summary>
EvolutionHistory::EvolutionHistory()
    : m_experimentName("")
{
}

/// <summary>
/// Creates an evolution history and assigns the experiment name.
/// </summary>
/// <param name="experimentName">Name of the experiment.</param>
EvolutionHistory::EvolutionHistory(const std::string& experimentName)
    : m_experimentName(experimentName)
{
}

/// <summary>
/// Adds a new generation record to the history.
/// </summary>
/// <param name="record">Generation data to store.</param>
void EvolutionHistory::addRecord(const GenerationRecord& record)
{
    m_records.push_back(record);
}

/// <summary>
/// Returns all stored generation records.
/// </summary>
/// <returns>Constant reference to the record vector.</returns>
const std::vector<EvolutionHistory::GenerationRecord>&
EvolutionHistory::getAllRecords() const
{
    return m_records;
}

/// <summary>
/// Finds a record by generation number.
/// </summary>
/// <param name="generationNumber">Generation to search for.</param>
/// <returns>Matching generation record.</returns>
/// <exception cref="std::out_of_range">
/// Thrown if the generation does not exist.
/// </exception>
const EvolutionHistory::GenerationRecord&
EvolutionHistory::getRecord(unsigned int generationNumber) const
{
    // Search through every stored generation record.
    for (const auto& currentRecord : m_records)
    {
        if (currentRecord.generationNumber == generationNumber)
        {
            return currentRecord;
        }
    }

    // No matching generation was found.
    throw std::out_of_range("Generation record not found.");
}

/// <summary>
/// Returns the number of records currently stored.
/// </summary>
/// <returns>Total record count.</returns>
unsigned int EvolutionHistory::getRecordCount() const
{
    return static_cast<unsigned int>(m_records.size());
}

/// <summary>
/// Gets the experiment name.
/// </summary>
/// <returns>Experiment name.</returns>
const std::string& EvolutionHistory::getExperimentName() const
{
    return m_experimentName;
}

/// <summary>
/// Sets or changes the experiment name.
/// </summary>
/// <param name="name">New experiment name.</param>
void EvolutionHistory::setExperimentName(const std::string& name)
{
    m_experimentName = name;
}

/// <summary>
/// Removes all stored records.
/// </summary>
void EvolutionHistory::clear()
{
    m_records.clear();
}

/// <summary>
/// Saves the evolution history to a text file.
/// </summary>
/// <param name="filePath">Destination file path.</param>
/// <returns>True if save succeeded; otherwise false.</returns>
bool EvolutionHistory::saveToFile(const std::string& filePath) const
{
    std::ofstream outputFile(filePath);

    // Ensure the file was opened successfully.
    if (!outputFile.is_open())
    {
        return false;
    }

    // Write experiment metadata.
    outputFile << m_experimentName << '\n';

    // Write number of stored records.
    outputFile << m_records.size() << '\n';

    // Write each generation record.
    for (const auto& currentRecord : m_records)
    {
        outputFile << currentRecord.generationNumber << ' '
                   << currentRecord.bestFitness << ' '
                   << currentRecord.averageFitness << ' '
                   << currentRecord.worstFitness << '\n';

        // If Genome::Watch serialization is implemented later,
        // the bestWatch object can be saved here as well.
    }

    return true;
}

/// <summary>
/// Loads evolution history data from a text file.
/// </summary>
/// <param name="filePath">Source file path.</param>
/// <returns>True if load succeeded; otherwise false.</returns>
bool EvolutionHistory::loadFromFile(const std::string& filePath)
{
    std::ifstream inputFile(filePath);

    // Ensure the file exists and can be opened.
    if (!inputFile.is_open())
    {
        return false;
    }

    // Remove any existing records before loading.
    clear();

    // Read experiment name from first line.
    std::getline(inputFile, m_experimentName);

    // Read total number of records.
    std::size_t recordCount = 0;
    inputFile >> recordCount;

    // Read each stored generation record.
    for (std::size_t recordIndex = 0;
         recordIndex < recordCount;
         ++recordIndex)
    {
        GenerationRecord newRecord;

        inputFile >> newRecord.generationNumber
                  >> newRecord.bestFitness
                  >> newRecord.averageFitness
                  >> newRecord.worstFitness;

        // Watch data is not currently serialized.
        newRecord.bestWatch = nullptr;

        m_records.push_back(newRecord);
    }

    return true;
}

} // namespace FileIO
} // namespace WatchGA