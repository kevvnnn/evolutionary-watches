#ifndef WATCH_GA_GENOME_COMPONENTS_JEWEL_H
#define WATCH_GA_GENOME_COMPONENTS_JEWEL_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

class Jewel : public WatchComponent {
private:
    double m_hardness;
    bool m_isCapJewel;

public:
    Jewel();
    Jewel(const std::string& name, double weight, double friction, double x, double y,
          double hardness, bool isCapJewel);
    ~Jewel() override = default;

    double GetHardness() const;
    bool IsCapJewel() const;

    void SetHardness(double hardness);
    void SetIsCapJewel(bool isCapJewel);

    double CalculateEfficiency() const override;
    std::unique_ptr<WatchComponent> Clone() const override;
    std::string ToString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_JEWEL_H