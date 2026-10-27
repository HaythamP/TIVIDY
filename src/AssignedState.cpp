#include "AssignedState.h"
#include "InProgressState.h"
#include "CancelledState.h"
#include "WorkItem.h"
#include <string>

namespace tividy {

template <typename T>
std::string AssignedState<T>::getName() const {
    return "Assigned";
}

template <typename T>
WorkItemState<T>* AssignedState<T>::copy() const {
    return new AssignedState<T>();
}

template <typename T>
void AssignedState<T>::start(WorkItem<T>& item) {
    item.setState(new InProgressState<T>());
}

template <typename T>
void AssignedState<T>::cancel(WorkItem<T>& item) {
    item.setState(new CancelledState<T>());
}

template class AssignedState<int>;
template class AssignedState<std::string>;

}
