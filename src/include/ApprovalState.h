#ifndef APPROVALSTATE_H
#define APPROVALSTATE_H

#include "WorkItemState.h"

namespace tividy {
    template <typename T>
class ApprovalState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    void approve(WorkItem<T>& item) override;
    void complete(WorkItem<T>& item) override;
    void reject(WorkItem<T>& item) override;
    void cancel(WorkItem<T>& item) override;
};
}
#endif