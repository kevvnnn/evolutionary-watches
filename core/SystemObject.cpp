#include "SystemObject.h"
#include <stdexcept>

namespace WatchGA {
namespace Core {

// ==============================================================================
// THE MAGIC LINE: Initializing the static variable.
// Without this single line, the Linker will crash the entire project.
// We start IDs at 1, reserving 0 as a potential "null" or "invalid" ID if needed later.
// ==============================================================================
unsigned int SystemObject::m_nextObjectId = 1;

// ---------------------------------------------------------
// CONSTRUCTORS
// ---------------------------------------------------------

SystemObject::SystemObject() 
    : m_objectId(m_nextObjectId++), 
      m_objectName("Unnamed_Object_" + std::to_string(m_objectId)) 
{
    // The initializer list grabs the current m_nextObjectId, assigns it to this object,
    // and then increments the static counter for the next object.
}

SystemObject::SystemObject(const std::string& name) 
    : m_objectId(m_nextObjectId++) 
{
    // We use the setter here to take advantage of any validation we might add later
    setObjectName(name);
}

// ---------------------------------------------------------
// GETTERS
// ---------------------------------------------------------

unsigned int SystemObject::getObjectId() const {
    return m_objectId;
}

const std::string& SystemObject::getObjectName() const {
    return m_objectName;
}

// ---------------------------------------------------------
// SETTERS
// ---------------------------------------------------------

void SystemObject::setObjectName(const std::string& name) {
    // Basic validation: Don't allow completely empty names
    if (name.empty()) {
        m_objectName = "Unnamed_Object_" + std::to_string(m_objectId);
    } else {
        m_objectName = name;
    }
}

// ---------------------------------------------------------
// VIRTUAL METHODS
// ---------------------------------------------------------

std::string SystemObject::toString() const {
    // Returns a clean, standardized string format: "[ID: 42] Mainspring"
    return "[ID: " + std::to_string(m_objectId) + "] " + m_objectName;
}

} // namespace Core
} // namespace WatchGA