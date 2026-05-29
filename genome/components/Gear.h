#ifndef GEAR_H
#define GEAR_H

#include "../WatchComponent.h"

class Gear : public WatchComponent {
    private:
    unsigned int m_toothCount;
    double m_diameter;
    double m_meshingQuality;

    public:
    Gear(unsigned int id, unsigned int teeth, double diameter, double meshQuality, double baseFriction);
    double calculateEfficiency() const override;
    double calculateActualFriction () const override;
    std::string getTypeName() const override;

    unsigned int getToothCount() const;
};

#endif