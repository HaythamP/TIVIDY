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
