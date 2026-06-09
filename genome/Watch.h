#ifndef WATCH_GA_GENOME_WATCH_H
#define WATCH_GA_GENOME_WATCH_H

#include "../core/SystemObject.h"
#include "WatchComponent.h"
#include <vector>
#include <memory>
#include <map>

namespace WatchGA {
namespace Genome {

/**
 * @class Watch
 * @brief Represents a single evolutionary genome (an individual watch assembly).
 * * The Watch class acts as the container holding the physical DNA (`WatchComponent`s) 
 * and the topological map of how they connect. It is deeply copyable to support 
 * safe crossover and mutation algorithms.
 */
class Watch : public Core::SystemObject {
private:
    std::vector<std::unique_ptr<WatchComponent>> m_components;
    std::map<unsigned int, std::vector<unsigned int>> m_connections;
    
    double m_fitnessScore;
    bool m_isValid;

    int m_canvasWidth  = 600; // Default fallback size for rendering
    int m_canvasHeight = 600;

public:
    // ---------------------------------------------------------
    // CONSTRUCTORS & MEMORY MANAGEMENT
    // ---------------------------------------------------------
    Watch();
    explicit Watch(const std::string& name);
    Watch(const Watch& other); // Deep copy
    Watch& operator=(const Watch& other);
    ~Watch() override = default;

    // ---------------------------------------------------------
    // COMPONENT DNA MANAGEMENT
    // ---------------------------------------------------------
    /**
     * @brief Adds a component to the genome, taking full memory ownership via unique_ptr
     */
    void addComponent(std::unique_ptr<WatchComponent> component);
    
    /**
     * @brief Safely erases a component and tears down any topological connections attached to it
     */
    bool removeComponent(unsigned int componentId);
    
    WatchComponent* getComponent(unsigned int componentId) const;
    const std::vector<std::unique_ptr<WatchComponent>>& getAllComponents() const;
    unsigned int getComponentCount() const;

    // ---------------------------------------------------------
    // TOPOLOGICAL CONNECTION SYSTEM
    // ---------------------------------------------------------
    void addConnection(unsigned int fromComponentId, unsigned int toComponentId);
    bool removeConnection(unsigned int fromComponentId, unsigned int toComponentId);
    const std::vector<unsigned int>& getConnections(unsigned int componentId) const;
    bool areConnected(unsigned int componentId1, unsigned int componentId2) const;

    // ---------------------------------------------------------
    // FITNESS & VALIDATION
    // ---------------------------------------------------------
    double getFitnessScore() const;
    void setFitnessScore(double score);
    
    bool isValid() const;
    void setValid(bool valid);
    
    /**
     * @brief The structural executioner. Verifies the exact 1-to-1 ratio of vital organs.
     * @return True if the watch has exactly 1 balance wheel, 2 springs, 2 hands, and 1+ gears.
     */
    bool checkEssentialComponents() const;

    // ---------------------------------------------------------
    // GENETIC HELPERS
    // ---------------------------------------------------------
    /**
     * @brief Deep clones the entire watch, allocating fresh heap memory to prevent pointer aliasing.
     */
    std::unique_ptr<Watch> clone() const;
    std::string toString() const override;
    
    void setCanvasSize(int width, int height) {
        m_canvasWidth  = width;
        m_canvasHeight = height;
    }

    /**
     * @brief The Generation 0 Factory. Wipes the watch and fills it with randomized, chaotic DNA.
     */
    void randomize();
};

} // namespace Genome
} // namespace WatchGA

#endif // WATCH_GA_GENOME_WATCH_H