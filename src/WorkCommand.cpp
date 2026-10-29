#include "WorkCommand.h"
#include <iostream>
#include "ComponentNotFoundException.h"

namespace tividy {

template <typename T>
WorkCommand<T>::WorkCommand(WorkComponent<T>* target) : target(target) {
    if (target == nullptr) {
        throw ComponentNotFoundException("A command requires a target component");
    }
}

template <typename T>
WorkCommand<T>::~WorkCommand() {
    for (std::size_t i = 0; i < mementos.size(); ++i) {
        delete mementos[i];
    }
}

template <typename T>
void WorkCommand<T>::execute() {
    std::cout << "> " << this->getDescription() << std::endl;
    for (std::size_t i = 0; i < mementos.size(); ++i) {
        delete mementos[i];
    }
    mementos.clear();
    resultingStates.clear();
    affected = getAffected();

    bool performing = false;
    std::vector<std::string> statesBefore;
    try {
        for (std::size_t i = 0; i < affected.size(); ++i) {
            mementos.push_back(affected[i]->createMemento());
            statesBefore.push_back(affected[i]->getStateName());
        }
        performing = true;
        perform();
        for (std::size_t i = 0; i < affected.size(); ++i) {
            resultingStates.push_back(affected[i]->getStateName());
        }
    } catch (...) {
        bool changed = false;
        for (std::size_t i = 0; performing && i < affected.size(); ++i) {
            if (affected[i]->getStateName() != statesBefore[i]) {
                changed = true;
            }
        }
        if (changed) {
            for (std::size_t i = mementos.size(); i > 0; --i) {
                affected[i - 1]->restore(*mementos[i - 1]);
            }
        }
        for (std::size_t i = 0; i < mementos.size(); ++i) {
            delete mementos[i];
        }
        mementos.clear();
        resultingStates.clear();
        affected.clear();
        throw;
    }
}

template <typename T>
bool WorkCommand<T>::undo() {
    if (mementos.empty()) {
        return false;
    }
    if (!canUndo()) {
        std::cout << "  Cannot undo '" << this->getDescription() << "': the affected work has already moved on." << std::endl;
        return false;
    }
    std::cout << "< Undo: " << this->getDescription() << std::endl;
    for (std::size_t i = mementos.size(); i > 0; --i) {
        affected[i - 1]->restore(*mementos[i - 1]);
    }
    for (std::size_t i = 0; i < mementos.size(); ++i) {
        delete mementos[i];
    }
    mementos.clear();
    resultingStates.clear();
    affected.clear();
    return true;
}

template <typename T>
bool WorkCommand<T>::canUndo() const {
    if (resultingStates.size() != affected.size()) {
        return false;
    }
    for (std::size_t i = 0; i < affected.size(); ++i) {
        if (affected[i]->getStateName() != resultingStates[i]) {
            return false;
        }
    }
    return true;
}

template <typename T>
std::vector<WorkComponent<T>*> WorkCommand<T>::getAffected() const {
    return std::vector<WorkComponent<T>*>(1, target);
}

template <typename T>
std::vector<WorkComponent<T>*> WorkCommand<T>::withSuccessors() const {
    std::vector<WorkComponent<T>*> result(1, target);
    Mediator<T>* mediator = target->getMediator();
    if (mediator != nullptr) {
        WorkComponent<T>* next = mediator->findNext(target);
        if (next != nullptr) {
            std::vector<WorkComponent<T>*> items = next->getItems();
            result.insert(result.end(), items.begin(), items.end());
        }
    }
    return result;
}

template class WorkCommand<int>;
template class WorkCommand<std::string>;

}
