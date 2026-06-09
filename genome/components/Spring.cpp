#include "Spring.h"

namespace WatchGA {
namespace Genome {
namespace Components {

// ---------------------------------------------------------
// CONSTRUCTORS
// ---------------------------------------------------------
Spring::Spring()
    : m_type(SpringType::MAINSPRING),
      m_elasticity(0.9),
      m_fatigueResistance(0.9),
      m_length(10.0)
{
}

Spring::Spring(const std::string& name, double weight, double friction,
               SpringType type, double elasticity, double fatigueResistance, double length)
    : WatchComponent(name, weight, friction),
      m_type(type),
      m_elasticity(elasticity),
      m_fatigueResistance(fatigueResistance),
      m_length(length)
{
    setElasticity(elasticity);
    setFatigueResistance(fatigueResistance);
    setLength(length);
}

// ---------------------------------------------------------
// GETTERS & SETTERS (With Metallurgic Clamping)
// ---------------------------------------------------------
Spring::SpringType Spring::getType() const { return m_type; }
void Spring::setType(SpringType type) { m_type = type; }

double Spring::getElasticity() const { return m_elasticity; }
void Spring::setElasticity(double elasticity) {
    if (elasticity < 0.0) {
        m_elasticity = 0.0;
    } 
    // Enforce realistic physical ceilings based on horological metallurgy
    else if (m_type == SpringType::MAINSPRING && elasticity > 0.95) {
        m_elasticity = 0.95; // 5% mandatory energy loss to internal/barrel friction
    } 
    else if (m_type == SpringType::HAIRSPRING && elasticity > 0.98) {
        m_elasticity = 0.98; // 2% mandatory energy loss to crystalline hysteresis
    } 
    else {
        m_elasticity = elasticity;
    }
}

double Spring::getFatigueResistance() const { return m_fatigueResistance; }
void Spring::setFatigueResistance(double fatigueResistance) {
    if (fatigueResistance < 0.0) {
        m_fatigueResistance = 0.0;
    } 
    // Clamp to high-grade engineering material limits (e.g., max 96% durability efficiency)
    else if (fatigueResistance > 0.96) {
        m_fatigueResistance = 0.96;
    } 
    else {
        m_fatigueResistance = fatigueResistance;
    }
}

double Spring::getLength() const { return m_length; }
void Spring::setLength(double length) {
    if (length < 1.0) m_length = 1.0;
    else m_length = length;
}

// =========================================================
// WATCHCOMPONENT OVERRIDES
// =========================================================

double Spring::calculateEfficiency() const {
    // Hairsprings provide a massive multiplier due to their role in isochronism
    double bonus = (m_type == SpringType::HAIRSPRING) ? 1.5 : 1.0;
    return m_elasticity * m_fatigueResistance * bonus;
}

std::unique_ptr<WatchComponent> Spring::clone() const {
    return std::make_unique<Spring>(*this);
}

std::string Spring::toString() const {
    std::string typeStr = (m_type == SpringType::MAINSPRING) ? "Mainspring" : "Hairspring";
    
    return WatchComponent::toString() + "\n"
           "• Type: " + typeStr + "\n"
           "• Elasticity: " + std::to_string(m_elasticity) + "\n"
           "• Fatigue Res: " + std::to_string(m_fatigueResistance) + "\n"
           "• Length: " + std::to_string(m_length) + "mm";
}

} // namespace Components
} // namespace Genome
} // namespace WatchGA