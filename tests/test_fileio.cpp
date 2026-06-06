#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <cstdio>
#include <stdexcept>

// ========================
// Genome::Watch stub
// ========================
namespace WatchGA { namespace Genome {

class Watch
{
private:
    int m_id = 0;
    double m_fitness = 0.0;
    std::string m_name;

public:
    Watch() = default;
    ~Watch() = default;

    void setId(int id) { m_id = id; }
    int getId() const { return m_id; }

    void setFitness(double f) { m_fitness = f; }
    double getFitness() const { return m_fitness; }

    void setName(const std::string& n) { m_name = n; }
    std::string getName() const { return m_name; }
};

} } // namespace WatchGA::Genome

// ========================
// FileIO Namespace
// ========================
namespace WatchGA { namespace FileIO {

// ========================
// ConfigManager
// ========================
class ConfigManager
{
private:
    std::string m_configFilePath;
    std::map<std::string, std::string> m_values;

public:
    ConfigManager() : m_configFilePath("config.txt") {}
    explicit ConfigManager(const std::string& path) : m_configFilePath(path) {}
    ~ConfigManager() = default;

    bool loadConfig()
    {
        std::ifstream inputFile(m_configFilePath);
        if (!inputFile.is_open()) return false;
        clear();
        std::string line;
        while (std::getline(inputFile, line))
        {
            if (line.empty()) continue;
            auto pos = line.find('=');
            if (pos == std::string::npos) continue;
            m_values[line.substr(0,pos)] = line.substr(pos+1);
        }
        return true;
    }

    bool saveConfig() const
    {
        std::ofstream outputFile(m_configFilePath);
        if (!outputFile.is_open()) return false;
        for (const auto& p : m_values)
            outputFile << p.first << "=" << p.second << '\n';
        return true;
    }

    std::string getString(const std::string& key, const std::string& def="") const
    {
        auto it = m_values.find(key);
        return it != m_values.end() ? it->second : def;
    }

    int getInt(const std::string& key, int def=0) const
    {
        auto it = m_values.find(key);
        if (it != m_values.end()) { try { return std::stoi(it->second); } catch (...) { return def; } }
        return def;
    }

    double getDouble(const std::string& key, double def=0.0) const
    {
        auto it = m_values.find(key);
        if (it != m_values.end()) { try { return std::stod(it->second); } catch (...) { return def; } }
        return def;
    }

    bool getBool(const std::string& key, bool def=false) const
    {
        auto it = m_values.find(key);
        if (it != m_values.end())
        {
            const std::string& val = it->second;
            return val=="true" || val=="TRUE" || val=="True" || val=="1";
        }
        return def;
    }

    void setString(const std::string& key, const std::string& value) { m_values[key] = value; }
    void setInt(const std::string& key, int value) { m_values[key] = std::to_string(value); }
    void setDouble(const std::string& key, double value) { m_values[key] = std::to_string(value); }
    void setBool(const std::string& key, bool value) { m_values[key] = value ? "true" : "false"; }

    bool hasKey(const std::string& key) const { return m_values.find(key)!=m_values.end(); }
    void removeKey(const std::string& key) { m_values.erase(key); }
    void clear() { m_values.clear(); }
};

// ========================
// EvolutionHistory
// ========================
class EvolutionHistory
{
public:
    struct GenerationRecord
    {
        unsigned int generationNumber;
        double bestFitness;
        double averageFitness;
        double worstFitness;
        std::shared_ptr<Genome::Watch> bestWatch;
    };

private:
    std::vector<GenerationRecord> m_records;
    std::string m_experimentName;

public:
    EvolutionHistory() : m_experimentName("") {}
    explicit EvolutionHistory(const std::string& name) : m_experimentName(name) {}
    ~EvolutionHistory() = default;

    void addRecord(const GenerationRecord& record) { m_records.push_back(record); }
    const std::vector<GenerationRecord>& getAllRecords() const { return m_records; }
    const GenerationRecord& getRecord(unsigned int gen) const { 
        if (gen >= m_records.size()) throw std::out_of_range("Generation index out of range");
        return m_records[gen];
    }
    unsigned int getRecordCount() const { return static_cast<unsigned int>(m_records.size()); }

