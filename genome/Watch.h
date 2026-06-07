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

    int m_canvasWidth  = 600;  // Default fallback size
    int m_canvasHeight = 600;

public:
    Watch();
    explicit Watch(const std::string& name);
    Watch(const Watch& other);
    Watch& operator=(const Watch& other);
    ~Watch() override = default;

    // Component management
    void addComponent(std::unique_ptr<WatchComponent> component);
    bool removeComponent(unsigned int componentId);
    WatchComponent* getComponent(unsigned int componentId) const;
    const std::vector<std::unique_ptr<WatchComponent>>& getAllComponents() const;
    unsigned int getComponentCount() const;

    // Connection system
    void addConnection(unsigned int fromComponentId, unsigned int toComponentId);
    bool removeConnection(unsigned int fromComponentId, unsigned int toComponentId);
    const std::vector<unsigned int>& getConnections(unsigned int componentId) const;
    bool areConnected(unsigned int componentId1, unsigned int componentId2) const;

    // Fitness & validity
    double getFitnessScore() const;
    void setFitnessScore(double score);
    bool isValid() const;
    void setValid(bool valid);
    bool checkEssentialComponents() const;

    // Clone for genetic algorithm
    std::unique_ptr<Watch> clone() const;
    std::string toString() const override;

    void setCanvasSize(int width, int height) {
        m_canvasWidth  = width;
        m_canvasHeight = height;
    }

    // Fill an empty watch for Generation 0 of the Genetic Algorithm
    void randomize();
};

} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_WATCH_H