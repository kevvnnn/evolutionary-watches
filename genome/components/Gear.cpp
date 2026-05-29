#include "Gear.h"

Gear::Gear(unsigned int id, unsigned int teeth, double diameter, double meshQuality, double baseFriction)
    : WatchComponent(id, GEAR, baseFriction), m_toothCount(teeth), m_diameter(diameter), m_meshingQuality(meshQuality) {

    }

double Gear::calculateEfficiency() const {
    return m_meshingQuality * (1.0 - (1.0 / m_toothCount)) * (1.0 - (m_diameter / 100.0));
}

double Gear::calculateActualFriction() const {
    return getBaseFriction();
}

std::string Gear::getTypeName() const {
    return "Gear";
}

unsigned int Gear ::getToothCount() const {
    return m_toothCount;
}