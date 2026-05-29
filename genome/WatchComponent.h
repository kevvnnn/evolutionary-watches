#ifndef WATCHCOMPONENT_H
#define WATCHCOMPONENT_H

#include <string>

enum ComponentType { // Unscoped enum, Can assign specific values for different parts. Good for the rng part
    GEAR, // 0
    JEWEL, // 1
    BALANCE_WHEEL, // 2
    SPRING, // 3
    HAND // 4
};

class WatchComponent {
    protected:
    unsigned int m_componentID; //id is for like gear 1, hand 1 etc.
    ComponentType m_componentType;
    double m_baseFriction;

    public:
    WatchComponent(unsigned int id, ComponentType type, double baseFriction);
    virtual ~WatchComponent();

    unsigned int getID() const;
    ComponentType getComponentType() const;
    double getBaseFriction() const;

    virtual double calculateEfficiency() const = 0;
    virtual double calculateActualFriction() const = 0;
    virtual std::string getTypeName() const = 0;
};

#endif