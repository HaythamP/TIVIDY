#include "RejectedState.h"
#include "InProgressState.h"
#include "InvalidTransitionException.h"
#include "WorkItem.h"
#include <string>

namespace tividy {

template <typename T>
RejectedState<T>::RejectedState(bool restartable) : restartable(restartable) {}

template <typename T>
std::string RejectedState<T>::getName() const {
    return "Rejected";
}

template <typename T>
WorkItemState<T>* RejectedState<T>::copy() const {
    return new RejectedState<T>(restartable);
}

}