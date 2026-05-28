add_library(watch_fileio STATIC
    WatchFileIO.cpp
    ConfigManager.cpp
    EvolutionHistory.cpp
)

target_include_directories(watch_fileio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Serialization reads layout rules directly from the genome libraries
target_link_libraries(watch_fileio PUBLIC watch_core watch_genome)