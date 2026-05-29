#include "WatchComponent.h"

WatchComponent::WatchComponent(unsigned int id, ComponentType type) : m_componentID(id), m_componentType(type) {

}

WatchComponent::~WatchComponent () {

}

unsigned int WatchComponent::getID() const {
    return m_componentID;
}

ComponentType WatchComponent::getComponentType() const {
    return m_componentType;
}