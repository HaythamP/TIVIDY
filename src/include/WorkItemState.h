#ifndef WORKITEMSTATE_H
#define WORKITEMSTATE_H

#include <string>
#include <vector>

namespace tividy {

template <typename T> class WorkItem;
template <typename T> class Participant;

/**
 * @brief State of the State pattern: behaviour of a WorkItem in one lifecycle state.
 *
 * Every transition method throws InvalidTransitionException by default; concrete states override exactly the transitions they allow and call WorkItem::setState() as their last statement to move to the next state. States are stateless value objects: they are created by other states, owned by one WorkItem (or Memento) and deleted by WorkItem::setState().
 * Lifecycle (see WorkItem for the table): Created, Available, Assigned, InProgress, then Escalated or Approval, ending in Completed, Rejected or Cancelled.
 */
template <typename T>
class WorkItemState {
public:
    /** @brief Virtual destructor. */
    virtual ~WorkItemState();

    /** @brief Returns the state name, e.g. "InProgress". */
    virtual std::string getName() const = 0;
    /** @brief Returns a new state of the same kind (used by mementos). */
    virtual WorkItemState<T>* copy() const = 0;

    /** @brief True only for Completed. */
    virtual bool isComplete() const;
    /** @brief True only for Available. */
    virtual bool isAvailable() const;
    /**
     * @brief True for Completed, Rejected and Cancelled: the item no longer holds participant workload.
     */
    virtual bool isTerminal() const;

    /**
     * @brief Release request: a harmless no-op by default so already progressed or cancelled items are ignored.
     */
    virtual void makeAvailable(WorkItem<T>& item);
    /** @brief Assigns participants to the item. */
    virtual void assign(WorkItem<T>& item, const std::vector<Participant<T>*>& participants);
    /** @brief Starts the item. */
    virtual void start(WorkItem<T>& item);
    /** @brief Completes the item. */
    virtual void complete(WorkItem<T>& item);
    /** @brief Rejects the item. */
    virtual void reject(WorkItem<T>& item);
    /** @brief Cancels the item. */
    virtual void cancel(WorkItem<T>& item);
    /** @brief Escalates the item. */
    virtual void escalate(WorkItem<T>& item);
    /** @brief Sends the item to or through approval. */
    virtual void approve(WorkItem<T>& item);
};

}

#endif
