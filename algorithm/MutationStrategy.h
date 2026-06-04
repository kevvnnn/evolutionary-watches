#ifndef WATCH_GA_ALGORITHM_MUTATION_STRATEGY_H
#define WATCH_GA_ALGORITHM_MUTATION_STRATEGY_H

#include "../core/IMutationStrategy.h"

namespace WatchGA {
namespace Algorithm {

/**
 * @class SwapMutation
 * @ownership Person B
 */
class SwapMutation : public Core::IMutationStrategy
{
public:
    void mutate(std::shared_ptr<Genome::Watch>& watch, double rate) const override;
    std::string getName() const override;
};

/**
 * @class ParameterMutation
 * @ownership Person B
 */
class ParameterMutation : public Core::IMutationStrategy
{
private:
    double m_mutationStrength;

public:
    explicit ParameterMutation(double strength = 0.1);

    double getMutationStrength() const;
    void setMutationStrength(double strength);

    void mutate(std::shared_ptr<Genome::Watch>& watch, double rate) const override;
    std::string getName() const override;
};

/**
 * @class AddRemoveMutation
 * @ownership Person B
 */
class AddRemoveMutation : public Core::IMutationStrategy
{
private:
    unsigned int m_maxComponents;

public:
    explicit AddRemoveMutation(unsigned int max = 50);

    unsigned int getMaxComponents() const;
    void setMaxComponents(unsigned int max);

    void mutate(std::shared_ptr<Genome::Watch>& watch, double rate) const override;
    std::string getName() const override;
};

} // namespace Algorithm
} // namespace WatchGA

#endif