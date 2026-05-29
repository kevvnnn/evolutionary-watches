#ifndef BALANCE_WHEEL_H
#define BALANCE_WHEEL_H

#include "../WatchComponent.h"

class BalanceWheel : public WatchComponent {
    private:
    double m_momentOfInertia;
    double m_isochronism;
    double m_amplitude;

    public:
    BalanceWheel(unsigned int id, double inertia, double isochronism, double amplitude, double baseFriction);
    double calculateEfficiency() const override;
    double calculateActualFriction() const override;
    std::string getTypeName() const override;

    double getIsochronism() const;
};

#endif