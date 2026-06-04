#ifndef WATCH_H
#define WATCH_H

#include <string>

namespace WatchGA {
namespace Genome {

class Watch
{
public:
    Watch();
    ~Watch();

    int getId() const;
    void setId(int id);

    double getFitness() const;
    void setFitness(double fitness);

    std::string getName() const;
    void setName(const std::string& name);

private:
    int m_id;
    double m_fitness;
    std::string m_name;
};

} // namespace Genome
} // namespace WatchGA

#endif