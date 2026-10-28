#include "EscalatedState.h"
#include "CompletedState.h"
#include "RejectedState.h"
#include "ApprovalState.h"
#include "CancelledState.h"
#include "WorkItem.h"
#include <string>

namespace tividy {

template <typename T>
std::string EscalatedState<T>::getName() const {
    return "Escalated";
}

template <typename T>
WorkItemState<T>* EscalatedState<T>::copy() const {
    return new EscalatedState<T>();
}

template <typename T>
void EscalatedState<T>::complete(WorkItem<T>& item) {
    item.setState(new CompletedState<T>());
}

template <typename T>
void EscalatedState<T>::reject(WorkItem<T>& item) {
    item.setState(new RejectedState<T>());
}

template <typename T>
void EscalatedState<T>::cancel(WorkItem<T>& item) {
    item.setState(new CancelledState<T>());
}

template <typename T>
void EscalatedState<T>::escalate(WorkItem<T>& item) {
}

}