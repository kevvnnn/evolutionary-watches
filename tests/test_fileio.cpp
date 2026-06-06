#include <iostream>
#include <cassert>
#include <memory>
#include <cstdio> // For std::remove

// FileIO Headers
#include "../fileio/EvolutionHistory.h"
#include "../fileio/WatchFileIO.h"

// Genome Header needed to test WatchFileIO
#include "../genome/Watch.h"

using namespace WatchGA;
using namespace WatchGA::FileIO;

// ==============================================================================
// 1. EVOLUTION HISTORY TESTS (TSV Text Format)
// ==============================================================================

void testEvolutionHistoryMemory() {
    std::cout << "Running testEvolutionHistoryMemory... ";
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

void testEvolutionHistoryFile() {
    std::cout << "Running testEvolutionHistoryFile... ";
    std::string testPath = "test_history.txt";

    // 1. Create and Save Data
    EvolutionHistory writerHistory("Round_Trip_Experiment");
    writerHistory.generateDummyData(10);
    bool saveSuccess = writerHistory.saveToFile(testPath);
    assert(saveSuccess && "Failed to open or write to the text file.");

    // 2. Load Data into a Brand New Object
    EvolutionHistory readerHistory;
    bool loadSuccess = readerHistory.loadFromFile(testPath);
    assert(loadSuccess && "Failed to find or read the text file.");

    // 3. Verify the Data Survived
    assert(readerHistory.getRecordCount() == 10);
    auto loadedRecord = readerHistory.getRecord(0);
    assert(loadedRecord.generationNumber == 0);

    // 4. Cleanup the test file
    std::remove(testPath.c_str());

    std::cout << "PASSED.\n";
}

// ==============================================================================
// 2. WATCH FILE I/O TESTS (RAII Binary Format)
// ==============================================================================

void testWatchFileIO() {
    std::cout << "Running testWatchFileIO (RAII Binary)... ";
    std::string testPath = "test_watch_data.bin";

    // Ensure we start with a clean slate
    std::remove(testPath.c_str());

    WatchFileIO fileIO(testPath);
    
    // Test 1: File shouldn't exist / should be empty initially
    assert(fileIO.getWatchCount() == 0 && "File should be empty initially");

    // Test 2: Create a dummy Watch to serialize
    Genome::Watch dummyWatch("Rolex_Simulation_1");
    dummyWatch.setFitnessScore(0.954);
    dummyWatch.setValid(true);

    // Test 3: Save the watch to binary using append mode
    bool saved = fileIO.saveWatch(dummyWatch, 0);
    assert(saved && "Failed to write Watch to binary file");

    // Test 4: Verify the file size grew
    assert(fileIO.getWatchCount() > 0 && "Binary file size did not increase after save");

    // Test 5: Load the watch back into a new pointer
    auto loadedWatch = fileIO.loadWatch(0);
    assert(loadedWatch != nullptr && "Failed to read Watch from binary file");
    
    // Test 6: Verify the data survived the binary translation perfectly
    assert(loadedWatch->getObjectName() == "Rolex_Simulation_1");
    assert(loadedWatch->isValid() == true);
    assert(loadedWatch->getFitnessScore() == 0.954);

    // 7. Cleanup the test file
    // std::remove(testPath.c_str());

    std::cout << "PASSED.\n";
}


// ==============================================================================
// MAIN EXECUTION
// ==============================================================================

int main() {
    std::cout << "==========================================\n";
    std::cout << " STARTING COMPLETE FILE I/O TEST SUITE\n";
    std::cout << "==========================================\n\n";

    // 1. Evolution History (Statistical TSV Logs)
    testEvolutionHistoryMemory();
    testEvolutionHistoryFile();

    // 2. Watch File IO (Physical Object Binary Streams)
    testWatchFileIO();

    std::cout << "\n==========================================\n";
    std::cout << " SUCCESS: ALL FILE I/O TESTS PASSED!\n";
    std::cout << "==========================================\n";

    return 0;
}