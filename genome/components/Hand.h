#ifndef WATCH_GA_GENOME_COMPONENTS_HAND_H
#define WATCH_GA_GENOME_COMPONENTS_HAND_H

#include "../WatchComponent.h"

namespace WatchGA {
namespace Genome {
namespace Components {

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

    HandType GetType() const;
    double GetLength() const;
    double GetBalance() const;

    void SetType(HandType type);
    void SetLength(double length);
    void SetBalance(double balance);

    double CalculateEfficiency() const override;
    std::unique_ptr<WatchComponent> Clone() const override;
    std::string ToString() const override;
};

} // namespace Components
} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_COMPONENTS_HAND_H