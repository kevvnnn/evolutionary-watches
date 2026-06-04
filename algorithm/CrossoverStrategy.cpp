#include "CrossoverStrategy.h"
#include "../genome/Watch.h"
#include "../genome/WatchComponent.h"
#include <random>    // For high-quality random number generation
#include <algorithm> // For std::min and std::max

namespace WatchGA {
namespace Algorithm {

// ---------------------------------------------------------
// HELPER FUNCTION: Random Number Generator
// ---------------------------------------------------------
static std::mt19937& getRng() {
    static std::random_device rd; // Hardware-level random seed
    static std::mt19937 gen(rd()); // The actual generator initialized with the seed
    return gen;
}

// =========================================================
// ONE-POINT CROSSOVER IMPLEMENTATION
// =========================================================
// Imagine lining up all components of Parent A and Parent B.
// We pick a random spot, slice both lines in half, and glue 
// the first half of A to the second half of B (and vice versa).

std::pair<std::shared_ptr<Genome::Watch>, std::shared_ptr<Genome::Watch>>
OnePointCrossover::crossover(
    const std::shared_ptr<Genome::Watch>& parent1,
    const std::shared_ptr<Genome::Watch>& parent2
) const {
    
    // 1. Create the "blank canvas" for our two children
    // We append "_Child1" to the parent's name so we can track the lineage in debugging
    auto child1 = std::make_shared<Genome::Watch>(parent1->getObjectName() + "_Child1");
    auto child2 = std::make_shared<Genome::Watch>(parent2->getObjectName() + "_Child2");

    // 2. Extract the "DNA" (component lists) from both parents
    const auto& p1Components = parent1->getAllComponents();
    const auto& p2Components = parent2->getAllComponents();

    // 3. Determine the safest place to slice the DNA
    // If Parent 1 has 5 parts and Parent 2 has 10 parts, we can only slice 
    // somewhere between index 1 and 4. Otherwise, we'd cause an out-of-bounds crash!
    size_t minSize = std::min(p1Components.size(), p2Components.size());
    
    // Safety Check: If a watch has 1 or 0 components, we can't slice it.
    // In this rare case, we just return exact genetic clones of the parents.
    if (minSize <= 1) {
        return {std::make_shared<Genome::Watch>(*parent1), std::make_shared<Genome::Watch>(*parent2)};
    }

    // 4. Roll the dice to pick the crossover point
    // This creates a distribution guaranteeing a number between 1 and (minSize - 1)
    std::uniform_int_distribution<size_t> dist(1, minSize - 1);
    size_t crossoverPoint = dist(getRng());

    // 5. Construct the Children!
    // CRITICAL C++ CONCEPT: We MUST use ->clone() here.
    // If we just pushed the parent's component pointer into the child, both watches 
    // would be sharing the exact same physical gear in memory. If the child mutated 
    // the gear, the parent's gear would mutate too. clone() manufactures a brand new copy.
    
    // Splice Parent 1's DNA
    for (size_t i = 0; i < p1Components.size(); ++i) {
        if (i < crossoverPoint) {
            // Left side of the slice goes to Child 1
            child1->addComponent(p1Components[i]->clone());
        } else {
            // Right side of the slice goes to Child 2
            child2->addComponent(p1Components[i]->clone());
        }
    }

    // Splice Parent 2's DNA
    for (size_t i = 0; i < p2Components.size(); ++i) {
        if (i < crossoverPoint) {
            // Left side of the slice goes to Child 2
            child2->addComponent(p2Components[i]->clone());
        } else {
            // Right side of the slice goes to Child 1
            child1->addComponent(p2Components[i]->clone());
        }
    }

    child1->setValid(true);
    child2->setValid(true);
    
    // Return the two new watches packed in a std::pair
    return {child1, child2};
}

std::string OnePointCrossover::getName() const {
    return "One-Point Crossover";
}

// =========================================================
// UNIFORM CROSSOVER IMPLEMENTATION
// =========================================================
// Instead of slicing the watch in half, we look at every single 
// component slot one by one and flip a coin.
// Heads: Child 1 gets Parent A's part. Tails: Child 1 gets Parent B's part.

std::pair<std::shared_ptr<Genome::Watch>, std::shared_ptr<Genome::Watch>>
UniformCrossover::crossover(
    const std::shared_ptr<Genome::Watch>& parent1,
    const std::shared_ptr<Genome::Watch>& parent2
) const {
    
    // 1. Create our blank children
    auto child1 = std::make_shared<Genome::Watch>(parent1->getObjectName() + "_Child1");
    auto child2 = std::make_shared<Genome::Watch>(parent2->getObjectName() + "_Child2");

    const auto& p1Components = parent1->getAllComponents();
    const auto& p2Components = parent2->getAllComponents();

    // 2. Find the largest parent genome
    // Unlike One-Point, we want to iterate through ALL available genetic material,
    // even if one parent is much larger than the other.
    size_t maxSize = std::max(p1Components.size(), p2Components.size());
    
    // Create a 50/50 coin flip generator (yields a decimal between 0.0 and 1.0)
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // 3. March through the DNA sequence, index by index
    for (size_t i = 0; i < maxSize; ++i) {
        
        // Flip the coin! If > 0.5, we swap where the genes go.
        bool swap = (dist(getRng()) > 0.5); 

        // Handle Parent 1's gene at this index (if it exists)
        if (i < p1Components.size()) {
            if (swap) {
                child2->addComponent(p1Components[i]->clone());
            } else {
                child1->addComponent(p1Components[i]->clone());
            }
        }

        // Handle Parent 2's gene at this index (if it exists)
        if (i < p2Components.size()) {
            if (swap) {
                child1->addComponent(p2Components[i]->clone());
            } else {
                child2->addComponent(p2Components[i]->clone());
            }
        }
    }

    child1->setValid(true);
    child2->setValid(true);
    
    // Return the two new watches packed in a std::pair
    return {child1, child2};
}

std::string UniformCrossover::getName() const {
    return "Uniform Crossover";
}

} // namespace Algorithm
} // namespace WatchGA