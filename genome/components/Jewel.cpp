#include "Jewel.h"

using namespace WatchGA::Genome;
using namespace WatchGA::Genome::Components;

Jewel::Jewel()
    : m_hardness(9.0),  // Real life sapphire/ruby hardness
      m_isCapJewel(false)
{
}

Jewel::Jewel(const std::string& name, double weight, double friction, double x, double y,
             double hardness, bool isCapJewel)
    : WatchComponent(name, weight, friction, x, y),
      m_hardness(hardness),
      m_isCapJewel(isCapJewel)
{
    setHardness(hardness);
}

// Getters
double Jewel::getHardness() const { return m_hardness; }
bool Jewel::isCapJewel() const { return m_isCapJewel; }

// Hardness clamped 0-10 (Mohs scale)
void Jewel::setHardness(double hardness) {
    if (hardness < 0.0) m_hardness = 0.0;
    else if (hardness > 10.0) m_hardness = 10.0;
    else m_hardness = hardness;
}

void Jewel::setIsCapJewel(bool isCapJewel) {
    m_isCapJewel = isCapJewel;
}

// Jewel efficiency = hardness + cap bonus
double Jewel::calculateEfficiency() const {
    return (m_hardness / 10.0) * (m_isCapJewel ? 1.2 : 1.0);
}

// Deep copy
std::unique_ptr<WatchComponent> Jewel::clone() const {
    return std::make_unique<Jewel>(*this);
}

std::string Jewel::toString() const {
    return WatchComponent::toString() +
           " [Jewel | Hardness: " + std::to_string(m_hardness) +
           ", Cap: " + (m_isCapJewel ? "Yes" : "No") + "]";
}