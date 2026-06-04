#include <iostream>
#include <memory>
#include "../genome/WatchComponent.h"
#include "../genome/Watch.h"
#include "../genome/FitnessEvaluator.h"
#include "../genome/components/Gear.h"
#include "../genome/components/Jewel.h"
#include "../genome/components/BalanceWheel.h"
#include "../genome/components/Spring.h"
#include "../genome/components/Hand.h"

using namespace WatchGA::Genome;
using namespace WatchGA::Genome::Components;
using namespace std;

int main()
{
    cout << "=========================================" << endl;
    cout << " WATCH GENETIC ALGORITHM - TEST PROGRAM  " << endl;
    cout << "=========================================" << endl;
    cout << endl;

    // 1. Create a new watch
    unique_ptr<Watch> myWatch = make_unique<Watch>("TestWatch_001");
    cout << "[INFO] Created watch: " << myWatch->toString() << endl;

    // 2. Add essential components
    myWatch->addComponent(make_unique<Gear>("Gear_Main", 2.0, 0.1, 10, 10, 12, 6.0, 0.9));
    myWatch->addComponent(make_unique<Jewel>("Jewel_Cap", 0.5, 0.02, 10, 15, 9.5, true));
    myWatch->addComponent(make_unique<BalanceWheel>("Balance_Main", 1.8, 0.05, 10, 20, 1.2, 0.95, 270.0));
    myWatch->addComponent(make_unique<Spring>("Spring_Main", 1.2, 0.03, 10, 25, Spring::SpringType::MAINSPRING, 0.92, 0.95, 12.0));
    myWatch->addComponent(make_unique<Spring>("Spring_Hair", 0.3, 0.01, 10, 30, Spring::SpringType::HAIRSPRING, 0.98, 0.97, 4.0));
    myWatch->addComponent(make_unique<Hand>("Hand_Hour", 0.7, 0.04, 10, 35, Hand::HandType::HOUR, 12.0, 0.96));
    myWatch->addComponent(make_unique<Hand>("Hand_Minute", 0.5, 0.03, 10, 40, Hand::HandType::MINUTE, 15.0, 0.97));

    cout << "[INFO] Added " << myWatch->getComponentCount() << " components." << endl;
    cout << endl;

    // 3. Connect some components (bidirectional)
    myWatch->addConnection(0, 1);
    myWatch->addConnection(1, 2);
    myWatch->addConnection(2, 4);
    cout << "[INFO] Created internal connections." << endl;
    cout << endl;

    // 4. Mark watch as valid
    myWatch->setValid(true);
    cout << "[INFO] Watch valid: " << boolalpha << myWatch->isValid() << endl;

    // 5. Check if all essential parts exist
    bool hasAllParts = myWatch->checkEssentialComponents();
    cout << "[CHECK] Has all essential components: " << boolalpha << hasAllParts << endl;
    cout << endl;

    // 6. Evaluate fitness
    FitnessEvaluator evaluator;
    double fitness = evaluator.evaluate(*myWatch);

    cout << "=========================================" << endl;
    cout << "FINAL FITNESS SCORE: " << fitness << endl;
    cout << "=========================================" << endl;
    cout << endl;

    // 7. Test component retrieval
    WatchComponent* comp = myWatch->getComponent(2);
    if (comp) {
        cout << "[TEST] Retrieved component: " << comp->toString() << endl;
    }

    // 8. Test removal
    bool removed = myWatch->removeComponent(6);
    cout << "[TEST] Removed component 6: " << boolalpha << removed << endl;
    cout << "[TEST] New component count: " << myWatch->getComponentCount() << endl;

    cout << endl;
    cout << "[SUCCESS] All tests completed!" << endl;

    return 0;
}