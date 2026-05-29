#include "BalanceWheel.h"
#include <cmath>

BalanceWheel::BalanceWheel(unsigned int id, double inertia, double isochronism, double amplitude, double baseFriction)
    : WatchComponent(id, BALANCE_WHEEL, baseFriction), m_momentOfInertia(inertia), 
    m_isochronism(isochronism), m_amplitude(amplitude) {

    }

double BalanceWheel::calculateEfficiency() const {
    return m_isochronism * (1.0 - std::abs(m_amplitude - 270.0) / 300.0) * (1.0 - 1.0 / (m_momentOfInertia + 1.0));
}

double BalanceWheel::calculateActualFriction() const {
    return getBaseFriction();
}

std::string BalanceWheel::getTypeName() const {
    return "BalanceWheel";
}

double BalanceWheel::getIsochronism() const {
    return m_isochronism;
}