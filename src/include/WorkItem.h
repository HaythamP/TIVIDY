#ifndef WORKITEM_H
#define WORKITEM_H

#include <string>
#include <vector>
#include "WorkComponent.h"
#include "WorkItemState.h"
#include "AssignmentStrategy.h"
#include "Participant.h"
#include "WorkMemento.h"

namespace tividy {

/**
 * @brief Leaf of the Composite pattern, Context of the State pattern and Originator of the Memento pattern: one step of a workflow.
 *
 * The item owns exactly one WorkItemState and delegates every operation to it; states replace themselves through setState(). Lifecycle:
 * | State | Allowed operations |
 * |---|---|
 * | Created | release to Available, cancel |
 * | Available | assign to Assigned, cancel |
 * | Assigned | start to InProgress, cancel |
 * | InProgress | complete, reject, escalate, approve, cancel |
 * | Escalated | complete, reject, cancel, escalate (self), approve |
 * | Approval | complete, reject, cancel, approve (self) |
 * | Completed, Rejected, Cancelled | terminal (Rejected from Approval can be started again) |
 *
 * Assignees are non-owning pointers to participants whose workload this class keeps correct. Every state change is printed to the console. After a completion the item tells its Mediator so the next step can be released.
 */
template <typename T>
class WorkItem : public WorkComponent<T> {
public:
    /** @brief Creates an item in the Created state without assignees. */
    WorkItem(const T& id, const std::string& name);
    /** @brief Deletes the current state. */
    ~WorkItem();

    const T& getId() const override;
    const std::string& getName() const override;
    std::string getStateName() const override;
    std::vector<Participant<T>*> getAssignees() const override;

    std::vector<WorkComponent<T>*> getItems() override;
    /** @brief Returns this item if the id matches. */
    WorkComponent<T>* find(const T& id) override;
    /** @brief Created to Available; a no-op in every other state. */
    void release() override;
    bool isComplete() const override;
    bool isAvailable() const override;

    /**
     * @brief Pure selection step; validates the arguments, applies the strategies in order and picks the assignees.
     */
    std::vector<Participant<T>*> assignWork(const std::vector<Participant<T>*>& pool, const std::vector<AssignmentStrategy<T>*>& strategies, int numAssignees = -1) const override;
    /**
     * @brief Delegates to the state; Available stores the assignees and moves to Assigned.
     *
     * @throws InvalidAssignmentArgumentException for an empty list or null participants.
     */
    void assignTo(const std::vector<Participant<T>*>& participants) override;
    void start() override;
    /** @brief Delegates to the state and, once Completed, notifies the Mediator. */
    void complete() override;
    void reject() override;
    void cancel() override;
    void escalate() override;
    void approve() override;

    /** @brief Snapshots a copy of the state and the assignees. */
    WorkMemento<T>* createMemento() const override;
    /** @brief Restores state and assignees from the memento and corrects the participants' workload. */
    void restore(const WorkMemento<T>& memento) override;

    /** @brief Returns a new, unassigned item in Created. */
    WorkComponent<T>* copy() const override;
    /** @brief Calls visitor.visit(item). */
    void accept(WorkVisitor<T>& visitor) override;
    WorkIterator<T>* createIterator() override;

    /**
     * @brief Replaces the current state (takes ownership, deletes the old one) and adjusts workload when the item enters or leaves a terminal state. States call this as their last statement.
     */
    void setState(WorkItemState<T>* state);
    /** @brief Stores the assignees and increases their workload. */
    void setAssignees(const std::vector<Participant<T>*>& participants);

private:
    /** @brief True while assignees exist and the item is not in a terminal state. */
    bool holdsWorkload() const;
    /** @brief Adds delta to the workload of every assignee. */
    void adjustWorkloads(int delta);

    T id;  ///< Item id.
    std::string name;  ///< Item name.
    WorkItemState<T>* state;  ///< Owned current state.
    std::vector<Participant<T>*> assignees;  ///< Non-owning assigned participants.
};

}

#endif