    const std::string& getExperimentName() const { return m_experimentName; }
    void setExperimentName(const std::string& name) { m_experimentName = name; }
    void clear() { m_records.clear(); m_experimentName.clear(); }

    bool saveToFile(const std::string& path) const
    {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open()) return false;

        size_t nameLength = m_experimentName.size();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(m_experimentName.data(), nameLength);

        unsigned int recordCount = getRecordCount();
        file.write(reinterpret_cast<const char*>(&recordCount), sizeof(recordCount));

        for (const auto& r : m_records)
        {
            file.write(reinterpret_cast<const char*>(&r.generationNumber), sizeof(r.generationNumber));
            file.write(reinterpret_cast<const char*>(&r.bestFitness), sizeof(r.bestFitness));
            file.write(reinterpret_cast<const char*>(&r.averageFitness), sizeof(r.averageFitness));
            file.write(reinterpret_cast<const char*>(&r.worstFitness), sizeof(r.worstFitness));

            if (r.bestWatch)
            {
                int id = r.bestWatch->getId();
                double fit = r.bestWatch->getFitness();
                std::string name = r.bestWatch->getName();
                size_t nlen = name.size();

                file.write(reinterpret_cast<const char*>(&id), sizeof(id));
                file.write(reinterpret_cast<const char*>(&fit), sizeof(fit));
                file.write(reinterpret_cast<const char*>(&nlen), sizeof(nlen));
                file.write(name.data(), nlen);
            }
        }
        return true;
    }

    bool loadFromFile(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) return false;

        clear();

        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        m_experimentName.resize(nameLength);
        file.read(&m_experimentName[0], nameLength);

        unsigned int recordCount;
        file.read(reinterpret_cast<char*>(&recordCount), sizeof(recordCount));

        for (unsigned int i=0;i<recordCount;i++)
        {
            GenerationRecord r;
            file.read(reinterpret_cast<char*>(&r.generationNumber), sizeof(r.generationNumber));
            file.read(reinterpret_cast<char*>(&r.bestFitness), sizeof(r.bestFitness));
            file.read(reinterpret_cast<char*>(&r.averageFitness), sizeof(r.averageFitness));
            file.read(reinterpret_cast<char*>(&r.worstFitness), sizeof(r.worstFitness));

            int id; double fit; size_t nlen;
            file.read(reinterpret_cast<char*>(&id), sizeof(id));
            file.read(reinterpret_cast<char*>(&fit), sizeof(fit));
            file.read(reinterpret_cast<char*>(&nlen), sizeof(nlen));

            std::string name; name.resize(nlen);
            file.read(&name[0], nlen);

            auto w = std::unique_ptr<Genome::Watch>(new Genome::Watch());
            w->setId(id);
            w->setFitness(fit);
            w->setName(name);

            r.bestWatch = std::move(w);
            m_records.push_back(r);
        }

        return true;
    }
};

// ========================
// WatchFileIO (fully functional)
// ========================
class WatchFileIO
{
private:
    std::string m_filePath;
    bool m_isOpen;

    void writeWatch(const Genome::Watch& watch, std::ofstream& outFile)
    {
        int id = watch.getId();
        double fit = watch.getFitness();
        std::string name = watch.getName();
        size_t nameLen = name.size();

        outFile.write(reinterpret_cast<const char*>(&id), sizeof(id));
        outFile.write(reinterpret_cast<const char*>(&fit), sizeof(fit));
        outFile.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        outFile.write(name.data(), nameLen);
    }

    std::unique_ptr<Genome::Watch> readWatch(std::ifstream& inFile)
    {
        int id; double fit; size_t nameLen;
        inFile.read(reinterpret_cast<char*>(&id), sizeof(id));
        inFile.read(reinterpret_cast<char*>(&fit), sizeof(fit));
        inFile.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));

        std::string name(nameLen, '\0');
        inFile.read(&name[0], nameLen);

        auto w = std::unique_ptr<Genome::Watch>(new Genome::Watch());
        w->setId(id);
        w->setFitness(fit);
        w->setName(name);

        return w;
    }

