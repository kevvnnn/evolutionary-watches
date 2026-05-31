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

    bool LoadConfig();

    bool SaveConfig() const;

    std::string GetString(const std::string& key, const std::string& defaultValue = "") const;
    int GetInt(const std::string& key, int defaultValue = 0) const;
    double GetDouble(const std::string& key, double defaultValue = 0.0) const;
    bool GetBool(const std::string& key, bool defaultValue = false) const;

    void SetString(const std::string& key, const std::string& value);
    void SetInt(const std::string& key, int value);
    void SetDouble(const std::string& key, double value);
    void SetBool(const std::string& key, bool value);

    bool HasKey(const std::string& key) const;
    void RemoveKey(const std::string& key);
    void Clear();
};

} 
} 

#endif 