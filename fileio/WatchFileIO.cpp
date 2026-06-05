#include "WatchFileIO.h"

#include "../genome/Watch.h"
#include "../algorithm/GeneticAlgorithm.h"

#include <fstream>
#include <iostream>

namespace WatchGA {
namespace FileIO {

// ---------------------------------------------------------------------------
// Constructors / Destructor
// ---------------------------------------------------------------------------

WatchFileIO::WatchFileIO()
    : m_filePath(""),
      m_isOpen(false)
{
}

WatchFileIO::WatchFileIO(const std::string& path)
    : m_filePath(path),
      m_isOpen(false)
{
}

WatchFileIO::~WatchFileIO()
{
    close();
}

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

void WatchFileIO::writeWatch(const Genome::Watch& watch)
{
    (void)watch;

    std::cerr << "writeWatch() not implemented.\n";
}

std::unique_ptr<Genome::Watch> WatchFileIO::readWatch()
{
    std::cerr << "readWatch() not implemented.\n";

    return std::unique_ptr<Genome::Watch>(
    new Genome::Watch()
);
}

// ---------------------------------------------------------------------------
// File operations
// ---------------------------------------------------------------------------

bool WatchFileIO::open(bool createIfMissing)
{
    if (m_isOpen)
    {
        return true;
    }

    if (m_filePath.empty())
    {
        return false;
    }

    std::ifstream input(m_filePath);

    if (!input.good())
    {
        if (!createIfMissing)
        {
            return false;
        }

        std::ofstream output(m_filePath);

        if (!output.good())
        {
            return false;
        }
    }

    m_isOpen = true;
    return true;
}

void WatchFileIO::close()
{
    m_isOpen = false;
}

bool WatchFileIO::isOpen() const
{
    return m_isOpen;
}

// ---------------------------------------------------------------------------
// Watch save/load
// ---------------------------------------------------------------------------

bool WatchFileIO::saveWatch(
    const Genome::Watch& watch,
    unsigned int index)
{
    if (!m_isOpen)
    {
        return false;
    }

    (void)watch;
    (void)index;

    std::cerr << "saveWatch() not implemented.\n";

    return true;
}

std::unique_ptr<Genome::Watch>
WatchFileIO::loadWatch(unsigned int index)
{
    if (!m_isOpen)
    {
        return nullptr;
    }

    (void)index;

    return readWatch();
}

// ---------------------------------------------------------------------------
// Population save/load
// ---------------------------------------------------------------------------

bool WatchFileIO::savePopulation(
    const Algorithm::GeneticAlgorithm& ga)
{
    if (!m_isOpen)
    {
        return false;
    }

    (void)ga;

    std::cerr << "savePopulation() not implemented.\n";

    return true;
}

bool WatchFileIO::loadPopulation(
    Algorithm::GeneticAlgorithm& ga)
{
    if (!m_isOpen)
    {
        return false;
    }

    (void)ga;

    std::cerr << "loadPopulation() not implemented.\n";

    return true;
}

// ---------------------------------------------------------------------------
// Complete state save/load
// ---------------------------------------------------------------------------

bool WatchFileIO::saveCompleteState(
    const Algorithm::GeneticAlgorithm& ga)
{
    if (!m_isOpen)
    {
        return false;
    }

    (void)ga;

    std::cerr << "saveCompleteState() not implemented.\n";

    return true;
}

bool WatchFileIO::loadCompleteState(
    Algorithm::GeneticAlgorithm& ga)
{
    if (!m_isOpen)
    {
        return false;
    }

    (void)ga;

    std::cerr << "loadCompleteState() not implemented.\n";

    return true;
}

// ---------------------------------------------------------------------------
// Utility
// ---------------------------------------------------------------------------

unsigned int WatchFileIO::getWatchCount() const
{
    if (!m_isOpen)
    {
        return 0;
    }

    std::cerr << "getWatchCount() not implemented.\n";

    return 0;
}

} // namespace FileIO
} // namespace WatchGA