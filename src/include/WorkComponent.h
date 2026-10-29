#ifndef WORKCOMPONENT_H
#define WORKCOMPONENT_H

#include <string>
#include <vector>

namespace tividy {

template <typename T> class WorkGroup;
template <typename T> class Mediator;
template <typename T> class WorkVisitor;
template <typename T> class WorkIterator;
template <typename T> class WorkMemento;
template <typename T> class AssignmentStrategy;
template <typename T> class Participant;
template <typename T> class Handler;

/**
 * @brief Common interface of the workflow tree: Component of the Composite pattern, Component of the Decorator pattern, Element of the Visitor pattern, Originator interface of the Memento pattern and Colleague of the Mediator pattern.
 *
 * A workflow is a tree of WorkGroup (composite) and WorkItem (leaf) objects. Decorators (AuditedWork, EscalatedWork, ApprovedWork) wrap a WorkItem and are themselves WorkComponents, so clients treat decorated and plain items identically.
 * Operations that only make sense for an individual item (start, complete, assign, ...) throw UnsupportedOperationException by default; WorkItem and the decorators override them. Groups therefore reject commands.
 * Ownership: `parent` and `mediator` are non-owning back references. Children are owned by their WorkGroup (or by the WorkflowDefinition for top-level steps).
 * @tparam T type of component ids (int or std::string).
 */
template <typename T>
class WorkComponent {
public:
    /** @brief Virtual destructor. */
    virtual ~WorkComponent();

    /** @brief Returns the component id (unique within a workflow). */
    virtual const T& getId() const = 0;
    /** @brief Returns the display name given by the user. */
    virtual const std::string& getName() const = 0;

    /** @brief Returns the enclosing WorkGroup, or null for a top-level step. */
    virtual WorkGroup<T>* getParent() const;
    /** @brief Sets the enclosing group (non-owning). Called by WorkGroup::add(). */
    virtual void setParent(WorkGroup<T>* parent);
    /** @brief Returns the mediator notified when an item completes. */
    virtual Mediator<T>* getMediator() const;
    /**
     * @brief Sets the mediator (non-owning). Groups propagate it to their children and decorators forward it to the wrapped item.
     */
    virtual void setMediator(Mediator<T>* mediator);

    /** @brief Returns the direct children (empty for items). Used by DepthFirstIterator. */
    virtual std::vector<WorkComponent<T>*> getChildren() const;
    /**
     * @brief Returns every individual item below this component, flattened. A decorated item reports its outermost decorator so commands always hit the decorators.
     */
    virtual std::vector<WorkComponent<T>*> getItems();
    /**
     * @brief Adds a child component (groups only; takes ownership).
     *
     * @throws UnsupportedOperationException on items.
     */
    virtual void add(WorkComponent<T>* child);
    /**
     * @brief Searches this component and its descendants for an id and returns the outermost matching component, or null.
     */
    virtual WorkComponent<T>* find(const T& id) = 0;

    /**
     * @brief Makes the component available for work (Created to Available). Called by the Mediator when the previous step completes, or by WorkflowDefinition::start() for the first step.
     */
    virtual void release() = 0;
    /**
     * @brief Returns true when the component is complete: an item is Completed, a group is complete when all of its children are.
     */
    virtual bool isComplete() const = 0;
    /** @brief Returns true when the item can currently be assigned. */
    virtual bool isAvailable() const = 0;

    /** @brief Returns the lifecycle state name of an item (empty for groups). */
    virtual std::string getStateName() const;
    /** @brief Returns the participants assigned to the item. */
    virtual std::vector<Participant<T>*> getAssignees() const;
    /** @brief Returns true if the item is decorated with EscalatedWork. */
    virtual bool hasEscalation() const;
    /** @brief Returns true if the item is decorated with ApprovedWork. */
    virtual bool hasApproval() const;
    /** @brief Returns a copy of the audit log (empty when the item is not audited). */
    virtual std::vector<std::string> getAuditLog() const;
    /** @brief Returns the participant currently handling the escalation, or null. */
    virtual Participant<T>* getEscalationContact() const;
    /** @brief Returns the participant currently handling the approval, or null. */
    virtual Participant<T>* getApprovalContact() const;

