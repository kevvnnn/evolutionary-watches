#include <iostream>
#include <iomanip>

#include "../genome/WatchComponent.h"
#include "../genome/Watch.h"
#include "../genome/FitnessEvaluator.h"
#include "../genome/components/Gear.h"
#include "../genome/components/Jewel.h"
#include "../genome/components/BalanceWheel.h"
#include "../genome/components/Spring.h"
#include "../genome/components/Hand.h"

using namespace std;

int main()
{
    cout << fixed << setprecision(3);

    Watch myWatch;
    unsigned int componentId = 1;

    cout << "=== WATCH GENOME TEST PROGRAM ===" << endl << endl;

    // Add required core components
    // Balance Wheel
    myWatch.addComponent(new BalanceWheel(
        componentId++,
        0.5,
        0.95,
        270,
        0.80
    ));

    // Mainspring
    myWatch.addComponent(new Spring(
        componentId++,
        SpringType::MAINSPRING,
        0.90,
        0.90,
        10.0,
        0.15
    ));

    // Hairspring
    myWatch.addComponent(new Spring(
        componentId++,
        SpringType::HAIRSPRING,
        0.95,
        0.95,
        6.0,
        0.15
    ));

    // Hour Hand
    myWatch.addComponent(new Hand(
        componentId++,
        HandType::HOUR,
        8.0,
        0.90,
        0.5,
        0.05
    ));

    // Minute Hand
    myWatch.addComponent(new Hand(
        componentId++,
        HandType::MINUTE,
        12.0,
        0.90,
        0.5,
        0.05
    ));

    // Add extra mechanical parts
    Gear* testGear = new Gear(
        componentId++,
        24,
        6.0,
        0.92,
        0.50
    );
    myWatch.addComponent(testGear);

    Jewel* testJewel = new Jewel(
        componentId++,
        9.0,
        true,
        0.03
    );
    myWatch.addComponent(testJewel);

    // Connect gear and jewel, then update friction detection
    myWatch.connect(testGear->getID(), testJewel->getID());
    myWatch.updateJewelPlacementFriction();

    // Print all component details
    cout << "--- LIST OF ALL COMPONENTS ---" << endl;
    for (const auto* comp : myWatch.getComponents())
    {
        cout << "[" << comp->getTypeName() << "]"
             << " Base Friction: " << comp->getBaseFriction()
             << " | Efficiency: " << comp->calculateEfficiency()
             << endl;

        // Check jewel placement status
        if (const Jewel* jewel = dynamic_cast<const Jewel*>(comp))
        {
            if (jewel->isPlacedCorrectly())
                cout << "   ✅ Jewel placed in valid high-friction zone" << endl;
            else
                cout << "   ❌ Jewel placed in low-friction zone (penalty applied)" << endl;
        }
        cout << "--------------------------------------------" << endl;
    }

    // Check if the watch assembly is valid
    bool watchValid = myWatch.isValid();
    cout << "\nWatch Assembly Valid: " << (watchValid ? "YES" : "NO") << endl;

    // Calculate and display performance scores
    FitnessEvaluator evaluator;
    double accuracy = evaluator.calculateAccuracy(myWatch);
    double avgEfficiency = evaluator.calculateAvgEfficiency(myWatch);
    double totalFriction = evaluator.calculateTotalFriction(myWatch);
    double jewelScore = evaluator.calculateJewelPlacementScore(myWatch);
    double finalFitness = evaluator.calculateFitness(myWatch);

    cout << "\n--- PERFORMANCE SCORES ---" << endl;
    cout << "Timekeeping Accuracy:   " << accuracy << endl;
    cout << "Average Efficiency:     " << avgEfficiency << endl;
    cout << "Total Watch Friction:   " << totalFriction << endl;
    cout << "Jewel Placement Score:  " << jewelScore << endl;
    cout << "Final Fitness Score:    " << finalFitness << endl;

    return 0;
}