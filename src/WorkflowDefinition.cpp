#include "WorkflowDefinition.h"
#include "WorkflowMediator.h"
#include "DepthFirstIterator.h"
#include "InvalidWorkflowException.h"
#include "InvalidAssignmentArgumentException.h"
#include "DuplicateIdException.h"
#include "NotPristineException.h"

namespace tividy {

template <typename T>
WorkflowDefinition<T>::WorkflowDefinition(const std::string& name)
    : name(name), mediator(new WorkflowMediator<T>()), history(new CommandHistory<T>()), wired(false) {}

template <typename T>
WorkflowDefinition<T>::~WorkflowDefinition() {
    delete history;
    for (std::size_t i = 0; i < steps.size(); ++i) {
        delete steps[i];
    }
    delete mediator;
}

template <typename T>
WorkflowDefinition<T>* WorkflowDefinition<T>::clone() const {
    for (std::size_t i = 0; i < steps.size(); ++i) {
        std::vector<WorkComponent<T>*> items = steps[i]->getItems();
        for (std::size_t j = 0; j < items.size(); ++j) {
            if (items[j]->getStateName() != "Created" || !items[j]->getAssignees().empty()) {
                throw NotPristineException("Workflow " + name + " has already progressed and cannot be cloned");
            }
        }
    }
    WorkflowDefinition<T>* copy = new WorkflowDefinition<T>(name);
    try {
        copy->participants = participants;
        for (std::size_t i = 0; i < steps.size(); ++i) {
            copy->addStep(steps[i]->copy());
        }
        copy->wire();
    } catch (...) {
        delete copy;
        throw;
    }
    return copy;
}

template <typename T>
const std::string& WorkflowDefinition<T>::getName() const {
    return name;
}

template <typename T>
void WorkflowDefinition<T>::addParticipant(Participant<T>* participant) {
    if (participant == nullptr) {
        throw InvalidAssignmentArgumentException("Cannot add a null participant");
    }
    for (std::size_t i = 0; i < participants.size(); ++i) {
        if (participants[i] == participant) {
            return;
        }
    }
    participants.push_back(participant);
}

template <typename T>
const std::vector<Participant<T>*>& WorkflowDefinition<T>::getParticipants() const {
    return participants;
}

template <typename T>
void WorkflowDefinition<T>::addStep(WorkComponent<T>* step) {
    if (step == nullptr) {
        throw InvalidWorkflowException("Cannot add a null step");
    }
    for (std::size_t i = 0; i < steps.size(); ++i) {
        if (steps[i]->find(step->getId()) != nullptr) {
            throw DuplicateIdException("A component with this id already exists in " + name);
        }
    }
    steps.push_back(step);
    wired = false;
}

template <typename T>
const std::vector<WorkComponent<T>*>& WorkflowDefinition<T>::getSteps() const {
    return steps;
}

template <typename T>
void WorkflowDefinition<T>::wire() {
    if (wired) {
        return;
    }
    delete mediator;
    mediator = new WorkflowMediator<T>();
    for (std::size_t i = 0; i < steps.size(); ++i) {
        steps[i]->setMediator(mediator);
        mediator->addColleague(steps[i]);
    }
    wired = true;
}

template <typename T>
WorkComponent<T>* WorkflowDefinition<T>::find(const T& id) {
    for (std::size_t i = 0; i < steps.size(); ++i) {
        WorkComponent<T>* found = steps[i]->find(id);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

template <typename T>
void WorkflowDefinition<T>::start() {
    if (steps.empty()) {
        throw InvalidWorkflowException("Workflow " + name + " has no steps");
    }
    wire();
    steps[0]->release();
}

template <typename T>
void WorkflowDefinition<T>::execute(Command<T>* command) {
    wire();
    history->run(command);
}

template <typename T>
bool WorkflowDefinition<T>::undoLast() {
    return history->undoLast();
}

template <typename T>
std::vector<std::string> WorkflowDefinition<T>::getLog() const {
    return history->getDescriptions();
}

template <typename T>
bool WorkflowDefinition<T>::isComplete() const {
    if (steps.empty()) {
        return false;
    }
    for (std::size_t i = 0; i < steps.size(); ++i) {
        if (!steps[i]->isComplete()) {
            return false;
        }
    }
    return true;
}

template <typename T>
void WorkflowDefinition<T>::accept(WorkVisitor<T>& visitor) {
    for (std::size_t i = 0; i < steps.size(); ++i) {
        steps[i]->accept(visitor);
    }
}

template <typename T>
WorkIterator<T>* WorkflowDefinition<T>::createIterator() {
    return new DepthFirstIterator<T>(steps);
}

template class WorkflowDefinition<int>;
template class WorkflowDefinition<std::string>;

}