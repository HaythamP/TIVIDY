#ifndef COMPLETEDSTATE_H
#define COMPLETEDSTATE_H

#include "WorkItemState.h"

namespace tividy {
    template <typename IdType>
class CompletedState : public WorkItemState<IdType> {
public:
    std::string getName() const override;
    bool isComplete() const override;
    WorkItemState<IdType>* copy() const override;
};
}
#endif