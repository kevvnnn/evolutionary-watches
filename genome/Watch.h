#ifndef WATCH_GA_GENOME_WATCH_H
#define WATCH_GA_GENOME_WATCH_H

#include "../core/SystemObject.h"
#include "WatchComponent.h"
#include <vector>
#include <memory>
#include <map>

namespace WatchGA {
namespace Genome {

class Watch : public Core::SystemObject {
private:
    std::vector<std::unique_ptr<WatchComponent>> m_components;
    std::map<unsigned int, std::vector<unsigned int>> m_connections;
    double m_fitnessScore;
    bool m_isValid;

public:
    Watch();
    explicit Watch(const std::string& name);
    Watch(const Watch& other);
    Watch& operator=(const Watch& other);
    ~Watch() override = default;

    void AddComponent(std::unique_ptr<WatchComponent> component);
    bool RemoveComponent(unsigned int componentId);
    WatchComponent* GetComponent(unsigned int componentId) const;
    const std::vector<std::unique_ptr<WatchComponent>>& GetAllComponents() const;
    unsigned int GetComponentCount() const;

    void AddConnection(unsigned int fromComponentId, unsigned int toComponentId);
    bool RemoveConnection(unsigned int fromComponentId, unsigned int toComponentId);
    const std::vector<unsigned int>& GetConnections(unsigned int componentId) const;
    bool AreConnected(unsigned int componentId1, unsigned int componentId2) const;

    double GetFitnessScore() const;
    void SetFitnessScore(double score);
    bool IsValid() const;
    void SetValid(bool valid);
    bool CheckEssentialComponents() const;

    std::unique_ptr<Watch> Clone() const;
    std::string ToString() const override;
};

} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_WATCH_H