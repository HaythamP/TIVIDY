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

}