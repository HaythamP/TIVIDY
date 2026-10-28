#include "CompletedState.h"
#include "WorkItem.h"
#include <string>

namespace tividy {

template <typename T>
std::string CompletedState<T>::getName() const {
    return "Completed";
}

template <typename T>
WorkItemState<T>* CompletedState<T>::copy() const {
    return new CompletedState<T>();
}

}