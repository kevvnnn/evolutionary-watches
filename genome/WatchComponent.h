#ifndef WATCH_GA_GENOME_WATCHCOMPONENT_H
#define WATCH_GA_GENOME_WATCHCOMPONENT_H

#include "../core/SystemObject.h"
#include <string>
#include <memory>

namespace WatchGA {
namespace Genome {

/**
 * @class WatchComponent
 * @brief The abstract polymorphic base class for all physical watch parts.
 * * Every piece of genetic material (Gears, Jewels, Springs, etc.) inherits from this.
 * It enforces standard properties like Weight and Friction, and mandates that all 
 * components know how to `clone()` themselves for the Genetic Algorithm.
 */
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

    // ---------------------------------------------------------
    // GETTERS & SETTERS
    // ---------------------------------------------------------
    std::string getName() const;
    double getWeight() const;
    double getFriction() const;

    void setWeight(double weight);
    void setFriction(double friction);

    // =========================================================
    // POLYMORPHIC INTERFACES (Must be overridden by children)
    // =========================================================
    /**
     * @brief Calculates the horological efficiency specific to the component type
     */
    virtual double calculateEfficiency() const = 0;
    
    /**
     * @brief Deep clone generator. Critical for safe genetic crossover.
     */
    virtual std::unique_ptr<WatchComponent> clone() const = 0;
    
    /**
     * @brief Returns the properties formatted cleanly for the Qt UI Inspector
     */
    virtual std::string toString() const;
};

} // namespace Genome
} // namespace WatchGA

#endif