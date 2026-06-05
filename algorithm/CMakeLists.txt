add_library(watch_algorithm STATIC
    GeneticAlgorithm.cpp
    SelectionStrategy.cpp
    CrossoverStrategy.cpp
    MutationStrategy.cpp
    ../core/SystemObject.cpp
)

target_include_directories(watch_algorithm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Algorithm depends heavily on both shared interfaces and the underlying structural genome
target_link_libraries(watch_algorithm PUBLIC watch_core watch_genome)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
target_link_libraries(watch_algorithm PUBLIC Qt6::Core Qt6::Widgets)