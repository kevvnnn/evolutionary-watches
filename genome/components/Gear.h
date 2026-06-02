#ifndef WATCH_GA_GENOME_COMPONENTS_GEAR_H
#define WATCH_GA_GENOME_COMPONENTS_GEAR_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

// Gear component for power transmission
class Gear : public WatchComponent {
private:
    unsigned int m_toothCount;
    double m_diameter;
    double m_meshingQuality;

public:
    Gear();
    Gear(const std::string& name, double weight, double friction, double x, double y,
         unsigned int toothCount, double diameter, double meshingQuality);
    ~Gear() override = default;

    // Getters
    unsigned int getToothCount() const;
    double getDiameter() const;
    double getMeshingQuality() const;

    // Setters
    void setToothCount(unsigned int toothCount);
    void setDiameter(double diameter);
    void setMeshingQuality(double quality);

    // Overrides
    double calculateEfficiency() const override;
    std::unique_ptr<WatchComponent> clone() const override;
    std::string toString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_GEAR_H