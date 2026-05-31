#ifndef WATCH_GA_FILEIO_EVOLUTIONHISTORY_H
#define WATCH_GA_FILEIO_EVOLUTIONHISTORY_H

#include <vector>
#include <string>
#include <memory>

namespace WatchGA { namespace Genome { class Watch; } }

namespace WatchGA {
namespace FileIO {

class EvolutionHistory {
public:
    struct GenerationRecord {
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
    explicit EvolutionHistory(const std::string& experimentName);
    ~EvolutionHistory() = default;

    void AddRecord(const GenerationRecord& record);

    const std::vector<GenerationRecord>& GetAllRecords() const;
    const GenerationRecord& GetRecord(unsigned int generationNumber) const;
    unsigned int GetRecordCount() const;

    const std::string& GetExperimentName() const;
    void SetExperimentName(const std::string& name);

    void Clear();

    bool SaveToFile(const std::string& filePath) const;

    bool LoadFromFile(const std::string& filePath);
};

}
}

#endif