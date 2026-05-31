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

    SpringType GetType() const;
    double GetElasticity() const;
    double GetFatigueResistance() const;
    double GetLength() const;

    void SetType(SpringType type);
    void SetElasticity(double elasticity);
    void SetFatigueResistance(double resistance);
    void SetLength(double length);

    double CalculateEfficiency() const override;
    std::unique_ptr<WatchComponent> Clone() const override;
    std::string ToString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_SPRING_H