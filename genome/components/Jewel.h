#ifndef JEWEL_H
#define JEWEL_H

#include "../WatchComponent.h"

class Jewel : public WatchComponent {
    private:
    double m_hardness;
    bool m_isCapJewel;

    public:
    Jewel(unsigned int id, double hardness, bool isCapJewel);
    double calculateEfficiency() const override;
    std::string getTypeName() const override;
};


#endif