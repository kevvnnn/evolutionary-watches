#include "Watch.h"
#include "components/BalanceWheel.h"
#include "components/Gear.h"
#include "components/Hand.h"
#include "components/Spring.h"
#include "components/Jewel.h"
#include <algorithm>
#include <random>

namespace WatchGA {
namespace Genome {

// =========================================================
// CONSTRUCTORS & MEMORY MANAGEMENT
// =========================================================

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

// DEEP COPY CONSTRUCTOR (Critical for the Genetic Algorithm)
Watch::Watch(const Watch& other)
    : Core::SystemObject(other),
      m_fitnessScore(other.m_fitnessScore),
      m_isValid(other.m_isValid)
{
    // 1. Safely clone every component into fresh heap memory
    for (const auto& comp : other.m_components) {
        m_components.push_back(comp->clone());
    }
    // 2. Copy the topological mapping
    m_connections = other.m_connections;
}

// ASSIGNMENT OPERATOR
Watch& Watch::operator=(const Watch& other) {
    if (this == &other) return *this;

    Core::SystemObject::operator=(other);
    m_components.clear();

    // 1. Safely clone all components
    for (const auto& comp : other.m_components) {
        m_components.push_back(comp->clone());
    }

    // 2. Copy properties
    m_connections = other.m_connections;
    m_fitnessScore = other.m_fitnessScore;
    m_isValid = other.m_isValid;

    return *this;
}

// =========================================================
// COMPONENT DNA MANAGEMENT
// =========================================================

void Watch::addComponent(std::unique_ptr<WatchComponent> component) {
    if (component) m_components.push_back(std::move(component));
}

bool Watch::removeComponent(unsigned int componentId) {
    if (componentId >= m_components.size())
        return false;

    // 1. Erase by index to prevent memory fragmentation
    m_components.erase(m_components.begin() + componentId);
    
    // 2. Tear down any dangling topological wires connected to the dead part
    m_connections.erase(componentId);
    return true;
}

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

// =========================================================
// TOPOLOGICAL CONNECTION SYSTEM
// =========================================================

void Watch::addConnection(unsigned int from, unsigned int to) {
    // Connections in a mechanical watch are strictly bidirectional
    m_connections[from].push_back(to);
    m_connections[to].push_back(from);
}

bool Watch::removeConnection(unsigned int from, unsigned int to) {
    auto& listFrom = m_connections[from];
    auto itFrom = std::remove(listFrom.begin(), listFrom.end(), to);

    auto& listTo = m_connections[to];
    auto itTo = std::remove(listTo.begin(), listTo.end(), from);

    bool removed = (itFrom != listFrom.end() || itTo != listTo.end());
    
    // Erase the dangling pointers
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

// =========================================================
// FITNESS & STRUCTURAL VALIDATION
// =========================================================

double Watch::getFitnessScore() const { return m_fitnessScore; }
void Watch::setFitnessScore(double score) { m_fitnessScore = score; }

bool Watch::isValid() const { return m_isValid; }
void Watch::setValid(bool valid) { m_isValid = valid; }

bool Watch::checkEssentialComponents() const {
    int balanceCount = 0;
    int mainCount = 0;
    int hairCount = 0;
    int hourCount = 0;
    int minuteCount = 0;
    int gearCount = 0;

    using namespace Components;

    // 1. Audit the entire genome to count vital organs
    for (const auto& comp : m_components) {
        if (dynamic_cast<const BalanceWheel*>(comp.get())) {
            balanceCount++;
        }
        else if (dynamic_cast<const Gear*>(comp.get())) {
            gearCount++;
        }
        else if (const auto* s = dynamic_cast<const Spring*>(comp.get())) {
            if (s->getType() == Spring::SpringType::MAINSPRING) mainCount++;
            else if (s->getType() == Spring::SpringType::HAIRSPRING) hairCount++;
        }
        else if (const auto* h = dynamic_cast<const Hand*>(comp.get())) {
            if (h->getType() == Hand::HandType::HOUR) hourCount++;
            else if (h->getType() == Hand::HandType::MINUTE) minuteCount++;
        }
    }

    // 2. THE EXECUTIONER:
    // A structurally viable watch MUST have EXACTLY 1 of every core organ, 
    // and AT LEAST 1 transmission gear. Any deviation returns false (Fitness = 0.0).
    return (balanceCount == 1 && 
            mainCount == 1 && 
            hairCount == 1 && 
            hourCount == 1 && 
            minuteCount == 1 && 
            gearCount >= 1); 
}

std::unique_ptr<Watch> Watch::clone() const {
    return std::make_unique<Watch>(*this);
}

std::string Watch::toString() const {
    return Core::SystemObject::toString() +
           " [Components: " + std::to_string(m_components.size()) +
           ", Fitness: " + std::to_string(m_fitnessScore) + "]";
}

// =========================================================
// THE GENERATION 0 FACTORY
// =========================================================

void Watch::randomize() {
    m_components.clear();
    m_connections.clear();
    m_isValid = false;
    m_fitnessScore = 0.0;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    using namespace Components;

    // ---------------------------------------------------------
    // THE CHAOS DISTRIBUTIONS (Making terrible watches)
    // ---------------------------------------------------------
    // We want the starting population to be genuinely bad so they are forced to evolve.
    std::uniform_real_distribution<double> weightDist(0.1, 5.0);
    std::uniform_real_distribution<double> frictionDist(0.01, 0.9); // 0.9 is basically sandpaper
    std::uniform_real_distribution<double> qualityDist(0.1, 1.0); 

    // ---------------------------------------------------------
    // 1. THE ESSENTIAL COMPONENTS
    // ---------------------------------------------------------
    
    // Hands: varying lengths and terrible balances
    std::uniform_real_distribution<double> handLenDist(5.0, 25.0);
    addComponent(std::make_unique<Hand>(
        "HourHand", weightDist(gen), frictionDist(gen),
        Hand::HandType::HOUR, handLenDist(gen), qualityDist(gen)
    ));
    addComponent(std::make_unique<Hand>(
        "MinuteHand", weightDist(gen), frictionDist(gen),
        Hand::HandType::MINUTE, handLenDist(gen), qualityDist(gen)
    ));

    // Springs: varying elasticity and fatigue resistance
    std::uniform_real_distribution<double> springLenDist(1.0, 50.0);
    addComponent(std::make_unique<Spring>(
        "MainSpring", weightDist(gen), frictionDist(gen),
        Spring::SpringType::MAINSPRING, qualityDist(gen), qualityDist(gen), springLenDist(gen)
    ));
    addComponent(std::make_unique<Spring>(
        "HairSpring", weightDist(gen), frictionDist(gen),
        Spring::SpringType::HAIRSPRING, qualityDist(gen), qualityDist(gen), springLenDist(gen)
    ));

    // BalanceWheel: random Moment of Inertia and terrible starting amplitudes
    std::uniform_real_distribution<double> moiDist(0.1, 10.0);
    std::uniform_real_distribution<double> ampDist(90.0, 360.0); // 270 is optimal
    addComponent(std::make_unique<BalanceWheel>(
        "BalWheel", weightDist(gen), frictionDist(gen),
        moiDist(gen), qualityDist(gen), ampDist(gen)
    ));

    // ---------------------------------------------------------
    // 2. THE MUTABLE MIDDLE (Gears containing Jewels)
    // ---------------------------------------------------------
    std::uniform_int_distribution<unsigned int> gearCountDist(3, 9); // Strict 9-gear cap
    unsigned int randomGears = gearCountDist(gen);

    std::uniform_int_distribution<unsigned int> teethDist(8, 64);      
    std::uniform_real_distribution<double> diameterDist(2.0, 20.0);
    
    // Distributions for the nested Jewels
    std::uniform_real_distribution<double> hardnessDist(8.5, 9.5);
    std::uniform_int_distribution<int> boolDist(0, 1); 

    for (unsigned int i = 0; i < randomGears; ++i) {
        
        // A. Create the base Gear
        std::string gearName = "Gear_" + std::to_string(i + 1);
        auto newGear = std::make_unique<Gear>(
            gearName, weightDist(gen), frictionDist(gen),
            teethDist(gen), diameterDist(gen), qualityDist(gen)
        );

        // B. 50% chance to forge a Jewel INSIDE this specific gear right at birth
        if (boolDist(gen) == 1) {
            std::string jewelName = "Jewel_For_" + gearName;
            auto newJewel = std::make_unique<Jewel>(
                jewelName, 0.01, 0.005,
                hardnessDist(gen), static_cast<bool>(boolDist(gen))
            );
            
            // Hand the jewel memory ownership directly to the gear
            newGear->setJewel(std::move(newJewel)); 
        }

        // C. Add the completed Gear to the watch genome
        addComponent(std::move(newGear));
    }

    // ---------------------------------------------------------
    // 3. THE TOPOLOGY (Random Connections)
    // ---------------------------------------------------------
    unsigned int totalComps = getComponentCount();
    if (totalComps > 1) {
        std::uniform_int_distribution<unsigned int> targetDist(0, totalComps - 1);
        
        for (unsigned int i = 0; i < totalComps; ++i) {
            std::uniform_int_distribution<unsigned int> connDist(1, 3);
            unsigned int numConnections = connDist(gen);
            
            for (unsigned int c = 0; c < numConnections; ++c) {
                unsigned int target = targetDist(gen);
                
                // Prevent connecting to self and prevent duplicate connections
                if (i != target && !areConnected(i, target)) {
                    addConnection(i, target);
                }
            }
        }
    }

    m_isValid = true;
}

} // namespace Genome
} // namespace WatchGA