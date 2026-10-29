#include "WorkItem.h"
#include <iostream>
#include "CreatedState.h"
#include "WorkVisitor.h"
#include "Mediator.h"
#include "DepthFirstIterator.h"
#include "InvalidAssignmentArgumentException.h"
#include "InsufficientParticipantsException.h"

namespace tividy {

template <typename T>
WorkItem<T>::WorkItem(const T& id, const std::string& name) : id(id), name(name), state(new CreatedState<T>()) {}

template <typename T>
WorkItem<T>::~WorkItem() {
    delete state;
}

template <typename T>
const T& WorkItem<T>::getId() const {
    return id;
}

template <typename T>
const std::string& WorkItem<T>::getName() const {
    return name;
}

template <typename T>
std::string WorkItem<T>::getStateName() const {
    return state->getName();
}

template <typename T>
std::vector<Participant<T>*> WorkItem<T>::getAssignees() const {
    return assignees;
}

template <typename T>
std::vector<WorkComponent<T>*> WorkItem<T>::getItems() {
    return std::vector<WorkComponent<T>*>(1, this);
}

template <typename T>
WorkComponent<T>* WorkItem<T>::find(const T& searchId) {
    return id == searchId ? this : nullptr;
}

template <typename T>
void WorkItem<T>::release() {
    state->makeAvailable(*this);
}

template <typename T>
bool WorkItem<T>::isComplete() const {
    return state->isComplete();
}

template <typename T>
bool WorkItem<T>::isAvailable() const {
    return state->isAvailable();
}

template <typename T>
std::vector<Participant<T>*> WorkItem<T>::assignWork(const std::vector<Participant<T>*>& pool, const std::vector<AssignmentStrategy<T>*>& strategies, int numAssignees) const {
    if (numAssignees == 0 || numAssignees < -1) {
        throw InvalidAssignmentArgumentException("numAssignees must be -1 (everyone) or a positive number");
    }
    if (pool.empty()) {
        throw InvalidAssignmentArgumentException("The assignment pool is empty");
    }
    for (std::size_t i = 0; i < pool.size(); ++i) {
        if (pool[i] == nullptr) {
            throw InvalidAssignmentArgumentException("The assignment pool contains a null participant");
        }
        for (std::size_t j = 0; j < i; ++j) {
            if (pool[j] == pool[i] || pool[j]->getId() == pool[i]->getId()) {
                throw InvalidAssignmentArgumentException("The assignment pool contains a duplicate participant");
            }
        }
    }
    for (std::size_t i = 0; i < strategies.size(); ++i) {
        if (strategies[i] == nullptr) {
            throw InvalidAssignmentArgumentException("A null assignment strategy was supplied");
        }
    }

    std::vector<Participant<T>*> candidates(pool);
    const AssignmentStrategy<T>* selector = nullptr;
    for (std::size_t i = 0; i < strategies.size(); ++i) {
        strategies[i]->filter(candidates);
        if (selector == nullptr && strategies[i]->overridesSelection()) {
            selector = strategies[i];
        }
    }

    int required = numAssignees == -1 ? static_cast<int>(candidates.size()) : numAssignees;
    if (candidates.empty() || static_cast<int>(candidates.size()) < required) {
        throw InsufficientParticipantsException("Not enough eligible participants for " + name);
    }

    if (selector != nullptr) {
        selector->select(candidates, required);
    } else {
        candidates.resize(static_cast<std::size_t>(required));
    }
    return candidates;
}

template <typename T>
void WorkItem<T>::assignTo(const std::vector<Participant<T>*>& participants) {
    if (participants.empty()) {
        throw InvalidAssignmentArgumentException("Cannot assign an empty list of participants");
    }
    for (std::size_t i = 0; i < participants.size(); ++i) {
        if (participants[i] == nullptr) {
            throw InvalidAssignmentArgumentException("Cannot assign a null participant");
        }
    }
    state->assign(*this, participants);
}

template <typename T>
void WorkItem<T>::start() {
    state->start(*this);
}

template <typename T>
void WorkItem<T>::complete() {
    state->complete(*this);
    if (state->isComplete() && this->mediator != nullptr) {
        this->mediator->notify(this);
    }
}

template <typename T>
void WorkItem<T>::reject() {
    state->reject(*this);
}

template <typename T>
void WorkItem<T>::cancel() {
    state->cancel(*this);
}

template <typename T>
void WorkItem<T>::escalate() {
    state->escalate(*this);
}

template <typename T>
void WorkItem<T>::approve() {
    state->approve(*this);
}

template <typename T>
WorkMemento<T>* WorkItem<T>::createMemento() const {
    WorkItemState<T>* snapshot = state->copy();
    try {
        return new WorkMemento<T>(snapshot, assignees);
    } catch (...) {
        delete snapshot;
        throw;
    }
}

template <typename T>
void WorkItem<T>::restore(const WorkMemento<T>& memento) {
    if (holdsWorkload()) {
        adjustWorkloads(-1);
    }
    WorkItemState<T>* restored = memento.getState()->copy();
    delete state;
    state = restored;
    assignees = memento.getAssignees();
    if (holdsWorkload()) {
        adjustWorkloads(1);
    }
    std::cout << "  [" << name << "] restored to " << state->getName() << std::endl;
}

template <typename T>
WorkComponent<T>* WorkItem<T>::copy() const {
    return new WorkItem<T>(id, name);
}

template <typename T>
void WorkItem<T>::accept(WorkVisitor<T>& visitor) {
    visitor.visit(*this);
}

template <typename T>
WorkIterator<T>* WorkItem<T>::createIterator() {
    std::vector<WorkComponent<T>*> roots(1, this);
    return new DepthFirstIterator<T>(roots);
}

template <typename T>
bool WorkItem<T>::holdsWorkload() const {
    return !assignees.empty() && !state->isTerminal();
}

template <typename T>
void WorkItem<T>::adjustWorkloads(int delta) {
    for (std::size_t i = 0; i < assignees.size(); ++i) {
        assignees[i]->adjustWorkload(delta);
    }
}

template <typename T>
void WorkItem<T>::setState(WorkItemState<T>* newState) {
    bool before = holdsWorkload();
    WorkItemState<T>* old = state;
    std::cout << "  [" << name << "] " << old->getName() << " -> " << newState->getName() << std::endl;
    state = newState;
    delete old;
    bool after = holdsWorkload();
    if (before && !after) {
        adjustWorkloads(-1);
    } else if (!before && after) {
        adjustWorkloads(1);
    }
}

template <typename T>
void WorkItem<T>::setAssignees(const std::vector<Participant<T>*>& participants) {
    if (holdsWorkload()) {
        adjustWorkloads(-1);
    }
    assignees = participants;
    if (holdsWorkload()) {
        adjustWorkloads(1);
    }
    std::cout << "  [" << name << "] assigned to ";
    for (std::size_t i = 0; i < assignees.size(); ++i) {
        std::cout << (i > 0 ? ", " : "") << assignees[i]->getName() << " (workload " << assignees[i]->getWorkload() << ")";
    }
    std::cout << std::endl;
}

template class WorkItem<int>;
template class WorkItem<std::string>;

}
