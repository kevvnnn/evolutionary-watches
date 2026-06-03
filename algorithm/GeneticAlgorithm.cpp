#include "GeneticAlgorithm.h"
#include <iostream> // TODO: temporary maybe
#include <random>
#include <algorithm>

// TODO FIX are keywords to check for skipped work to be done

namespace WatchGA {
namespace Algorithm {

// ------------------------------
// CONSTRUCTORS
// ------------------------------
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

// ------------------------------
// PRIVATE HELPER METHODS
// ------------------------------
void GeneticAlgorithm::initializePopulation() {
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

        // Trigger the random physical generation
        newWatch->randomize();
		
		m_population.push_back(newWatch);
	}

	std::cout << "Initialization complete. " << m_population.size() << " watches created.\n";
}

void GeneticAlgorithm::evaluatePopulation() {
	// TODO: temp cout
	std::cout << "Evaluating generation " << m_currentGeneration << "...\n";

	m_fitnessScores.clear();
	m_bestFitness = -1.0; // reset best tracker
	m_worstFitness = 101.0; // reset worst tracker // TODO: test this!
	double totalFitness = 0.0;

	for (const auto& watch : m_population) {
        // We pass the watch by dereferencing the shared_ptr (*watch)
        // because Evaluate() expects a const Watch& reference.
        double score = m_fitnessEvaluator->evaluate(*watch);
        
        // Store the score in our parallel array
        m_fitnessScores.push_back(score);
        
        // Update the watch's internal record
        watch->setFitnessScore(score);
        
        totalFitness += score;
        
        // Track the best watch of this generation
        if (score > m_bestFitness || m_bestWatch == nullptr) {
            m_bestFitness = score;
            m_bestWatch = watch; // shared_ptr copy! The reference count goes up safely.
        }
        // Track the worst watch of this generation
        else if (score < m_worstFitness) {
            m_worstFitness = score;
        }
    }

	m_averageFitness = totalFitness / m_populationSize;
    
    std::cout << "Evaluation complete. Best Fitness: " << m_bestFitness 
              << " | Avg Fitness: " << m_averageFitness << "\n";
}

// ---------------------------------------------------------
// CONFIGURATION GETTERS & SETTERS
// ---------------------------------------------------------
unsigned int GeneticAlgorithm::getPopulationSize() const {
    return m_populationSize;
}

void GeneticAlgorithm::setPopulationSize(unsigned int size) {
    // Prevent a population size of 0, which would cause divide-by-zero 
    // errors when calculating average fitness later.
    m_populationSize = (size > 0) ? size : 1;
}

unsigned int GeneticAlgorithm::getCurrentGeneration() const {
    return m_currentGeneration;
}

double GeneticAlgorithm::getMutationRate() const {
    return m_mutationRate;
}

void GeneticAlgorithm::setMutationRate(double rate) {
    // Clamp between 0.0 (0%) and 1.0 (100%)
    if (rate < 0.0) m_mutationRate = 0.0;
    else if (rate > 1.0) m_mutationRate = 1.0;
    else m_mutationRate = rate;
}

double GeneticAlgorithm::getCrossoverRate() const {
    return m_crossoverRate;
}

void GeneticAlgorithm::setCrossoverRate(double rate) {
    // Clamp between 0.0 (0%) and 1.0 (100%)
    if (rate < 0.0) m_crossoverRate = 0.0;
    else if (rate > 1.0) m_crossoverRate = 1.0;
    else m_crossoverRate = rate;
}

unsigned int GeneticAlgorithm::getElitismCount() const {
    return m_elitismCount;
}

void GeneticAlgorithm::setElitismCount(unsigned int count) {
    m_elitismCount = count;
}

void GeneticAlgorithm::updateStatistics() {
    // Note: In the current evaluatePopulation() code, we are already calculating 
    // the best, worst, and average fitness, as well as tracking the best watch.
}

void GeneticAlgorithm::performElitism(std::vector<std::shared_ptr<Genome::Watch>>& newPopulation) {
    if (m_elitismCount == 0) return;

    // Create a copy of the population to sort (so we don't mess up the original order)
    std::vector<std::shared_ptr<Genome::Watch>> sortedPop = m_population;

    // Sort descending (highest fitness score at index 0)
    std::sort(sortedPop.begin(), sortedPop.end(),
        [](const std::shared_ptr<Genome::Watch>& a, const std::shared_ptr<Genome::Watch>& b) {
            return a->getFitnessScore() > b->getFitnessScore();
        }
    );

    // Safely copy the elite watches
    unsigned int actualElites = std::min(m_elitismCount, static_cast<unsigned int>(sortedPop.size()));
    for (unsigned int i = 0; i < actualElites; ++i) {
        // CRITICAL: We MUST clone the elite watches. 
        // If we just pass the shared_ptr, it might accidentally get mutated later!
        newPopulation.push_back(std::shared_ptr<Genome::Watch>(sortedPop[i]->clone()));
    }
}

void GeneticAlgorithm::runGeneration() {
    // Safety check
    if (m_population.empty()) {
        std::cout << "[ERROR] Cannot run generation: Population is empty. Did you call reset()?\n";
        return;
    }

    // Prepare the next generation's container
    std::vector<std::shared_ptr<Genome::Watch>> newPopulation;
    newPopulation.reserve(m_populationSize);

    // 1. Save the best watches from the current generation (Elitism)
    performElitism(newPopulation);

    // Setup RNG for crossover probability
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // 2. Fill the rest of the new population
    while (newPopulation.size() < m_populationSize) {
        
        // A. Select 2 Parents
        auto parents = m_selectionStrategy->select(m_population, m_fitnessScores, 2);
        
        std::shared_ptr<Genome::Watch> child1;
        std::shared_ptr<Genome::Watch> child2;

        // B. Crossover (based on crossover rate)
        if (dist(gen) <= m_crossoverRate && parents.size() >= 2) {
            auto children = m_crossoverStrategy->crossover(parents[0], parents[1]);
            child1 = children.first;
            child2 = children.second;
        } else {
            // If crossover fails, the children are just clones of the parents
            child1 = std::shared_ptr<Genome::Watch>(parents[0]->clone());
            if (parents.size() > 1) {
                child2 = std::shared_ptr<Genome::Watch>(parents[1]->clone());
            } else {
                child2 = std::shared_ptr<Genome::Watch>(parents[0]->clone()); // Fallback
            }
        }

        // C. Mutate Child 1 and add to population
        m_mutationStrategy->mutate(child1, m_mutationRate);
        newPopulation.push_back(child1);

        // D. Mutate Child 2 and add to population (if we have space)
        if (newPopulation.size() < m_populationSize) {
            m_mutationStrategy->mutate(child2, m_mutationRate);
            newPopulation.push_back(child2);
        }
    }

    // 3. Overwrite the old generation with the newly evolved one
    m_population = std::move(newPopulation);
    m_currentGeneration++;

    // 4. Score the new generation
    evaluatePopulation();
    updateStatistics();
}

void GeneticAlgorithm::runGenerations(unsigned int numGenerations) {
    for (unsigned int i = 0; i < numGenerations; ++i) {
        runGeneration();
    }
}

// ---------------------------------------------------------
// STATISTIC GETTERS
// ---------------------------------------------------------
double GeneticAlgorithm::getBestFitness() const {
    return m_bestFitness;
}

double GeneticAlgorithm::getAverageFitness() const {
    return m_averageFitness;
}

double GeneticAlgorithm::getWorstFitness() const {
    return m_worstFitness;
}

const std::vector<std::shared_ptr<Genome::Watch>>& GeneticAlgorithm::getPopulation() const {
    return m_population;
}

const std::shared_ptr<Genome::Watch>& GeneticAlgorithm::getBestWatch() const {
    return m_bestWatch;
}

// ---------------------------------------------------------
// STRATEGY SETTERS
// ---------------------------------------------------------
void GeneticAlgorithm::setSelectionStrategy(std::unique_ptr<Core::ISelectionStrategy> strategy) {
    // Only accept the new strategy if it is a valid pointer.
    // This prevents accidental null-pointer crashes if the UI passes empty data.
    if (strategy) {
        // We must use std::move again to transfer ownership from the 
        // local parameter into the class member variable.
        m_selectionStrategy = std::move(strategy);
    }
}

void GeneticAlgorithm::setCrossoverStrategy(std::unique_ptr<Core::ICrossoverStrategy> strategy) {
    if (strategy) {
        m_crossoverStrategy = std::move(strategy);
    }
}

void GeneticAlgorithm::setMutationStrategy(std::unique_ptr<Core::IMutationStrategy> strategy) {
    if (strategy) {
        m_mutationStrategy = std::move(strategy);
    }
}

// ---------------------------------------------------------
// FITNESS EVALUATOR SETTER
// ---------------------------------------------------------
void GeneticAlgorithm::setFitnessEvaluator(std::unique_ptr<Genome::FitnessEvaluator> evaluator) {
    if (evaluator) {
        m_fitnessEvaluator = std::move(evaluator);
    }
}

// ---------------------------------------------------------
// PUBLIC WORKFLOW METHODS
// ---------------------------------------------------------
void GeneticAlgorithm::reset() {
    std::cout << "Resetting Genetic Algorithm...\n";
    
    // 1. reset generation counter
    m_currentGeneration = 0;
    
    // 2. Safely call the private helper methods in the exact required order
    initializePopulation();
    evaluatePopulation();
    
    // 3. reset any history or UI tracking variables here if necessary
    
    std::cout << "GA is ready for Generation 1.\n";
}

} // namespace Algorithm
} // namespace WatchGA