#include "Gear.h"
#include <stdexcept>
#include <sstream>  

namespace WatchGA {
namespace Genome {
namespace Components {

// =========================================================
// CONSTRUCTORS
// =========================================================

Gear::Gear()
    : m_toothCount(10),
      m_diameter(5.0),
      m_meshingQuality(0.8),
      m_jewel(nullptr) // Initialize empty jewel pointer
{
}

Gear::Gear(const std::string& name, double weight, double friction,
           unsigned int toothCount, double diameter, double meshingQuality)
    : WatchComponent(name, weight, friction),
      m_toothCount(toothCount),
      m_diameter(diameter),
      m_meshingQuality(meshingQuality),
      m_jewel(nullptr) // Jewels are generated separately via setJewel()
{
    setToothCount(toothCount);
    setDiameter(diameter);
    setMeshingQuality(meshingQuality);
}

// =========================================================
// DEEP COPY SEMANTICS (CRITICAL FOR GA Crossover/Mutation)
// =========================================================

Gear::Gear(const Gear& other)
    : WatchComponent(other), // Copy base class properties
      m_toothCount(other.m_toothCount),
      m_diameter(other.m_diameter),
      m_meshingQuality(other.m_meshingQuality)
{
    // 1. If the parent gear had a jewel, we must safely clone it into the offspring gear.
    // We cannot just copy the pointer, or both gears would share the same physical jewel!
    if (other.m_jewel) {
        m_jewel = std::make_unique<Jewel>(*other.m_jewel);
    }
}

Gear& Gear::operator=(const Gear& other) {
    // 1. Guard against self-assignment
    if (this == &other) return *this;
    
    // 2. Assign base class properties
    WatchComponent::operator=(other);
    
    m_toothCount = other.m_toothCount;
    m_diameter = other.m_diameter;
    m_meshingQuality = other.m_meshingQuality;
    
    // 3. Safely transfer the nested jewel ownership state
    if (other.m_jewel) {
        m_jewel = std::make_unique<Jewel>(*other.m_jewel);
    } else {
        m_jewel.reset(); // If the other gear has no jewel, destroy our current one
    }
    
    return *this;
}

// =========================================================
// JEWEL MANAGEMENT (Composition Interface)
// =========================================================

void Gear::setJewel(std::unique_ptr<Jewel> jewel) {
    // Takes explicit ownership of the memory block
    m_jewel = std::move(jewel);
}

const Jewel* Gear::getJewel() const {
    // Returns a raw observation pointer. The Gear still owns the memory.
    return m_jewel.get();
}

bool Gear::hasJewel() const {
    return m_jewel != nullptr;
}

// =========================================================
// GEAR PROPERTIES (Getters & Setters)
// =========================================================

unsigned int Gear::getToothCount() const { return m_toothCount; }
double Gear::getDiameter() const { return m_diameter; }
double Gear::getMeshingQuality() const { return m_meshingQuality; }

void Gear::setToothCount(unsigned int toothCount) {
    // Prevent the simulation from generating gears that physically cannot interlock
    if (toothCount < 8)
        throw std::invalid_argument("Gear tooth count minimum is 8 to prevent physics engine failures.");
    m_toothCount = toothCount;
}

void Gear::setDiameter(double diameter) {
    // Structural guard against micro-gears
    if (diameter < 1.0)
        throw std::invalid_argument("Gear diameter minimum is 1.0mm.");
    m_diameter = diameter;
}

void Gear::setMeshingQuality(double quality) {
    if (quality < 0.0) m_meshingQuality = 0.0;
    else if (quality > 0.92) m_meshingQuality = 0.92; // 0.92 NOT 1.0, to simulate inevitable energy losses
    else m_meshingQuality = quality;
}

// =========================================================
// WATCHCOMPONENT OVERRIDES
// =========================================================

double Gear::calculateEfficiency() const {
    // Base efficiency evaluates the kinetic transfer penalty of the teeth and diameter
    double efficiency = m_meshingQuality * (1.0 - (1.0 / m_toothCount)) * (1.0 - (m_diameter / 100.0));
    
    // THE HOROLOGICAL EXPLOIT: 
    // If the gear has a jewel, it receives a massive friction reduction multiplier!
    if (hasJewel()) {
        efficiency *= 1.05; // 5% efficiency boost per jeweled gear
    }
    
    return efficiency;
}

std::unique_ptr<WatchComponent> Gear::clone() const {
    return std::make_unique<Gear>(*this);
}

std::string Gear::toString() const {
    std::ostringstream oss;
    
    oss << WatchComponent::toString() << "\n"
        << "• Tooth Count: " << m_toothCount << "\n"
        << "• Diameter: " << m_diameter << "mm\n"
        << "• Meshing Quality: " << m_meshingQuality;
        
    // Actively check for and append the nested sub-component DNA
    if (hasJewel()) {
        oss << "\n• Includes Cap Jewel: " << (m_jewel->isCapJewel() ? "Yes" : "No");
    }
    
    return oss.str();
}

} // namespace Components
} // namespace Genome
} // namespace WatchGA