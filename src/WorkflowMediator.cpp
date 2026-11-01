#include "WorkflowMediator.h"
#include <iostream>
#include "WorkComponent.h"
#include "WorkGroup.h"
#include "InvalidWorkflowException.h"

namespace tividy {

template <typename T>
WorkflowMediator<T>::WorkflowMediator() {}

template <typename T>
int WorkflowMediator<T>::indexOfStep(WorkComponent<T>* sender) const {
    if (sender == nullptr) {
        return -1;
    }
    WorkComponent<T>* top = sender;
    while (top->getParent() != nullptr) {
        top = top->getParent();
    }
    for (std::size_t i = 0; i < colleagues.size(); ++i) {
        if (colleagues[i]->getId() == top->getId()) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

template <typename T>
void WorkflowMediator<T>::notify(WorkComponent<T>* sender) {
    int index = indexOfStep(sender);
    if (index < 0) {
        return;
    }
    WorkComponent<T>* step = colleagues[static_cast<std::size_t>(index)];
    if (!step->isComplete()) {
        return;
    }
    std::size_t nextIndex = static_cast<std::size_t>(index) + 1;
    if (nextIndex < colleagues.size()) {
        std::cout << "  Step '" << step->getName() << "' is complete - releasing '" << colleagues[nextIndex]->getName() << "'." << std::endl;
        colleagues[nextIndex]->release();
    } else {
        std::cout << "  Final step '" << step->getName() << "' is complete - the workflow is finished." << std::endl;
    }
}

template <typename T>
WorkComponent<T>* WorkflowMediator<T>::findNext(WorkComponent<T>* sender) const {
    int index = indexOfStep(sender);
    if (index < 0) {
        return nullptr;
    }
    std::size_t nextIndex = static_cast<std::size_t>(index) + 1;
    if (nextIndex < colleagues.size()) {
        return colleagues[nextIndex];
    }
    return nullptr;
}

template <typename T>
void WorkflowMediator<T>::addColleague(WorkComponent<T>* colleague) {
    if (colleague == nullptr) {
        throw InvalidWorkflowException("Cannot register a null step with the mediator");
    }
    colleagues.push_back(colleague);
}

template class WorkflowMediator<int>;
template class WorkflowMediator<std::string>;

}