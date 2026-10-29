#include "CompleteCommand.h"
#include "UnsupportedOperationException.h"

namespace tividy {

template <typename T>
CompleteCommand<T>::CompleteCommand(WorkComponent<T>* target) : WorkCommand<T>(target) {}

template <typename T>
std::string CompleteCommand<T>::getDescription() const {
    return "Complete " + this->target->getName();
}

template <typename T>
void CompleteCommand<T>::perform() {
    this->target->complete();
}

template <typename T>
std::vector<WorkComponent<T>*> CompleteCommand<T>::getAffected() const {
    return this->withSuccessors();
}

template class CompleteCommand<int>;
template class CompleteCommand<std::string>;

}
