#include "Jewel.h"

const double MIN_REQUIRED_FRICTION_FOR_JEWEL = 0.25;

Jewel::Jewel(unsigned int id, double hardness, bool isCapJewel, double baseFriction)
    : WatchComponent(id, JEWEL, baseFriction), m_hardness(hardness), m_isCapJewel(isCapJewel), m_connectedComponentFriction(0.0) {

    }

double Jewel::calculateEfficiency() const {
    double capBonus = m_isCapJewel ? 1.2 : 1.0;
    return (m_hardness / 10.0) * capBonus;
}

double Jewel::calculateActualFriction() const {
    double frictionReduction = (m_hardness / 10.0) * 0.8;
    return getBaseFriction() * (1.0 - frictionReduction);
}

std::string Jewel::getTypeName() const {
    return "Jewel";
}

void Jewel:: setConnectedFriction(double friction) {
    m_connectedComponentFriction = friction;
}

bool Jewel::isPlacedCorrectly() const {
    return m_connectedComponentFriction >= MIN_REQUIRED_FRICTION_FOR_JEWEL;
}

double Jewel::getPlacementBonus() const {
    return isPlacedCorrectly() ? 0.15 : 0.0;
}

double Jewel::getPlacementPenalty() const {
    return isPlacedCorrectly() ? 0.0 : 0.25;
}