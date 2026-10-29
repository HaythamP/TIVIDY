#include "EscalateCommand.h"
#include "UnsupportedOperationException.h"

namespace tividy {

template <typename T>
EscalateCommand<T>::EscalateCommand(WorkComponent<T>* target) : WorkCommand<T>(target) {}

template <typename T>
std::string EscalateCommand<T>::getDescription() const {
    return "Escalate " + this->target->getName();
}

template <typename T>
void EscalateCommand<T>::perform() {
    if (!this->target->hasEscalation()) {
        throw UnsupportedOperationException(this->target->getName() + " does not support escalation");
    }
    this->target->escalate();
}

template <typename T>
std::vector<WorkComponent<T>*> EscalateCommand<T>::getAffected() const {
    return this->withSuccessors();
}

template class EscalateCommand<int>;
template class EscalateCommand<std::string>;

}
