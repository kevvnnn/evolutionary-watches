#ifndef WATCH_GA_GENOME_COMPONENTS_GEAR_H
#define WATCH_GA_GENOME_COMPONENTS_GEAR_H

#include "../WatchComponent.h"
#include "Jewel.h" // Make sure this path is correct for your directory structure
#include <memory>
#include <string>

namespace WatchGA {
namespace Genome {
namespace Components {

// Gear component for power transmission
class Gear : public WatchComponent {
private:
    unsigned int m_toothCount;
    double m_diameter;
    double m_meshingQuality;
    
    // The Gear owns its Jewel (Composition)
    std::unique_ptr<Jewel> m_jewel;

public:
    // Constructors & Destructor
    Gear();
    Gear(const std::string& name, double weight, double friction, double x, double y,
         unsigned int toothCount, double diameter, double meshingQuality);
    ~Gear() override = default;

    // Deep copy semantics (required because we own a unique_ptr)
    Gear(const Gear& other);
    Gear& operator=(const Gear& other);

    // ---------------------------------------------------------
    // Jewel Management
    // ---------------------------------------------------------
    void setJewel(std::unique_ptr<Jewel> jewel);
    const Jewel* getJewel() const;
    bool hasJewel() const;

    // ---------------------------------------------------------
    // Gear Properties (Getters & Setters)
    // ---------------------------------------------------------
    unsigned int getToothCount() const;
    void setToothCount(unsigned int toothCount);

    double getDiameter() const;
    void setDiameter(double diameter);

    double getMeshingQuality() const;
    void setMeshingQuality(double quality);

    // ---------------------------------------------------------
    // WatchComponent Overrides
    // ---------------------------------------------------------
    double calculateEfficiency() const override;
    std::unique_ptr<WatchComponent> clone() const override;
    std::string toString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_GEAR_H