#ifndef WATCH_GA_FILEIO_WATCHFILEIO_H
#define WATCH_GA_FILEIO_WATCHFILEIO_H

#include <string>
#include <memory>
#include <vector>

namespace WatchGA { namespace Genome { class Watch; } }
namespace WatchGA { namespace Algorithm { class GeneticAlgorithm; } }

namespace WatchGA {
namespace FileIO {

/// <summary>
/// Handles reading and writing watch genomes and genetic algorithm
/// data to and from files.
///
/// This class provides functionality for saving individual watches,
/// entire populations, and complete genetic algorithm states so that
/// experiments can be persisted and restored later.
/// </summary>
class WatchFileIO {
private:

    /// <summary>
    /// Path to the file used for reading and writing data.
    /// </summary>
    std::string m_filePath;

    /// <summary>
    /// Indicates whether the file is currently open.
    /// </summary>
    bool m_isOpen;

    /// <summary>
    /// Writes a watch genome to the currently open file.
    /// </summary>
    /// <param name="watch">
    /// Watch object to serialize and store.
    /// </param>
    void writeWatch(const Genome::Watch& watch);

    /// <summary>
    /// Reads a watch genome from the currently open file.
    /// </summary>
    /// <returns>
    /// A dynamically allocated watch object containing the loaded data.
    /// </returns>
    std::unique_ptr<Genome::Watch> readWatch();

public:

    /// <summary>
    /// Creates a file handler with no file path assigned.
    /// </summary>
    WatchFileIO();

    /// <summary>
    /// Creates a file handler using the specified file path.
    /// </summary>
    /// <param name="filePath">
    /// Path of the file to open and manage.
    /// </param>
    explicit WatchFileIO(const std::string& filePath);

    /// <summary>
    /// Closes any open file and releases resources.
    /// </summary>
    ~WatchFileIO();

    /// <summary>
    /// Opens the file for reading and writing.
    /// </summary>
    /// <param name="createIfNotExists">
    /// If true, a new file will be created if the specified
    /// file does not already exist.
    /// </param>
    /// <returns>
    /// True if the file was opened successfully; otherwise false.
    /// </returns>
    bool open(bool createIfNotExists = true);

    /// <summary>
    /// Closes the currently open file.
    /// </summary>
    void close();

    /// <summary>
    /// Determines whether a file is currently open.
    /// </summary>
    /// <returns>
    /// True if a file is open; otherwise false.
    /// </returns>
    bool isOpen() const;

    /// <summary>
    /// Saves a watch genome at the specified index.
    /// </summary>
    /// <param name="watch">
    /// Watch object to save.
    /// </param>
    /// <param name="index">
    /// Position where the watch should be stored.
    /// </param>
    /// <returns>
    /// True if the save operation succeeds; otherwise false.
    /// </returns>
    bool saveWatch(
        const Genome::Watch& watch,
        unsigned int index);

    /// <summary>
    /// Loads a watch genome from the specified index.
    /// </summary>
    /// <param name="index">
    /// Position of the watch to load.
    /// </param>
    /// <returns>
    /// Pointer to the loaded watch object.
    /// Returns nullptr if loading fails.
    /// </returns>
    std::unique_ptr<Genome::Watch> loadWatch(
        unsigned int index);

    /// <summary>
    /// Saves an entire genetic algorithm population.
    /// </summary>
    /// <param name="ga">
    /// Genetic algorithm containing the population to save.
    /// </param>
    /// <returns>
    /// True if the save operation succeeds; otherwise false.
    /// </returns>
    bool savePopulation(
        const Algorithm::GeneticAlgorithm& ga);

    /// <summary>
    /// Loads a population into the specified genetic algorithm.
    /// </summary>
    /// <param name="ga">
    /// Genetic algorithm that will receive the loaded population.
    /// </param>
    /// <returns>
    /// True if loading succeeds; otherwise false.
    /// </returns>
    bool loadPopulation(
        Algorithm::GeneticAlgorithm& ga);

    /// <summary>
    /// Saves the complete state of a genetic algorithm.
    /// </summary>
    /// <param name="ga">
    /// Genetic algorithm whose state should be saved.
    /// </param>
    /// <returns>
    /// True if the save operation succeeds; otherwise false.
    /// </returns>
    bool saveCompleteState(
        const Algorithm::GeneticAlgorithm& ga);

    /// <summary>
    /// Loads a previously saved genetic algorithm state.
    /// </summary>
    /// <param name="ga">
    /// Genetic algorithm object that will receive the loaded state.
    /// </param>
    /// <returns>
    /// True if loading succeeds; otherwise false.
    /// </returns>
    bool loadCompleteState(
        Algorithm::GeneticAlgorithm& ga);

    /// <summary>
    /// Returns the total number of watches stored in the file.
    /// </summary>
    /// <returns>
    /// Number of watch genomes available.
    /// </returns>
    unsigned int getWatchCount() const;
};

}
}

#endif