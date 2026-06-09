#ifndef WATCH_GA_GENOME_COMPONENTS_JEWEL_H
#define WATCH_GA_GENOME_COMPONENTS_JEWEL_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

// Jewel bearing for low-friction movement
class Jewel : public WatchComponent {
private:
    double m_hardness;
    bool m_isCapJewel;

public:
    Jewel();
    Jewel(const std::string& name, double weight, double friction,
          double hardness, bool isCapJewel);
    ~Jewel() override = default;

    // Getters
    double getHardness() const;
    bool isCapJewel() const;

    // Setters
    void setHardness(double hardness);
    void setIsCapJewel(bool isCapJewel);

    // Overrides
    double calculateEfficiency() const override;
    std::unique_ptr<WatchComponent> clone() const override;
    std::string toString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_JEWEL_H