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
 * @brief Handles saving and loading of watches, populations, and algorithm state
 *
 * This class manages binary file I/O for all persistent data in the project.
 * It can save individual watches, full GA populations, and complete evolution state.
 */
class WatchFileIO {
private:
    std::string m_filePath; 

    // Internal helpers that take active, localized streams
    void writeWatch(std::ostream& out, const Genome::Watch& watch) const;

    // Internal helper: Reads and reconstructs a Watch from a binary stream
    std::unique_ptr<Genome::Watch> readWatch(std::istream& in) const;

public:
    /**
     * @brief Default constructor
     * Uses default file name: population_data.bin
     */
    WatchFileIO();

    /**
     * @brief Constructor with custom file path
     * @param filePath Path to save/load file
     */
    explicit WatchFileIO(const std::string& filePath);
    ~WatchFileIO() = default;

    /**
     * @brief Sets the target file path
     * @param path New file path
     */
    void setFilePath(const std::string& path);

    /**
     * @brief Gets current file path
     * @return Path string
     */
    const std::string& getFilePath() const;

    // Watch save/load

    /**
     * @brief Saves one Watch to file
     * @param watch Watch to save
     * @param index Unused for single watch
     * @return True if successful
     */
    bool saveWatch(const Genome::Watch& watch, unsigned int index);

    /**
     * @brief Loads one Watch from file
     * @param index Watch index to load
     * @return Loaded Watch or nullptr
     */
    std::unique_ptr<Genome::Watch> loadWatch(unsigned int index) const;

    // Population save/load
    /**
     * @brief Saves entire GA population
     * @param ga Genetic algorithm with population
     * @return True if successful
     */
    bool savePopulation(const Algorithm::GeneticAlgorithm& ga);

    /**
     * @brief Loads population into GA (placeholder)
     * @param ga Target GA
     * @return True
     */
    bool loadPopulation(Algorithm::GeneticAlgorithm& ga) const;

    // Complete state save/load

    /**
     * @brief Saves full evolution state (generation, settings, population)
     * @param ga Source algorithm
     * @return True if successful
     */
    bool saveCompleteState(const Algorithm::GeneticAlgorithm& ga);

    /**
     * @brief Loads full evolution state (generation, settings, population)
     * @param ga Target algorithm
     * @return True if successful
     */
    bool loadCompleteState(Algorithm::GeneticAlgorithm& ga) const;

    /**
     * @brief Gets estimated data size
     * @return File size in bytes
     */
    unsigned int getWatchCount() const;
};

} // namespace FileIO
} // namespace WatchGA

#endif // WATCH_GA_FILEIO_WATCHFILEIO_H