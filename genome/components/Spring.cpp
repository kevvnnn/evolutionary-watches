#include "Spring.h"

Spring::Spring(unsigned int id, SpringType type, double elasticity, double fatigueResistance, double length, double baseFriction)
    : WatchComponent(id, SPRING, baseFriction), m_type(type), m_elasticity(elasticity), m_fatigueResistance(fatigueResistance), m_length(length) {

    }

double Spring::calculateEfficiency() const {
    double hairspringBonus = (m_type == SpringType::HAIRSPRING) ? 1.5 : 1.0;
    return m_elasticity * m_fatigueResistance * hairspringBonus;
}

double Spring::calculateActualFriction() const {
    return getBaseFriction();
}

std::string Spring::getTypeName() const {
    return "Spring";
}

SpringType Spring::getSpringType() const {
    return m_type;
}