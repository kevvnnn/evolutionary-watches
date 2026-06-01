#ifndef WATCH_GA_CORE_IMUTATIONSTRATEGY_H
#define WATCH_GA_CORE_IMUTATIONSTRATEGY_H

#include <memory>

// Forward declaration
namespace WatchGA { namespace Genome { class Watch; } }

namespace WatchGA {
namespace Core {

/**
 * @brief INTERFACE for all mutation strategies
 * 
 * Mutation introduces random changes into the genome.
 * This prevents the algorithm from getting stuck in "local optima"
 * (good but not great solutions).
 * 
 * OWNER: Person B (Algorithm module)
 */
class IMutationStrategy {
public:
    virtual ~IMutationStrategy() = default;

    /**
     * @brief Applies random mutations to a watch genome
     * 
     * @param watch The watch to mutate
     * @param mutationRate Probability of mutation for each gene (0.0 to 1.0)
     * 
     * Example: A mutation rate of 0.01 means each gene has a 1% chance to mutate.
     */
    virtual void mutate(
        std::shared_ptr<Genome::Watch>& watch,
        double mutationRate
    ) const = 0;

    /**
     * @brief Gets the name of this mutation strategy
     * @return The strategy name (e.g., "Swap", "Parameter", "Add/Remove")
     */
    virtual std::string getName() const = 0;
};

} // namespace Core
} // namespace WatchGA

#endif // WATCH_GA_CORE_IMUTATIONSTRATEGY_H