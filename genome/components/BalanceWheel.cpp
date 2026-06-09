#include "BalanceWheel.h"
#include <cmath>
#include <stdexcept>

namespace WatchGA {
namespace Genome {
namespace Components {

// ---------------------------------------------------------
// CONSTRUCTORS
// ---------------------------------------------------------
BalanceWheel::BalanceWheel()
    : m_momentOfInertia(1.0),
      m_isochronism(0.9),
      m_amplitude(270.0)
{
}

BalanceWheel::BalanceWheel(const std::string& name, double weight, double friction,
                           double momentOfInertia, double isochronism, double amplitude)
    : WatchComponent(name, weight, friction),
      m_momentOfInertia(momentOfInertia),
      m_isochronism(isochronism),
      m_amplitude(amplitude)
{
    setMomentOfInertia(momentOfInertia);
    setIsochronism(isochronism);
    setAmplitude(amplitude);
}

// ---------------------------------------------------------
// GETTERS & SETTERS (With Physics Clamping)
// ---------------------------------------------------------
double BalanceWheel::getMomentOfInertia() const { return m_momentOfInertia; }
double BalanceWheel::getIsochronism() const { return m_isochronism; }
double BalanceWheel::getAmplitude() const { return m_amplitude; }

void BalanceWheel::setMomentOfInertia(double moi) {
    if (moi < 0.1)
        throw std::invalid_argument("Moment of inertia minimum is 0.1 to prevent physics engine crash.");
    m_momentOfInertia = moi;
}

void BalanceWheel::setIsochronism(double isochronism) {
    if (isochronism < 0.0) m_isochronism = 0.0;
    else if (isochronism > 0.98) m_isochronism = 0.98;
    else m_isochronism = isochronism;
}

void BalanceWheel::setAmplitude(double amplitude) {
    if (amplitude < 0.0) m_amplitude = 0.0;
    else if (amplitude > 360.0) m_amplitude = 360.0;
    else m_amplitude = amplitude;
}

// =========================================================
// WATCHCOMPONENT OVERRIDES
// =========================================================

double BalanceWheel::calculateEfficiency() const {
    // Punishes deviations from the horological ideal of 270 degrees
    return m_isochronism *
           (1.0 - std::abs(m_amplitude - 270.0) / 360.0) *
           (1.0 - 1.0 / (m_momentOfInertia + 1.0));
}

std::unique_ptr<WatchComponent> BalanceWheel::clone() const {
    return std::make_unique<BalanceWheel>(*this);
}

std::string BalanceWheel::toString() const {
    return WatchComponent::toString() + "\n"
           "• Isochronism: " + std::to_string(m_isochronism) + "\n"
           "• Amplitude: " + std::to_string(m_amplitude) + " deg\n"
           "• MoI: " + std::to_string(m_momentOfInertia);
}

} // namespace Components
} // namespace Genome
} // namespace WatchGA