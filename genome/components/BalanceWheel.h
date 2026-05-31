#ifndef WATCH_GA_GENOME_COMPONENTS_BALANCEWHEEL_H
#define WATCH_GA_GENOME_COMPONENTS_BALANCEWHEEL_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

class BalanceWheel : public WatchComponent {
private:
    double m_momentOfInertia;
    double m_isochronism;
    double m_amplitude;

public:
    BalanceWheel();
    BalanceWheel(const std::string& name, double weight, double friction, double x, double y,
                 double momentOfInertia, double isochronism, double amplitude);
    ~BalanceWheel() override = default;

    double GetMomentOfInertia() const;
    double GetIsochronism() const;
    double GetAmplitude() const;

    void SetMomentOfInertia(double moi);
    void SetIsochronism(double isochronism);
    void SetAmplitude(double amplitude);

    double CalculateEfficiency() const override;
    std::unique_ptr<WatchComponent> Clone() const override;
    std::string ToString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_BALANCEWHEEL_H