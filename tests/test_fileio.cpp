#include <iostream>
#include <cassert>
#include "../fileio/EvolutionHistory.h"

using namespace WatchGA::FileIO;

void testMemoryOperations() {
    std::cout << "Running testMemoryOperations... ";
    EvolutionHistory history("Test_Experiment");

    // Test Adding
    EvolutionHistory::GenerationRecord rec1{0, 0.8, 0.5, 0.2, nullptr};
    history.addRecord(rec1);
    assert(history.getRecordCount() == 1);

    // Test Retrieval
    auto retrieved = history.getRecord(0);
    assert(retrieved.bestFitness == 0.8);

    // Test Clearing
    history.clear();
    assert(history.getRecordCount() == 0);

    std::cout << "PASSED.\n";
}

void testFileRoundTrip() {
    std::cout << "Running testFileRoundTrip... ";
    std::string testPath = "test_history.txt";

    // 1. Create and Save Data
    EvolutionHistory writerHistory("Round_Trip_Experiment");
    writerHistory.generateDummyData(10);
    bool saveSuccess = writerHistory.saveToFile(testPath);
    assert(saveSuccess && "Failed to open or write to the test file.");

    // 2. Load Data into a Brand New Object
    EvolutionHistory readerHistory;
    bool loadSuccess = readerHistory.loadFromFile(testPath);
    assert(loadSuccess && "Failed to find or read the test file.");

    // 3. Verify the Data Survived
    assert(readerHistory.getExperimentName() == "Round_Trip_Experiment");
    assert(readerHistory.getRecordCount() == 10);
    
    // Verify a specific data point
    auto loadedRecord = readerHistory.getRecord(0);
    assert(loadedRecord.generationNumber == 0);

    std::cout << "PASSED.\n";
}

int main() {
    std::cout << "==========================================\n";
    std::cout << " STARTING FILE I/O TEST SUITE\n";
    std::cout << "==========================================\n\n";

    testMemoryOperations();
    testFileRoundTrip();

    std::cout << "\n==========================================\n";
    std::cout << " SUCCESS: ALL FILE I/O TESTS PASSED!\n";
    std::cout << "==========================================\n";

    return 0;
}

// // tests/test_fileio.cpp

// #include "../fileio/WatchFileIO.h"
// #include "../fileio/ConfigManager.h"
// #include "../fileio/EvolutionHistory.h"

// #include <iostream>

// using namespace WatchGA;

// int main()
// {
//     std::cout << "=================================\n";
//     std::cout << "Running FileIO Module Tests\n";
//     std::cout << "=================================\n";

//     // ----------------------------------------------------
//     // WatchFileIO
//     // ----------------------------------------------------

//     {
//         std::cout << "\nTesting WatchFileIO...\n";

//         FileIO::WatchFileIO fileIO("test.bin");

//         bool opened = fileIO.open(true);

//         std::cout << "Open: "
//                   << opened
//                   << "\n";

//         std::cout << "Is Open: "
//                   << fileIO.isOpen()
//                   << "\n";

//         std::cout << "Watch Count: "
//                   << fileIO.getWatchCount()
//                   << "\n";

//         fileIO.close();

//         std::cout << "Closed successfully\n";
//     }

//     // ----------------------------------------------------
//     // ConfigManager
//     // ----------------------------------------------------

//     {
//         std::cout << "\nTesting ConfigManager...\n";

//         FileIO::ConfigManager config;

//         std::cout << "ConfigManager constructed.\n";
//     }

//     // ----------------------------------------------------
//     // EvolutionHistory
//     // ----------------------------------------------------

//     {
//         std::cout << "\nTesting EvolutionHistory...\n";

//         FileIO::EvolutionHistory history;

//         std::cout << "EvolutionHistory constructed.\n";
//     }

//     std::cout << "\n=================================\n";
//     std::cout << "All FileIO tests completed.\n";
//     std::cout << "=================================\n";

//     return 0;
// }