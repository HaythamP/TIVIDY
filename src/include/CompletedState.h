#ifndef COMPLETEDSTATE_H
#define COMPLETEDSTATE_H

#include "WorkItemState.h"

namespace tividy {

/** @brief ConcreteState: terminal state. The item is finished and the Mediator releases the next step. */
template <typename T>
class CompletedState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    bool isComplete() const override;
    bool isTerminal() const override;
};

}

#endif

