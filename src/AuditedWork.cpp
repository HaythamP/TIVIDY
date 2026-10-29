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