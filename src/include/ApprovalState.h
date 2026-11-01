#ifndef APPROVALSTATE_H
#define APPROVALSTATE_H

#include "WorkItemState.h"

namespace tividy {

/**
 * @brief ConcreteState: An approval chain is running. Allowed: complete to Completed, reject to a restartable Rejected, cancel to Cancelled, approve (stays in Approval while the chain advances).
 */
template <typename T>
class ApprovalState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    void complete(WorkItem<T>& item) override;
    void reject(WorkItem<T>& item) override;
    void cancel(WorkItem<T>& item) override;
    void approve(WorkItem<T>& item) override;
};

}

#endif
