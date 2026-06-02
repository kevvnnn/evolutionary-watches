#ifndef WATCH_GA_FILEIO_CONFIGMANAGER_H
#define WATCH_GA_FILEIO_CONFIGMANAGER_H

#include <string>
#include <map>

namespace WatchGA {
namespace FileIO {

class ConfigManager {
private:
    std::string m_configFilePath;
    std::map<std::string, std::string> m_configValues;

public:
    ConfigManager();

    explicit ConfigManager(const std::string& configFilePath);

    ~ConfigManager() = default;

    bool loadConfig();

    bool saveConfig() const;

    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue = 0) const;
    double getDouble(const std::string& key, double defaultValue = 0.0) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;

    void setString(const std::string& key, const std::string& value);
    void setInt(const std::string& key, int value);
    void setDouble(const std::string& key, double value);
    void setBool(const std::string& key, bool value);

    bool hasKey(const std::string& key) const;
    void removeKey(const std::string& key);
    void Clear();
};

} 
} 

#endif 