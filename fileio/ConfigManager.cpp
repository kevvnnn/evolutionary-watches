#include <iostream>
#include "ConfigManager.h"

using namespace WatchGA::FileIO;

int main()
{
    // Create config manager with config file path
    ConfigManager configManager("config.txt");

    // Load existing configuration
    if (configManager.loadConfig())
    {
        std::cout << "Configuration loaded successfully.\n";
    }
    else
    {
        std::cout << "No configuration file found. Creating defaults...\n";
    }

    // Set values
    configManager.setString("username", "admin");
    configManager.setInt("port", 8080);
    configManager.setDouble("version", 1.5);
    configManager.setBool("debug", true);

    // Save configuration
    if (configManager.saveConfig())
    {
        std::cout << "Configuration saved.\n";
    }

    // Read values
    std::string userName = configManager.getString("username");
    int portNumber = configManager.getInt("port");
    double versionNumber = configManager.getDouble("version");
    bool debugMode = configManager.getBool("debug");

    // Display values
    std::cout << "\nCurrent Settings:\n";
    std::cout << "Username: " << userName << '\n';
    std::cout << "Port: " << portNumber << '\n';
    std::cout << "Version: " << versionNumber << '\n';
    std::cout << "Debug: " << (debugMode ? "true" : "false") << '\n';

    // Check if key exists
    if (configManager.hasKey("username"))
    {
        std::cout << "\nUsername key exists.\n";
    }

    // Remove a key
    configManager.removeKey("version");

    // Save changes again
    configManager.saveConfig();

    return 0;
}