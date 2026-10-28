#ifndef CANCELLEDSTATE_H
#define CANCELLEDSTATE_H

#include "WorkItemState.h"

namespace tividy {

/**
 * @brief ConcreteState: terminal state. The workflow halts because nothing after a cancelled item is released.
 */
template <typename T>
class CancelledState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    bool isTerminal() const override;
};

}

#endif
