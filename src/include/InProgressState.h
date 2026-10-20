#ifndef INPROGRESSSTATE_H
#define INPROGRESSSTATE_H

#include "WorkItemState.h"

namespace tividy {
    template <typename IdType>
class InProgressState : public WorkItemState<IdType> {
public:
    std::string getName() const override;
    WorkItemState<IdType>* copy() const override;
    void complete(WorkItem<IdType>& item) override;
};
}
#endif
