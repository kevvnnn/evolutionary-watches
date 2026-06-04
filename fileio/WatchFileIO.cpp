#include "WatchFileIO.h"

#include "../genome/Watch.h"
#include "../algorithm/GeneticAlgorithm.h"

#include <fstream>
#include <iostream>

namespace WatchGA {
namespace FileIO {

using namespace Genome;
using namespace Algorithm;

// Default constructor
WatchFileIO::WatchFileIO()
    : m_filePath(""),
      m_isOpen(false)
{
}

// Constructor with path
WatchFileIO::WatchFileIO(const std::string& path)
    : m_filePath(path),
      m_isOpen(false)
{
}

// Destructor
WatchFileIO::~WatchFileIO()
{
    if (m_isOpen)
    {
        close();
    }
}

// Writes a Watch object to file
void WatchFileIO::writeWatch(const Genome::Watch& watch)
{
    (void)watch;

    std::cerr << "writeWatch() not implemented.\n";
}

// Reads a Watch object from file
std::unique_ptr<Genome::Watch> WatchFileIO::readWatch()
{
    std::cerr << "readWatch() not implemented.\n";

    return std::unique_ptr<Genome::Watch>();
}

// Opens file
bool WatchFileIO::open(bool createIfMissing)
{
    if (m_filePath.empty())
    {
        std::cerr << "No file path specified.\n";
        return false;
    }

    std::ifstream input(m_filePath, std::ios::binary);

    if (!input.is_open())
    {
        if (!createIfMissing)
        {
            return false;
        }

        std::ofstream output(m_filePath, std::ios::binary);

        if (!output.is_open())
        {
            std::cerr << "Unable to create file: "
                      << m_filePath << '\n';
            return false;
        }
    }

    m_isOpen = true;
    return true;
}

// Close file
void WatchFileIO::close()
{
    m_isOpen = false;
}

// Check open state
bool WatchFileIO::isOpen() const
{
    return m_isOpen;
}

// Save a watch
bool WatchFileIO::saveWatch(
    const Genome::Watch& watch,
    unsigned int index)
{
    if (!m_isOpen)
    {
        return false;
    }

    (void)index;

    writeWatch(watch);

    return true;
}

// Load a watch
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

// Save GA population
bool WatchFileIO::savePopulation(
    const Algorithm::GeneticAlgorithm& ga)
{
    (void)ga;

    if (!m_isOpen)
    {
        return false;
    }

    std::cerr << "savePopulation() not implemented.\n";
    return false;
}

// Load GA population
bool WatchFileIO::loadPopulation(
    Algorithm::GeneticAlgorithm& ga)
{
    (void)ga;

    if (!m_isOpen)
    {
        return false;
    }

    std::cerr << "loadPopulation() not implemented.\n";
    return false;
}

// Save complete GA state
bool WatchFileIO::saveCompleteState(
    const Algorithm::GeneticAlgorithm& ga)
{
    (void)ga;

    if (!m_isOpen)
    {
        return false;
    }

    std::cerr << "saveCompleteState() not implemented.\n";
    return false;
}

// Load complete GA state
bool WatchFileIO::loadCompleteState(
    Algorithm::GeneticAlgorithm& ga)
{
    (void)ga;

    if (!m_isOpen)
    {
        return false;
    }

    std::cerr << "loadCompleteState() not implemented.\n";
    return false;
}

// Count watches in file
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