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

template <typename T>
bool RejectedState<T>::isTerminal() const {
    return true;
}

template <typename T>
bool RejectedState<T>::isRestartable() const {
    return restartable;
}

template <typename T>
void RejectedState<T>::start(WorkItem<T>& item) {
    if (!restartable) {
        throw InvalidTransitionException(getName(), "start");
    }
    item.setState(new InProgressState<T>());
}

template class RejectedState<int>;
template class RejectedState<std::string>;

}
