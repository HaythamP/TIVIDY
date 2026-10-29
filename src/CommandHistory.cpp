#include "CommandHistory.h"
#include "InvalidWorkflowException.h"

namespace tividy {

template <typename T>
CommandHistory<T>::CommandHistory() {}

template <typename T>
CommandHistory<T>::~CommandHistory() {
    for (std::size_t i = 0; i < executed.size(); ++i) {
        delete executed[i];
    }
}

template <typename T>
void CommandHistory<T>::run(Command<T>* command) {
    if (command == nullptr) {
        throw InvalidWorkflowException("Cannot run a null command");
    }
    try {
        command->execute();
    } catch (...) {
        delete command;
        throw;
    }
    executed.push_back(command);
}

template <typename T>
bool CommandHistory<T>::undoLast() {
    if (executed.empty()) {
        return false;
    }
    Command<T>* last = executed.back();
    if (!last->undo()) {
        return false;
    }
    executed.pop_back();
    delete last;
    return true;
}

template <typename T>
std::vector<std::string> CommandHistory<T>::getDescriptions() const {
    std::vector<std::string> descriptions;
    for (std::size_t i = 0; i < executed.size(); ++i) {
        descriptions.push_back(executed[i]->getDescription());
    }
    return descriptions;
}

template class CommandHistory<int>;
template class CommandHistory<std::string>;

}
