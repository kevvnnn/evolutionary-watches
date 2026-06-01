#ifndef WATCH_GA_CORE_ICROSSOVERSTRATEGY_H
#define WATCH_GA_CORE_ICROSSOVERSTRATEGY_H

#include <memory>
#include <utility>

// Forward declaration
namespace WatchGA { namespace Genome { class Watch; } }

namespace WatchGA {
namespace Core {

/**
 * @brief INTERFACE for all crossover (mating) strategies
 * 
 * crossover is how we combine the genes of two parents to create offspring.
 * Different crossover strategies mix the genes in different ways.
 * 
 * OWNER: Person B (Algorithm module)
 */
class ICrossoverStrategy {
public:
    virtual ~ICrossoverStrategy() = default;

    /**
     * @brief Combines two parent watches to create two child watches
     * 
     * Think of this as "genetic mating". Each child gets some genes
     * from parent 1 and some from parent 2.
     * 
     * @param parent1 The first parent watch
     * @param parent2 The second parent watch
     * @return A pair of child watches (offspring)
     */
    virtual std::pair<std::shared_ptr<Genome::Watch>, std::shared_ptr<Genome::Watch>> crossover(
        const std::shared_ptr<Genome::Watch>& parent1,
        const std::shared_ptr<Genome::Watch>& parent2
    ) const = 0;

    /**
     * @brief Gets the name of this crossover strategy
     * @return The strategy name (e.g., "One-Point", "Uniform")
     */
    virtual std::string getName() const = 0;
};

} // namespace Core
} // namespace WatchGA

#endif // WATCH_GA_CORE_ICROSSOVERSTRATEGY_H