#ifndef WATCH_H
#define WATCH_H

#include <vector>
#include <map>

#include "WatchComponent.h"

class Watch {
    private:
    std::vector<WatchComponent*> m_components;
    std::map<unsigned int, std::vector<unsigned int>> m_connections;

    WatchComponent* findConponentByID(unsigned int id) const;

    public:
    Watch() = default;
    ~Watch();

    void addComponent(WatchComponent* component);
    void connect(unsigned int idA, unsigned int idB);
    bool isValid() const;
    const std::vector<WatchComponent*>& getComponents() const;

    void updateJewelPlacementFriction();
};

#endif