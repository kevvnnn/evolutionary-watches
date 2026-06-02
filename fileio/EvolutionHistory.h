#ifndef WATCH_GA_FILEIO_EVOLUTIONHISTORY_H
#define WATCH_GA_FILEIO_EVOLUTIONHISTORY_H

#include <vector>
#include <string>
#include <memory>

namespace WatchGA { namespace Genome { class Watch; } }

namespace WatchGA {
namespace FileIO {

/// <summary>
/// Stores the history of a genetic algorithm evolution run.
///
/// Each generation's fitness statistics and best-performing
/// watch genome are recorded so that the experiment can be
/// analyzed, visualized, saved, or loaded later.
/// </summary>
class EvolutionHistory {
public:

    /// <summary>
    /// Represents the data collected for a single generation.
    /// </summary>
    struct GenerationRecord {
        unsigned int generationNumber;
        double bestFitness;
        double averageFitness;
        double worstFitness;
        std::shared_ptr<Genome::Watch> bestWatch;
    };

private:

    /// <summary>
    /// Collection of generation records stored in chronological order.
    /// </summary>
    std::vector<GenerationRecord> m_records;

    /// <summary>
    /// Name of the experiment associated with this evolution history.
    /// </summary>
    std::string m_experimentName;

public:

    /// <summary>
    /// Creates an empty evolution history.
    /// </summary>
    EvolutionHistory();

    /// <summary>
    /// Creates an evolution history with a specified experiment name.
    /// </summary>
    explicit EvolutionHistory(const std::string& experimentName);

    ~EvolutionHistory() = default;

    /// <summary>
    /// Adds a generation record to the history.
    /// </summary>
    void addRecord(const GenerationRecord& record);

    /// <summary>
    /// Retrieves all stored generation records.
    /// </summary>
    const std::vector<GenerationRecord>& getAllRecords() const;

    /// <summary>
    /// Retrieves the record for a specific generation.
    /// </summary>
    const GenerationRecord& getRecord(unsigned int generationNumber) const;

    /// <summary>
    /// Returns the total number of stored generation records.
    /// </summary>
    unsigned int getRecordCount() const;

    /// <summary>
    /// Gets the name of the experiment.
    /// </summary>
    const std::string& getExperimentName() const;

    /// <summary>
    /// Sets or updates the experiment name.
    /// </summary>
    void setExperimentName(const std::string& name);

    /// <summary>
    /// Removes all stored generation records.
    /// </summary>
    void clear();

    /// <summary>
    /// Saves the evolution history to a file.
    /// </summary>
    bool saveToFile(const std::string& filePath) const;

    /// <summary>
    /// Loads evolution history data from a file.
    /// </summary>
    bool loadFromFile(const std::string& filePath);
};

}
}

#endif