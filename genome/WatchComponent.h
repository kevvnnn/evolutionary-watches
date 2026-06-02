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

    double getWeight() const;
    double getFriction() const;
    double getX() const;
    double getY() const;
    double getRotation() const;

    void setWeight(double weight);
    void setFriction(double friction);
    void setPosition(double x, double y);
    void setRotation(double rotation);

    virtual double calculateEfficiency() const = 0;
    virtual std::unique_ptr<WatchComponent> clone() const = 0;
    std::string toString() const override;
};

} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_WATCHCOMPONENT_H