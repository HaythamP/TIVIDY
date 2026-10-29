#include "CancelCommand.h"
#include "UnsupportedOperationException.h"

namespace tividy {

template <typename T>
CancelCommand<T>::CancelCommand(WorkComponent<T>* target) : WorkCommand<T>(target) {}

template <typename T>
std::string CancelCommand<T>::getDescription() const {
    return "Cancel " + this->target->getName();
}

template <typename T>
void CancelCommand<T>::perform() {
    this->target->cancel();
}


template class CancelCommand<int>;
template class CancelCommand<std::string>;

}
