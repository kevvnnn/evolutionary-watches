#ifndef SPRING_H
#define SPRING_H

#include "../WatchComponent.h"

enum class SpringType {
    MAINSPRING,
    HAIRSPRING
};

class Spring : public WatchComponent {
    private:
    SpringType m_type;
    double m_elasticity;
    double m_fatigueResistance;
    double m_length;

    public:
    Spring(unsigned int id, SpringType type, double elasticity, double fatigueResistance, double length, double baseFriction);
    double calculateEfficiency() const override;
    double calculateActualFriction() const override;
    std::string getTypeName() const override;

    SpringType getSpringType() const;
};

#endif