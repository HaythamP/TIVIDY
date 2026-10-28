#ifndef ASSIGNEDSTATE_H
#define ASSIGNEDSTATE_H

#include "WorkItemState.h"

namespace tividy {

/** @brief ConcreteState: Participants chosen. Allowed: start to InProgress, cancel to Cancelled. */
template <typename T>
class AssignedState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    void start(WorkItem<T>& item) override;
    void cancel(WorkItem<T>& item) override;
};

}

#endif
