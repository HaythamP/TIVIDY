#ifndef ESCALATEDSTATE_H
#define ESCALATEDSTATE_H

#include "WorkItemState.h"

namespace tividy {

/**
 * @brief ConcreteState: An escalation chain is running. Allowed: complete to Completed, reject to Rejected, cancel to Cancelled, escalate (stays Escalated while the chain advances), approve to Approval (hand-over when the item also needs approval).
 */
template <typename T>
class EscalatedState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    void complete(WorkItem<T>& item) override;
    void reject(WorkItem<T>& item) override;
    void cancel(WorkItem<T>& item) override;
    void escalate(WorkItem<T>& item) override;
    void approve(WorkItem<T>& item) override;
};

}

#endif

