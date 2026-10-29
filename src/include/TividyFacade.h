#ifndef TIVIDYFACADE_H
#define TIVIDYFACADE_H

#include <string>
#include <vector>
#include "WorkflowBuilder.h"
#include "WorkflowDirector.h"
#include "WorkflowDefinition.h"
#include "Participant.h"
#include "AssignmentStrategy.h"
#include "RoleAssignment.h"
#include "LeastLoadedAssignment.h"
#include "RuleAssignment.h"
#include "Handler.h"
#include "LevelHandler.h"
#include "FallbackHandler.h"
#include "AssignCommand.h"
#include "StartCommand.h"
#include "CompleteCommand.h"
#include "RejectCommand.h"
#include "CancelCommand.h"
#include "EscalateCommand.h"
#include "ApproveCommand.h"
#include "ProgressReport.h"
#include "TreePrinter.h"
#include "ComponentNotFoundException.h"

namespace tividy {

template <typename T>
class TividyFacade {
public:
    TividyFacade();
    ~TividyFacade();

    WorkflowDefinition<T>* constructIncidentResponseWorkflow(const std::vector<T>& ids);
    WorkflowDefinition<T>* constructJobApplicationWorkflow(const std::vector<T>& ids);
    WorkflowBuilder<T>& getBuilder();

    WorkflowDefinition<T>* cloneWorkflow(const WorkflowDefinition<T>* workflow) const;
    void startWorkflow(WorkflowDefinition<T>* workflow);

    void assignWithStrategies(WorkflowDefinition<T>* workflow, const T& workId,
                              const std::vector<AssignmentStrategy<T>*>& strategies,
                              int numAssignees = -1,
                              const std::vector<Participant<T>*>& escalationParticipants = std::vector<Participant<T>*>(),
                              const std::vector<Participant<T>*>& approvalParticipants = std::vector<Participant<T>*>());
    void assignToParticipants(WorkflowDefinition<T>* workflow, const T& workId,
                              const std::vector<Participant<T>*>& participants,
                              int numAssignees = -1,
                              const std::vector<Participant<T>*>& escalationParticipants = std::vector<Participant<T>*>(),
                              const std::vector<Participant<T>*>& approvalParticipants = std::vector<Participant<T>*>());
    void startWork(WorkflowDefinition<T>* workflow, const T& workId);
    void completeWork(WorkflowDefinition<T>* workflow, const T& workId);
    void rejectWork(WorkflowDefinition<T>* workflow, const T& workId);
    void cancelWork(WorkflowDefinition<T>* workflow, const T& workId);
    void escalateWork(WorkflowDefinition<T>* workflow, const T& workId);
    void approveWork(WorkflowDefinition<T>* workflow, const T& workId);

    std::vector<std::string> getAuditLog(WorkflowDefinition<T>* workflow, const T& workId);
    void addAuditEntry(WorkflowDefinition<T>* workflow, const T& workId, const std::string& entry);
    void editAuditEntry(WorkflowDefinition<T>* workflow, const T& workId, std::size_t index, const std::string& entry);
    void deleteAuditEntry(WorkflowDefinition<T>* workflow, const T& workId, std::size_t index);

    bool undo(WorkflowDefinition<T>* workflow);
    bool isComplete(const WorkflowDefinition<T>* workflow) const;
    std::string getProgressReport(WorkflowDefinition<T>* workflow);
    std::string printWorkflow(WorkflowDefinition<T>* workflow);

private:
    TividyFacade(const TividyFacade&) = delete;
    TividyFacade& operator=(const TividyFacade&) = delete;

    WorkComponent<T>* locate(WorkflowDefinition<T>* workflow, const T& workId) const;
    void buildChains(WorkComponent<T>* target, const std::vector<Participant<T>*>& escalationParticipants, const std::vector<Participant<T>*>& approvalParticipants, Handler<T>*& escalationChain, Handler<T>*& approvalChain) const;

    WorkflowBuilder<T>* builder;
};

}

#endif