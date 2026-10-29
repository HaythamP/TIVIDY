#include "ApproveCommand.h"
#include "UnsupportedOperationException.h"

namespace tividy {

template <typename T>
ApproveCommand<T>::ApproveCommand(WorkComponent<T>* target) : WorkCommand<T>(target) {}

template <typename T>
std::string ApproveCommand<T>::getDescription() const {
    return "Approve " + this->target->getName();
}

template <typename T>
void ApproveCommand<T>::perform() {
    if (!this->target->hasApproval()) {
        throw UnsupportedOperationException(this->target->getName() + " does not support approval");
    }
    this->target->approve();
}

template <typename T>
std::vector<WorkComponent<T>*> ApproveCommand<T>::getAffected() const {
    return this->withSuccessors();
}

template class ApproveCommand<int>;
template class ApproveCommand<std::string>;

}