public:
    WatchFileIO() : m_filePath(""), m_isOpen(false) {}
    explicit WatchFileIO(const std::string& path) : m_filePath(path), m_isOpen(false) {}
    ~WatchFileIO() { close(); }

    bool open(bool createIfMissing = true)
    {
        if (m_isOpen) return true;
        if (m_filePath.empty()) return false;

        std::ifstream input(m_filePath);
        if (!input.good() && createIfMissing)
        {
            std::ofstream output(m_filePath);
            if (!output.good()) return false;
        }
        m_isOpen = true;
        return true;
    }

    void close() { m_isOpen=false; }
    bool isOpen() const { return m_isOpen; }

    bool saveWatch(const Genome::Watch& watch, unsigned int)
    {
        if (!m_isOpen) return false;
        std::ofstream file(m_filePath, std::ios::binary | std::ios::app);
        if (!file.is_open()) return false;
        writeWatch(watch, file);
        return true;
    }

    std::unique_ptr<Genome::Watch> loadWatch(unsigned int)
    {
        if (!m_isOpen) return nullptr;
        std::ifstream file(m_filePath, std::ios::binary);
        if (!file.is_open()) return nullptr;
        return readWatch(file);
    }

    unsigned int getWatchCount() const
    {
        if (!m_isOpen) return 0;
        std::ifstream file(m_filePath, std::ios::binary);
        if (!file.is_open()) return 0;

        unsigned int count = 0;
        while (file.peek() != EOF)
        {
            int id; double fit; size_t nlen;
            file.read(reinterpret_cast<char*>(&id), sizeof(id));
            file.read(reinterpret_cast<char*>(&fit), sizeof(fit));
            file.read(reinterpret_cast<char*>(&nlen), sizeof(nlen));
            file.seekg(nlen, std::ios::cur);
            ++count;
        }
        return count;
    }
};

}} // namespace WatchGA::FileIO

// ========================
// TEST
// ========================
using namespace WatchGA;
using namespace WatchGA::FileIO;

static std::string tempFile(const std::string& name) { return "./temp_" + name; }

int main()
{
    // ConfigManager test
    std::string cfgPath = tempFile("config.cfg");
    ConfigManager cfg(cfgPath);
    cfg.setString("name","fileio_test"); cfg.setInt("population",100); cfg.setDouble("mutation",0.25); cfg.setBool("elitism",true);
    assert(cfg.saveConfig());
    ConfigManager cfg2(cfgPath); assert(cfg2.loadConfig());
    assert(cfg2.getString("name")=="fileio_test");
    std::remove(cfgPath.c_str());

    // EvolutionHistory test
    std::string histPath = tempFile("history.bin");
    EvolutionHistory hist("exp_test");
    EvolutionHistory::GenerationRecord rec{1,99.0,50.0,10.0,std::make_shared<Genome::Watch>()};
    rec.bestWatch->setId(7); rec.bestWatch->setFitness(99.0); rec.bestWatch->setName("elite");
    hist.addRecord(rec); assert(hist.saveToFile(histPath));
    EvolutionHistory loaded; assert(loaded.loadFromFile(histPath));
    assert(loaded.getRecordCount()==1);
    std::remove(histPath.c_str());

    // WatchFileIO test
    std::string watchPath = tempFile("watch.dat");
    WatchFileIO io(watchPath);
    assert(io.open(true)); assert(io.isOpen());
    Genome::Watch watch; watch.setId(1); watch.setFitness(2.5); watch.setName("watch_test");
    assert(io.saveWatch(watch,0));
    auto lw = io.loadWatch(0); assert(lw!=nullptr);
    assert(io.getWatchCount() == 1);
    io.close(); std::remove(watchPath.c_str());

    std::cout << "All FileIO tests passed!\n";
    return 0;
}