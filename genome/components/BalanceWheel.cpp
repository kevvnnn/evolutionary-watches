#include "BalanceWheel.h"
#include <cmath>
#include <stdexcept>

using namespace WatchGA::Genome;
using namespace WatchGA::Genome::Components;

BalanceWheel::BalanceWheel()
    : m_momentOfInertia(1.0),
      m_isochronism(0.9),
      m_amplitude(270.0)  // Optimal default
{
}

BalanceWheel::BalanceWheel(const std::string& name, double weight, double friction, double x, double y,
                           double momentOfInertia, double isochronism, double amplitude)
    : WatchComponent(name, weight, friction, x, y),
      m_momentOfInertia(momentOfInertia),
      m_isochronism(isochronism),
      m_amplitude(amplitude)
{
    SetMomentOfInertia(momentOfInertia);
    SetIsochronism(isochronism);
    SetAmplitude(amplitude);
}

// Getters
double BalanceWheel::GetMomentOfInertia() const { return m_momentOfInertia; }
double BalanceWheel::GetIsochronism() const { return m_isochronism; }
double BalanceWheel::GetAmplitude() const { return m_amplitude; }

// ------------------------------
// Validation
// ------------------------------
void BalanceWheel::SetMomentOfInertia(double moi) {
    if (moi < 0.1)
        throw std::invalid_argument("Moment of inertia minimum is 0.1");
    m_momentOfInertia = moi;
}

void BalanceWheel::SetIsochronism(double isochronism) {
    if (isochronism < 0.0) m_isochronism = 0.0;
    else if (isochronism > 1.0) m_isochronism = 1.0;
    else m_isochronism = isochronism;
}

void BalanceWheel::SetAmplitude(double amplitude) {
    if (amplitude < 0.0) m_amplitude = 0.0;
    else if (amplitude > 360.0) m_amplitude = 360.0;
    else m_amplitude = amplitude;
}

// MOST IMPORTANT: Timekeeping efficiency
double BalanceWheel::CalculateEfficiency() const {
    return m_isochronism *
           (1.0 - std::abs(m_amplitude - 270.0) / 360.0) *
           (1.0 - 1.0 / (m_momentOfInertia + 1.0));
}

// Deep copy for evolution
std::unique_ptr<WatchComponent> BalanceWheel::Clone() const {
    return std::make_unique<BalanceWheel>(*this);
}

std::string BalanceWheel::ToString() const {
    return WatchComponent::ToString() +
           " [BalanceWheel | Isochronism: " + std::to_string(m_isochronism) + "]";
}