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

