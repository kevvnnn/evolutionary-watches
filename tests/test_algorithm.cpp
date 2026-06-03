#include "../algorithm/GeneticAlgorithm.h"
#include "../algorithm/SelectionStrategy.h"
#include "../algorithm/CrossoverStrategy.h"
#include "../genome/Watch.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <memory>
#include <vector>
#include <utility> // For std::pair
#include <iomanip>

#include "../algorithm/MutationStrategy.h" 
#include "../genome/components/BalanceWheel.h"
#include "../genome/components/Gear.h"
#include "../genome/components/Hand.h"
#include "../genome/components/Spring.h"
#include "../genome/components/Jewel.h"
#include <iomanip>

using namespace WatchGA::Algorithm;
using namespace WatchGA::Genome;

// TODO: Overhaul this to include mutation once it's finished, and make it to show
// some generations to show how the watch develop over time

// ==============================================================================
// 1. CORE ALGORITHM TESTS
// ==============================================================================

void testDefaultConstructor() {
    std::cout << "Running testDefaultConstructor... ";
    GeneticAlgorithm ga;
    
    assert(ga.getPopulationSize() == 100);
    assert(ga.getCurrentGeneration() == 0);
    assert(ga.getMutationRate() == 0.05);
    
    std::cout << "PASSED.\n";
}

void testConfigurationValidation() {
    std::cout << "Running testConfigurationValidation... ";
    GeneticAlgorithm ga;

    ga.setPopulationSize(0);
    assert(ga.getPopulationSize() == 1); 
    ga.setMutationRate(1.5);
    assert(ga.getMutationRate() == 1.0); 

    std::cout << "PASSED.\n";
}

void testPhysicalInitialization() {
    std::cout << "Running testPhysicalInitialization... ";
    GeneticAlgorithm ga;
    
    ga.setPopulationSize(50);
    ga.reset(); // Changed to lowercase
    
    auto population = ga.getPopulation();
    assert(population.size() == 50);
    
    bool isDiverse = false;
    for (size_t i = 1; i < population.size(); ++i) {
        if (population[i]->getComponentCount() != population[0]->getComponentCount()) {
            isDiverse = true;
            break; 
        }
    }
    
    assert(isDiverse && "Population lacks genetic diversity! randomize() might be failing.");
    std::cout << "PASSED.\n";
}

// ==============================================================================
// 2. STRATEGY TESTS (Selection & Crossover)
// ==============================================================================

void testSelectionStrategies() {
    std::cout << "Running testSelectionStrategies... ";

    // 1. Setup a dummy population of 4 watches
    std::vector<std::shared_ptr<Watch>> pop;
    std::vector<double> scores = {10.0, 50.0, 90.0, 0.0};
    
    for(int i = 0; i < 4; i++) {
        auto w = std::make_shared<Watch>("W" + std::to_string(i));
        w->setFitnessScore(scores[i]);
        pop.push_back(w);
    }

    // 2. Test Roulette Wheel Selection
    RouletteWheelSelection roulette;
    auto rouletteParents = roulette.select(pop, scores, 2);
    assert(rouletteParents.size() == 2 && "Roulette did not return the correct number of parents!");

    // 3. Test Tournament Selection
    TournamentSelection tournament(3); // Tournament size of 3
    auto tournamentParents = tournament.select(pop, scores, 2);
    assert(tournamentParents.size() == 2 && "Tournament did not return the correct number of parents!");

    // 4. Test Zero-Fitness Edge Case (Ensures fallback RNG works in Roulette)
    std::vector<double> zeroScores = {0.0, 0.0, 0.0, 0.0};
    auto fallbackParents = roulette.select(pop, zeroScores, 2);
    assert(fallbackParents.size() == 2 && "Roulette crashed when handling an all-zero fitness pool!");

    std::cout << "PASSED.\n";
}

