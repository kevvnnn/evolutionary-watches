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

public:
    WatchComponent();
    WatchComponent(const std::string& name, double weight, double friction);
    virtual ~WatchComponent() = default;

    // Getters
    std::string getName() const;
    double getWeight() const;
    double getFriction() const;

    // Setters
    void setWeight(double weight);
    void setFriction(double friction);

    // Virtual methods
    virtual double calculateEfficiency() const = 0;
    virtual std::unique_ptr<WatchComponent> clone() const = 0;
    virtual std::string toString() const;
};

} // namespace Genome
} // namespace WatchGA

#endif