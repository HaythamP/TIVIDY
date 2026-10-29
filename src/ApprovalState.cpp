#include "ApprovalState.h"
#include "CompletedState.h"
#include "RejectedState.h"
#include "CancelledState.h"
#include "WorkItem.h"
#include <string>

namespace tividy {

template <typename T>
std::string ApprovalState<T>::getName() const {
    return "Approval";
}

template <typename T>
WorkItemState<T>* ApprovalState<T>::copy() const {
    return new ApprovalState<T>();
}

template <typename T>
void ApprovalState<T>::complete(WorkItem<T>& item) {
    item.setState(new CompletedState<T>());
}

template <typename T>
void ApprovalState<T>::reject(WorkItem<T>& item) {
    item.setState(new RejectedState<T>(true));
}

template <typename T>
void ApprovalState<T>::cancel(WorkItem<T>& item) {
    item.setState(new CancelledState<T>());
}

template <typename T>
void ApprovalState<T>::approve(WorkItem<T>& item) {
}

template class ApprovalState<int>;
template class ApprovalState<std::string>;

}