void testCrossoverStrategies() {
    std::cout << "Running testCrossoverStrategies... ";

    // 1. Create and physically randomize two parents
    auto parent1 = std::make_shared<Watch>("Parent_A");
    parent1->randomize();
    
    auto parent2 = std::make_shared<Watch>("Parent_B");
    parent2->randomize();
    
    size_t totalParentParts = parent1->getComponentCount() + parent2->getComponentCount();

    // 2. Test One-Point Crossover (C++14 Compliant Pair Handling)
    OnePointCrossover onePoint;
    auto onePointResult = onePoint.crossover(parent1, parent2);
    auto c1_one = onePointResult.first;
    auto c2_one = onePointResult.second;
    
    assert(c1_one != nullptr && c2_one != nullptr);
    assert(c1_one->getComponentCount() > 0 && "Child 1 is empty after One-Point Crossover!");
    
    // Conservation of Mass Check
    size_t totalOnePointChildParts = c1_one->getComponentCount() + c2_one->getComponentCount();
    assert(totalParentParts == totalOnePointChildParts && "One-Point Crossover destroyed or duplicated components!");

    // 3. Test Uniform Crossover (C++14 Compliant Pair Handling)
    UniformCrossover uniform;
    auto uniformResult = uniform.crossover(parent1, parent2);
    auto c1_uni = uniformResult.first;
    auto c2_uni = uniformResult.second;
    
    assert(c1_uni != nullptr && c2_uni != nullptr);
    
    // Conservation of Mass Check
    size_t totalUniformChildParts = c1_uni->getComponentCount() + c2_uni->getComponentCount();
    assert(totalParentParts == totalUniformChildParts && "Uniform Crossover destroyed or duplicated components!");

    std::cout << "PASSED.\n";
}


// ==============================================================================
// 3. VISUAL SHOWCASE
// ==============================================================================

void displayPopulationSamples() {
    std::cout << "\n============================================================\n";
    std::cout << "   GENERATION 0 DEEP DIVE: PROVING GENETIC DIVERSITY\n";
    std::cout << "============================================================\n\n";
    
    GeneticAlgorithm ga;
    ga.setPopulationSize(100);
    ga.reset(); // This automatically triggers population evaluation 
    
    auto pop = ga.getPopulation();
    std::vector<size_t> sampleIndices = {0, 49, 99};
    
    for (size_t idx : sampleIndices) {
        auto watch = pop[idx];
        std::cout << ">>> WATCH #" << idx + 1 << " | Name: " << watch->getObjectName() << "\n";
        
        // =========================================================
        // ADDED: EXPLICIT FITNESS SCORE DISPLAY
        // =========================================================
        std::cout << "    [FITNESS] Raw Evaluated Score: " << std::fixed << std::setprecision(6) 
                  << watch->getFitnessScore() << "\n";
        std::cout << "    Total Components: " << watch->getComponentCount() << "\n";
        
        int gearCount = 0;
        
        for (const auto& comp : watch->getAllComponents()) {
            // 1. Inspect the Balance Wheel
            if (auto* bw = dynamic_cast<const Components::BalanceWheel*>(comp.get())) {
                std::cout << "    [-] Balance Wheel -> Isochronism: " << std::fixed << std::setprecision(2) << bw->getIsochronism()
                          << " | Amplitude: " << bw->getAmplitude() << " deg\n";
            }
            // 2. Inspect the Springs
            else if (auto* spring = dynamic_cast<const Components::Spring*>(comp.get())) {
                std::string type = (spring->getType() == Components::Spring::SpringType::MAINSPRING) ? "Mainspring" : "Hairspring";
                std::cout << "    [-] " << type << "  -> Elasticity: " << spring->getElasticity()
                          << " | Fatigue Res: " << spring->getFatigueResistance() << "\n";
            }
            // 3. Inspect the Hands
            else if (auto* hand = dynamic_cast<const Components::Hand*>(comp.get())) {
                std::string type = (hand->getType() == Components::Hand::HandType::HOUR) ? "Hour Hand " : "Minute Hand";
                std::cout << "    [-] " << type << "  -> Balance: " << hand->getBalance()
                          << " | Length: " << hand->getLength() << "mm\n";
            }
            // 4. Inspect the Gears
            else if (auto* gear = dynamic_cast<const Components::Gear*>(comp.get())) {
                gearCount++;
                if (gearCount == 1) {
                    std::cout << "    [-] First Gear   -> Teeth: " << gear->getToothCount()
                              << " | Quality: " << gear->getMeshingQuality() << "\n";
                }
            }
        }
        std::cout << "    [-] Total Gears  -> " << gearCount << "\n\n";
    }
}

