#ifndef WATCH_GA_FILEIO_WATCHFILEIO_H
#define WATCH_GA_FILEIO_WATCHFILEIO_H

#include <string>
#include <memory>
#include <vector>
#include <fstream>

// Forward declarations
namespace WatchGA { namespace Genome { class Watch; } }
namespace WatchGA { namespace Algorithm { class GeneticAlgorithm; } }

namespace WatchGA {
namespace FileIO {

/**
 * @brief Handles RANDOM FILE PROCESSING for watch designs
 * Uses RAII binary streaming for deep-data serialization.
 * * OWNER: Person C (with Backend Integration)
 */
class WatchFileIO {
private:
    std::string m_filePath; 

    // Internal helpers that take active, localized streams
    void writeWatch(std::ostream& out, const Genome::Watch& watch) const;
    std::unique_ptr<Genome::Watch> readWatch(std::istream& in) const;

public:
    WatchFileIO();
    explicit WatchFileIO(const std::string& filePath);
    ~WatchFileIO() = default;

    // File Operations
    void setFilePath(const std::string& path);
    const std::string& getFilePath() const;

    // Watch save/load
    bool saveWatch(const Genome::Watch& watch, unsigned int index);
    std::unique_ptr<Genome::Watch> loadWatch(unsigned int index) const;

    // Population save/load
    bool savePopulation(const Algorithm::GeneticAlgorithm& ga);
    bool loadPopulation(Algorithm::GeneticAlgorithm& ga) const;

    // Complete state save/load
    bool saveCompleteState(const Algorithm::GeneticAlgorithm& ga);
    bool loadCompleteState(Algorithm::GeneticAlgorithm& ga) const;

    // Utility
    unsigned int getWatchCount() const;
};

} // namespace FileIO
} // namespace WatchGA

#endif // WATCH_GA_FILEIO_WATCHFILEIO_H