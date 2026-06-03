#ifndef WATCH_GA_FILE_IO_EVOLUTION_HISTORY_H
#define WATCH_GA_FILE_IO_EVOLUTION_HISTORY_H

#include <vector>
#include <string>
#include <memory>

namespace WatchGA { namespace Genome { class Watch; } }

namespace WatchGA {
namespace FileIO {

/**
 * @class EvolutionHistory
 * @brief Saves generation stats & best watches
 * @ownership Person C
 */
class EvolutionHistory
{
public:
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
    EvolutionHistory();
    explicit EvolutionHistory(const std::string& name);
    ~EvolutionHistory() = default;

    void addRecord(const GenerationRecord& record);
    const std::vector<GenerationRecord>& getAllRecords() const;
    const GenerationRecord& getRecord(unsigned int gen) const;
    unsigned int getRecordCount() const;

    const std::string& getExperimentName() const;
    void setExperimentName(const std::string& name);

    void clear();
    bool saveToFile(const std::string& path) const;
    bool loadFromFile(const std::string& path);
};

} // namespace FileIO
} // namespace WatchGA

#endif