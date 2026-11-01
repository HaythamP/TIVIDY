#ifndef WORKFLOWBUILDER_H
#define WORKFLOWBUILDER_H

#include <string>
#include <vector>
#include "Builder.h"
#include "WorkflowDefinition.h"
#include "WorkGroup.h"
#include "WorkItem.h"
#include "Participant.h"

namespace tividy {

/**
 * @brief ConcreteBuilder: assembles a WorkflowDefinition step by step and returns it from getResult().
 *
 * Decorators are applied in a fixed order (Audited outermost, then Escalated, then Approved, then the item) whatever order the with...() calls were made in. Ids must be unique. It owns the product until getResult() hands it to the caller, and owns the pending item until endWorkItem().
 * @throws DuplicateIdException for a repeated id.
 * @throws InvalidWorkflowException for misuse (unfinished group or item, empty group or workflow).
 */
template <typename T>
class WorkflowBuilder : public Builder<T> {
public:
    /** @brief Creates a builder; call reset() before building. */
    WorkflowBuilder();
    /** @brief Deletes an unreleased product and pending item. */
    ~WorkflowBuilder();

    /** @brief Discards any unfinished product and starts a new one. */
    WorkflowBuilder& reset(const std::string& workflowName) override;
    WorkflowBuilder& addParticipant(Participant<T>* participant) override;

    WorkflowBuilder& beginGroup(const T& id, const std::string& name) override;
    WorkflowBuilder& endGroup() override;

    WorkflowBuilder& newWorkItem(const T& id, const std::string& name) override;
    WorkflowBuilder& withAudit() override;
    WorkflowBuilder& withApproval() override;
    WorkflowBuilder& withEscalation() override;
    WorkflowBuilder& endWorkItem() override;

    /**
     * @brief Validates and returns the finished workflow; ownership moves to the caller.
     *
     * @throws InvalidWorkflowException if groups or items are unfinished or the workflow is empty.
     */
    WorkflowDefinition<T>* getResult();

private:
    WorkflowBuilder(const WorkflowBuilder&) = delete;
    WorkflowBuilder& operator=(const WorkflowBuilder&) = delete;

    /** @brief Adds a component to the open group, or as a top-level step. */
    void attach(WorkComponent<T>* component);

    WorkflowDefinition<T>* workflow;  ///< Owned product under construction.
    WorkItem<T>* pendingItem;  ///< Owned item not yet wrapped and attached.
    bool audited;  ///< Wrap with AuditedWork.
    bool approved;  ///< Wrap with ApprovedWork.
    bool escalated;  ///< Wrap with EscalatedWork.
    std::vector<WorkGroup<T>*> openGroups;  ///< Stack of open groups (non-owning).
};

}

#endif