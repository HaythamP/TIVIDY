#include "AuditedWork.h"
#include "WorkVisitor.h"
#include "AuditEntryNotFoundException.h"

namespace tividy {

template <typename T>
AuditedWork<T>::AuditedWork(WorkComponent<T>* inner) : WorkItemDecorator<T>(inner){}

template <typename T>
void AuditedWork<T>::record(const std::string& operation, const std::string& before){
    auditLog.push_back(operation + ": " + before + " -> " + this->inner->getStateName());
}

template <typename T>
void AuditedWork<T>::release(){
    std::string before = this->inner->getStateName();
    this->inner->release();
    if (this->inner->getStateName() != before){
        record("release", before);
    }
}

template <typename T>
void AuditedWork<T>::assignTo(const std::vector<Participant<T>*>& participants){
    std::string before = this->inner->getStateName();
    this->inner->assignTo(participants);
    record("assign", before);
}

template <typename T>
void AuditedWork<T>::start(){
    std::string before = this->inner->getStateName();
    this->inner->start();
    record("start", before);
}

template <typename T>
void AuditedWork<T>::complete() {
    std::string before = this->inner->getStateName();
    this->inner->complete();
    record("complete", before);
}

template <typename T>
void AuditedWork<T>::reject() {
    std::string before = this->inner->getStateName();
    this->inner->reject();
    record("reject", before);
}

template <typename T>
void AuditedWork<T>::cancel() {
    std::string before = this->inner->getStateName();
    this->inner->cancel();
    record("cancel", before);
}

template <typename T>
void AuditedWork<T>::escalate() {
    std::string before = this->inner->getStateName();
    this->inner->escalate();
    record("escalate", before);
}

template <typename T>
void AuditedWork<T>::approve() {
    std::string before = this->inner->getStateName();
    this->inner->approve();
    record("approve", before);
}

template <typename T>
void AuditedWork<T>::restore(const WorkMemento<T>& memento) {
    std::string before = this->inner->getStateName();
    this->inner->restore(memento);
    record("undone", before);
}

template <typename T>
std::vector<std::string> AuditedWork<T>::getAuditLog() const {
    return auditLog;
}

template <typename T>
void AuditedWork<T>::addAuditEntry(const std::string& entry) {
    auditLog.push_back(entry);
}

template <typename T>
void AuditedWork<T>::editAuditEntry(std::size_t index, const std::string& entry) {
    if (index >= auditLog.size()) {
        throw AuditEntryNotFoundException("Audit entry " + std::to_string(index) + " does not exist");
    }
    auditLog[index] = entry;
}

template <typename T>
void AuditedWork<T>::deleteAuditEntry(std::size_t index) {
    if (index >= auditLog.size()) {
        throw AuditEntryNotFoundException("Audit entry " + std::to_string(index) + " does not exist");
    }
    auditLog.erase(auditLog.begin() + static_cast<std::ptrdiff_t>(index));
}

template <typename T>
WorkComponent<T>* AuditedWork<T>::copy() const {
    return new AuditedWork<T>(this->inner->copy());
}