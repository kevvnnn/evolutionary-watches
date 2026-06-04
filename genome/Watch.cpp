#include "Watch.h"

namespace WatchGA {
namespace Genome {

Watch::Watch()
    : m_id(0),
      m_fitness(0.0),
      m_name("DummyWatch")
{
}

Watch::~Watch()
{
}

int Watch::getId() const
{
    return m_id;
}

void Watch::setId(int id)
{
    m_id = id;
}

double Watch::getFitness() const
{
    return m_fitness;
}

void Watch::setFitness(double fitness)
{
    m_fitness = fitness;
}

std::string Watch::getName() const
{
    return m_name;
}

void Watch::setName(const std::string& name)
{
    m_name = name;
}

} // namespace Genome
} // namespace WatchGA