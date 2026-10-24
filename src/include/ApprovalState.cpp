#include "ApprovalState.h"

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
    void ApprovalState<T>::approve(WorkItem<T>& item) {
        return;
    }

    template <typename T>
    void ApprovalState<T>::complete(WorkItem<T>& item) {
        return;
    }

    template <typename T>
    void ApprovalState<T>::reject(WorkItem<T>& item) {
        return;
    }

    template <typename T>
    void ApprovalState<T>::cancel(WorkItem<T>& item) {
        return;
    }
}