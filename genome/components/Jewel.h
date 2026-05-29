#ifndef JEWEL_H
#define JEWEL_H

#include "../WatchComponent.h"

class Jewel : public WatchComponent {
    private:
    double m_hardness;
    bool m_isCapJewel;
    double m_connectedComponentFriction;

    public:
    Jewel(unsigned int id, double hardness, bool isCapJewel, double baseFriction = 0.03);
    double calculateEfficiency() const override;
    double calculateActualFriction() const override;
    std::string getTypeName() const override;

    void setConnectedFriction(double friction);
    bool isPlacedCorrectly() const;
    double getPlacementBonus() const;
    double getPlacementPenalty() const;
};


#endif