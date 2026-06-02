#ifndef WATCH_GA_GENOME_COMPONENTS_SPRING_H
#define WATCH_GA_GENOME_COMPONENTS_SPRING_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

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
    Spring();
    Spring(const std::string& name, double weight, double friction, double x, double y,
           SpringType type, double elasticity, double fatigueResistance, double length);
    ~Spring() override = default;

    SpringType getType() const;
    double getElasticity() const;
    double getFatigueResistance() const;
    double getLength() const;

    void setType(SpringType type);
    void setElasticity(double elasticity);
    void setFatigueResistance(double resistance);
    void setLength(double length);

    double calculateEfficiency() const override;
    std::unique_ptr<WatchComponent> clone() const override;
    std::string toString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_SPRING_H