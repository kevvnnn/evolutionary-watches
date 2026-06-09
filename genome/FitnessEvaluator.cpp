#include "FitnessEvaluator.h"
#include "components/BalanceWheel.h"
#include "components/Gear.h"
#include "components/Hand.h"
#include <cmath>

namespace WatchGA {
namespace Genome {

// ---------------------------------------------------------
// CONSTRUCTORS
// ---------------------------------------------------------

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

// ---------------------------------------------------------
// INTERNAL SCORING HELPERS
// ---------------------------------------------------------

double FitnessEvaluator::calculateAccuracyScore(const Watch& watch) const {
    using namespace Components;
    
    // Find the Balance Wheel and return its isochronism as the baseline accuracy
    for (const auto& comp : watch.getAllComponents()) {
        if (const auto* bw = dynamic_cast<const BalanceWheel*>(comp.get())) {
            return bw->getIsochronism();
        }
    }
    return 0.0;
}

double FitnessEvaluator::calculateEfficiencyScore(const Watch& watch) const {
    double total = 0.0;
    unsigned int count = 0;
    
    for (const auto& comp : watch.getAllComponents()) {
        total += comp->calculateEfficiency();
        count++;
    }
    // Return the averaged efficiency of all parts combined
    return (count == 0) ? 0.0 : (total / count);
}

double FitnessEvaluator::calculateComplexityPenalty(const Watch& watch) const {
    // Punishes watches for "part-spamming" (e.g., using 20 gears when 5 would do)
    return watch.getComponentCount() * m_complexityPenaltyFactor;
}

double FitnessEvaluator::applyLogScaling(double raw) const {
    if (raw <= 0.0) return 0.0;
    return log(raw * (m_logBase - 1.0) + 1.0) / log(m_logBase);
}

// =========================================================================
// MAIN FITNESS FUNCTION (THE PHYSICS ENGINE)
// =========================================================================

double FitnessEvaluator::evaluate(const Watch& watch) const {
    // 1. Immediate Execution Guard: Invalid or missing core organs result in instant death
    if (!watch.isValid() || !watch.checkEssentialComponents())
        return 0.0;

    // 2. Calculate Base Physics Score
    double accuracy = calculateAccuracyScore(watch);
    double efficiency = calculateEfficiencyScore(watch);
    double penalty = calculateComplexityPenalty(watch);

    double raw = (accuracy * m_accuracyWeight) + (efficiency * m_efficiencyWeight) - penalty;

    // =========================================================================
    // 3. CONTINUOUS QUADRATIC PENALTIES (The "Smooth" Physics Guard)
    // =========================================================================
    double hourHandLength = 0.0;
    double minuteHandLength = 0.0;

    using namespace Components;
    for (const auto& comp : watch.getAllComponents()) {
        
        // Case A: Guard Against Knocking (Amplitude limit)
        if (auto* bw = dynamic_cast<const BalanceWheel*>(comp.get())) {
            double amp = bw->getAmplitude();
            if (amp > 315.0) {
                double violation = amp - 315.0;
                raw -= 0.001 * (violation * violation); // Smooth quadratic drop
            }
        }
        
        // Case B: Guard Against Micro-Gears (Module limit)
        else if (auto* gear = dynamic_cast<const Gear*>(comp.get())) {
            double module = gear->getDiameter() / static_cast<double>(gear->getToothCount());
            if (module < 0.08) {
                double violation = 0.08 - module;
                raw -= 100.0 * (violation * violation); // Massive quadratic multiplier for impossible physics
            }
        }

        // Case C: Extract Hand Lengths for Case Geometry Check
        else if (auto* hand = dynamic_cast<const Hand*>(comp.get())) {
            if (hand->getType() == Hand::HandType::HOUR) {
                hourHandLength = hand->getLength();
            } else if (hand->getType() == Hand::HandType::MINUTE) {
                minuteHandLength = hand->getLength();
            }
        }
    }

    // =========================================================================
    // 4. 36MM DIAL & LEGIBILITY CHECKS
    // =========================================================================
    if (hourHandLength > 0.0 && minuteHandLength > 0.0) {
        
        // A. Case Scraping: Hand cannot physically exceed the 18mm dial radius
        if (hourHandLength > 18.0) {
            double violation = hourHandLength - 18.0;
            raw -= 0.02 * (violation * violation);
        }
        if (minuteHandLength > 18.0) {
            double violation = minuteHandLength - 18.0;
            raw -= 0.02 * (violation * violation);
        }

        // B. Legibility Gap: Minute hand must be noticeably longer than hour hand
        double gap = minuteHandLength - hourHandLength;
        if (gap < 4.0) {
            double violation = 4.0 - gap;
            raw -= 0.02 * (violation * violation);
        }

        // C. Dial Sweeping: Minute hand shouldn't be awkwardly short
        if (minuteHandLength < 15.0) {
            double violation = 15.0 - minuteHandLength;
            raw -= 0.02 * (violation * violation);
        }
    }
    
    // =========================================================================
    // 5. THE GEAR TRAIN TORQUE REWARD
    // =========================================================================
    int totalGears = 0;
    for (const auto& comp : watch.getAllComponents()) {
        if (dynamic_cast<const Components::Gear*>(comp.get())) totalGears++;
    }

    // Step-up ratio physics simulation based on total gears
    if (totalGears == 1) {
        raw -= 0.15; // Too weak, cannot step up the gear ratio
    } else if (totalGears == 2) {
        raw -= 0.05; // Barely enough torque
    } else if (totalGears >= 3 && totalGears <= 5) {
        raw += 0.05; // The Horological Goldilocks Zone! (Massive Reward)
    } else if (totalGears > 5) {
        raw -= 0.05 * (totalGears - 5); // Too much friction, energy is lost
    }

    // 6. Safety Clamp and Log Scaling
    if (raw < 0.0) raw = 0.0;
    return applyLogScaling(raw);
}

// ---------------------------------------------------------
// GETTERS & SETTERS
// ---------------------------------------------------------
double FitnessEvaluator::getAccuracyWeight() const { return m_accuracyWeight; }
void FitnessEvaluator::setAccuracyWeight(double weight) { m_accuracyWeight = weight; }

double FitnessEvaluator::getEfficiencyWeight() const { return m_efficiencyWeight; }
void FitnessEvaluator::setEfficiencyWeight(double weight) { m_efficiencyWeight = weight; }

double FitnessEvaluator::getComplexityPenaltyFactor() const { return m_complexityPenaltyFactor; }
void FitnessEvaluator::setComplexityPenaltyFactor(double factor) { m_complexityPenaltyFactor = factor; }

double FitnessEvaluator::getLogBase() const { return m_logBase; }
void FitnessEvaluator::setLogBase(double logBase) { m_logBase = logBase; }

} // namespace Genome
} // namespace WatchGA