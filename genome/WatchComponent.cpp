#include "WatchComponent.h"

WatchComponent::WatchComponent(unsigned int id, ComponentType type, double baseFriction)
 : m_componentID(id), m_componentType(type), m_baseFriction(baseFriction) {

}

WatchComponent::~WatchComponent () {

}

unsigned int WatchComponent::getID() const {
    return m_componentID;
}

ComponentType WatchComponent::getComponentType() const {
    return m_componentType;
}

double WatchComponent::getBaseFriction() const {
    return m_baseFriction;
}