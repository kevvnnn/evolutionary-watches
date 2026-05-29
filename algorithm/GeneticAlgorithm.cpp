#include "GeneticAlgorithm.h"
#include <iostream> // TODO: temporary maybe

// TODO FIX are keywords to check for skipped work to be done

namespace WatchGA {
namespace Algorithm {

// Default constructor
GeneticAlgorithm::GeneticAlgorithm()
	: m_populationSize(100), // these are default values to pass in
	m_currentGeneration(0),
	m_mutationRate(0.05),
	m_crossoverRate(0.8),
	m_elitismCount(5)
{
	m_fitnessEvaluator = std::make_unique<Genome::FitnessEvaluator>();
	// TODO: Incoming default selection/crossover/mutation here too! 
}

// Constructor with parameters
GeneticAlgorithm::GeneticAlgorithm(unsigned int populationSize, double mutationRate, double crossoverRate,
                     unsigned int elitismCount,
                     std::unique_ptr<Core::ISelectionStrategy> selectionStrategy,
                     std::unique_ptr<Core::ICrossoverStrategy> crossoverStrategy,
                     std::unique_ptr<Core::IMutationStrategy> mutationStrategy,
                     std::unique_ptr<Genome::FitnessEvaluator> fitnessEvaluator)
	: m_populationSize(populationSize),
	m_currentGeneration(0), // Always initialize current population to 0
	m_mutationRate(mutationRate),
	m_crossoverRate(crossoverRate),
	m_elitismCount(elitismCount),

	// Because they are of unique_ptr, transfer ownership of the strategies via std::move!
	m_selectionStrategy(std::move(selectionStrategy)),
	m_crossoverStrategy(std::move(crossoverStrategy)),
	m_mutationStrategy(std::move(mutationStrategy)),
	m_fitnessEvaluator(std::move(fitnessEvaluator))
{
	// Initializer list has done all the initialization necessary
}

void GeneticAlgorithm::InitializePopulation() {
	// TODO: temp cout
	std::cout << "Initializing population of " << m_populationSize << "...\n";

	// Clear any existing data just in case this is called during a reset
    m_population.clear();
    m_fitnessScores.clear();
    
    // Pre-allocate memory for efficiency
    m_population.reserve(m_populationSize);
    m_fitnessScores.reserve(m_populationSize);

	// Create watches for the population according to the population size
	for (unsigned int i = 0; i < m_populationSize; i++) {
		std::string watchName = "Random_Watch_" + std::to_string(i + 1);
        
        // This creates both the Watch on the Heap and the Reference Counter in one go.
        // It is significantly safer and faster than using 'new'.
        auto newWatch = std::make_shared<Genome::Watch>(watchName);
		
		m_population.push_back(newWatch);
	}

	std::cout << "Initialization complete. " << m_population.size() << " watches created.\n";
}

void GeneticAlgorithm::EvaluatePopulation() {
	// TODO: temp cout
	std::cout << "Evaluating generation " << m_currentGeneration << "...\n";

	m_fitnessScores.clear();
	m_bestFitness = -1.0; // reset best tracker
	// m_worstFitness = 101.0; // reset worst tracker // TODO: test this!
	double totalFitness = 0.0;

	for (const auto& watch : m_population) {
        // We pass the watch by dereferencing the shared_ptr (*watch)
        // because Evaluate() expects a const Watch& reference.
        double score = m_fitnessEvaluator->Evaluate(*watch);
        
        // Store the score in our parallel array
        m_fitnessScores.push_back(score);
        
        // Update the watch's internal record
        watch->SetFitnessScore(score);
        
        totalFitness += score;
        
        // Track the best watch of this generation
        if (score > m_bestFitness || m_bestWatch == nullptr) {
            m_bestFitness = score;
            m_bestWatch = watch; // shared_ptr copy! The reference count goes up safely.
        }
    }

	m_averageFitness = totalFitness / m_populationSize;
    
    std::cout << "Evaluation complete. Best Fitness: " << m_bestFitness 
              << " | Avg Fitness: " << m_averageFitness << "\n";
}

// ---------------------------------------------------------
// GETTERS (Required for testing)
// ---------------------------------------------------------
const std::vector<std::shared_ptr<Genome::Watch>>& GeneticAlgorithm::GetPopulation() const {
    return m_population;
}

const std::shared_ptr<Genome::Watch>& GeneticAlgorithm::GetBestWatch() const {
    return m_bestWatch;
}

// ---------------------------------------------------------
// PUBLIC WORKFLOW METHODS
// ---------------------------------------------------------
void GeneticAlgorithm::Reset() {
    std::cout << "Resetting Genetic Algorithm...\n";
    
    // 1. Reset generation counter
    m_currentGeneration = 0;
    
    // 2. Safely call the private helper methods in the exact required order
    InitializePopulation();
    EvaluatePopulation();
    
    // 3. Reset any history or UI tracking variables here if necessary
    
    std::cout << "GA is ready for Generation 1.\n";
}

} // namespace Algorithm
} // namespace WatchGA