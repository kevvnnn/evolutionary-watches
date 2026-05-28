add_library(watch_core INTERFACE)

# Expose headers cleanly to any target that links this
target_include_directories(watch_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})