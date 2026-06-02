#include "Gear.h"
#include <stdexcept>

namespace WatchGA {
namespace Genome {
namespace Components {

// Default gear
Gear::Gear()
    : m_toothCount(10),
      m_diameter(5.0),
      m_meshingQuality(0.8)
{
}

// Full constructor with validation
Gear::Gear(const std::string& name, double weight, double friction, double x, double y,
           unsigned int toothCount, double diameter, double meshingQuality)
    : WatchComponent(name, weight, friction, x, y),
      m_toothCount(toothCount),
      m_diameter(diameter),
      m_meshingQuality(meshingQuality)
{
    setToothCount(toothCount);
    setDiameter(diameter);
    setMeshingQuality(meshingQuality);
}

// Getter
unsigned int Gear::getToothCount() const { return m_toothCount; }
double Gear::getDiameter() const { return m_diameter; }
double Gear::getMeshingQuality() const { return m_meshingQuality; }

// Validation
void Gear::setToothCount(unsigned int toothCount) {
    if (toothCount < 8)
        throw std::invalid_argument("Tooth count minimum is 8.");
    m_toothCount = toothCount;
}

void Gear::setDiameter(double diameter) {
    if (diameter < 1.0)
        throw std::invalid_argument("Diameter minimum is 1mm.");
    m_diameter = diameter;
}

void Gear::setMeshingQuality(double quality) {
    if (quality < 0.0) m_meshingQuality = 0.0;
    else if (quality > 1.0) m_meshingQuality = 1.0;
    else m_meshingQuality = quality;
}

// Gear efficiency formula
double Gear::calculateEfficiency() const {
    return m_meshingQuality * (1.0 - (1.0 / m_toothCount)) * (1.0 - (m_diameter / 100.0));
}

// Deep copy for GA
std::unique_ptr<WatchComponent> Gear::clone() const {
    return std::make_unique<Gear>(*this);
}

// Debug string
std::string Gear::toString() const {
    return WatchComponent::toString() +
           " [Gear | Teeth: " + std::to_string(m_toothCount) +
           ", Diameter: " + std::to_string(m_diameter) + "mm]";
}

} // namespace Components
} // namespace Genome
} // namespace WatchGA