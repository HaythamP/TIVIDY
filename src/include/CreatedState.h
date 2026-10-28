#ifndef CREATEDSTATE_H
#define CREATEDSTATE_H

#include "WorkItemState.h"

namespace tividy {

/** @brief ConcreteState: Initial state. Allowed: makeAvailable to Available, cancel to Cancelled. */
template <typename T>
class CreatedState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    void makeAvailable(WorkItem<T>& item) override;
    void cancel(WorkItem<T>& item) override;
};

}

#endif
