#ifndef WATCH_GA_GENOME_FITNESSEVALUATOR_H
#define WATCH_GA_GENOME_FITNESSEVALUATOR_H

#include "Watch.h"
#include <memory>

namespace WatchGA {
namespace Genome {

/**
 * @class FitnessEvaluator
 * @brief Calculates the evolutionary survival score of a Watch genome.
 * * This class acts as the "Physics Engine" and "Horological Judge" of the genetic algorithm.
 * It replaces simple pass/fail checks with Continuous Quadratic Penalties, ensuring
 * the AI is smoothly guided away from physically impossible or destructive designs.
 */
class FitnessEvaluator {
private:
    // ---------------------------------------------------------
    // WEIGHTS & SCALING FACTORS
    // ---------------------------------------------------------
    double m_accuracyWeight;
    double m_efficiencyWeight;
    double m_complexityPenaltyFactor;
    double m_logBase;

    // ---------------------------------------------------------
    // INTERNAL SCORING HELPERS
    // ---------------------------------------------------------
    double calculateAccuracyScore(const Watch& watch) const;
    double calculateEfficiencyScore(const Watch& watch) const;
    double calculateComplexityPenalty(const Watch& watch) const;
    
    /**
     * @brief Compresses the raw score logarithmically to prevent exponential score inflation
     */
    double applyLogScaling(double rawScore) const;

public:
    /**
     * @brief Default constructor initializing standard horological weights
     */
    FitnessEvaluator();
    
    /**
     * @brief Overloaded constructor for custom evaluation weights
     */
    FitnessEvaluator(double accuracyWeight, double efficiencyWeight,
                     double complexityPenaltyFactor, double logBase);
    
    ~FitnessEvaluator() = default;

    // ---------------------------------------------------------
    // GETTERS & SETTERS
    // ---------------------------------------------------------
    double getAccuracyWeight() const;
    void setAccuracyWeight(double weight);
    
    double getEfficiencyWeight() const;
    void setEfficiencyWeight(double weight);
    
    double getComplexityPenaltyFactor() const;
    void setComplexityPenaltyFactor(double factor);
    
    double getLogBase() const;
    void setLogBase(double logBase);

    // =========================================================
    // MAIN EVALUATION FUNCTION
    // =========================================================
    /**
     * @brief The master function that scores a watch based on its physics and topology
     * @param watch The watch genome to evaluate
     * @return A normalized double representing the watch's fitness (higher is better, 0.0 is dead)
     */
    double evaluate(const Watch& watch) const;
};

} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_FITNESSEVALUATOR_H