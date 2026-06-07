#include "Gear.h"
#include <stdexcept>
#include <sstream>  // For clean string formatting

namespace WatchGA {
namespace Genome {
namespace Components {

// ---------------------------------------------------------
// Constructors
// ---------------------------------------------------------

// Default gear
Gear::Gear()
    : m_toothCount(10),
      m_diameter(5.0),
      m_meshingQuality(0.8),
      m_jewel(nullptr) // Initialize empty jewel pointer
{
}

// Full constructor with validation
Gear::Gear(const std::string& name, double weight, double friction, double x, double y,
           unsigned int toothCount, double diameter, double meshingQuality)
    : WatchComponent(name, weight, friction, x, y),
      m_toothCount(toothCount),
      m_diameter(diameter),
      m_meshingQuality(meshingQuality),
      m_jewel(nullptr) // Jewels are added separately via setJewel()
{
    setToothCount(toothCount);
    setDiameter(diameter);
    setMeshingQuality(meshingQuality);
}

// ---------------------------------------------------------
// Deep Copy Semantics (CRITICAL FOR GA Crossover/Mutation)
// ---------------------------------------------------------

// Copy Constructor
Gear::Gear(const Gear& other)
    : WatchComponent(other), // Copy base class properties
      m_toothCount(other.m_toothCount),
      m_diameter(other.m_diameter),
      m_meshingQuality(other.m_meshingQuality)
{
    // If the parent gear had a jewel, clone it into the offspring gear
    if (other.m_jewel) {
        m_jewel = std::make_unique<Jewel>(*other.m_jewel);
    }
}

// Assignment Operator
Gear& Gear::operator=(const Gear& other) {
    if (this == &other) return *this;
    
    WatchComponent::operator=(other); // Assign base class properties
    
    m_toothCount = other.m_toothCount;
    m_diameter = other.m_diameter;
    m_meshingQuality = other.m_meshingQuality;
    
    // Safely transfer jewel ownership state
    if (other.m_jewel) {
        m_jewel = std::make_unique<Jewel>(*other.m_jewel);
    } else {
        m_jewel.reset();
    }
    
    return *this;
}

// ---------------------------------------------------------
// Jewel Management
// ---------------------------------------------------------

void Gear::setJewel(std::unique_ptr<Jewel> jewel) {
    m_jewel = std::move(jewel);
}

const Jewel* Gear::getJewel() const {
    return m_jewel.get();
}

bool Gear::hasJewel() const {
    return m_jewel != nullptr;
}

// ---------------------------------------------------------
// Gear Properties (Getters & Setters)
// ---------------------------------------------------------

unsigned int Gear::getToothCount() const { return m_toothCount; }
double Gear::getDiameter() const { return m_diameter; }
double Gear::getMeshingQuality() const { return m_meshingQuality; }

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
    else if (quality > 0.92) m_meshingQuality = 0.92;
    // 0.92 NOT 1.0, to simulate inevitable energy losses due to friction
    else m_meshingQuality = quality;
}

// ---------------------------------------------------------
// WatchComponent Overrides
// ---------------------------------------------------------

// Gear efficiency formula
double Gear::calculateEfficiency() const {
    double efficiency = m_meshingQuality * (1.0 - (1.0 / m_toothCount)) * (1.0 - (m_diameter / 100.0));
    
    // If the gear has a jewel, it receives a massive friction reduction bonus!
    if (hasJewel()) {
        efficiency *= 1.05; // 5% efficiency boost per jeweled gear
    }
    
    return efficiency;
}

// Deep copy clone for GA population generation
std::unique_ptr<WatchComponent> Gear::clone() const {
    return std::make_unique<Gear>(*this);
}

// Debug string for Assembly Manifest — BULLET POINT FORMAT
std::string Gear::toString() const {
    std::ostringstream oss;
    oss << WatchComponent::toString() << "\n"
        << "• Tooth Count: " << m_toothCount << "\n"
        << "• Diameter: " << m_diameter << "mm\n"
        << "• Meshing Quality: " << m_meshingQuality;
    
    // Append jewel info if present
    if (hasJewel()) {
        oss << "\n• Includes Cap Jewel: " << (m_jewel->isCapJewel() ? "Yes" : "No");
    }
    
    return oss.str();
}

} // namespace Components
} // namespace Genome
} // namespace WatchGA