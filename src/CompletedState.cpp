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

template <typename T>
bool CompletedState<T>::isComplete() const {
    return true;
}

template <typename T>
bool CompletedState<T>::isTerminal() const {
    return true;
}

template class CompletedState<int>;
template class CompletedState<std::string>;

}
