#include "StartCommand.h"
#include "UnsupportedOperationException.h"

namespace tividy {

template <typename T>
StartCommand<T>::StartCommand(WorkComponent<T>* target) : WorkCommand<T>(target) {}

template <typename T>
std::string StartCommand<T>::getDescription() const {
    return "Start " + this->target->getName();
}

template <typename T>
void StartCommand<T>::perform() {
    this->target->start();
}


template class StartCommand<int>;
template class StartCommand<std::string>;

}