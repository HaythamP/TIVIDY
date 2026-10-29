#include "WorkItemDecorator.h"
#include "WorkGroup.h"
#include "DepthFirstIterator.h"
#include "InvalidWorkflowException.h"

namespace tividy {

template <typename T>
WorkItemDecorator<T>::WorkItemDecorator(WorkComponent<T>* inner) : inner(inner) {
    if (inner == nullptr) {
        throw InvalidWorkflowException("A decorator requires a component to wrap");
    }
}

template <typename T>
WorkItemDecorator<T>::~WorkItemDecorator() {
    delete inner;
}

template <typename T>
const T& WorkItemDecorator<T>::getId() const {
    return inner->getId();
}

template <typename T>
const std::string& WorkItemDecorator<T>::getName() const {
    return inner->getName();
}

template <typename T>
WorkGroup<T>* WorkItemDecorator<T>::getParent() const {
    return inner->getParent();
}

template <typename T>
void WorkItemDecorator<T>::setParent(WorkGroup<T>* newParent) {
    inner->setParent(newParent);
}

template <typename T>
Mediator<T>* WorkItemDecorator<T>::getMediator() const {
    return inner->getMediator();
}

template <typename T>
void WorkItemDecorator<T>::setMediator(Mediator<T>* newMediator) {
    inner->setMediator(newMediator);
}

template <typename T>
std::vector<WorkComponent<T>*> WorkItemDecorator<T>::getChildren() const {
    return inner->getChildren();
}

template <typename T>
std::vector<WorkComponent<T>*> WorkItemDecorator<T>::getItems() {
    return std::vector<WorkComponent<T>*>(1, this);
}

template <typename T>
void WorkItemDecorator<T>::add(WorkComponent<T>* child) {
    inner->add(child);
}

template <typename T>
WorkComponent<T>* WorkItemDecorator<T>::find(const T& id) {
    return inner->find(id) != nullptr ? this : nullptr;
}

template <typename T>
void WorkItemDecorator<T>::release() {
    inner->release();
}

template <typename T>
bool WorkItemDecorator<T>::isComplete() const {
    return inner->isComplete();
}

template <typename T>
bool WorkItemDecorator<T>::isAvailable() const {
    return inner->isAvailable();
}

template <typename T>
std::string WorkItemDecorator<T>::getStateName() const {
    return inner->getStateName();
}

template <typename T>
std::vector<Participant<T>*> WorkItemDecorator<T>::getAssignees() const {
    return inner->getAssignees();
}

template <typename T>
bool WorkItemDecorator<T>::hasEscalation() const {
    return inner->hasEscalation();
}

template <typename T>
bool WorkItemDecorator<T>::hasApproval() const {
    return inner->hasApproval();
}

template <typename T>
std::vector<std::string> WorkItemDecorator<T>::getAuditLog() const {
    return inner->getAuditLog();
}

template <typename T>
Participant<T>* WorkItemDecorator<T>::getEscalationContact() const {
    return inner->getEscalationContact();
}

template <typename T>
Participant<T>* WorkItemDecorator<T>::getApprovalContact() const {
    return inner->getApprovalContact();
}

template <typename T>
void WorkItemDecorator<T>::addAuditEntry(const std::string& entry) {
    inner->addAuditEntry(entry);
}

template <typename T>
void WorkItemDecorator<T>::editAuditEntry(std::size_t index, const std::string& entry) {
    inner->editAuditEntry(index, entry);
}

template <typename T>
void WorkItemDecorator<T>::deleteAuditEntry(std::size_t index) {
    inner->deleteAuditEntry(index);
}

template <typename T>
void WorkItemDecorator<T>::setEscalationChain(Handler<T>* chain) {
    inner->setEscalationChain(chain);
}

template <typename T>
void WorkItemDecorator<T>::setApprovalChain(Handler<T>* chain) {
    inner->setApprovalChain(chain);
}

template <typename T>
std::vector<Participant<T>*> WorkItemDecorator<T>::assignWork(const std::vector<Participant<T>*>& pool, const std::vector<AssignmentStrategy<T>*>& strategies, int numAssignees) const {
    return inner->assignWork(pool, strategies, numAssignees);
}

template <typename T>
void WorkItemDecorator<T>::assignTo(const std::vector<Participant<T>*>& participants) {
    inner->assignTo(participants);
}

template <typename T>
void WorkItemDecorator<T>::start() {
    inner->start();
}

template <typename T>
void WorkItemDecorator<T>::complete() {
    inner->complete();
}

template <typename T>
void WorkItemDecorator<T>::reject() {
    inner->reject();
}

template <typename T>
void WorkItemDecorator<T>::cancel() {
    inner->cancel();
}

template <typename T>
void WorkItemDecorator<T>::escalate() {
    inner->escalate();
}

template <typename T>
void WorkItemDecorator<T>::approve() {
    inner->approve();
}

template <typename T>
WorkMemento<T>* WorkItemDecorator<T>::createMemento() const {
    return inner->createMemento();
}

template <typename T>
void WorkItemDecorator<T>::restore(const WorkMemento<T>& memento) {
    inner->restore(memento);
}

template <typename T>
WorkIterator<T>* WorkItemDecorator<T>::createIterator() {
    std::vector<WorkComponent<T>*> roots(1, this);
    return new DepthFirstIterator<T>(roots);
}

template class WorkItemDecorator<int>;
template class WorkItemDecorator<std::string>;

}
