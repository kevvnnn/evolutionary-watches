#ifndef WATCH_GA_FILE_IO_WATCH_FILE_IO_H
#define WATCH_GA_FILE_IO_WATCH_FILE_IO_H

#include <string>
#include <memory>
#include <vector>

namespace WatchGA {
namespace Genome { class Watch; }
namespace Algorithm { class GeneticAlgorithm; }

namespace FileIO {

/**
 * @class WatchFileIO
 * @brief Binary watch/population save/load
 */
class WatchFileIO
{
private:
    std::string m_filePath;
    bool m_isOpen;

    void writeWatch(const Genome::Watch& watch);
    std::unique_ptr<Genome::Watch> readWatch();

public:
    WatchFileIO();
    explicit WatchFileIO(const std::string& path);
    ~WatchFileIO();

    bool open(bool createIfMissing = true);
    void close();
    bool isOpen() const;

    bool saveWatch(const Genome::Watch& watch, unsigned int index);
    std::unique_ptr<Genome::Watch> loadWatch(unsigned int index);

    bool savePopulation(const Algorithm::GeneticAlgorithm& ga);
    bool loadPopulation(Algorithm::GeneticAlgorithm& ga);

    bool saveCompleteState(const Algorithm::GeneticAlgorithm& ga);
    bool loadCompleteState(Algorithm::GeneticAlgorithm& ga);

    unsigned int getWatchCount() const;
};

} // namespace FileIO
} // namespace WatchGA

#endif