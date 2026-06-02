#include "WatchComponent.h"
#include <stdexcept>

using namespace WatchGA::Genome;

WatchComponent::WatchComponent()
    : m_weight(1.0),
      m_friction(0.0),
      m_x(0.0),
      m_y(0.0),
      m_rotation(0.0)
{
}

WatchComponent::WatchComponent(const std::string& name, double weight, double friction, double x, double y)
    : Core::SystemObject(name),
      m_weight(weight),
      m_friction(friction),
      m_x(x),
      m_y(y),
      m_rotation(0.0)
{
    setWeight(weight);
    setFriction(friction);
}

double WatchComponent::getWeight() const { return m_weight; }
double WatchComponent::getFriction() const { return m_friction; }
double WatchComponent::getX() const { return m_x; }
double WatchComponent::getY() const { return m_y; }
double WatchComponent::getRotation() const { return m_rotation; }

void WatchComponent::setWeight(double weight) {
    if (weight <= 0.0)
        throw std::invalid_argument("Weight must be positive.");
    m_weight = weight;
}

void WatchComponent::setFriction(double friction) {
    if (friction < 0.0) m_friction = 0.0;
    else if (friction > 1.0) m_friction = 1.0;
    else m_friction = friction;
}

void WatchComponent::setPosition(double x, double y) {
    m_x = x;
    m_y = y;
}

void WatchComponent::setRotation(double rotation) {
    m_rotation = rotation;
    while (m_rotation >= 360.0) m_rotation -= 360.0;
    while (m_rotation < 0.0) m_rotation += 360.0;
}

std::string WatchComponent::toString() const {
    return Core::SystemObject::toString() +
           " [Weight: " + std::to_string(m_weight) +
           "g, Friction: " + std::to_string(m_friction) + "]";
}