#ifndef WATCH_GA_GENOME_COMPONENTS_BALANCEWHEEL_H
#define WATCH_GA_GENOME_COMPONENTS_BALANCEWHEEL_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

// Critical timekeeping component
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

    // Getters
    double getMomentOfInertia() const;
    double getIsochronism() const;
    double getAmplitude() const;

    // Setters
    void setMomentOfInertia(double moi);
    void setIsochronism(double isochronism);
    void setAmplitude(double amplitude);

    // Overrides
    double calculateEfficiency() const override;
    std::unique_ptr<WatchComponent> clone() const override;
    std::string toString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_BALANCEWHEEL_H