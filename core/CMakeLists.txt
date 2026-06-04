add_library(watch_core STATIC
    SystemObject.cpp
)

target_include_directories(watch_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})