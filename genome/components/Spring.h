#ifndef WATCH_GA_GENOME_COMPONENTS_SPRING_H
#define WATCH_GA_GENOME_COMPONENTS_SPRING_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

/**
 * @class Spring
 * @brief Represents the kinetic energy storage and regulation components.
 * * The Mainspring provides the raw motive force for the entire watch.
 * The Hairspring provides the delicate counter-force for the Balance Wheel.
 */
class Spring : public WatchComponent {
public:
    enum class SpringType {
        MAINSPRING,
        HAIRSPRING
    };

private:
    SpringType m_type;
    double m_elasticity;
    double m_fatigueResistance;
    double m_length;

public:
    // ---------------------------------------------------------
    // CONSTRUCTORS
    // ---------------------------------------------------------
    Spring();
    Spring(const std::string& name, double weight, double friction,
           SpringType type, double elasticity, double fatigueResistance, double length);
    ~Spring() override = default;

    // ---------------------------------------------------------
    // GETTERS & SETTERS
    // ---------------------------------------------------------
    SpringType getType() const;
    void setType(SpringType type);

    double getElasticity() const;
    void setElasticity(double elasticity);

    double getFatigueResistance() const;
    void setFatigueResistance(double resistance);

    double getLength() const;
    void setLength(double length);

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

#endif // WATCH_GA_GENOME_COMPONENTS_SPRING_H