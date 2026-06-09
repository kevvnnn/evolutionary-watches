#include "WatchComponent.h"
#include <sstream>

namespace WatchGA {
namespace Genome {

// =========================================================
// CONSTRUCTORS
// =========================================================

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

// =========================================================
// GETTERS & SETTERS
// =========================================================

std::string WatchComponent::getName() const { 
    return m_name; 
}

double WatchComponent::getWeight() const { 
    return m_weight; 
}

double WatchComponent::getFriction() const { 
    return m_friction; 
}

void WatchComponent::setWeight(double weight) {
    // Prevent negative mass to maintain physical reality in the simulation
    if (weight >= 0.0) {
        m_weight = weight;
    } else {
        m_weight = 0.0;
    }
}

void WatchComponent::setFriction(double friction) {
    // Prevent negative friction to maintain thermodynamic reality
    if (friction >= 0.0) {
        m_friction = friction;
    } else {
        m_friction = 0.0;
    }
}

// =========================================================
// POLYMORPHIC OVERRIDES
// =========================================================

std::string WatchComponent::toString() const {
    // Formats the base attributes as clean bullet points. 
    // Derived classes (Gear, BalanceWheel, etc.) will call this base method 
    // and append their specific DNA attributes to the bottom of the string.
    std::ostringstream oss;
    oss << "• Name: "     << m_name   << "\n"
        << "• Weight: "   << m_weight << "g\n"
        << "• Friction: " << m_friction;
        
    return oss.str();
}

} // namespace Genome
} // namespace WatchGA