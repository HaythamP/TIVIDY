#ifndef AVAILABLESTATE_H
#define AVAILABLESTATE_H

#include "WorkItemState.h"

namespace tividy {

template <typename T>
class AvailableState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    bool isAvailable() const override;
    void assign(WorkItem<T>& item, const std::vector<Participant<T>*>& participants) override;
    void cancel(WorkItem<T>& item) override;
};

}

#endif
