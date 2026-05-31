#ifndef WATCH_GA_GENOME_WATCHCOMPONENT_H
#define WATCH_GA_GENOME_WATCHCOMPONENT_H

#include "../core/SystemObject.h"
#include <utility>
#include <string>
#include <memory>

namespace WatchGA {
namespace Genome {

class WatchComponent : public Core::SystemObject {
private:
    double m_weight;
    double m_friction;
    double m_x;
    double m_y;
    double m_rotation;

public:
    WatchComponent();
    WatchComponent(const std::string& name, double weight, double friction, double x, double y);
    ~WatchComponent() override = default;

    double GetWeight() const;
    double GetFriction() const;
    double GetX() const;
    double GetY() const;
    double GetRotation() const;

    void SetWeight(double weight);
    void SetFriction(double friction);
    void SetPosition(double x, double y);
    void SetRotation(double rotation);

    virtual double CalculateEfficiency() const = 0;
    virtual std::unique_ptr<WatchComponent> Clone() const = 0;
    std::string ToString() const override;
};

} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_WATCHCOMPONENT_H