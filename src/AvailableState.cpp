#include "AvailableState.h"
#include "AssignedState.h"
#include "CancelledState.h"
#include "WorkItem.h"
#include <string>

namespace tividy {

template <typename T>
std::string AvailableState<T>::getName() const {
    return "Available";
}

template <typename T>
WorkItemState<T>* AvailableState<T>::copy() const {
    return new AvailableState<T>();
}

template <typename T>
bool AvailableState<T>::isAvailable() const {
    return true;
}

template <typename T>
void AvailableState<T>::assign(WorkItem<T>& item, const std::vector<Participant<T>*>& participants) {
    item.setAssignees(participants);
    item.setState(new AssignedState<T>());
}

}