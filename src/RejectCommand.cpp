#include "RejectCommand.h"
#include "UnsupportedOperationException.h"

namespace tividy {

template <typename T>
RejectCommand<T>::RejectCommand(WorkComponent<T>* target) : WorkCommand<T>(target) {}

template <typename T>
std::string RejectCommand<T>::getDescription() const {
    return "Reject " + this->target->getName();
}

template <typename T>
void RejectCommand<T>::perform() {
    this->target->reject();
}


template class RejectCommand<int>;
template class RejectCommand<std::string>;

}
