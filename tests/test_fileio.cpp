// tests/test_fileio.cpp

#include "../fileio/WatchFileIO.h"
#include "../fileio/ConfigManager.h"
#include "../fileio/EvolutionHistory.h"

#include <iostream>

using namespace WatchGA;

int main()
{
    std::cout << "=================================\n";
    std::cout << "Running FileIO Module Tests\n";
    std::cout << "=================================\n";

    // ----------------------------------------------------
    // WatchFileIO
    // ----------------------------------------------------

    {
        std::cout << "\nTesting WatchFileIO...\n";

        FileIO::WatchFileIO fileIO("test.bin");

        bool opened = fileIO.open(true);

        std::cout << "Open: "
                  << opened
                  << "\n";

        std::cout << "Is Open: "
                  << fileIO.isOpen()
                  << "\n";

        std::cout << "Watch Count: "
                  << fileIO.getWatchCount()
                  << "\n";

        fileIO.close();

        std::cout << "Closed successfully\n";
    }

    // ----------------------------------------------------
    // ConfigManager
    // ----------------------------------------------------

    {
        std::cout << "\nTesting ConfigManager...\n";

        FileIO::ConfigManager config;

        std::cout << "ConfigManager constructed.\n";
    }

    // ----------------------------------------------------
    // EvolutionHistory
    // ----------------------------------------------------

    {
        std::cout << "\nTesting EvolutionHistory...\n";

        FileIO::EvolutionHistory history;

        std::cout << "EvolutionHistory constructed.\n";
    }

    std::cout << "\n=================================\n";
    std::cout << "All FileIO tests completed.\n";
    std::cout << "=================================\n";

    return 0;
}