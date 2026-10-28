#include "InProgressState.h"
#include "CompletedState.h"
#include "RejectedState.h"
#include "EscalatedState.h"
#include "ApprovalState.h"
#include "CancelledState.h"
#include "WorkItem.h"
#include <string>

namespace tividy {

template <typename T>
std::string InProgressState<T>::getName() const {
    return "InProgress";
}

template <typename T>
WorkItemState<T>* InProgressState<T>::copy() const {
    return new InProgressState<T>();
}

template <typename T>
void InProgressState<T>::complete(WorkItem<T>& item) {
    item.setState(new CompletedState<T>());
}
}