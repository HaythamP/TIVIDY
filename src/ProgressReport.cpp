#include "ProgressReport.h"
#include "WorkItem.h"
#include "WorkGroup.h"
#include "AuditedWork.h"
#include "EscalatedWork.h"
#include "ApprovedWork.h"
#include <sstream>

namespace tividy {

template <typename T>
ProgressReport<T>::ProgressReport() : totalItems(0), completedItems(0) {}

template <typename T>
void ProgressReport<T>::visit(WorkItem<T>& item) {
    ++totalItems;
    if (item.isComplete()) {
        ++completedItems;
    }
    ++stateCounts[item.getStateName()];
}

template <typename T>
void ProgressReport<T>::visit(WorkGroup<T>& group) {}

template <typename T>
void ProgressReport<T>::visit(AuditedWork<T>& decorator) {}

template <typename T>
void ProgressReport<T>::visit(EscalatedWork<T>& decorator) {}

template <typename T>
void ProgressReport<T>::visit(ApprovedWork<T>& decorator) {}

template <typename T>
int ProgressReport<T>::getTotalItems() const {
    return totalItems;
}

template <typename T>
int ProgressReport<T>::getCompletedItems() const {
    return completedItems;
}

template <typename T>
std::string ProgressReport<T>::getSummary() const {
    std::ostringstream summary;
    summary << completedItems << "/" << totalItems << " items completed";
    if (!stateCounts.empty()) {
        summary << " (";
        bool first = true;
        for (std::map<std::string, int>::const_iterator it = stateCounts.begin(); it != stateCounts.end(); ++it) {
            if (!first) {
                summary << ", ";
            }
            summary << it->first << ": " << it->second;
            first = false;
        }
        summary << ")";
    }
    return summary.str();
}

template class ProgressReport<int>;
template class ProgressReport<std::string>;

}
