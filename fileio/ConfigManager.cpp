#include "ConfigManager.h"

#include <fstream>
#include <sstream>

namespace WatchGA {
namespace FileIO {

// Create an empty configuration manager.
ConfigManager::ConfigManager()
    : m_configFilePath("")
{
}

// Create a configuration manager with a file path.
ConfigManager::ConfigManager(const std::string& path)
    : m_configFilePath(path)
{
}

// Load configuration values from the file.
bool ConfigManager::loadConfig()
{
    std::ifstream inputFile(m_configFilePath);

    if (!inputFile.is_open())
    {
        return false;
    }

    // Clear old values before loading new ones.
    clear();

    std::string line;

    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::size_t separatorPosition = line.find('=');

        // Skip invalid lines.
        if (separatorPosition == std::string::npos)
        {
            continue;
        }

        std::string key = line.substr(0, separatorPosition);
        std::string value = line.substr(separatorPosition + 1);

        m_values[key] = value;
    }

    return true;
}

// Save all configuration values to the file.
bool ConfigManager::saveConfig() const
{
    std::ofstream outputFile(m_configFilePath);

    if (!outputFile.is_open())
    {
        return false;
    }

    // Write each key-value pair.
    for (const auto& pair : m_values)
    {
        outputFile << pair.first
                   << "="
                   << pair.second
                   << '\n';
    }

    return true;
}

// Get a string value.
std::string ConfigManager::getString(
    const std::string& key,
    const std::string& def) const
{
    auto iterator = m_values.find(key);

    if (iterator != m_values.end())
    {
        return iterator->second;
    }

    return def;
}

// Get an integer value.
int ConfigManager::getInt(
    const std::string& key,
    int def) const
{
    auto iterator = m_values.find(key);

    if (iterator != m_values.end())
    {
        try
        {
            return std::stoi(iterator->second);
        }
        catch (...)
        {
            return def;
        }
    }

    return def;
}

// Get a double value.
double ConfigManager::getDouble(
    const std::string& key,
    double def) const
{
    auto iterator = m_values.find(key);

    if (iterator != m_values.end())
    {
        try
        {
            return std::stod(iterator->second);
        }
        catch (...)
        {
            return def;
        }
    }

    return def;
}

// Get a boolean value.
bool ConfigManager::getBool(
    const std::string& key,
    bool def) const
{
    auto iterator = m_values.find(key);

    if (iterator != m_values.end())
    {
        const std::string& value = iterator->second;

        return value == "true" ||
               value == "TRUE" ||
               value == "True" ||
               value == "1";
    }

    return def;
}

// Store a string value.
void ConfigManager::setString(
    const std::string& key,
    const std::string& value)
{
    m_values[key] = value;
}

// Store an integer value.
void ConfigManager::setInt(
    const std::string& key,
    int value)
{
    m_values[key] = std::to_string(value);
}

// Store a double value.
void ConfigManager::setDouble(
    const std::string& key,
    double value)
{
    m_values[key] = std::to_string(value);
}

// Store a boolean value.
void ConfigManager::setBool(
    const std::string& key,
    bool value)
{
    m_values[key] = value ? "true" : "false";
}

// Check if a key exists.
bool ConfigManager::hasKey(const std::string& key) const
{
    return m_values.find(key) != m_values.end();
}

// Remove a key from the configuration.
void ConfigManager::removeKey(const std::string& key)
{
    m_values.erase(key);
}

// Remove all stored values.
void ConfigManager::clear()
{
    m_values.clear();
}

} // namespace FileIO
} // namespace WatchGA