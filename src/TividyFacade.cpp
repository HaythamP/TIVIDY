#include "TividyFacade.h"
#include "InvalidWorkflowException.h"

namespace tividy {

namespace {

template <typename T>
Handler<T>* buildChain(const std::vector<Participant<T>*>& participants, bool addFallback) {
    Handler<T>* head = nullptr;
    Handler<T>* tail = nullptr;
    try {
        for (std::size_t i = 0; i < participants.size(); ++i) {
            Handler<T>* level = new LevelHandler<T>(participants[i], static_cast<int>(i));
            if (head == nullptr) {
                head = level;
            } else {
                tail->setNext(level);
            }
            tail = level;
        }
        if (addFallback) {
            Handler<T>* fallback = new FallbackHandler<T>();
            if (head == nullptr) {
                head = fallback;
            } else {
                tail->setNext(fallback);
            }
        }
    } catch (...) {
        delete head;
        throw;
    }
    return head;
}

}

template <typename T>
TividyFacade<T>::TividyFacade() : builder(new WorkflowBuilder<T>()) {}

template <typename T>
TividyFacade<T>::~TividyFacade() {
    delete builder;
}

template <typename T>
WorkflowDefinition<T>* TividyFacade<T>::constructIncidentResponseWorkflow(const std::vector<T>& ids) {
    WorkflowDirector<T> director;
    director.constructIncidentResponseWorkflow(*builder, ids);
    return builder->getResult();
}

template <typename T>
WorkflowDefinition<T>* TividyFacade<T>::constructJobApplicationWorkflow(const std::vector<T>& ids) {
    WorkflowDirector<T> director;
    director.constructJobApplicationWorkflow(*builder, ids);
    return builder->getResult();
}

template <typename T>
WorkflowBuilder<T>& TividyFacade<T>::getBuilder() {
    return *builder;
}

template <typename T>
WorkflowDefinition<T>* TividyFacade<T>::cloneWorkflow(const WorkflowDefinition<T>* workflow) const {
    if (workflow == nullptr) {
        throw InvalidWorkflowException("Cannot clone a null workflow");
    }
    return workflow->clone();
}

template <typename T>
void TividyFacade<T>::startWorkflow(WorkflowDefinition<T>* workflow) {
    if (workflow == nullptr) {
        throw InvalidWorkflowException("Cannot start a null workflow");
    }
    workflow->start();
}

template <typename T>
WorkComponent<T>* TividyFacade<T>::locate(WorkflowDefinition<T>* workflow, const T& workId) const {
    if (workflow == nullptr) {
        throw InvalidWorkflowException("The workflow is null");
    }
    WorkComponent<T>* component = workflow->find(workId);
    if (component == nullptr) {
        throw ComponentNotFoundException("No component with the requested id exists in " + workflow->getName());
    }
    return component;
}

template <typename T>
void TividyFacade<T>::buildChains(WorkComponent<T>* target,
                                  const std::vector<Participant<T>*>& escalationParticipants,
                                  const std::vector<Participant<T>*>& approvalParticipants,
                                  Handler<T>*& escalationChain,
                                  Handler<T>*& approvalChain) const {
    try {
        if (target->hasEscalation() || !escalationParticipants.empty()) {
            escalationChain = buildChain<T>(escalationParticipants, true);
        }
        if (!approvalParticipants.empty()) {
            approvalChain = buildChain<T>(approvalParticipants, false);
        }
    } catch (...) {
        delete escalationChain;
        delete approvalChain;
        escalationChain = nullptr;
        approvalChain = nullptr;
        throw;
    }
}

template <typename T>
void TividyFacade<T>::assignWithStrategies(WorkflowDefinition<T>* workflow, const T& workId,
                                           const std::vector<AssignmentStrategy<T>*>& strategies,
                                           int numAssignees,
                                           const std::vector<Participant<T>*>& escalationParticipants,
                                           const std::vector<Participant<T>*>& approvalParticipants) {
    WorkComponent<T>* target = locate(workflow, workId);
    Handler<T>* escalationChain = nullptr;
    Handler<T>* approvalChain = nullptr;
    buildChains(target, escalationParticipants, approvalParticipants, escalationChain, approvalChain);
    workflow->execute(new AssignCommand<T>(target, *workflow, strategies, numAssignees, escalationChain, approvalChain));
}

template <typename T>
void TividyFacade<T>::assignToParticipants(WorkflowDefinition<T>* workflow, const T& workId,
                                           const std::vector<Participant<T>*>& participants,
                                           int numAssignees,
                                           const std::vector<Participant<T>*>& escalationParticipants,
                                           const std::vector<Participant<T>*>& approvalParticipants) {
    WorkComponent<T>* target = locate(workflow, workId);
    Handler<T>* escalationChain = nullptr;
    Handler<T>* approvalChain = nullptr;
    buildChains(target, escalationParticipants, approvalParticipants, escalationChain, approvalChain);
    workflow->execute(new AssignCommand<T>(target, participants, numAssignees, escalationChain, approvalChain));
}

template <typename T>
void TividyFacade<T>::startWork(WorkflowDefinition<T>* workflow, const T& workId) {
    workflow->execute(new StartCommand<T>(locate(workflow, workId)));
}

template <typename T>
void TividyFacade<T>::completeWork(WorkflowDefinition<T>* workflow, const T& workId) {
    workflow->execute(new CompleteCommand<T>(locate(workflow, workId)));
}

template <typename T>
void TividyFacade<T>::rejectWork(WorkflowDefinition<T>* workflow, const T& workId) {
    workflow->execute(new RejectCommand<T>(locate(workflow, workId)));
}

template <typename T>
void TividyFacade<T>::cancelWork(WorkflowDefinition<T>* workflow, const T& workId) {
    workflow->execute(new CancelCommand<T>(locate(workflow, workId)));
}

template <typename T>
void TividyFacade<T>::escalateWork(WorkflowDefinition<T>* workflow, const T& workId) {
    workflow->execute(new EscalateCommand<T>(locate(workflow, workId)));
}

template <typename T>
void TividyFacade<T>::approveWork(WorkflowDefinition<T>* workflow, const T& workId) {
    workflow->execute(new ApproveCommand<T>(locate(workflow, workId)));
}

template <typename T>
std::vector<std::string> TividyFacade<T>::getAuditLog(WorkflowDefinition<T>* workflow, const T& workId) {
    return locate(workflow, workId)->getAuditLog();
}

template <typename T>
void TividyFacade<T>::addAuditEntry(WorkflowDefinition<T>* workflow, const T& workId, const std::string& entry) {
    locate(workflow, workId)->addAuditEntry(entry);
}

template <typename T>
void TividyFacade<T>::editAuditEntry(WorkflowDefinition<T>* workflow, const T& workId, std::size_t index, const std::string& entry) {
    locate(workflow, workId)->editAuditEntry(index, entry);
}

template <typename T>
void TividyFacade<T>::deleteAuditEntry(WorkflowDefinition<T>* workflow, const T& workId, std::size_t index) {
    locate(workflow, workId)->deleteAuditEntry(index);
}

template <typename T>
bool TividyFacade<T>::undo(WorkflowDefinition<T>* workflow) {
    if (workflow == nullptr) {
        throw InvalidWorkflowException("The workflow is null");
    }
    return workflow->undoLast();
}

template <typename T>
bool TividyFacade<T>::isComplete(const WorkflowDefinition<T>* workflow) const {
    if (workflow == nullptr) {
        throw InvalidWorkflowException("The workflow is null");
    }
    return workflow->isComplete();
}

template <typename T>
std::string TividyFacade<T>::getProgressReport(WorkflowDefinition<T>* workflow) {
    ProgressReport<T> report;
    workflow->accept(report);
    return report.getSummary();
}

template <typename T>
std::string TividyFacade<T>::printWorkflow(WorkflowDefinition<T>* workflow) {
    TreePrinter<T> printer;
    workflow->accept(printer);
    return printer.getOutput();
}

template class TividyFacade<int>;
template class TividyFacade<std::string>;

}