    /**
     * @brief Appends a manual entry to the audit log.
     *
     * @throws UnsupportedOperationException if the item is not audited.
     */
    virtual void addAuditEntry(const std::string& entry);
    /**
     * @brief Replaces the audit entry at a zero-based index.
     *
     * @throws UnsupportedOperationException if the item is not audited.
     * @throws AuditEntryNotFoundException if the index does not exist.
     */
    virtual void editAuditEntry(std::size_t index, const std::string& entry);
    /**
     * @brief Removes the audit entry at a zero-based index.
     *
     * @throws UnsupportedOperationException if the item is not audited.
     * @throws AuditEntryNotFoundException if the index does not exist.
     */
    virtual void deleteAuditEntry(std::size_t index);

    /**
     * @brief Installs the escalation Chain of Responsibility. Takes ownership only on success (EscalatedWork only).
     */
    virtual void setEscalationChain(Handler<T>* chain);
    /**
     * @brief Installs the approval Chain of Responsibility. Takes ownership only on success (ApprovedWork only).
     */
    virtual void setApprovalChain(Handler<T>* chain);
    /**
     * @brief Selects assignees without changing anything (Strategy pattern).
     *
     * The candidates are the given pool; each strategy filters the result of the previous one in list order; then numAssignees are chosen (the first ones, or random ones if a RuleAssignment is present). Pass -1 to select everyone left.
     * @throws InvalidAssignmentArgumentException for invalid arguments.
     * @throws InsufficientParticipantsException if too few candidates remain.
     */
    virtual std::vector<Participant<T>*> assignWork(const std::vector<Participant<T>*>& pool, const std::vector<AssignmentStrategy<T>*>& strategies, int numAssignees = -1) const;
    /** @brief Assigns the given participants (Available to Assigned) and increases their workload. */
    virtual void assignTo(const std::vector<Participant<T>*>& participants);
    /** @brief Starts work (Assigned to InProgress, or restarts an item rejected from Approval). */
    virtual void start();
    /** @brief Completes the item and notifies the Mediator so the next step can be released. */
    virtual void complete();
    /**
     * @brief Rejects the item (fail). Halts the workflow; only an item rejected from Approval can be started again.
     */
    virtual void reject();
    /** @brief Cancels the item. Halts the workflow. */
    virtual void cancel();
    /** @brief Starts or advances the escalation chain. */
    virtual void escalate();
    /** @brief Starts or advances the approval chain. */
    virtual void approve();

    /** @brief Creates a snapshot of the item (Memento pattern) used to undo commands. */
    virtual WorkMemento<T>* createMemento() const;
    /** @brief Restores the item from a snapshot produced by createMemento(). */
    virtual void restore(const WorkMemento<T>& memento);

    /**
     * @brief Returns a deep copy: a fresh, unassigned component in the Created state (Prototype support).
     */
    virtual WorkComponent<T>* copy() const = 0;
    /**
     * @brief Accepts a visitor (Visitor pattern). Decorators first announce themselves, then forward to the wrapped item.
     */
    virtual void accept(WorkVisitor<T>& visitor) = 0;
    /** @brief Creates a depth-first iterator rooted at this component. The caller deletes it. */
    virtual WorkIterator<T>* createIterator() = 0;

protected:
    /** @brief Protected constructor: parent and mediator start as null. */
    WorkComponent();

    WorkGroup<T>* parent;  ///< Enclosing group (non-owning).
    Mediator<T>* mediator;  ///< Colleague link to the mediator (non-owning).

private:
    WorkComponent(const WorkComponent&) = delete;
    WorkComponent& operator=(const WorkComponent&) = delete;
};

}

#endif
