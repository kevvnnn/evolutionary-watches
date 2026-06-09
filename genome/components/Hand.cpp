#include "Hand.h"
#include <stdexcept>

namespace WatchGA {
namespace Genome {
namespace Components {

// ---------------------------------------------------------
// CONSTRUCTORS
// ---------------------------------------------------------
Hand::Hand()
    : m_type(HandType::HOUR),
      m_length(10.0),
      m_balance(0.9)
{
}

Hand::Hand(const std::string& name, double weight, double friction,
           HandType type, double length, double balance)
    : WatchComponent(name, weight, friction),
      m_type(type),
      m_length(length),
      m_balance(balance)
{
    setLength(length);
    setBalance(balance);
}

// ---------------------------------------------------------
// GETTERS & SETTERS
// ---------------------------------------------------------
Hand::HandType Hand::getType() const { return m_type; }
void Hand::setType(HandType type) { m_type = type; }

double Hand::getLength() const { return m_length; }
void Hand::setLength(double length) {
    if (length < 5.0)
        throw std::invalid_argument("Hand length minimum is 5mm.");
    m_length = length;
}

double Hand::getBalance() const { return m_balance; }
void Hand::setBalance(double balance) {
    if (balance < 0.0) m_balance = 0.0;
    else if (balance > 1.0) m_balance = 1.0;
    else m_balance = balance;
}

// =========================================================
// WATCHCOMPONENT OVERRIDES
// =========================================================

double Hand::calculateEfficiency() const {
    // Heavier, longer hands drain more power. High balance mitigates this.
    return m_balance * (1.0 - (getWeight() * m_length / 1000.0));
}

std::unique_ptr<WatchComponent> Hand::clone() const {
    return std::make_unique<Hand>(*this);
}

std::string Hand::toString() const {
    std::string typeStr;
    switch (m_type) {
        case HandType::HOUR: typeStr = "Hour Hand"; break;
        case HandType::MINUTE: typeStr = "Minute Hand"; break;
        case HandType::SECOND: typeStr = "Second Hand"; break;
    }
    
    return WatchComponent::toString() + "\n"
           "• Type: " + typeStr + "\n"
           "• Balance: " + std::to_string(m_balance) + "\n"
           "• Length: " + std::to_string(m_length) + "mm";
}

} // namespace Components
} // namespace Genome
} // namespace WatchGA