#ifndef WATCH_GA_FILEIO_WATCHFILEIO_H
#define WATCH_GA_FILEIO_WATCHFILEIO_H

#include <string>
#include <memory>
#include <vector>

namespace WatchGA { namespace Genome { class Watch; } }
namespace WatchGA { namespace Algorithm { class GeneticAlgorithm; } }

namespace WatchGA {
namespace FileIO {

class WatchFileIO {
private:
    std::string m_filePath;
    bool m_isOpen;

    void WriteWatch(const Genome::Watch& watch);

    std::unique_ptr<Genome::Watch> ReadWatch();

public:
    WatchFileIO();
    explicit WatchFileIO(const std::string& filePath);
    ~WatchFileIO();

    bool Open(bool createIfNotExists = true);

    void Close();

    bool IsOpen() const;

    bool SaveWatch(const Genome::Watch& watch, unsigned int index);

    std::unique_ptr<Genome::Watch> LoadWatch(unsigned int index);

    bool SavePopulation(const Algorithm::GeneticAlgorithm& ga);

    bool LoadPopulation(Algorithm::GeneticAlgorithm& ga);

    bool SaveCompleteState(const Algorithm::GeneticAlgorithm& ga);

    bool LoadCompleteState(Algorithm::GeneticAlgorithm& ga);

    unsigned int GetWatchCount() const;
};

}
}

#endif