#ifndef WATCH_GA_GENOME_COMPONENTS_BALANCEWHEEL_H
#define WATCH_GA_GENOME_COMPONENTS_BALANCEWHEEL_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

/**
 * @class BalanceWheel
 * @brief Represents the "heartbeat" and regulating organ of the watch.
 * * It swings back and forth, determining the fundamental accuracy of the timepiece.
 * Its kinetic amplitude must be carefully guarded by the AI to prevent knocking.
 */
class BalanceWheel : public WatchComponent {
private:
    double m_momentOfInertia;
    double m_isochronism;
    double m_amplitude;

public:
    // ---------------------------------------------------------
    // CONSTRUCTORS
    // ---------------------------------------------------------
    BalanceWheel();
    BalanceWheel(const std::string& name, double weight, double friction,
                 double momentOfInertia, double isochronism, double amplitude);
    ~BalanceWheel() override = default;

    // ---------------------------------------------------------
    // GETTERS & SETTERS
    // ---------------------------------------------------------
    double getMomentOfInertia() const;
    void setMomentOfInertia(double moi);

    double getIsochronism() const;
    void setIsochronism(double isochronism);

    double getAmplitude() const;
    void setAmplitude(double amplitude);

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

#endif // WATCH_GA_GENOME_COMPONENTS_BALANCEWHEEL_H