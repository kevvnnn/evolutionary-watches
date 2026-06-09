#ifndef WATCH_GA_FILE_IO_EVOLUTION_HISTORY_H
#define WATCH_GA_FILE_IO_EVOLUTION_HISTORY_H

#include <vector>
#include <string>
#include <memory>

namespace WatchGA { namespace Genome { class Watch; } }

namespace WatchGA {
namespace FileIO {

/**
 * @brief Stores and manages evolution statistics across generations
 *
 * Records fitness values (best, average, worst) for each generation.
 * Supports saving/loading to text files for analysis and reporting.
 */
class EvolutionHistory
{
public:
    /**
     * @brief Stores data for one full generation
     *
     * Contains generation number and all fitness statistics.
     */
    struct GenerationRecord
    {
        unsigned int generationNumber;
        double bestFitness;
        double averageFitness;
        double worstFitness;
        std::shared_ptr<Genome::Watch> bestWatch;
    };

private:
    std::vector<GenerationRecord> m_records;
    std::string m_experimentName;

public:
    /**
     * @brief Default constructor
     * Creates empty history with no experiment name
     */
    EvolutionHistory();

    /**
     * @brief Constructor with experiment name
     * @param name Name for this evolution run
     */
    explicit EvolutionHistory(const std::string& name);

    ~EvolutionHistory() = default;

    /**
     * @brief Adds a new generation’s data to history
     * @param record Generation stats to store
     */
    void addRecord(const GenerationRecord& record);

    /**
     * @brief Gets all recorded generation data
     * @return List of all records
     */
    const std::vector<GenerationRecord>& getAllRecords() const;

    /**
     * @brief Gets one record by generation number
     * @param gen Target generation
     * @return Matching record
     */
    const GenerationRecord& getRecord(unsigned int gen) const;

    /**
     * @brief Gets total number of recorded generations
     * @return Count of records
     */
    unsigned int getRecordCount() const;

    /**
     * @brief Gets the name of the experiment
     * @return Experiment name
     */
    const std::string& getExperimentName() const;

    /**
     * @brief Sets the name of the experiment
     * @param name New experiment name
     */
    void setExperimentName(const std::string& name);

    /**
     * @brief Clears all recorded generation data
     */
    void clear();

    /**
     * @brief Saves history to a formatted text file
     * @param path File path to save
     * @return True if successful
     */
    bool saveToFile(const std::string& path) const;

    /**
     * @brief Loads history from a text file
     * @param path File path to load
     * @return True if successful
     */
    bool loadFromFile(const std::string& path);

    /**
     * @brief Generates test data for GUI preview
     * @param numGenerations Number of fake generations
     */
    void generateDummyData(unsigned int numGenerations = 50);
};

} // namespace FileIO
} // namespace WatchGA

#endif