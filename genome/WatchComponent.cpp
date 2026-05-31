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
    SetWeight(weight);
    SetFriction(friction);
}

double WatchComponent::GetWeight() const {
    return m_weight;
}

double WatchComponent::GetFriction() const {
    return m_friction;
}

double WatchComponent::GetX() const {
    return m_x;
}

double WatchComponent::GetY() const {
    return m_y;
}

double WatchComponent::GetRotation() const {
    return m_rotation;
}

void WatchComponent::SetWeight(double weight) {
    if (weight <= 0.0)
        throw std::invalid_argument("Weight must be positive.");
    m_weight = weight;
}

void WatchComponent::SetFriction(double friction) {
    if (friction < 0.0) m_friction = 0.0;
    else if (friction > 1.0) m_friction = 1.0;
    else m_friction = friction;
}

void WatchComponent::SetPosition(double x, double y) {
    m_x = x;
    m_y = y;
}

void WatchComponent::SetRotation(double rotation) {
    m_rotation = rotation;
    while (m_rotation >= 360.0) m_rotation -= 360.0;
    while (m_rotation < 0.0) m_rotation += 360.0;
}

std::string WatchComponent::ToString() const {
    return Core::SystemObject::ToString() +
           " [Weight: " + std::to_string(m_weight) +
           "g, Friction: " + std::to_string(m_friction) + "]";
}