#ifndef WATCH_GA_FILE_IO_CONFIG_MANAGER_H
#define WATCH_GA_FILE_IO_CONFIG_MANAGER_H

#include <string>
#include <map>

namespace WatchGA {
namespace FileIO {

/**
 * @class ConfigManager
 * @brief JSON config loader/saver
 * @ownership Person C
 */
class ConfigManager
{
private:
    std::string m_configFilePath;
    std::map<std::string, std::string> m_values;

public:
    ConfigManager();
    explicit ConfigManager(const std::string& path);
    ~ConfigManager() = default;

    bool loadConfig();
    bool saveConfig() const;

    // Getters
    std::string getString(const std::string& key, const std::string& def = "") const;
    int getInt(const std::string& key, int def = 0) const;
    double getDouble(const std::string& key, double def = 0.0) const;
    bool getBool(const std::string& key, bool def = false) const;

    // Setters
    void setString(const std::string& key, const std::string& value);
    void setInt(const std::string& key, int value);
    void setDouble(const std::string& key, double value);
    void setBool(const std::string& key, bool value);

    bool hasKey(const std::string& key) const;
    void removeKey(const std::string& key);
    void clear();
};

} // namespace FileIO
} // namespace WatchGA

#endif