#ifndef TIVIDY_AVAILABLESTATE_H
#define TIVIDY_AVAILABLESTATE_H

#include "WorkItemState.h"

namespace tividy {
    template <typename IdType>
class AvailableState : public WorkItemState<IdType> {
public:
    std::string getName() const override;
    bool canRevertToCreated() const override;
    WorkItemState<IdType>* copy() const override;
    void assign(WorkItem<IdType>& item, Participant<IdType>* participant) override;
    void revertToCreated(WorkItem<IdType>& item) override;
};
}
#endif
