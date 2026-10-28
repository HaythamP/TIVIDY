#include "CreatedState.h"
#include "AvailableState.h"
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

template <typename T>
void CreatedState<T>::makeAvailable(WorkItem<T>& item) {
    item.setState(new AvailableState<T>());
}

template <typename T>
void CreatedState<T>::cancel(WorkItem<T>& item) {
    item.setState(new CancelledState<T>());
}

template class CreatedState<int>;
template class CreatedState<std::string>;

}