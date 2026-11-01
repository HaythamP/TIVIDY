#include "TreePrinter.h"
#include "WorkItem.h"
#include "WorkGroup.h"
#include "AuditedWork.h"
#include "EscalatedWork.h"
#include "ApprovedWork.h"
#include <sstream>

namespace tividy {

template <typename T>
TreePrinter<T>::TreePrinter() {}

template <typename T>
std::string TreePrinter<T>::indentation(int depth) {
    return std::string(static_cast<std::size_t>(depth) * 4, ' ');
}

template <typename T>
void TreePrinter<T>::visit(WorkItem<T>& item) {
    int depth = 0;
    for (WorkGroup<T>* ancestor = item.getParent(); ancestor != nullptr; ancestor = ancestor->getParent()) {
        ++depth;
    }
    std::ostringstream line;
    line << indentation(depth) << "* " << item.getName() << " (" << item.getId() << ") [" << item.getStateName() << "]";
    if (!tags.empty()) {
        line << " {";
        for (std::size_t i = 0; i < tags.size(); ++i) {
            if (i > 0) {
                line << ", ";
            }
            line << tags[i];
        }
        line << "}";
    }
    std::vector<Participant<T>*> assignees = item.getAssignees();
    if (!assignees.empty()) {
        line << " <- ";
        for (std::size_t i = 0; i < assignees.size(); ++i) {
            if (i > 0) {
                line << ", ";
            }
            line << assignees[i]->getName();
        }
    }
    output += line.str() + "\n";
    tags.clear();
}

template <typename T>
void TreePrinter<T>::visit(WorkGroup<T>& group) {
    int depth = 0;
    for (WorkGroup<T>* ancestor = group.getParent(); ancestor != nullptr; ancestor = ancestor->getParent()) {
        ++depth;
    }
    std::ostringstream line;
    line << indentation(depth) << "+ [Group] " << group.getName() << " (" << group.getId() << ")";
    output += line.str() + "\n";
}

template <typename T>
void TreePrinter<T>::visit(AuditedWork<T>& decorator) {
    tags.push_back("Audited");
}

template <typename T>
void TreePrinter<T>::visit(EscalatedWork<T>& decorator) {
    tags.push_back("Escalation");
}

template <typename T>
void TreePrinter<T>::visit(ApprovedWork<T>& decorator) {
    tags.push_back("Approval");
}

template <typename T>
const std::string& TreePrinter<T>::getOutput() const {
    return output;
}

template class TreePrinter<int>;
template class TreePrinter<std::string>;

}
