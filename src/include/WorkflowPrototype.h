#ifndef WORKFLOWPROTOTYPE_H
#define WORKFLOWPROTOTYPE_H

namespace tividy {

/** @brief Prototype of the Prototype pattern: declares clone(). */
template <typename T>
class WorkflowPrototype {
public:
    /** @brief Virtual destructor. */
    virtual ~WorkflowPrototype() = default;
    /** @brief Returns a deep copy owned by the caller. */
    virtual WorkflowPrototype<T>* clone() const = 0;
};

}

#endif
