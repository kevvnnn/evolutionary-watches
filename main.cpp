#include "gui/main_gui.h"

// Main entry point for the WatchGA application
// This delegates to the GUI launcher
int main(int argc, char *argv[])
{
    // Launch the GUI application
    return WatchGA::GUI::LaunchGUI(argc, argv);
}