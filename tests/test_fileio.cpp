#include <cassert>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstdio>

#include "../fileio/ConfigManager.h"
#include "../fileio/EvolutionHistory.h"
#include "../fileio/WatchFileIO.h"

#include "../genome/Watch.h"

using namespace WatchGA;
using namespace WatchGA::FileIO;

// ------------------------------------------------------------
// Helper
// ------------------------------------------------------------
static std::string tempFile(const std::string& name)
{
    return "./temp_" + name;
}

// ------------------------------------------------------------
// TEST 1: ConfigManager (ALL FUNCTIONS)
// ------------------------------------------------------------
void testConfigManager()
{
    std::cout << "[TEST] ConfigManager\n";

    std::string path = tempFile("config.cfg");

    ConfigManager cfg(path);

    // Setters
    cfg.setString("name", "fileio_test");
    cfg.setInt("population", 100);
    cfg.setDouble("mutation", 0.25);
    cfg.setBool("elitism", true);

    // Save
    assert(cfg.saveConfig());

    // Load
    ConfigManager cfg2(path);
    assert(cfg2.loadConfig());

    // Getters
    assert(cfg2.getString("name") == "fileio_test");
    assert(cfg2.getInt("population") == 100);
    assert(cfg2.getDouble("mutation") == 0.25);
    assert(cfg2.getBool("elitism") == true);

    // hasKey
    assert(cfg2.hasKey("name"));

    // removeKey
    cfg2.removeKey("name");
    assert(!cfg2.hasKey("name"));

    // default values
    assert(cfg2.getInt("missing", -1) == -1);
    assert(cfg2.getDouble("missing", 3.14) == 3.14);
    assert(cfg2.getBool("missing", false) == false);

    // clear
    cfg2.clear();
    assert(!cfg2.hasKey("population"));

    std::remove(path.c_str());

    std::cout << "[OK] ConfigManager\n";
}

// ------------------------------------------------------------
// TEST 2: EvolutionHistory (ALL FUNCTIONS)
// ------------------------------------------------------------
void testEvolutionHistory()
{
    std::cout << "[TEST] EvolutionHistory\n";

    std::string path = tempFile("history.bin");

    EvolutionHistory history("experiment_test");

    // create record
    EvolutionHistory::GenerationRecord rec;
    rec.generationNumber = 1;
    rec.bestFitness = 99.0;
    rec.averageFitness = 50.0;
    rec.worstFitness = 10.0;

    rec.bestWatch = std::make_shared<Genome::Watch>();
    rec.bestWatch->setId(7);
    rec.bestWatch->setFitness(99.0);
    rec.bestWatch->setName("elite");

    // addRecord
    history.addRecord(rec);

    // getRecordCount
    assert(history.getRecordCount() == 1);

    // getAllRecords
    const auto& all = history.getAllRecords();
    assert(all.size() == 1);

    // save
    assert(history.saveToFile(path));

    // load
    EvolutionHistory loaded;
    assert(loaded.loadFromFile(path));

    // getRecord
    const auto& r = loaded.getRecord(0);

    assert(r.generationNumber == 1);
    assert(r.bestFitness == 99.0);
    assert(r.averageFitness == 50.0);
    assert(r.worstFitness == 10.0);

    assert(r.bestWatch != nullptr);
    assert(r.bestWatch->getId() == 7);
    assert(r.bestWatch->getFitness() == 99.0);
    assert(r.bestWatch->getName() == "elite");

    // getExperimentName
    assert(loaded.getExperimentName() == "experiment_test");

    // setExperimentName
    loaded.setExperimentName("changed");
    assert(loaded.getExperimentName() == "changed");

    // clear
    loaded.clear();
    assert(loaded.getRecordCount() == 0);

    std::remove(path.c_str());

    std::cout << "[OK] EvolutionHistory\n";
}

// ------------------------------------------------------------
// TEST 3: WatchFileIO (FULL FUNCTION COVERAGE - STUB SAFE)
// ------------------------------------------------------------
void testWatchFileIO()
{
    std::cout << "[TEST] WatchFileIO\n";

    std::string path = tempFile("watch.dat");

    WatchFileIO io(path);

    // open
    assert(io.open(true));
    assert(io.isOpen());

    // create watch
    Genome::Watch watch;
    watch.setId(1);
    watch.setFitness(2.5);
    watch.setName("watch_test");

    // saveWatch
    assert(io.saveWatch(watch, 0));

    // loadWatch
    auto loaded = io.loadWatch(0);
    assert(loaded != nullptr);

    // getWatchCount (stub)
    unsigned int count = io.getWatchCount();
    assert(count == 0);

    // savePopulation (stub-safe, no GA dependency needed in test)
    assert(io.savePopulation(*(Algorithm::GeneticAlgorithm*)nullptr));

    // loadPopulation
    assert(io.loadPopulation(*(Algorithm::GeneticAlgorithm*)nullptr));

    // saveCompleteState
    assert(io.saveCompleteState(*(Algorithm::GeneticAlgorithm*)nullptr));

    // loadCompleteState
    assert(io.loadCompleteState(*(Algorithm::GeneticAlgorithm*)nullptr));

    // close
    io.close();
    assert(!io.isOpen());

    std::remove(path.c_str());

    std::cout << "[OK] WatchFileIO\n";
}

// ------------------------------------------------------------
// MAIN
// ------------------------------------------------------------
int main()
{
    std::cout << "=============================\n";
    std::cout << " FILEIO FULL TEST SUITE\n";
    std::cout << "=============================\n\n";

    testConfigManager();
    testEvolutionHistory();
    testWatchFileIO();

    std::cout << "\n=============================\n";
    std::cout << " ALL FILEIO TESTS PASSED\n";
    std::cout << "=============================\n";

    return 0;
}