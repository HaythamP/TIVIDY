#include "WorkItemState.h"
#include "InvalidTransitionException.h"

namespace tividy {

template <typename T>
WorkItemState<T>::~WorkItemState() {}

template <typename T>
bool WorkItemState<T>::isComplete() const {
    return false;
}

template <typename T>
bool WorkItemState<T>::isAvailable() const {
    return false;
}

template <typename T>
bool WorkItemState<T>::isTerminal() const {
    return false;
}

template <typename T>
void WorkItemState<T>::makeAvailable(WorkItem<T>& item) {}

template <typename T>
void WorkItemState<T>::assign(WorkItem<T>& item, const std::vector<Participant<T>*>& participants) {
    throw InvalidTransitionException(getName(), "assign");
}

}