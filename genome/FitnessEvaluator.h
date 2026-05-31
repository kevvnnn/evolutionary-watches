#ifndef FITNESS_EVALUATOR_H
#define FITNESS_EVALUATOR_H

#include "Watch.h"

class FitnessEvaluator {
private:
    double m_accuracyWeight = 0.7;
    double m_efficiencyWeight = 0.3;
    double m_frictionPenaltyFactor = 0.4; // Higher friction = bigger penalty
    double m_complexityPenaltyFactor = 0.01;
    double m_logBase = 10.0;

public:
    double calculateAccuracy(const Watch& watch) const;
    double calculateAvgEfficiency(const Watch& watch) const;
    double calculateTotalFriction(const Watch& watch) const;
    double calculateJewelPlacementScore(const Watch& watch) const;

    double calculateFitness(const Watch& watch) const;
    void setWeights(double accuracy, double efficiency, double frictionPenalty, double complexity);
};

#endif