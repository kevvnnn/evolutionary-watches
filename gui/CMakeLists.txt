# Look for modern Qt6 installation, fall back to stable Qt5 if necessary
find_package(Qt6 COMPONENTS Widgets REQUIRED QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(QT_LIBRARIES Qt5::Widgets)
else()
    set(QT_LIBRARIES Qt6::Widgets)
endif()

add_library(watch_gui STATIC
    main_gui.cpp
    MainWindow.cpp
    ControlPanel.cpp
    WatchCanvas.cpp
    StatsPanel.cpp
    ComponentInspector.cpp
    ui_ControlPanel.h
    ControlPanel.ui
)

target_include_directories(watch_gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# UI layer acts as the supreme consumer of all algorithm, storage, and domain structures
target_link_libraries(watch_gui PUBLIC 
    watch_core 
    watch_genome 
    watch_algorithm 
    watch_fileio 
    ${QT_LIBRARIES}
)