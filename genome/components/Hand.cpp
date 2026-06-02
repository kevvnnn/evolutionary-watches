#include "Hand.h"
#include <stdexcept>

namespace WatchGA {
namespace Genome {
namespace Components {

Hand::Hand()
    : m_type(HandType::HOUR),
      m_length(10.0),
      m_balance(0.9)
{
}

Hand::Hand(const std::string& name, double weight, double friction, double x, double y,
           HandType type, double length, double balance)
    : WatchComponent(name, weight, friction, x, y),
      m_type(type),
      m_length(length),
      m_balance(balance)
{
    setLength(length);
    setBalance(balance);
}

// Getters
Hand::HandType Hand::getType() const { return m_type; }
double Hand::getLength() const { return m_length; }
double Hand::getBalance() const { return m_balance; }

void Hand::setType(HandType type) { m_type = type; }

void Hand::setLength(double length) {
    if (length < 5.0)
        throw std::invalid_argument("Hand length minimum is 5mm.");
    m_length = length;
}

void Hand::setBalance(double balance) {
    if (balance < 0.0) m_balance = 0.0;
    else if (balance > 1.0) m_balance = 1.0;
    else m_balance = balance;
}

// Efficiency depends on balance and weight
double Hand::calculateEfficiency() const {
    return m_balance * (1.0 - (getWeight() * m_length / 1000.0));
}

// Deep copy
std::unique_ptr<WatchComponent> Hand::clone() const {
    return std::make_unique<Hand>(*this);
}

std::string Hand::toString() const {
    std::string typeStr;
    switch (m_type) {
        case HandType::HOUR: typeStr = "Hour"; break;
        case HandType::MINUTE: typeStr = "Minute"; break;
        case HandType::SECOND: typeStr = "Second"; break;
    }
    return WatchComponent::toString() + " [" + typeStr + " Hand]";
}

} // namespace Components
} // namespace Genome
} // namespace WatchGA