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

}