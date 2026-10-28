#include "CancelledState.h"
#include "WorkItem.h"
#include <string>

namespace tividy {

template <typename T>
std::string CancelledState<T>::getName() const {
    return "Cancelled";
}

template <typename T>
WorkItemState<T>* CancelledState<T>::copy() const {
    return new CancelledState<T>();
}

}