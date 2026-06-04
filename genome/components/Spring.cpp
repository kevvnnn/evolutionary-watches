#include "Spring.h"

namespace WatchGA {
namespace Genome {
namespace Components {

// Default
Spring::Spring()
    : m_type(SpringType::MAINSPRING),
      m_elasticity(0.9),
      m_fatigueResistance(0.9),
      m_length(10.0)
{
}

Spring::Spring(const std::string& name, double weight, double friction, double x, double y,
               SpringType type, double elasticity, double fatigueResistance, double length)
    : WatchComponent(name, weight, friction, x, y),
      m_type(type),
      m_elasticity(elasticity),
      m_fatigueResistance(fatigueResistance),
      m_length(length)
{
    setElasticity(elasticity);
    setFatigueResistance(fatigueResistance);
    setLength(length);
}

// Getters
Spring::SpringType Spring::getType() const { return m_type; }
double Spring::getElasticity() const { return m_elasticity; }
double Spring::getFatigueResistance() const { return m_fatigueResistance; }
double Spring::getLength() const { return m_length; }

void Spring::setType(SpringType type) { m_type = type; }

void Spring::setElasticity(double elasticity) {
    if (elasticity < 0.0) {
        m_elasticity = 0.0;
    } 
    // Enforce realistic physical ceilings based on horological metallurgy
    else if (m_type == SpringType::MAINSPRING && elasticity > 0.95) {
        m_elasticity = 0.95; // 5% energy loss to internal/barrel friction
    } 
    else if (m_type == SpringType::HAIRSPRING && elasticity > 0.98) {
        m_elasticity = 0.98; // 2% energy loss to crystalline hysteresis
    } 
    else {
        m_elasticity = elasticity;
    }
}

void Spring::setFatigueResistance(double fatigueResistance) {
    if (fatigueResistance < 0.0) {
        m_fatigueResistance = 0.0;
    } 
    // Clamp to high-grade engineering material limits (e.g., max 96-98% durability efficiency)
    else if (fatigueResistance > 0.96) {
        m_fatigueResistance = 0.96; 
    } 
    else {
        m_fatigueResistance = fatigueResistance;
    }
}

void Spring::setLength(double length) {
    if (length < 1.0) m_length = 1.0;
    else m_length = length;
}

// Hairspring gets accuracy bonus
double Spring::calculateEfficiency() const {
    double bonus = (m_type == SpringType::HAIRSPRING) ? 1.5 : 1.0;
    return m_elasticity * m_fatigueResistance * bonus;
}

// Deep copy
std::unique_ptr<WatchComponent> Spring::clone() const {
    return std::make_unique<Spring>(*this);
}

std::string Spring::toString() const {
    std::string typeStr = (m_type == SpringType::MAINSPRING) ? "Mainspring" : "Hairspring";
    return WatchComponent::toString() + " [" + typeStr + "]";
}

} // namespace Components
} // namespace Genome
} // namespace WatchGA