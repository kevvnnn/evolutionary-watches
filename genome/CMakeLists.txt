add_library(watch_genome STATIC
    WatchComponent.cpp
    Watch.cpp
    FitnessEvaluator.cpp
    components/Gear.cpp
    components/Jewel.cpp
    components/BalanceWheel.cpp
    components/Spring.cpp
    components/Hand.cpp
    ${PROJECT_SOURCE_DIR}/Core/SystemObject.cpp
)

# Publicly expose the base genome path, and link back to core interfaces
target_include_directories(watch_genome PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(watch_genome PUBLIC watch_core)

target_include_directories(watch_genome PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Core
    ${PROJECT_SOURCE_DIR}/genome
    ${PROJECT_SOURCE_DIR}/genome/components
)