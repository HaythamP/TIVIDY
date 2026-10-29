#include "WorkComponent.h"
#include "WorkGroup.h"
#include "Participant.h"
#include "UnsupportedOperationException.h"

namespace tividy {

template <typename T>
WorkComponent<T>::WorkComponent() : parent(nullptr), mediator(nullptr) {}

template <typename T>
WorkComponent<T>::~WorkComponent() {}

template <typename T>
WorkGroup<T>* WorkComponent<T>::getParent() const {
    return parent;
}

template <typename T>
void WorkComponent<T>::setParent(WorkGroup<T>* newParent) {
    parent = newParent;
}

template <typename T>
Mediator<T>* WorkComponent<T>::getMediator() const {
    return mediator;
}

template <typename T>
void WorkComponent<T>::setMediator(Mediator<T>* newMediator) {
    mediator = newMediator;
}

template <typename T>
std::vector<WorkComponent<T>*> WorkComponent<T>::getChildren() const {
    return std::vector<WorkComponent<T>*>();
}

template <typename T>
std::vector<WorkComponent<T>*> WorkComponent<T>::getItems() {
    return std::vector<WorkComponent<T>*>();
}

template <typename T>
void WorkComponent<T>::add(WorkComponent<T>* child) {
    throw UnsupportedOperationException("add is not supported on " + getName());
}

template <typename T>
std::string WorkComponent<T>::getStateName() const {
    return std::string();
}

template <typename T>
std::vector<Participant<T>*> WorkComponent<T>::getAssignees() const {
    return std::vector<Participant<T>*>();
}

template <typename T>
bool WorkComponent<T>::hasEscalation() const {
    return false;
}

template <typename T>
bool WorkComponent<T>::hasApproval() const {
    return false;
}

template <typename T>
std::vector<std::string> WorkComponent<T>::getAuditLog() const {
    return std::vector<std::string>();
}

template <typename T>
Participant<T>* WorkComponent<T>::getEscalationContact() const {
    return nullptr;
}

template <typename T>
Participant<T>* WorkComponent<T>::getApprovalContact() const {
    return nullptr;
}

template <typename T>
void WorkComponent<T>::addAuditEntry(const std::string& entry) {
    throw UnsupportedOperationException(getName() + " is not audited");
}

template <typename T>
void WorkComponent<T>::editAuditEntry(std::size_t index, const std::string& entry) {
    throw UnsupportedOperationException(getName() + " is not audited");
}

template <typename T>
void WorkComponent<T>::deleteAuditEntry(std::size_t index) {
    throw UnsupportedOperationException(getName() + " is not audited");
}

template <typename T>
void WorkComponent<T>::setEscalationChain(Handler<T>* chain) {
    throw UnsupportedOperationException("setEscalationChain is not supported on " + getName());
}

template <typename T>
void WorkComponent<T>::setApprovalChain(Handler<T>* chain) {
    throw UnsupportedOperationException("setApprovalChain is not supported on " + getName());
}

template <typename T>
std::vector<Participant<T>*> WorkComponent<T>::assignWork(const std::vector<Participant<T>*>& pool, const std::vector<AssignmentStrategy<T>*>& strategies, int numAssignees) const {
    throw UnsupportedOperationException("assignWork is not supported on " + getName());
}

template <typename T>
void WorkComponent<T>::assignTo(const std::vector<Participant<T>*>& participants) {
    throw UnsupportedOperationException("assignTo is not supported on " + getName());
}

template <typename T>
void WorkComponent<T>::start() {
    throw UnsupportedOperationException("start is not supported on " + getName());
}

template <typename T>
void WorkComponent<T>::complete() {
    throw UnsupportedOperationException("complete is not supported on " + getName());
}

template <typename T>
void WorkComponent<T>::reject() {
    throw UnsupportedOperationException("reject is not supported on " + getName());
}

template <typename T>
void WorkComponent<T>::cancel() {
    throw UnsupportedOperationException("cancel is not supported on " + getName());
}

template <typename T>
void WorkComponent<T>::escalate() {
    throw UnsupportedOperationException("escalate is not supported on " + getName());
}

template <typename T>
void WorkComponent<T>::approve() {
    throw UnsupportedOperationException("approve is not supported on " + getName());
}

template <typename T>
WorkMemento<T>* WorkComponent<T>::createMemento() const {
    throw UnsupportedOperationException("createMemento is not supported on " + getName());
}

template <typename T>
void WorkComponent<T>::restore(const WorkMemento<T>& memento) {
    throw UnsupportedOperationException("restore is not supported on " + getName());
}

template class WorkComponent<int>;
template class WorkComponent<std::string>;

}
