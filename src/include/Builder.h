#ifndef BUILDER_H
#define BUILDER_H

#include <string>

namespace tividy {

template <typename T> class Participant;

/**
 * @brief Builder of the Builder pattern: the abstract steps for assembling a workflow tree. Calls can be chained.
 */
template <typename T>
class Builder {
public:
    /** @brief Virtual destructor. */
    virtual ~Builder();

    /** @brief Starts a new workflow with the given name. */
    virtual Builder<T>& reset(const std::string& workflowName) = 0;
    /** @brief Adds a participant to the new workflow's pool. */
    virtual Builder<T>& addParticipant(Participant<T>* participant) = 0;

    /** @brief Opens a WorkGroup (nesting allowed). Children added until endGroup() run in parallel. */
    virtual Builder<T>& beginGroup(const T& id, const std::string& name) = 0;
    /** @brief Closes the innermost group. */
    virtual Builder<T>& endGroup() = 0;

    /** @brief Starts a WorkItem with a user supplied id and name. */
    virtual Builder<T>& newWorkItem(const T& id, const std::string& name) = 0;
    /** @brief Decorates the current item with AuditedWork. */
    virtual Builder<T>& withAudit() = 0;
    /** @brief Decorates the current item with ApprovedWork. */
    virtual Builder<T>& withApproval() = 0;
    /** @brief Decorates the current item with EscalatedWork. */
    virtual Builder<T>& withEscalation() = 0;
    /** @brief Finishes the item (applies decorators) and adds it to the current group or workflow. */
    virtual Builder<T>& endWorkItem() = 0;
};

}

#endif