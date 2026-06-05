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

        FileIO::WatchFileIO fileIO("test.txt");

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

        std::cout << "ConfigManager constructed with default path.\n";

        std::cout << "Loaded new Config: " << std::boolalpha << config.loadConfig() << "\n";
        
        config.setInt("int1", 22);
        config.setBool("bool1", true);
        config.setDouble("double1", 1.12);
        config.setString("string1", "Hello2");
        config.saveConfig();
        std::cout << "Set example values and saved\n\n";
        // std::cout << "Loaded new Config: " << std::boolalpha << config.loadConfig() << "\n";
        std::cout << "int value: " << config.getInt("int1") << "\n";
        std::cout << "bool value: " << config.getBool("bool1") << "\n";
        std::cout << "double value: " << config.getDouble("double1") << "\n";
        std::cout << "string value: " << config.getString("string1") << "\n";
        

    }

    // ----------------------------------------------------
    // EvolutionHistory
    // ----------------------------------------------------

    {
        std::cout << "\n\nTesting EvolutionHistory...\n";

        FileIO::EvolutionHistory history;

        std::cout << "EvolutionHistory constructed.\n";
    }

    std::cout << "\n=================================\n";
    std::cout << "All FileIO tests completed.\n";
    std::cout << "=================================\n";

    return 0;
}