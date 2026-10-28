#include "CancelledState.h"
#include "WorkItem.h"
#include <string>

namespace tividy {

template <typename T>
std::string CreatedState<T>::getName() const {
    return "Created";
}

template <typename T>
WorkItemState<T>* CreatedState<T>::copy() const {
    return new CreatedState<T>();
}

}