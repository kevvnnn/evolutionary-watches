#include "Hand.h"

Hand::Hand(unsigned int id, HandType type, double length, double balance, double weight, double baseFriction)
    : WatchComponent(id, HAND, baseFriction), m_type(type),
    m_length(length), m_balance(balance), m_weight(weight) {

    }

double Hand::calculateEfficiency() const {
    return m_balance * (1.0 - (m_weight * m_length / 1000.0));
}

double Hand::calculateActualFriction() const {
    return getBaseFriction();
}

std::string Hand::getTypeName() const {
    return "Hand";
}

HandType Hand::getHandType() const {
    return m_type;
}