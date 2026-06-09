#include "Jewel.h"

namespace WatchGA {
namespace Genome {
namespace Components {

// ---------------------------------------------------------
// CONSTRUCTORS
// ---------------------------------------------------------
Jewel::Jewel()
    : m_hardness(9.0),  // Default to real-life sapphire/ruby hardness
      m_isCapJewel(false)
{
}

Jewel::Jewel(const std::string& name, double weight, double friction,
             double hardness, bool isCapJewel)
    : WatchComponent(name, weight, friction),
      m_hardness(hardness),
      m_isCapJewel(isCapJewel)
{
    setHardness(hardness);
}

// ---------------------------------------------------------
// GETTERS & SETTERS
// ---------------------------------------------------------
double Jewel::getHardness() const { return m_hardness; }
bool Jewel::isCapJewel() const { return m_isCapJewel; }

void Jewel::setHardness(double hardness) {
    // Safely clamp the hardness to the Mohs hardness scale bounds (0-10)
    if (hardness < 0.0) m_hardness = 0.0;
    else if (hardness > 10.0) m_hardness = 10.0; // 10.0 represents Diamond
    else m_hardness = hardness;
}

void Jewel::setIsCapJewel(bool isCapJewel) {
    m_isCapJewel = isCapJewel;
}

// =========================================================
// WATCHCOMPONENT OVERRIDES
// =========================================================

double Jewel::calculateEfficiency() const {
    // Jewels provide pure, massive friction reduction.
    // Cap jewels absorb vertical shock, granting a 20% multiplier.
    return (m_hardness / 10.0) * (m_isCapJewel ? 1.2 : 1.0);
}

std::unique_ptr<WatchComponent> Jewel::clone() const {
    return std::make_unique<Jewel>(*this);
}

std::string Jewel::toString() const {
    return WatchComponent::toString() + "\n"
           "• Type: Jewel\n"
           "• Hardness: " + std::to_string(m_hardness) + "\n"
           "• Cap: " + (m_isCapJewel ? "Yes" : "No");
}

} // namespace Components
} // namespace Genome
} // namespace WatchGA