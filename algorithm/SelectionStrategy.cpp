#include "SelectionStrategy.h"
#include <random>
#include <numeric>
#include <stdexcept>
#include <algorithm>

namespace WatchGA {
namespace Algorithm {

// ==========================================
// ROULETTE WHEEL SELECTION
// ==========================================

std::vector<std::shared_ptr<Genome::Watch>> RouletteWheelSelection::select(
    const std::vector<std::shared_ptr<Genome::Watch>>& population,
    const std::vector<double>& fitnessScores,
    unsigned int numParents
) const {
    if (population.empty() || population.size() != fitnessScores.size()) {
        throw std::invalid_argument("Population and fitness scores must be valid and matching sizes.");
    }

    std::vector<std::shared_ptr<Genome::Watch>> parents;
    parents.reserve(numParents);

    // Calculate total fitness for the wheel
    double totalFitness = std::accumulate(fitnessScores.begin(), fitnessScores.end(), 0.0);

    // Setup RNG
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Edge Case: If all watches have 0 fitness, fall back to pure random selection
    if (totalFitness <= 0.0) {
        std::uniform_int_distribution<size_t> dist(0, population.size() - 1);
        for (unsigned int i = 0; i < numParents; ++i) {
            parents.push_back(population[dist(gen)]);
        }
        return parents;
    }

    std::uniform_real_distribution<double> dist(0.0, totalFitness);

    // Spin the wheel 'numParents' times
    for (unsigned int i = 0; i < numParents; ++i) {
        double randomValue = dist(gen);
        double currentSum = 0.0;

        for (size_t j = 0; j < population.size(); ++j) {
            currentSum += fitnessScores[j];
            if (currentSum >= randomValue) {
                parents.push_back(population[j]);
                break;
            }
        }
    }

    return parents;
}

std::string RouletteWheelSelection::getName() const {
    return "Roulette Wheel";
}

// ==========================================
// TOURNAMENT SELECTION
// ==========================================

TournamentSelection::TournamentSelection(unsigned int tournamentSize) 
    : m_tournamentSize(tournamentSize) {}

unsigned int TournamentSelection::GetTournamentSize() const { return m_tournamentSize; }
void TournamentSelection::SetTournamentSize(unsigned int size) { m_tournamentSize = size; }

std::vector<std::shared_ptr<Genome::Watch>> TournamentSelection::select(
    const std::vector<std::shared_ptr<Genome::Watch>>& population,
    const std::vector<double>& fitnessScores,
    unsigned int numParents
) const {
    if (population.empty() || population.size() != fitnessScores.size()) {
        throw std::invalid_argument("Population and fitness scores must be valid and matching sizes.");
    }
    
    // Prevent tournament size from exceeding the actual population size
    unsigned int actualTournamentSize = std::min(m_tournamentSize, static_cast<unsigned int>(population.size()));

    std::vector<std::shared_ptr<Genome::Watch>> parents;
    parents.reserve(numParents);

    // Setup RNG
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, population.size() - 1);

    for (unsigned int i = 0; i < numParents; ++i) {
        // Pick the first combatant
        size_t bestIndex = dist(gen); 
        double bestFitness = fitnessScores[bestIndex];

        // Challenge with other random watches
        for (unsigned int j = 1; j < actualTournamentSize; ++j) {
            size_t contenderIndex = dist(gen);
            if (fitnessScores[contenderIndex] > bestFitness) {
                bestIndex = contenderIndex;
                bestFitness = fitnessScores[contenderIndex];
            }
        }
        
        // The winner gets to reproduce
        parents.push_back(population[bestIndex]);
    }

    return parents;
}

std::string TournamentSelection::getName() const {
    return "Tournament";
}

} // namespace Algorithm
} // namespace WatchGA