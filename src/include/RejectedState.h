#ifndef REJECTEDSTATE_H
#define REJECTEDSTATE_H

#include "WorkItemState.h"

namespace tividy {

template <typename T>
class RejectedState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    bool isTerminal() const override;
};

}

#endif
