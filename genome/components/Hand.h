#ifndef WATCH_GA_GENOME_COMPONENTS_HAND_H
#define WATCH_GA_GENOME_COMPONENTS_HAND_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

/**
 * @class Hand
 * @brief Represents the visual time indicators (Hour, Minute, Second).
 * * Hands must be physically balanced to prevent draining torque from the gear train.
 * Their length is strictly audited by the FitnessEvaluator to prevent scraping the case.
 */
class Hand : public WatchComponent {
public:
    enum class HandType {
        HOUR,
        MINUTE,
        SECOND
    };

private:
    HandType m_type;
    double m_length;
    double m_balance;

public:
    // ---------------------------------------------------------
    // CONSTRUCTORS
    // ---------------------------------------------------------
    Hand();
    Hand(const std::string& name, double weight, double friction,
         HandType type, double length, double balance);
    ~Hand() override = default;

    // ---------------------------------------------------------
    // GETTERS & SETTERS
    // ---------------------------------------------------------
    HandType getType() const;
    void setType(HandType type);

    double getLength() const;
    void setLength(double length);

    double getBalance() const;
    void setBalance(double balance);

    // =========================================================
    // WATCHCOMPONENT OVERRIDES
    // =========================================================
    double calculateEfficiency() const override;
    std::unique_ptr<WatchComponent> clone() const override;
    std::string toString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_HAND_H