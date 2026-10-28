#include "WorkItemState.h"
#include "InvalidTransitionException.h"

namespace tividy {

template <typename T>
WorkItemState<T>::~WorkItemState() {}

template <typename T>
bool WorkItemState<T>::isComplete() const {
    return false;
}

}