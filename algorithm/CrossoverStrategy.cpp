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
    
    auto child1 = std::make_shared<Genome::Watch>(parent1->getObjectName() + "_Child1");
    auto child2 = std::make_shared<Genome::Watch>(parent2->getObjectName() + "_Child2");

    const auto& p1 = parent1->getAllComponents();
    const auto& p2 = parent2->getAllComponents();

    // 1. COPY CORE ORGANS (Indices 0-4)
    // These 5 parts must exist in every child exactly as they were in the parents.
    // We limit the loop to the size of the parent in case a watch is somehow invalid/too small.
    for (size_t i = 0; i < 5 && i < p1.size(); ++i) {
        child1->addComponent(p1[i]->clone());
    }
    for (size_t i = 0; i < 5 && i < p2.size(); ++i) {
        child2->addComponent(p2[i]->clone());
    }

    // 2. CROSSOVER GEARS (Indices 5 to end)
    // We slice only the gear/jewel section of the genome.
    size_t p1GearCount = (p1.size() > 5) ? (p1.size() - 5) : 0;
    size_t p2GearCount = (p2.size() > 5) ? (p2.size() - 5) : 0;
    size_t minGears = std::min(p1GearCount, p2GearCount);

    if (minGears > 0) {
        // Pick a crossover point relative to the start of the gear section
        std::uniform_int_distribution<size_t> dist(1, minGears);
        size_t slice = dist(getRng());

        // Child 1: Core(P1) + Gears(P1, 0 to slice) + Gears(P2, slice to end)
        for (size_t i = 5; i < 5 + slice; ++i) child1->addComponent(p1[i]->clone());
        for (size_t i = 5 + slice; i < p2.size(); ++i) child1->addComponent(p2[i]->clone());

        // Child 2: Core(P2) + Gears(P2, 0 to slice) + Gears(P1, slice to end)
        for (size_t i = 5; i < 5 + slice; ++i) child2->addComponent(p2[i]->clone());
        for (size_t i = 5 + slice; i < p1.size(); ++i) child2->addComponent(p1[i]->clone());
    } else {
        // If no gears exist, just copy remaining components to avoid emptiness
        for (size_t i = 5; i < p1.size(); ++i) child1->addComponent(p1[i]->clone());
        for (size_t i = 5; i < p2.size(); ++i) child2->addComponent(p2[i]->clone());
    }

    child1->setValid(true);
    child2->setValid(true);
    
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