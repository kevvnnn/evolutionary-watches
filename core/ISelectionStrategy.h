#ifndef WATCH_GA_CORE_ISELECTIONSTRATEGY_H
#define WATCH_GA_CORE_ISELECTIONSTRATEGY_H

#include <vector>
#include <memory>

// Forward declaration: We don't need the full Watch definition here,
// just to know that it exists. This speeds up compilation.
namespace WatchGA { namespace Genome { class Watch; } }

namespace WatchGA {
namespace Core {

/**
 * @brief INTERFACE for all parent selection strategies
 * 
 * An interface is a "contract" that says:
 * "Any class that implements this interface MUST have these methods."
 * 
 * This allows us to easily swap out different selection algorithms
 * without changing any other code in the genetic algorithm.
 * 
 * OWNER: Person B (Algorithm module)
 */
class ISelectionStrategy {
public:
    /**
     * @brief Virtual destructor (required for interfaces)
     */
    virtual ~ISelectionStrategy() = default;

    /**
     * @brief Selects parents from the population for reproduction
     * 
     * In genetic algorithms, we select "fit" individuals to be parents
     * so their good genes get passed on to the next generation.
     * 
     * @param population The current group of watches
     * @param fitnessScores How good each watch is (higher = better)
     * @param numParents How many parents we need to select
     * @return A list of selected parent watches
     */
    virtual std::vector<std::shared_ptr<Genome::Watch>> select(
        const std::vector<std::shared_ptr<Genome::Watch>>& population,
        const std::vector<double>& fitnessScores,
        unsigned int numParents
    ) const = 0;

    /**
     * @brief Gets the name of this selection strategy
     * @return The strategy name (e.g., "Roulette Wheel", "Tournament")
     */
    virtual std::string getName() const = 0;
};

} // namespace Core
} // namespace WatchGA

#endif // WATCH_GA_CORE_ISELECTIONSTRATEGY_H