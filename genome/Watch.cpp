#include "Watch.h"
#include "components/BalanceWheel.h"
#include "components/Spring.h"
#include "components/Hand.h"
#include <algorithm>

using namespace WatchGA::Genome;

Watch::Watch()
    : m_fitnessScore(0.0),
      m_isValid(false)
{
}

Watch::Watch(const std::string& name)
    : Core::SystemObject(name),
      m_fitnessScore(0.0),
      m_isValid(false)
{
}

// DEEP COPY — required for genetic algorithm
Watch::Watch(const Watch& other)
    : Core::SystemObject(other),
      m_fitnessScore(other.m_fitnessScore),
      m_isValid(other.m_isValid)
{
    for (const auto& comp : other.m_components) {
        m_components.push_back(comp->clone());
    }
    m_connections = other.m_connections;
}

Watch& Watch::operator=(const Watch& other) {
    if (this == &other) return *this;

    Core::SystemObject::operator=(other);
    m_components.clear();

    // Clone all components
    for (const auto& comp : other.m_components) {
        m_components.push_back(comp->clone());
    }

    m_connections = other.m_connections;
    m_fitnessScore = other.m_fitnessScore;
    m_isValid = other.m_isValid;

    return *this;
}

// Add component (takes ownership)
void Watch::addComponent(std::unique_ptr<WatchComponent> component) {
    if (component) m_components.push_back(std::move(component));
}

// FIXED: Removed invalid getId() call. Uses index-based ID.
bool Watch::removeComponent(unsigned int componentId) {
    if (componentId >= m_components.size())
        return false;

    // Erase by index (safe, no missing functions)
    m_components.erase(m_components.begin() + componentId);
    m_connections.erase(componentId);
    return true;
}

// FIXED: Uses index-based ID
WatchComponent* Watch::getComponent(unsigned int componentId) const {
    if (componentId >= m_components.size())
        return nullptr;
    return m_components[componentId].get();
}

const std::vector<std::unique_ptr<WatchComponent>>& Watch::getAllComponents() const {
    return m_components;
}

unsigned int Watch::getComponentCount() const {
    return static_cast<unsigned int>(m_components.size());
}

// Connections are bidirectional
void Watch::addConnection(unsigned int from, unsigned int to) {
    m_connections[from].push_back(to);
    m_connections[to].push_back(from);
}

bool Watch::removeConnection(unsigned int from, unsigned int to) {
    auto& listFrom = m_connections[from];
    auto itFrom = std::remove(listFrom.begin(), listFrom.end(), to);

    auto& listTo = m_connections[to];
    auto itTo = std::remove(listTo.begin(), listTo.end(), from);

    bool removed = (itFrom != listFrom.end() || itTo != listTo.end());
    listFrom.erase(itFrom, listFrom.end());
    listTo.erase(itTo, listTo.end());
    return removed;
}

const std::vector<unsigned int>& Watch::getConnections(unsigned int id) const {
    static const std::vector<unsigned int> empty;
    auto it = m_connections.find(id);
    return (it != m_connections.end()) ? it->second : empty;
}

bool Watch::areConnected(unsigned int a, unsigned int b) const {
    auto it = m_connections.find(a);
    if (it == m_connections.end()) return false;
    return std::find(it->second.begin(), it->second.end(), b) != it->second.end();
}

// Fitness & validity
double Watch::getFitnessScore() const { return m_fitnessScore; }
void Watch::setFitnessScore(double score) { m_fitnessScore = score; }
bool Watch::isValid() const { return m_isValid; }
void Watch::setValid(bool valid) { m_isValid = valid; }

// FIXED: Added missing namespace for enums
bool Watch::checkEssentialComponents() const {
    bool hasBalance = false, hasMain = false, hasHair = false, hasHour = false, hasMinute = false;
    using namespace Components;

    for (const auto& comp : m_components) {
        if (dynamic_cast<const BalanceWheel*>(comp.get()))
            hasBalance = true;

        if (const auto* s = dynamic_cast<const Spring*>(comp.get())) {
            if (s->getType() == Spring::SpringType::MAINSPRING)
                hasMain = true;
            if (s->getType() == Spring::SpringType::HAIRSPRING)
                hasHair = true;
        }

        if (const auto* h = dynamic_cast<const Hand*>(comp.get())) {
            if (h->getType() == Hand::HandType::HOUR)
                hasHour = true;
            if (h->getType() == Hand::HandType::MINUTE)
                hasMinute = true;
        }
    }
    return hasBalance && hasMain && hasHair && hasHour && hasMinute;
}

// Full clone for GA population
std::unique_ptr<Watch> Watch::clone() const {
    return std::make_unique<Watch>(*this);
}

std::string Watch::toString() const {
    return Core::SystemObject::toString() +
           " [Components: " + std::to_string(m_components.size()) +
           ", Fitness: " + std::to_string(m_fitnessScore) + "]";
}