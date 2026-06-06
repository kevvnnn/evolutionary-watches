# --------------------------
# WATCH ALGORITHM LIBRARY (ONLY ALGORITHM CODE!)
# --------------------------
add_library(watch_algorithm STATIC
    GeneticAlgorithm.cpp
    SelectionStrategy.cpp
    CrossoverStrategy.cpp
    MutationStrategy.cpp
    ../core/SystemObject.cpp
)

target_include_directories(watch_algorithm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Algorithm uses Qt Core ONLY — NO WIDGETS, NO GUI!
find_package(Qt6 REQUIRED COMPONENTS Core)

target_link_libraries(watch_algorithm PUBLIC
    watch_core
    watch_genome
    Qt6::Core
)