#ifndef HAND_H
#define HAND_H

#include "../WatchComponent.h"

enum class HandType {
    HOUR,
    MINUTE,
    SECOND
};

class Hand : public WatchComponent {
    private:
    HandType m_type;
    double m_length;
    double m_balance;
    double m_weight;

    public:
    Hand(unsigned int id, HandType type, double length, double balance, double weight = 0.5, double baseFriction = 0.05);
    double calculateEfficiency() const override;
    double calculateActualFriction() const override;
    std::string getTypeName() const override;

    HandType getHandType() const;
};

#endif