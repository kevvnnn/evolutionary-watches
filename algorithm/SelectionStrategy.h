#ifndef WATCH_GA_ALGORITHM_SELECTIONSTRATEGY_H
#define WATCH_GA_ALGORITHM_SELECTIONSTRATEGY_H

#include "../core/ISelectionStrategy.h"

namespace WatchGA {
namespace Algorithm {

/**
 * @brief ROULETTE WHEEL selection strategy
 * 
 * Selects parents with probability proportional to their fitness.
 * Think of it like a roulette wheel where each watch gets a slice
 * proportional to its fitness score.
 * 
 * Pros: Simple to implement
 * Cons: Can lead to premature convergence if one watch is much better than others
 * 
 * OWNER: Kevin
 */
class RouletteWheelSelection : public Core::ISelectionStrategy {
public:
    std::vector<std::shared_ptr<Genome::Watch>> select(
        const std::vector<std::shared_ptr<Genome::Watch>>& population,
        const std::vector<double>& fitnessScores,
        unsigned int numParents
    ) const override;

    std::string getName() const override;
};

/**
 * @brief TOURNAMENT selection strategy
 * 
 * Randomly selects k watches from the population, then picks the best one.
 * Repeat until we have enough parents.
 * 
 * Pros: More robust than roulette wheel, less likely to converge prematurely
 * Cons: Slightly more complex
 * 
 * OWNER: Kevin
 */
class TournamentSelection : public Core::ISelectionStrategy {
private:
    unsigned int m_tournamentSize; // Number of watches in each tournament (default: 3)

public:
    explicit TournamentSelection(unsigned int tournamentSize = 3);

    unsigned int GetTournamentSize() const;
    void SetTournamentSize(unsigned int size);

    std::vector<std::shared_ptr<Genome::Watch>> select(
        const std::vector<std::shared_ptr<Genome::Watch>>& population,
        const std::vector<double>& fitnessScores,
        unsigned int numParents
    ) const override;

    std::string getName() const override;
};

} // namespace Algorithm
} // namespace WatchGA

#endif // WATCH_GA_ALGORITHM_SELECTIONSTRATEGY_H