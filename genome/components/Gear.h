#ifndef WATCH_GA_GENOME_COMPONENTS_GEAR_H
#define WATCH_GA_GENOME_COMPONENTS_GEAR_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

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

    unsigned int GetToothCount() const;
    double GetDiameter() const;
    double GetMeshingQuality() const;

    void SetToothCount(unsigned int toothCount);
    void SetDiameter(double diameter);
    void SetMeshingQuality(double quality);

    double CalculateEfficiency() const override;
    std::unique_ptr<WatchComponent> Clone() const override;
    std::string ToString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_GEAR_H