#ifndef WATCH_GA_ALGORITHM_GENETICALGORITHM_H
#define WATCH_GA_ALGORITHM_GENETICALGORITHM_H

#include "../core/ISelectionStrategy.h"
#include "../core/ICrossoverStrategy.h"
#include "../core/IMutationStrategy.h"
#include "../genome/FitnessEvaluator.h"
#include "../genome/Watch.h"
#include <vector>
#include <memory>
#include <string>

namespace WatchGA {
namespace Algorithm {

/**
 * @brief The MAIN GENETIC ALGORITHM controller
 * 
 * This class manages the entire evolution process:
 * 1. Creates an initial population of random watches
 * 2. Evaluates how good each watch is
 * 3. Selects parents to reproduce
 * 4. Creates offspring through crossover
 * 5. Mutates the offspring
 * 6. Replaces the old population with the new one
 * 7. Repeats for many generations
 * 
 * OWNER: Person B
 */
class GeneticAlgorithm {
private:
    // ------------------------------
    // CONFIGURATION
    // ------------------------------
    unsigned int m_populationSize;    // Number of watches in each generation
    unsigned int m_currentGeneration; // Current generation number
    double m_mutationRate;            // Probability of mutation (0.0 to 1.0)
    double m_crossoverRate;           // Probability of crossover (0.0 to 1.0)
    unsigned int m_elitismCount;      // Number of best watches to keep each generation

    // ------------------------------
    // POPULATION
    // ------------------------------
    std::vector<std::shared_ptr<Genome::Watch>> m_population; // All watches in current generation
    std::vector<double> m_fitnessScores; // Fitness score for each watch

    // ------------------------------
    // STRATEGIES (Pluggable algorithms)
    // ------------------------------
    std::unique_ptr<Core::ISelectionStrategy> m_selectionStrategy;
    std::unique_ptr<Core::ICrossoverStrategy> m_crossoverStrategy;
    std::unique_ptr<Core::IMutationStrategy> m_mutationStrategy;

    // ------------------------------
    // FITNESS EVALUATOR
    // ------------------------------
    std::unique_ptr<Genome::FitnessEvaluator> m_fitnessEvaluator;

    // ------------------------------
    // STATISTICS
    // ------------------------------
    double m_bestFitness;             // Best fitness score in current generation
    double m_averageFitness;          // Average fitness score in current generation
    double m_worstFitness;            // Worst fitness score in current generation
    std::shared_ptr<Genome::Watch> m_bestWatch; // Best watch in current generation

    // ------------------------------
    // PRIVATE HELPER METHODS
    // ------------------------------
    
    /**
     * @brief Creates an initial population of random watches
     */
    void initializePopulation();
    
    /**
     * @brief Evaluates the fitness of all watches in the population
     */
    void evaluatePopulation();
    
    /**
     * @brief Updates the statistics (best, average, worst fitness)
     */
    void updateStatistics();
    
    /**
     * @brief Performs elitism (keeps the best watches from the previous generation)
     * @param newPopulation The new population to add the elite watches to
     */
    void performElitism(std::vector<std::shared_ptr<Genome::Watch>>& newPopulation);

public:
    /**
     * @brief Default constructor
     * Uses default parameters and strategies
     */
    GeneticAlgorithm();
    
    /**
     * @brief Constructor with custom parameters and strategies
     */
    GeneticAlgorithm(unsigned int populationSize, double mutationRate, double crossoverRate,
                     unsigned int elitismCount,
                     std::unique_ptr<Core::ISelectionStrategy> selectionStrategy,
                     std::unique_ptr<Core::ICrossoverStrategy> crossoverStrategy,
                     std::unique_ptr<Core::IMutationStrategy> mutationStrategy,
                     std::unique_ptr<Genome::FitnessEvaluator> fitnessEvaluator);
    
    ~GeneticAlgorithm() = default;

    // ------------------------------
    // CONFIGURATION GETTERS & SETTERS
    // ------------------------------
    
    unsigned int getPopulationSize() const;
    void setPopulationSize(unsigned int size);
    unsigned int getCurrentGeneration() const;
    double getMutationRate() const;
    void setMutationRate(double rate);
    double getCrossoverRate() const;
    void setCrossoverRate(double rate);
    unsigned int getElitismCount() const;
    void setElitismCount(unsigned int count);

    // ------------------------------
    // STRATEGY SETTERS
    // ------------------------------
    // These allow us to change the algorithm behavior at runtime without changing code
    
    void setSelectionStrategy(std::unique_ptr<Core::ISelectionStrategy> strategy);
    void setCrossoverStrategy(std::unique_ptr<Core::ICrossoverStrategy> strategy);
    void setMutationStrategy(std::unique_ptr<Core::IMutationStrategy> strategy);

    // ------------------------------
    // FITNESS EVALUATOR SETTER
    // ------------------------------
    
    void setFitnessEvaluator(std::unique_ptr<Genome::FitnessEvaluator> evaluator);

    // ------------------------------
    // STATISTICS GETTERS
    // ------------------------------
    
    double getBestFitness() const;
    double getAverageFitness() const;
    double getWorstFitness() const;
    const std::shared_ptr<Genome::Watch>& getBestWatch() const;
    const std::vector<std::shared_ptr<Genome::Watch>>& getPopulation() const;

    // ------------------------------
    // MAIN EVOLUTION METHODS
    // ------------------------------
    
    /**
     * @brief Runs ONE generation of evolution
     */
    void runGeneration();
    
    /**
     * @brief Runs MULTIPLE generations of evolution
     * @param numGenerations Number of generations to run
     */
    void runGenerations(unsigned int numGenerations);
    
    /**
     * @brief Resets the algorithm to its initial state
     * Clears the population and statistics, and starts over.
     */
    void reset();
};

} // namespace Algorithm
} // namespace WatchGA

#endif // WATCH_GA_ALGORITHM_GENETICALGORITHM_H