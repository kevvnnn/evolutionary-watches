#include "WatchComponent.h"
#include <sstream>

using namespace WatchGA::Genome;

WatchComponent::WatchComponent()
    : m_name("Unnamed"),
      m_weight(0.0),
      m_friction(0.0),
      m_x(0.0),
      m_y(0.0)
{
}

WatchComponent::WatchComponent(const std::string& name, double weight, double friction, double x, double y)
    : m_name(name),
      m_weight(weight),
      m_friction(friction),
      m_x(x),
      m_y(y)
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

double WatchComponent::getX() const
{
    return m_x;
}

double WatchComponent::getY() const
{
    return m_y;
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

void WatchComponent::setPosition(double x, double y)
{
    m_x = x;
    m_y = y;
}

// Bullet-point tooltip format
std::string WatchComponent::toString() const
{
    std::ostringstream oss;
    oss << "• Name: "         << m_name       << "\n"
        << "• Weight: "       << m_weight     << "g\n"
        << "• Friction: "     << m_friction   << "\n"
        << "• Position: ("    << m_x << ", " << m_y << ")";
    return oss.str();
}