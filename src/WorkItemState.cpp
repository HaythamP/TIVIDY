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

template <typename T>
void WorkItemState<T>::start(WorkItem<T>& item) {
    throw InvalidTransitionException(getName(), "start");
}

template <typename T>
void WorkItemState<T>::complete(WorkItem<T>& item) {
    throw InvalidTransitionException(getName(), "complete");
}

template <typename T>
void WorkItemState<T>::reject(WorkItem<T>& item) {
    throw InvalidTransitionException(getName(), "reject");
}

template <typename T>
void WorkItemState<T>::cancel(WorkItem<T>& item) {
    throw InvalidTransitionException(getName(), "cancel");
}

template <typename T>
void WorkItemState<T>::escalate(WorkItem<T>& item) {
    throw InvalidTransitionException(getName(), "escalate");
}

template <typename T>
void WorkItemState<T>::approve(WorkItem<T>& item) {
    throw InvalidTransitionException(getName(), "approve");
}

template class WorkItemState<int>;
template class WorkItemState<std::string>;

}
