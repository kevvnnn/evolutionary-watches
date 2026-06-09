#ifndef WATCH_GA_FILE_IO_CONFIG_MANAGER_H
#define WATCH_GA_FILE_IO_CONFIG_MANAGER_H

#include <string>
#include <map>

namespace WatchGA {
namespace FileIO {

/**
 * @brief Manages key-value configuration storage
 *
 * Loads and saves simple text-based config files (key=value).
 * Supports strings, integers, doubles, and booleans.
 * Used for storing application and algorithm settings.
 */
class ConfigManager
{
private:
    std::string m_configFilePath;
    std::map<std::string, std::string> m_values;

public:
    /**
     * @brief Default constructor
     * Uses default config file: config.txt
     */
    ConfigManager();

    /**
     * @brief Constructor with custom config path
     * @param path Path to configuration file
     */
    explicit ConfigManager(const std::string& path);

    ~ConfigManager() = default;

    /**
     * @brief Loads key-value pairs from the config file
     * @return True if loaded successfully
     */
    bool loadConfig();

    /**
     * @brief Saves current key-value pairs to file
     * @return True if saved successfully
     */
    bool saveConfig() const;

    // ------------------------------
    // Getters
    // ------------------------------

    /**
     * @brief Gets a string value
     * @param key Config key name
     * @param def Default value if key not found
     * @return String value
     */
    std::string getString(const std::string& key, const std::string& def = "") const;

    /**
     * @brief Gets an integer value
     * @param key Config key name
     * @param def Default value if key not found
     * @return Integer value
     */
    int getInt(const std::string& key, int def = 0) const;

    /**
     * @brief Gets a floating-point value
     * @param key Config key name
     * @param def Default value if key not found
     * @return Double value
     */
    double getDouble(const std::string& key, double def = 0.0) const;

    /**
     * @brief Gets a boolean value
     * @param key Config key name
     * @param def Default value if key not found
     * @return True or false
     */
    bool getBool(const std::string& key, bool def = false) const;

    // ------------------------------
    // Setters
    // ------------------------------

    /**
     * @brief Stores a string value
     * @param key Config key name
     * @param value String to save
     */
    void setString(const std::string& key, const std::string& value);

    /**
     * @brief Stores an integer value
     * @param key Config key name
     * @param value Integer to save
     */
    void setInt(const std::string& key, int value);

    /**
     * @brief Stores a floating-point value
     * @param key Config key name
     * @param value Double to save
     */
    void setDouble(const std::string& key, double value);

    /**
     * @brief Stores a boolean value
     * @param key Config key name
     * @param value True or false
     */
    void setBool(const std::string& key, bool value);

    // ------------------------------
    // Utility
    // ------------------------------

    /**
     * @brief Checks if a key exists in the config
     * @param key Key to check
     * @return True if exists
     */
    bool hasKey(const std::string& key) const;

    /**
     * @brief Removes a key from the config
     * @param key Key to remove
     */
    void removeKey(const std::string& key);

    /**
     * @brief Clears all stored configuration values
     */
    void clear();
};

} // namespace FileIO
} // namespace WatchGA

#endif