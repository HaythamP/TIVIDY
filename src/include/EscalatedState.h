#ifndef ESCALATEDSTATE_H
#define ESCALATEDSTATE_H

#include "WorkItemState.h"

namespace tividy {
    template <typename T>
class EscalatedState : public WorkItemState<T> {
public:
    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    void assign(WorkItem<T>& item, const std::vector<Participant<T>*>& participants) override;
    void escalate(WorkItem<T>& item) override;
    void complete(WorkItem<T>& item) override;
    void cancel(WorkItem<T>& item) override;
};
}
#endif
