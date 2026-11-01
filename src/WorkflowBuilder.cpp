#include "WorkflowBuilder.h"
#include "AuditedWork.h"
#include "EscalatedWork.h"
#include "ApprovedWork.h"
#include "DuplicateIdException.h"
#include "InvalidWorkflowException.h"

namespace tividy {

template <typename T>
WorkflowBuilder<T>::WorkflowBuilder() : workflow(nullptr), pendingItem(nullptr), audited(false), approved(false), escalated(false) {}

template <typename T>
WorkflowBuilder<T>::~WorkflowBuilder() {
    delete pendingItem;
    delete workflow;
}

template <typename T>
WorkflowBuilder<T>& WorkflowBuilder<T>::reset(const std::string& workflowName) {
    delete pendingItem;
    pendingItem = nullptr;
    delete workflow;
    workflow = nullptr;
    openGroups.clear();
    audited = false;
    approved = false;
    escalated = false;
    workflow = new WorkflowDefinition<T>(workflowName);
    return *this;
}

template <typename T>
WorkflowBuilder<T>& WorkflowBuilder<T>::addParticipant(Participant<T>* participant) {
    if (workflow == nullptr) {
        throw InvalidWorkflowException("Call reset before adding participants");
    }
    workflow->addParticipant(participant);
    return *this;
}

template <typename T>
void WorkflowBuilder<T>::attach(WorkComponent<T>* component) {
    if (!openGroups.empty()) {
        openGroups.back()->add(component);
    } else {
        workflow->addStep(component);
    }
}

template <typename T>
WorkflowBuilder<T>& WorkflowBuilder<T>::beginGroup(const T& id, const std::string& name) {
    if (workflow == nullptr) {
        throw InvalidWorkflowException("Call reset before building");
    }
    if (pendingItem != nullptr) {
        throw InvalidWorkflowException("Finish the current work item before starting a group");
    }
    if (workflow->find(id) != nullptr) {
        throw DuplicateIdException("A component with this id already exists");
    }
    WorkGroup<T>* group = new WorkGroup<T>(id, name);
    try {
        attach(group);
    } catch (...) {
        delete group;
        throw;
    }
    openGroups.push_back(group);
    return *this;
}

template <typename T>
WorkflowBuilder<T>& WorkflowBuilder<T>::endGroup() {
    if (openGroups.empty()) {
        throw InvalidWorkflowException("There is no open group to end");
    }
    if (pendingItem != nullptr) {
        throw InvalidWorkflowException("Finish the current work item before ending the group");
    }
    if (openGroups.back()->getChildren().empty()) {
        throw InvalidWorkflowException("A group must contain at least one component");
    }
    openGroups.pop_back();
    return *this;
}

template <typename T>
WorkflowBuilder<T>& WorkflowBuilder<T>::newWorkItem(const T& id, const std::string& name) {
    if (workflow == nullptr) {
        throw InvalidWorkflowException("Call reset before building");
    }
    if (pendingItem != nullptr) {
        throw InvalidWorkflowException("Finish the current work item first");
    }
    if (workflow->find(id) != nullptr) {
        throw DuplicateIdException("A component with this id already exists");
    }
    pendingItem = new WorkItem<T>(id, name);
    audited = false;
    approved = false;
    escalated = false;
    return *this;
}

template <typename T>
WorkflowBuilder<T>& WorkflowBuilder<T>::withAudit() {
    if (pendingItem == nullptr) {
        throw InvalidWorkflowException("No work item is being built");
    }
    audited = true;
    return *this;
}

template <typename T>
WorkflowBuilder<T>& WorkflowBuilder<T>::withApproval() {
    if (pendingItem == nullptr) {
        throw InvalidWorkflowException("No work item is being built");
    }
    approved = true;
    return *this;
}

template <typename T>
WorkflowBuilder<T>& WorkflowBuilder<T>::withEscalation() {
    if (pendingItem == nullptr) {
        throw InvalidWorkflowException("No work item is being built");
    }
    escalated = true;
    return *this;
}

template <typename T>
WorkflowBuilder<T>& WorkflowBuilder<T>::endWorkItem() {
    if (pendingItem == nullptr) {
        throw InvalidWorkflowException("No work item is being built");
    }
    WorkComponent<T>* component = pendingItem;
    pendingItem = nullptr;
    if (approved) {
        component = new ApprovedWork<T>(component);
    }
    if (escalated) {
        component = new EscalatedWork<T>(component);
    }
    if (audited) {
        component = new AuditedWork<T>(component);
    }
    audited = false;
    approved = false;
    escalated = false;
    try {
        attach(component);
    } catch (...) {
        delete component;
        throw;
    }
    return *this;
}

template <typename T>
WorkflowDefinition<T>* WorkflowBuilder<T>::getResult() {
    if (workflow == nullptr) {
        throw InvalidWorkflowException("There is no workflow to return");
    }
    if (!openGroups.empty()) {
        throw InvalidWorkflowException("A group has not been ended");
    }
    if (pendingItem != nullptr) {
        throw InvalidWorkflowException("A work item has not been ended");
    }
    if (workflow->getSteps().empty()) {
        throw InvalidWorkflowException("The workflow has no steps");
    }
    WorkflowDefinition<T>* result = workflow;
    workflow = nullptr;
    result->wire();
    return result;
}

template class WorkflowBuilder<int>;
template class WorkflowBuilder<std::string>;

}