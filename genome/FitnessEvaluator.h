#ifndef WATCH_GA_GENOME_FITNESSEVALUATOR_H
#define WATCH_GA_GENOME_FITNESSEVALUATOR_H

#include "Watch.h"
#include <memory>

namespace WatchGA {
namespace Genome {

class FitnessEvaluator {
private:
    double m_accuracyWeight;
    double m_efficiencyWeight;
    double m_complexityPenaltyFactor;
    double m_logBase;

    double calculateAccuracyScore(const Watch& watch) const;
    double calculateEfficiencyScore(const Watch& watch) const;
    double calculateComplexityPenalty(const Watch& watch) const;
    double applyLogScaling(double rawScore) const;

public:
    FitnessEvaluator();
    FitnessEvaluator(double accuracyWeight, double efficiencyWeight,
                     double complexityPenaltyFactor, double logBase);
    ~FitnessEvaluator() = default;

    double getAccuracyWeight() const;
    void setAccuracyWeight(double weight);
    double getEfficiencyWeight() const;
    void setEfficiencyWeight(double weight);
    double getComplexityPenaltyFactor() const;
    void setComplexityPenaltyFactor(double factor);
    double getLogBase() const;
    void setLogBase(double logBase);

    double evaluate(const Watch& watch) const;
};

} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_FITNESSEVALUATOR_H