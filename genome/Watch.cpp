#include "Watch.h"
#include "components/BalanceWheel.h"
#include "components/Spring.h"
#include "components/Hand.h"
#include "components/Jewel.h"
#include "components/Gear.h"

Watch::~Watch() {
    for (auto* comp : m_components) delete comp;
}

void Watch::addComponent(WatchComponent* component) {
    m_components.push_back(component);
}

void Watch::connect(unsigned int idA, unsigned int idB) {
    m_connections[idA].push_back(idB);
    m_connections[idB].push_back(idA);
}

WatchComponent* Watch::findConponentByID(unsigned int id) const {
    for (auto* comp : m_components) {
        if (comp->getID() == id) return comp;
    }
    return nullptr;
}

//This part auto sets the friction fir jewels based on the connected parts
void Watch::updateJewelPlacementFriction() {
    for (auto* comp : m_components) {
        if (Jewel* jewel = dynamic_cast<Jewel*>(comp)) {
            unsigned int jewelID = comp->getID();
            double maxConnectedFriction = 0.0;

            for (unsigned int connectedID : m_connections[jewelID]) {
                WatchComponent* connected = findConponentByID(connectedID);
                if (connected && connected -> getBaseFriction() > maxConnectedFriction) {
                    maxConnectedFriction = connected -> getBaseFriction();
                }
            }
            jewel->setConnectedFriction(maxConnectedFriction);
        }
    }
}

bool Watch::isValid() const {
    bool hasBalance = false, hasMainspring = false, hasHairspring = false;
    bool hasHour = false, hasMinute = false;

    for (const auto* comp : m_components) {
        if (dynamic_cast<const BalanceWheel*>(comp)) hasBalance = true;
        if (const auto* s = dynamic_cast<const Spring*>(comp)) {
            if (s->getSpringType() == SpringType::MAINSPRING) hasMainspring = true;
            if (s->getSpringType() == SpringType::HAIRSPRING) hasHairspring = true;
        }
        if (const auto* h = dynamic_cast<const Hand*>(comp)) {
            if (h->getHandType() == HandType::HOUR) hasHour = true;
            if (h->getHandType() == HandType::MINUTE) hasMinute = true;
        }
    }
    return hasBalance && hasMainspring && hasHairspring && hasHour && hasMinute;
}

const std::vector<WatchComponent*>& Watch::getComponents() const {
    return m_components;
}