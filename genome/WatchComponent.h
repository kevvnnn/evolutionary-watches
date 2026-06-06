#ifndef WATCH_GA_GENOME_WATCHCOMPONENT_H
#define WATCH_GA_GENOME_WATCHCOMPONENT_H

#include "../core/SystemObject.h"
#include <string>
#include <memory>

namespace WatchGA {
namespace Genome {

class WatchComponent: public Core::SystemObject
{
protected:
    std::string m_name;
    double m_weight;
    double m_friction;
    double m_x;
    double m_y;

public:
    WatchComponent();
    WatchComponent(const std::string& name, double weight, double friction, double x, double y);
    virtual ~WatchComponent() = default;

    // Getters
    std::string getName() const;
    double getWeight() const;
    double getFriction() const;
    double getX() const;
    double getY() const;

    // Setters
    void setWeight(double weight);
    void setFriction(double friction);
    void setPosition(double x, double y);

    // Virtual methods
    virtual double calculateEfficiency() const = 0;
    virtual std::unique_ptr<WatchComponent> clone() const = 0;
    virtual std::string toString() const;
};

} // namespace Genome
} // namespace WatchGA

#endif