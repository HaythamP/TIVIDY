#ifndef TIVIDY_CREATEDSTATE_H
#define TIVIDY_CREATEDSTATE_H

#include "WorkItemState.h"

namespace tividy {
    template <typename IdType>
class CreatedState : public WorkItemState<IdType> {
public:
    std::string getName() const override;
    bool canRevertToCreated() const override;
    WorkItemState<IdType>* copy() const override;
    void makeAvailable(WorkItem<IdType>& item) override;
    void revertToCreated(WorkItem<IdType>& item) override;
};
}
#endif
