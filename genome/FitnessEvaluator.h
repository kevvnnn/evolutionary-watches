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

    double CalculateAccuracyScore(const Watch& watch) const;
    double CalculateEfficiencyScore(const Watch& watch) const;
    double CalculateComplexityPenalty(const Watch& watch) const;
    double ApplyLogScaling(double rawScore) const;

public:
    FitnessEvaluator();
    FitnessEvaluator(double accuracyWeight, double efficiencyWeight,
                     double complexityPenaltyFactor, double logBase);
    ~FitnessEvaluator() = default;

    double GetAccuracyWeight() const;
    void SetAccuracyWeight(double weight);
    double GetEfficiencyWeight() const;
    void SetEfficiencyWeight(double weight);
    double GetComplexityPenaltyFactor() const;
    void SetComplexityPenaltyFactor(double factor);
    double GetLogBase() const;
    void SetLogBase(double logBase);

    double Evaluate(const Watch& watch) const;
};

} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_FITNESSEVALUATOR_H