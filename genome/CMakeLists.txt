add_library(watch_genome STATIC
    WatchComponent.cpp
    Watch.cpp
    FitnessEvaluator.cpp
    components/Gear.cpp
    components/Jewel.cpp
    components/BalanceWheel.cpp
    components/Spring.cpp
)

# Publicly expose the base genome path, and link back to core interfaces
target_include_directories(watch_genome PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(watch_genome PUBLIC watch_core)