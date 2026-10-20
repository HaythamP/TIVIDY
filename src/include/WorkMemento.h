#ifndef WORKMEMENTO_H
#define WORKMEMENTO_H

#include <map>
#include <vector>
#include "WorkItemState.h"

namespace tividy {

template <typename T> class WorkItem;
template <typename T> class WorkComponent;
template <typename T> class Participant;


template <typename T>
class WorkMemento {
public:
    ~WorkMemento();

    const WorkItemState<T>* getState() const;
    const std::vector<Participant<T>*>& getAssignees() const;

    void setChainPosition(const WorkComponent<T>* layer, int position);
    int getChainPosition(const WorkComponent<T>* layer) const;

private:
    friend class WorkItem<T>;
    WorkMemento(WorkItemState<T>* state, const std::vector<Participant<T>*>& assignees);
    WorkMemento(const WorkMemento&) = delete;
    WorkMemento& operator=(const WorkMemento&) = delete;

    WorkItemState<T>* state;
    std::vector<Participant<T>*> assignees;
    std::map<const WorkComponent<T>*, int> chainPositions;
};

} // namespace tividy

#endif