void runEvolutionTimeLapse() {
    std::cout << "\n============================================================\n";
    std::cout << "   EVOLUTIONARY TIME-LAPSE (0 -> 50 GENERATIONS)\n";
    std::cout << "============================================================\n";
    
    GeneticAlgorithm ga;
    
    // 1. Configure the GA for a successful evolution run
    ga.setPopulationSize(200);      // Large population for genetic diversity
    ga.setMutationRate(0.20);       // 20% mutation chance
    ga.setCrossoverRate(0.85);      // 85% mating chance
    ga.setElitismCount(5);          // Save the top 5 perfect watches from death

    // 2. Inject your strategies
    ga.setSelectionStrategy(std::make_unique<TournamentSelection>(5));
    ga.setCrossoverStrategy(std::make_unique<UniformCrossover>());
    ga.setMutationStrategy(std::make_unique<ParameterMutation>(0.15)); // 15% stat nudge
    
    ga.reset(); 

    // Helper lambda to print a watch's stats beautifully
    auto printAlphaWatch = [](const std::shared_ptr<Watch>& watch, int genNum) {
        std::cout << "\n>>> GENERATION " << genNum << " ALPHA WATCH <<<\n";
        std::cout << "    Fitness Score : " << std::fixed << std::setprecision(5) << watch->getFitnessScore() << "\n";
        std::cout << "    Total Parts   : " << watch->getComponentCount() << "\n";
        
        for (const auto& comp : watch->getAllComponents()) {
            if (auto* bw = dynamic_cast<const Components::BalanceWheel*>(comp.get())) {
                std::cout << "    [CRITICAL] Balance Wheel -> Isochronism: " << bw->getIsochronism() 
                          << " | Amplitude: " << bw->getAmplitude() << "\n";
            }
            else if (auto* spring = dynamic_cast<const Components::Spring*>(comp.get())) {
                if(spring->getType() == Components::Spring::SpringType::HAIRSPRING) {
                    std::cout << "    [CRITICAL] Hairspring    -> Elasticity : " << spring->getElasticity() << "\n";
                }
            }
        }
        std::cout << "------------------------------------------------------------\n";
    };

    // Print Gen 0 Alpha
    printAlphaWatch(ga.getBestWatch(), ga.getCurrentGeneration());

    // Evolve to Gen 25
    std::cout << "\n[INFO] Evolving 25 Generations...\n";
    for(int i = 0; i < 25; i++) ga.runGeneration();
    printAlphaWatch(ga.getBestWatch(), ga.getCurrentGeneration());

    // Evolve to Gen 50
    std::cout << "\n[INFO] Evolving to 50 Generations...\n";
    for(int i = 0; i < 25; i++) ga.runGeneration();
    printAlphaWatch(ga.getBestWatch(), ga.getCurrentGeneration());
}

// ==============================================================================
// 4. MAIN EXECUTION
// ==============================================================================

int main() {
    std::cout << "==========================================\n";
    std::cout << " STARTING GENETIC ALGORITHM TEST SUITE\n";
    std::cout << "==========================================\n\n";

    // Run your standard unit tests first
    testDefaultConstructor();
    testConfigurationValidation();
    testPhysicalInitialization();
    testSelectionStrategies();
    testCrossoverStrategies();

    // Run the grand finale
    runEvolutionTimeLapse();

    std::cout << "\n==========================================\n";
    std::cout << " SUCCESS: ALL TESTS AND EVOLUTION PASSED!\n";
    std::cout << "==========================================\n";

    return 0;
}