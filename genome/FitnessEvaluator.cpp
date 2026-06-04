#include "FitnessEvaluator.h"
#include "components/BalanceWheel.h"
#include "components/Hand.h"
#include <cmath>

namespace WatchGA {
namespace Genome {

// Default weights for good watch evolution
FitnessEvaluator::FitnessEvaluator()
    : m_accuracyWeight(0.7),
      m_efficiencyWeight(0.3),
      m_complexityPenaltyFactor(0.01),
      m_logBase(10.0)
{
}

FitnessEvaluator::FitnessEvaluator(double accuracyWeight, double efficiencyWeight,
                                   double complexityPenaltyFactor, double logBase)
    : m_accuracyWeight(accuracyWeight),
      m_efficiencyWeight(efficiencyWeight),
      m_complexityPenaltyFactor(complexityPenaltyFactor),
      m_logBase(logBase)
{
}

// FIXED: Added namespace to find BalanceWheel
double FitnessEvaluator::calculateAccuracyScore(const Watch& watch) const {
    using namespace Components;

    for (const auto& comp : watch.getAllComponents()) {
        if (const auto* bw = dynamic_cast<const BalanceWheel*>(comp.get())) {
            return bw->getIsochronism();
        }
    }
    return 0.0;
}

// Average efficiency of all components
double FitnessEvaluator::calculateEfficiencyScore(const Watch& watch) const {
    double total = 0.0;
    unsigned int count = 0;
    for (const auto& comp : watch.getAllComponents()) {
        total += comp->calculateEfficiency();
        count++;
    }
    return (count == 0) ? 0.0 : (total / count);
}

// Penalize overly complex watches
double FitnessEvaluator::calculateComplexityPenalty(const Watch& watch) const {
    return watch.getComponentCount() * m_complexityPenaltyFactor;
}

// Log scaling prevents score inflation
double FitnessEvaluator::applyLogScaling(double raw) const {
    if (raw <= 0.0) return 0.0;
    return log(raw * (m_logBase - 1.0) + 1.0) / log(m_logBase);
}

// MAIN FITNESS FUNCTION
double FitnessEvaluator::evaluate(const Watch& watch) const {
    if (!watch.isValid() || !watch.checkEssentialComponents())
        return 0.0;

    double accuracy = calculateAccuracyScore(watch);
    double efficiency = calculateEfficiencyScore(watch);
    double penalty = calculateComplexityPenalty(watch);

    // Calculate the raw base score
    double raw = (accuracy * m_accuracyWeight) + (efficiency * m_efficiencyWeight) - penalty;

    // Watch-hand relationship check
    double hourHandLength = 0.0;
    double minuteHandLength = 0.0;

    // Scan the assembly to find the dimensions of both hands
    using namespace Components;
    for (const auto& comp : watch.getAllComponents()) {
        if (const auto* hand = dynamic_cast<const Hand*>(comp.get())) {
            if (hand->getType() == Hand::HandType::HOUR) {
                hourHandLength = hand->getLength();
            } else if (hand->getType() == Hand::HandType::MINUTE) {
                minuteHandLength = hand->getLength();
            }
        }
    }

    // If the hour hand is realistically longer than or equal to the minute hand,
    // apply a severe penalty to tank its survival chances in the tournament.
    if (hourHandLength >= minuteHandLength && minuteHandLength > 0.0) {
        raw -= 0.35; // Harsh deduction
    }
    // =========================================================================

    // Floor the raw score to 0.0 so negative penalties don't break the log math
    if (raw < 0.0) raw = 0.0;

    return applyLogScaling(raw);
}
// Getters and setters
double FitnessEvaluator::getAccuracyWeight() const { return m_accuracyWeight; }
void FitnessEvaluator::setAccuracyWeight(double weight) { m_accuracyWeight = weight; }
double FitnessEvaluator::getEfficiencyWeight() const { return m_efficiencyWeight; }
void FitnessEvaluator::setEfficiencyWeight(double weight) { m_efficiencyWeight = weight; }
double FitnessEvaluator::getComplexityPenaltyFactor() const { return m_complexityPenaltyFactor; }
void FitnessEvaluator::setComplexityPenaltyFactor(double factor) { m_complexityPenaltyFactor = factor; }
double FitnessEvaluator::getLogBase() const { return m_logBase; }
void FitnessEvaluator::setLogBase(double logBase) { m_logBase = logBase; }

}
}