#ifndef WATCH_GA_ALGORITHM_CROSSOVERSTRATEGY_H
#define WATCH_GA_ALGORITHM_CROSSOVERSTRATEGY_H

#include "../core/ICrossoverStrategy.h"

namespace WatchGA {
namespace Algorithm {

/**
 * @brief ONE-POINT crossover strategy
 * 
 * Splits the component list of both parents at a random point.
 * Child 1 gets the first part from parent 1 and the second part from parent 2.
 * Child 2 gets the first part from parent 2 and the second part from parent 1.
 * 
 * OWNER: Kevin
 */
class OnePointCrossover : public Core::ICrossoverStrategy {
public:
    std::pair<std::shared_ptr<Genome::Watch>, std::shared_ptr<Genome::Watch>> crossover(
        const std::shared_ptr<Genome::Watch>& parent1,
        const std::shared_ptr<Genome::Watch>& parent2
    ) const override;

    std::string getName() const override;
};

/**
 * @brief UNIFORM crossover strategy
 * 
 * For each component, randomly choose whether to take it from parent 1 or parent 2.
 * Each component has a 50% chance to come from either parent.
 * 
 * OWNER: Kevin
 */
class UniformCrossover : public Core::ICrossoverStrategy {
public:
    std::pair<std::shared_ptr<Genome::Watch>, std::shared_ptr<Genome::Watch>> crossover(
        const std::shared_ptr<Genome::Watch>& parent1,
        const std::shared_ptr<Genome::Watch>& parent2
    ) const override;

    std::string getName() const override;
};

} // namespace Algorithm
} // namespace WatchGA

#endif // WATCH_GA_ALGORITHM_CROSSOVERSTRATEGY_H