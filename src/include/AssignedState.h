#ifndef ASSIGNEDSTATE_H
#define ASSIGNEDSTATE_H

#include "WorkItemState.h"

namespace tividy {
    template <typename IdType>
class AssignedState : public WorkItemState<IdType> {
public:
    std::string getName() const override;
    bool canRevertToCreated() const override;
    WorkItemState<IdType>* copy() const override;
    void assign(WorkItem<IdType>& item, Participant<IdType>* participant) override;
    void start(WorkItem<IdType>& item) override;
    void revertToCreated(WorkItem<IdType>& item) override;
};
}
#endif
