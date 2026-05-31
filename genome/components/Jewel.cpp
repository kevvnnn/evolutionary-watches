#include "Jewel.h"

using namespace WatchGA::Genome;
using namespace WatchGA::Genome::Components;

Jewel::Jewel()
    : m_hardness(9.0),  // Sapphire/ruby hardness
      m_isCapJewel(false)
{
}

Jewel::Jewel(const std::string& name, double weight, double friction, double x, double y,
             double hardness, bool isCapJewel)
    : WatchComponent(name, weight, friction, x, y),
      m_hardness(hardness),
      m_isCapJewel(isCapJewel)
{
    SetHardness(hardness);
}

// Getters
double Jewel::GetHardness() const { return m_hardness; }
bool Jewel::IsCapJewel() const { return m_isCapJewel; }

// Hardness clamped 0–10 (Mohs scale)
void Jewel::SetHardness(double hardness) {
    if (hardness < 0.0) m_hardness = 0.0;
    else if (hardness > 10.0) m_hardness = 10.0;
    else m_hardness = hardness;
}

void Jewel::SetIsCapJewel(bool isCapJewel) {
    m_isCapJewel = isCapJewel;
}

// Jewel efficiency = hardness + cap bonus
double Jewel::CalculateEfficiency() const {
    return (m_hardness / 10.0) * (m_isCapJewel ? 1.2 : 1.0);
}

// Deep copy
std::unique_ptr<WatchComponent> Jewel::Clone() const {
    return std::make_unique<Jewel>(*this);
}

std::string Jewel::ToString() const {
    return WatchComponent::ToString() +
           " [Jewel | Hardness: " + std::to_string(m_hardness) +
           ", Cap: " + (m_isCapJewel ? "Yes" : "No") + "]";
}