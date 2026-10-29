#ifndef APPROVEDWORK_H
#define APPROVEDWORK_H

#include "WorkItemDecorator.h"
#include "Handler.h"
#include "Participant.h"
#include "WorkMemento.h"

namespace tividy {

/**
 * @brief ConcreteDecoratorA: adds a multi-level approval to a work item and acts as the client of an approval Chain of Responsibility.
 *
 * The chain contains only LevelHandlers (one per approver, no fallback). Each Approve command moves one link forward:
 * - the first command moves InProgress (or Escalated) to Approval, starts the chain and passes the request to the first approver in the same step;
 * - every further command means the previous approver signed off and the next approver takes over;
 * - when the approver reached is the last one the item completes immediately, so N approvers need N Approve commands.
 * `complete()` is refused until approval has finished. Reject and Cancel are allowed during approval; an item rejected from Approval stays Rejected (halting the workflow) until the Start command restarts it, which also restarts the approval chain.
 */
template <typename T>
class ApprovedWork : public WorkItemDecorator<T> {
public:
    /** @brief Wraps the component; no chain yet (null), position -1. */
    explicit ApprovedWork(WorkComponent<T>* inner);
    /** @brief Deletes the chain. */
    ~ApprovedWork();

    bool hasApproval() const override;
    Participant<T>* getApprovalContact() const override;
    /**
     * @brief Takes ownership of the chain and resets the progress.
     *
     * @throws InvalidHandlerChainException if the chain is null/empty or contains anything but LevelHandlers.
     */
    void setApprovalChain(Handler<T>* chain) override;

    /** @brief Starts or restarts the item and resets the approval progress. */
    void start() override;
    /**
     * @brief Starts or advances the chain and prints who is approving; completes the item at the last approver.
     *
     * @throws InvalidHandlerChainException if no chain was assigned.
     * @throws InvalidTransitionException if the item cannot be approved in its state.
     */
    void approve() override;
    /**
     * @brief Completes the item only when no approval is outstanding.
     *
     * @throws ApprovalIncompleteException while approvers are still pending.
     */
    void complete() override;

    /** @brief Adds a copy of the chain, position and contact to the inner memento. */
    WorkMemento<T>* createMemento() const override;
    /** @brief Restores the chain copy, position and contact. */
    void restore(const WorkMemento<T>& memento) override;

    /** @brief Copies the wrapped component and the chain. */
    WorkComponent<T>* copy() const override;
    void accept(WorkVisitor<T>& visitor) override;

private:
    Handler<T>* chain;  ///< Owned head of the approval chain (null until assigned).
    int chainLength;  ///< Number of approvers.
    int position;  ///< Index of the approver currently responsible (-1 before the first approval).
    Participant<T>* currentContact;  ///< Current approver (non-owning).
};

}

#endif
