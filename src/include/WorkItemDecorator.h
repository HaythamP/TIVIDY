#ifndef WORKITEMDECORATOR_H
#define WORKITEMDECORATOR_H

#include <string>
#include <vector>
#include "WorkComponent.h"

namespace tividy {

/**
 * @brief Decorator of the Decorator pattern: wraps another WorkComponent and forwards everything to it.
 *
 * Concrete decorators override only what they add. The decorator owns the wrapped component (`inner`). Queries that identify the component (find, getItems, iteration) report the outermost decorator so commands always run through every layer; parent and mediator are forwarded to the innermost WorkItem, which is the real colleague of the Mediator.
 * The WorkflowBuilder always wraps in this order: AuditedWork( EscalatedWork( ApprovedWork( WorkItem ) ) ).
 */
template <typename T>
class WorkItemDecorator : public WorkComponent<T> {
public:
    /**
     * @brief Wraps the given component and takes ownership of it.
     *
     * @throws InvalidWorkflowException if it is null.
     */
    explicit WorkItemDecorator(WorkComponent<T>* inner);
    /** @brief Deletes the wrapped component. */
    ~WorkItemDecorator();

    const T& getId() const override;
    const std::string& getName() const override;
    WorkGroup<T>* getParent() const override;
    void setParent(WorkGroup<T>* parent) override;
    Mediator<T>* getMediator() const override;
    void setMediator(Mediator<T>* mediator) override;

    std::vector<WorkComponent<T>*> getChildren() const override;
    /** @brief Returns this decorator (not the wrapped item). */
    std::vector<WorkComponent<T>*> getItems() override;
    void add(WorkComponent<T>* child) override;
    /** @brief Returns this decorator if the wrapped component matches the id. */
    WorkComponent<T>* find(const T& id) override;

    void release() override;
    bool isComplete() const override;
    bool isAvailable() const override;

    std::string getStateName() const override;
    std::vector<Participant<T>*> getAssignees() const override;
    bool hasEscalation() const override;
    bool hasApproval() const override;
    std::vector<std::string> getAuditLog() const override;
    Participant<T>* getEscalationContact() const override;
    Participant<T>* getApprovalContact() const override;
    void addAuditEntry(const std::string& entry) override;
    void editAuditEntry(std::size_t index, const std::string& entry) override;
    void deleteAuditEntry(std::size_t index) override;

    void setEscalationChain(Handler<T>* chain) override;
    void setApprovalChain(Handler<T>* chain) override;
    std::vector<Participant<T>*> assignWork(const std::vector<Participant<T>*>& pool, const std::vector<AssignmentStrategy<T>*>& strategies, int numAssignees = -1) const override;
    void assignTo(const std::vector<Participant<T>*>& participants) override;
    void start() override;
    void complete() override;
    void reject() override;
    void cancel() override;
    void escalate() override;
    void approve() override;

    WorkMemento<T>* createMemento() const override;
    void restore(const WorkMemento<T>& memento) override;

    /** @brief Iterator over this single decorated item. */
    WorkIterator<T>* createIterator() override;

protected:
    WorkComponent<T>* inner;  ///< Owned wrapped component.
};

}

#endif
