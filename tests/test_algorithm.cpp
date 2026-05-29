#include "../algorithm/GeneticAlgorithm.h"
#include <iostream>

int main() {
    WatchGA::Algorithm::GeneticAlgorithm ga;
    
    ga.Reset();
    
    auto best = ga.GetBestWatch();
    if (best) {
        std::cout << "\nWinner: " << best->GetObjectName() 
                  << " with score: " << best->GetFitnessScore() << "\n";
    }
    
    return 0;
}