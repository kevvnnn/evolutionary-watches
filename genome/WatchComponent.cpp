#include "WatchComponent.h"
#include <sstream>

using namespace WatchGA::Genome;

WatchComponent::WatchComponent()
    : m_name("Unnamed"),
      m_weight(0.0),
      m_friction(0.0)
{
}

WatchComponent::WatchComponent(const std::string& name, double weight, double friction)
    : SystemObject(name), 
      m_name(name),
      m_weight(weight),
      m_friction(friction)
{
}

std::string WatchComponent::getName() const
{
    return m_name;
}

double WatchComponent::getWeight() const
{
    return m_weight;
}

double WatchComponent::getFriction() const
{
    return m_friction;
}

void WatchComponent::setWeight(double weight)
{
    if (weight >= 0)
        m_weight = weight;
}

void WatchComponent::setFriction(double friction)
{
    if (friction >= 0)
        m_friction = friction;
}

// Bullet-point tooltip format
std::string WatchComponent::toString() const
{
    std::ostringstream oss;
    oss << "• Name: "         << m_name       << "\n"
        << "• Weight: "       << m_weight     << "g\n"
        << "• Friction: "     << m_friction;
    return oss.str();
}