#ifndef WATCH_GA_FILEIO_CONFIGMANAGER_H
#define WATCH_GA_FILEIO_CONFIGMANAGER_H

#include <string>
#include <map>

namespace WatchGA {
namespace FileIO {

/// <summary>
/// Manages application configuration settings.
///
/// This class loads configuration values from a file,
/// stores them in memory, and allows values to be
/// retrieved or modified using key-value pairs.
/// </summary>
class ConfigManager {
private:
    /// <summary>
    /// Path to the configuration file.
    /// </summary>
    std::string m_configFilePath;

    /// <summary>
    /// Stores configuration settings as key-value pairs.
    /// </summary>
    std::map<std::string, std::string> m_configValues;

public:

    /// <summary>
    /// Creates an empty configuration manager.
    /// </summary>
    ConfigManager();

    /// <summary>
    /// Creates a configuration manager using the specified file path.
    /// </summary>
    explicit ConfigManager(const std::string& configFilePath);

    ~ConfigManager() = default;

    /// <summary>
    /// Loads configuration settings from the file.
    /// </summary>
    bool loadConfig();

    /// <summary>
    /// Saves all configuration settings to the file.
    /// </summary>
    bool saveConfig() const;

    /// <summary>
    /// Retrieves a string value from the configuration.
    /// </summary>
    std::string getString(
        const std::string& key,
        const std::string& defaultValue = "") const;

    /// <summary>
    /// Retrieves an integer value from the configuration.
    /// </summary>
    int getInt(
        const std::string& key,
        int defaultValue = 0) const;

    /// <summary>
    /// Retrieves a floating-point value from the configuration.
    /// </summary>
    double getDouble(
        const std::string& key,
        double defaultValue = 0.0) const;

    /// <summary>
    /// Retrieves a boolean value from the configuration.
    /// </summary>
    bool getBool(
        const std::string& key,
        bool defaultValue = false) const;

    /// <summary>
    /// Stores a string value in the configuration.
    /// </summary>
    void setString(
        const std::string& key,
        const std::string& value);

    /// <summary>
    /// Stores an integer value in the configuration.
    /// </summary>
    void setInt(
        const std::string& key,
        int value);

    /// <summary>
    /// Stores a floating-point value in the configuration.
    /// </summary>
    void setDouble(
        const std::string& key,
        double value);

    /// <summary>
    /// Stores a boolean value in the configuration.
    /// </summary>
    void setBool(
        const std::string& key,
        bool value);

    /// <summary>
    /// Checks whether a configuration key exists.
    /// </summary>
    bool hasKey(const std::string& key) const;

    /// <summary>
    /// Removes a configuration entry from memory.
    /// </summary>
    void removeKey(const std::string& key);

    /// <summary>
    /// Removes all configuration entries.
    /// </summary>
    void clear();
};

}
}

#endif