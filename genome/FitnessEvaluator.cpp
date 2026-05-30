#include "FitnessEvaluator.h"
#include "components/BalanceWheel.h"
#include "components/Jewel.h"
#include <cmath>

// Time keeping accuracy (isochronism)
double FitnessEvaluator::calculateAccuracy(const Watch& watch) const {
    for (const auto* comp : watch.getComponents()) {
        if (const auto* b = dynamic_cast<const BalanceWheel*>(comp)) return b->getIsochronism();
    }
    return 0.0;
}

// Average efficiency for all components
double FitnessEvaluator::calculateAvgEfficiency(const Watch& watch) const {
    double total = 0.0;
    const auto& comps = watch.getComponents();
    for (const auto* c : comps) total += c->calculateEfficiency();
    return comps.empty() ? 0.0 : total / comps.size();
}

// Total watch friction
double FitnessEvaluator::calculateTotalFriction(const Watch& watch) const {
    double total = 0.0;
    for (const auto* c : watch.getComponents()) total += c->calculateActualFriction();
    return total;
}

// Jewel score
double FitnessEvaluator::calculateJewelPlacementScore(const Watch& watch) const {
    double score = 0.0;
    for (const auto* comp : watch.getComponents()) {
        if (const Jewel* j = dynamic_cast<const Jewel*>(comp)) {
            score += j->getPlacementBonus();
            score -= j->getPlacementPenalty();
        }
    }
    return score;
}

// Final friction score (including the jewel)
double FitnessEvaluator::calculateFitness(const Watch& watch) const {
    if (!watch.isValid()) return 0.0;

    double accuracy = calculateAccuracy(watch);
    double efficiency = calculateAvgEfficiency(watch);
    double totalFriction = calculateTotalFriction(watch);
    double jewelScore = calculateJewelPlacementScore(watch);
    int complexity = watch.getComponents().size();

    double fitness = (accuracy * m_accuracyWeight) + (efficiency * m_efficiencyWeight);

    fitness -= (totalFriction * m_frictionPenaltyFactor);

    fitness += jewelScore;

    fitness -= m_complexityPenaltyFactor * std::log(complexity + 1) / std::log(m_logBase);

    return std::max(0.0, std::min(1.0, fitness));
}

void FitnessEvaluator::setWeights(double acc, double eff, double friction, double complexity) {
    m_accuracyWeight = acc;
    m_efficiencyWeight = eff;
    m_frictionPenaltyFactor = friction;
    m_complexityPenaltyFactor = complexity;
}