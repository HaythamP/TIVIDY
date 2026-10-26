#ifndef INPROGRESSSTATE_H
#define INPROGRESSSTATE_H

#include "WorkItemState.h"

namespace tividy {

template <typename T>
class InProgressState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    void complete(WorkItem<T>& item) override;
    void reject(WorkItem<T>& item) override;
    void escalate(WorkItem<T>& item) override;
    void approve(WorkItem<T>& item) override;
    void cancel(WorkItem<T>& item) override;
};

}

#endif
