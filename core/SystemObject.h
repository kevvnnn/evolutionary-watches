#ifndef WATCH_GA_CORE_SYSTEMOBJECT_H
#define WATCH_GA_CORE_SYSTEMOBJECT_H

#include <string>

// All code in our project lives inside the WatchGA namespace
// This prevents naming conflicts with other libraries
namespace WatchGA {
namespace Core {

/**
 * @brief The ROOT BASE CLASS for EVERY object in the entire system
 * 
 * EVERY SINGLE CLASS in this project inherits from SystemObject directly or indirectly.
 * This gives all objects:
 * 1. A unique ID number (automatically assigned)
 * 2. A human-readable name
 * 3. A standard way to convert themselves to a string (for debugging/saving)
 * 
 * This is the most important class in the entire project.
 * If you change this, you break EVERYTHING. Think twice before modifying.
 */
class SystemObject {
private:
    // Unique ID for this object. Automatically incremented for each new object.
    unsigned int m_objectId;
    
    // Human-readable name (e.g., "Main Gear", "Balance Spring")
    std::string m_objectName;
    
    // Static counter that keeps track of the next available ID
    // This is shared by ALL SystemObject instances
    static unsigned int m_nextObjectId;

public:
    /**
     * @brief Default constructor
     * Creates a SystemObject with a default name like "Object_123"
     */
    SystemObject();
    
    /**
     * @brief Constructor with custom name
     * @param name The human-readable name for this object
     */
    explicit SystemObject(const std::string& name);
    
    /**
     * @brief Virtual destructor
     * This is REQUIRED for proper polymorphism.
     * If you delete a derived class through a base class pointer,
     * this ensures the correct destructor is called.
     */
    virtual ~SystemObject() = default;

    // ------------------------------
    // GETTERS (Read-only access)
    // ------------------------------
    
    /**
     * @brief Gets the unique ID of this object
     * @return The object's ID number
     */
    unsigned int GetObjectId() const;
    
    /**
     * @brief Gets the human-readable name of this object
     * @return The object's name
     */
    const std::string& GetObjectName() const;

    // ------------------------------
    // SETTERS (Write access with validation)
    // ------------------------------
    
    /**
     * @brief Sets the human-readable name of this object
     * @param name The new name (cannot be empty)
     */
    void SetObjectName(const std::string& name);

    // ------------------------------
    // VIRTUAL METHODS (Polymorphic)
    // ------------------------------
    
    /**
     * @brief Converts this object to a string representation
     * @return A string describing the object
     * 
     * Every derived class should override this to include its own properties.
     * This is used for debugging, logging, and saving to files.
     */
    virtual std::string ToString() const;
};

} // namespace Core
} // namespace WatchGA

#endif // WATCH_GA_CORE_SYSTEMOBJECT_H