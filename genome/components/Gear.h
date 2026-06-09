#ifndef WATCH_GA_GENOME_COMPONENTS_GEAR_H
#define WATCH_GA_GENOME_COMPONENTS_GEAR_H

#include "../WatchComponent.h"
#include "Jewel.h" 
#include <memory>
#include <string>

namespace WatchGA {
namespace Genome {
namespace Components {

/**
 * @class Gear
 * @brief Represents a power transmission wheel in the watch train.
 * * STRUCTURAL NOTE: This class utilizes Object-Oriented Composition.
 * A Gear physically "owns" a Jewel bearing via a std::unique_ptr, guaranteeing
 * at the memory level that a single gear can never hoard more than one jewel.
 */
class Gear : public WatchComponent {
private:
    unsigned int m_toothCount;
    double m_diameter;
    double m_meshingQuality;
    
    // The Gear owns its Jewel (Composition)
    std::unique_ptr<Jewel> m_jewel;

public:
    // ---------------------------------------------------------
    // CONSTRUCTORS
    // ---------------------------------------------------------
    Gear();
    Gear(const std::string& name, double weight, double friction,
         unsigned int toothCount, double diameter, double meshingQuality);
    ~Gear() override = default;

    // ---------------------------------------------------------
    // DEEP COPY SEMANTICS (Required because we own a unique_ptr)
    // ---------------------------------------------------------
    Gear(const Gear& other);
    Gear& operator=(const Gear& other);

    // =========================================================
    // JEWEL MANAGEMENT (Composition Interface)
    // =========================================================
    void setJewel(std::unique_ptr<Jewel> jewel);
    const Jewel* getJewel() const;
    bool hasJewel() const;

    // ---------------------------------------------------------
    // GEAR PROPERTIES (Getters & Setters)
    // ---------------------------------------------------------
    unsigned int getToothCount() const;
    void setToothCount(unsigned int toothCount);

    double getDiameter() const;
    void setDiameter(double diameter);

    double getMeshingQuality() const;
    void setMeshingQuality(double quality);

    // =========================================================
    // WATCHCOMPONENT OVERRIDES
    // =========================================================
    double calculateEfficiency() const override;
    std::unique_ptr<WatchComponent> clone() const override;
    std::string toString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_GEAR_H