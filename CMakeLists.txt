cmake_minimum_required(VERSION 3.16)
project(WatchGA VERSION 1.0.0 LANGUAGES CXX)

# Force modern C++ standards
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Workflow Optimization Toggles
option(BUILD_GUI "Build the Qt-based graphical user interface" ON)
option(BUILD_TESTS "Build unit tests for the system" ON)

# Global compiler optimizations & warnings for clean collaboration
if(MSVC)
    add_compile_options(/W4 /WX /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Automatic Qt processing options (will be applied down the line if GUI is enabled)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Build submodules step-by-step
add_subdirectory(core)
add_subdirectory(genome)
add_subdirectory(algorithm)
add_subdirectory(fileio)

if(BUILD_GUI)
    add_subdirectory(gui)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# Main Orchestrator Executable
add_executable(watch_ga main.cpp)

# Target links are clean and explicit
target_link_libraries(watch_ga PRIVATE
    watch_core
    watch_genome
    watch_algorithm
    watch_fileio
)

if(BUILD_GUI)
    target_link_libraries(watch_ga PRIVATE watch_gui)
    target_compile_definitions(watch_ga PRIVATE HAS_GUI_SUPPORT)
endif()