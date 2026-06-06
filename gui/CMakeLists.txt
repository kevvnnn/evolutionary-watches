# gui/CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

add_library(watch_gui STATIC
    main_gui.cpp
    MainWindow.cpp
    ControlPanel.cpp
    WatchCanvas.cpp
    StatsPanel.cpp
    ComponentInspector.cpp
    ControlPanel.ui
)

target_include_directories(watch_gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(watch_gui PUBLIC
    watch_algorithm
    watch_core
    watch_genome
    watch_fileio
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Charts
)