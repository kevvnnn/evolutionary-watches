#include "MutationStrategy.h"
#include "../genome/Watch.h"
#include "../genome/components/BalanceWheel.h"
#include "../genome/components/Gear.h"
#include "../genome/components/Hand.h"
#include "../genome/components/Spring.h"
#include "../genome/components/Jewel.h"
#include <random>
#include <algorithm>

namespace WatchGA {
namespace Algorithm {

// -----------------------------------------------------------------------------
// HELPER FUNCTION: Thread-Safe Static Random Number Generator
// -----------------------------------------------------------------------------
// We use a static local generator here so that we don't re-seed the random 
// engine every single time a mutation function is called. Re-seeding rapidly 
// can cause duplicate "random" sequences. This pattern guarantees one persistent engine.
static std::mt19937& getRng() {
    static std::random_device rd;  // Hardware-level entropy source 
    static std::mt19937 gen(rd()); // The engine initialized with the hardware seed 
    return gen;
}

// =============================================================================
// SWAP MUTATION IMPLEMENTATION (Topological Network Rewiring)
// =============================================================================
// This mutation focuses purely on the structural arrangement (the connections).
// It does NOT change the physical parts inside the watch. Instead, it rolls a dice 
// to see if it should rewire how two components talk to each other.
void SwapMutation::mutate(std::shared_ptr<Genome::Watch>& watch, double mutationRate) const {
    std::uniform_real_distribution<double> chanceDist(0.0, 1.0);
    
    // 1. Roll against the mutation rate to decide if this watch should mutate at all
    if (chanceDist(getRng()) >= mutationRate) return;
    
    // 2. Structural Guard: We cannot swap connections if the watch has fewer than 2 parts 
    if (watch->getComponentCount() < 2) return;

    // 3. Select two random component indices from the watch 
    std::uniform_int_distribution<unsigned int> idDist(0, watch->getComponentCount() - 1);
    unsigned int compA = idDist(getRng());
    unsigned int compB = idDist(getRng());

    // 4. Perform the topological flip (ensure we aren't trying to connect a part to itself!)
    if (compA != compB) {
        // DESIGN ADVANTAGE: Instead of shifting vectors, we flip the connection status.
        // If they are already connected, we sever the tie. If they are disconnected, we bridge them.
        if (watch->areConnected(compA, compB)) {
            watch->removeConnection(compA, compB); // Sever connection 
        } else {
            watch->addConnection(compA, compB);    // Create connection 
        }
    }
}

std::string SwapMutation::getName() const { 
    return "Topological Swap Mutation"; 
}

// =============================================================================
// PARAMETER MUTATION IMPLEMENTATION (The Evolutionary "Nudge")
// =============================================================================
// This is the most crucial mutation strategy for fine-tuning performance. It acts 
// like a cosmic radiation hit that slightly alters physical stats (e.g., shaving 
// down a gear tooth, loosening a spring, or smoothing out a friction boundary).
ParameterMutation::ParameterMutation(double mutationStrength) 
    : m_mutationStrength(mutationStrength) {}

double ParameterMutation::getMutationStrength() const { return m_mutationStrength; }
void ParameterMutation::setMutationStrength(double strength) { m_mutationStrength = strength; }

void ParameterMutation::mutate(std::shared_ptr<Genome::Watch>& watch, double mutationRate) const {
    std::uniform_real_distribution<double> chanceDist(0.0, 1.0);
    
    // Setup a distribution that produces small positive or negative fractional nudges 
    // bounded by our configured mutation strength (e.g., -0.15 to +0.15)
    std::uniform_real_distribution<double> nudgeDist(-m_mutationStrength, m_mutationStrength);
    
    using namespace Genome::Components;

    // 1. Walk through every individual component inside the watch genome 
    for (unsigned int i = 0; i < watch->getComponentCount(); ++i) {
        
        // 2. Roll a completely unique dice check for *each* physical part
        if (chanceDist(getRng()) >= mutationRate) continue;

        auto* comp = watch->getComponent(i);
        if (!comp) continue;

        // 3. Mutate Base Properties (Shared by all WatchComponents) 
        // Friction exists on the base class, so we can mutate it universally.
        double newFriction = comp->getFriction() + nudgeDist(getRng());
        comp->setFriction(newFriction); // The setter automatically clamps this to 0.0-1.0 

        // 4. Polymorphic Deep Dive (Mutating Type-Specific Properties)
        // Because the vector stores base pointers, we must safely query what the true 
        // underlying subclass type is using C++ Run-Time Type Information (RTTI).

        // Case A: Is this component actually a Balance Wheel? 
        if (auto* bw = dynamic_cast<BalanceWheel*>(comp)) {
            // Adjust the balance wheel's timing consistency
            bw->setIsochronism(bw->getIsochronism() + nudgeDist(getRng()));
            
            // Adjust the rotational inertia 
            double newMoi = bw->getMomentOfInertia() + (nudgeDist(getRng()) * 2.0);
            // CRITICAL DEFENSIVE CODING: The setter will throw an exception if MOI falls below 0.1.
            // We guard this setter to prevent runtime crashes during extreme mutations.
            if (newMoi >= 0.1) bw->setMomentOfInertia(newMoi); 
        } 
        // Case B: Is this component actually a Gear? 
        else if (auto* gear = dynamic_cast<Gear*>(comp)) {
            // A. Mutate the surface alignment quality
            gear->setMeshingQuality(gear->getMeshingQuality() + nudgeDist(getRng()));
            
            // B. Mutate the physical tooth count (add or subtract 1 tooth)
            int teethChange = (nudgeDist(getRng()) > 0) ? 1 : -1;
            if (static_cast<int>(gear->getToothCount()) + teethChange >= 8) {
                gear->setToothCount(gear->getToothCount() + teethChange);
            }

            // C. NEW: Mutate the physical gear diameter size
            double newDia = gear->getDiameter() + nudgeDist(getRng());
            // Guard against Person A's 1.0mm minimum constraint to prevent runtime crashes
            if (newDia >= 1.0) {
                gear->setDiameter(newDia);
            }
        }
        // Case C: Is this component actually a Spring? 
        else if (auto* spring = dynamic_cast<Spring*>(comp)) {
            // Adjust elasticity coefficients and material breakdown speeds 
            spring->setElasticity(spring->getElasticity() + nudgeDist(getRng()));
            spring->setFatigueResistance(spring->getFatigueResistance() + nudgeDist(getRng()));
        }
    }
}

std::string ParameterMutation::getName() const { 
    return "Parameter Nudge Mutation"; 
}

// =============================================================================
// ADD/REMOVE MUTATION IMPLEMENTATION (Genome Size Adaptation)
// =============================================================================
// This mutation allows the structural framework of our watch to grow or shrink over 
// generations. It prevents the algorithm from being locked into a fixed scope, 
// allowing it to discover if adding auxiliary gears improves mechanical efficiency.
AddRemoveMutation::AddRemoveMutation(unsigned int maxComponents) 
    : m_maxComponents(maxComponents) {}

unsigned int AddRemoveMutation::getMaxComponents() const { return m_maxComponents; }
void AddRemoveMutation::setMaxComponents(unsigned int max) { m_maxComponents = max; }

void AddRemoveMutation::mutate(std::shared_ptr<Genome::Watch>& watch, double mutationRate) const {
    std::uniform_real_distribution<double> chanceDist(0.0, 1.0);
    
    // 1. Roll against the mutation rate to decide if a structural shift occurs
    if (chanceDist(getRng()) >= mutationRate) return;

    // 2. Flip a 50/50 coin to decide whether we add structural mass or remove structural mass
    std::uniform_int_distribution<int> actionDist(0, 1);
    bool addPart = (actionDist(getRng()) == 0);

    // ACTION 1: Spawning a New Gear
    if (addPart && watch->getComponentCount() < m_maxComponents) {
        std::uniform_int_distribution<unsigned int> teethDist(8, 40);
        std::uniform_real_distribution<double> statDist(0.1, 1.0);
        
        // Create a completely raw, randomized mechanical Gear on the heap 
        auto newGear = std::make_unique<Genome::Components::Gear>(
            "Mutated_Gear", 
            statDist(getRng()), // Random weight 
            statDist(getRng()), // Random friction 
            0.0, 0.0,           // Coordinates
            teethDist(getRng()),// Random tooth sizing 
            10.0,               // Standard millimeter diameter 
            statDist(getRng())  // Random meshing profile 
        );

        // Hand over the unique pointer to the watch assembly 
        watch->addComponent(std::move(newGear));

        // TOPOLOGY LINKING: A floating gear that touches nothing does absolutely nothing.
        // We look back at our array and immediately force a link between this new part 
        // and one random pre-existing component to bind it to the gear train system.
        if (watch->getComponentCount() > 1) {
            std::uniform_int_distribution<unsigned int> targetDist(0, watch->getComponentCount() - 2);
            watch->addConnection(watch->getComponentCount() - 1, targetDist(getRng()));
        }
    } 
    // ACTION 2: Deleting a Component (Destructive Filtering)
    else if (!addPart && watch->getComponentCount() > 5) {
        // CRITICAL GUARD FOR NATURAL SELECTION: A watch requires exactly 5 core parts to tick. 
        // If our component count is 5, we completely abort deletion. 
        // If it is above 5, we can drop a gear without instantly failing structural validity checks.
        
        std::uniform_int_distribution<unsigned int> targetDist(0, watch->getComponentCount() - 1);
        unsigned int targetIndex = targetDist(getRng());
        
        // EVOLUTION NOTE: We do not check if the selected part is essential here. 
        // If the algorithm rolls a dice and deletes the vital Balance Wheel, the watch 
        // will get an automatic fitness score of 0.0 on the next pass and die out naturally.
        watch->removeComponent(targetIndex);
    }
}

std::string AddRemoveMutation::getName() const { 
    return "Add/Remove Component Mutation"; 
}

} // namespace Algorithm
} // namespace WatchGA