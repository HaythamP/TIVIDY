#ifndef ESCALATEDWORK_H
#define ESCALATEDWORK_H

#include "WorkItemDecorator.h"
#include "Handler.h"
#include "Participant.h"
#include "WorkMemento.h"

namespace tividy {

/**
 * @brief ConcreteDecoratorA: adds escalation to a work item and acts as the client of an escalation Chain of Responsibility.
 *
 * The chain (installed by AssignCommand) is made of LevelHandlers followed by one FallbackHandler. Each Escalate command moves one link forward:
 * - the first command moves InProgress to Escalated, starts the chain and passes the request to the first handler in the same step;
 * - every further command means the previous handler could not resolve it, so the next handler takes over;
 * - when the next handler is the FallbackHandler the escalation completes immediately;
 * - a Complete command stops the traversal (a handler resolved the request).
 * Resolving the escalation completes the item, or, if the item is also an ApprovedWork, hands it over to the approval chain (Escalate, then Approve, then Completed). Reject and Cancel stop the escalation.
 * The decorator owns the chain; `position` and the current contact are saved in mementos so undo works.
 */
template <typename T>
class EscalatedWork : public WorkItemDecorator<T> {
public:
    /** @brief Wraps the component; no chain yet (null), position -1. */
    explicit EscalatedWork(WorkComponent<T>* inner);
    /** @brief Deletes the chain. */
    ~EscalatedWork();

    bool hasEscalation() const override;
    Participant<T>* getEscalationContact() const override;
    /**
     * @brief Takes ownership of the chain and resets the progress.
     *
     * @throws InvalidHandlerChainException if the chain is null or does not end with a FallbackHandler.
     */
    void setEscalationChain(Handler<T>* chain) override;

    /** @brief Starts or restarts the item and resets the escalation progress. */
    void start() override;
    /**
     * @brief Starts or advances the chain by one handler and prints who is handling the request.
     *
     * @throws InvalidHandlerChainException if no chain was assigned.
     * @throws InvalidTransitionException if the item cannot escalate in its state.
     */
    void escalate() override;
    /**
     * @brief Ends an active escalation: completes the item, or approves it when it also has ApprovedWork.
     */
    void complete() override;
    /** @brief Rejects the item and ends the escalation. */
    void reject() override;
    /** @brief Cancels the item and ends the escalation. */
    void cancel() override;
    /**
     * @brief Refused while an escalation is active; otherwise forwarded.
     *
     * @throws InvalidTransitionException while escalating.
     */
    void approve() override;

    /** @brief Adds a copy of the chain, position, contact and escalating flag to the inner memento. */
    WorkMemento<T>* createMemento() const override;
    /** @brief Restores the chain copy, position, contact and flag. */
    void restore(const WorkMemento<T>& memento) override;

    /** @brief Copies the wrapped component and the chain. */
    WorkComponent<T>* copy() const override;
    void accept(WorkVisitor<T>& visitor) override;

private:
    Handler<T>* chain;  ///< Owned head of the escalation chain (null until assigned).
    int position;  ///< Index of the handler currently responsible (-1 before the first escalation).
    bool escalating;  ///< True while an escalation is in progress.
    Participant<T>* currentContact;  ///< Participant of the current handler (non-owning).
};

}

#endif
