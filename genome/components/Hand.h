#ifndef WATCH_GA_GENOME_COMPONENTS_HAND_H
#define WATCH_GA_GENOME_COMPONENTS_HAND_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

// Time indicator hands
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
    Hand();
    Hand(const std::string& name, double weight, double friction, double x, double y,
         HandType type, double length, double balance);
    ~Hand() override = default;

    // Getters
    HandType getType() const;
    double getLength() const;
    double getBalance() const;

    // Setters
    void setType(HandType type);
    void setLength(double length);
    void setBalance(double balance);

    // Overrides
    double calculateEfficiency() const override;
    std::unique_ptr<WatchComponent> clone() const override;
    std::string toString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_HAND_H