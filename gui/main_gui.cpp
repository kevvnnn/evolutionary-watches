#include <QApplication>
#include "MainWindow.h"

namespace WatchGA {
namespace GUI {

/**
 * @brief Launches the GUI application
 * @param argc Command line argument count
 * @param argv Command line arguments
 * @return Exit code
 */
int LaunchGUI(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("WatchGA");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("WatchGA Team");
    
    // Create and show the main window
    MainWindow window;
    window.show();
    
    // Run the application event loop
    return app.exec();
}

} // namespace GUI
} // namespace WatchGA