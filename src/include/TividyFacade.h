#ifndef TIVIDYFACADE_H
#define TIVIDYFACADE_H

#include <string>
#include <vector>
#include "WorkflowBuilder.h"
#include "WorkflowDefinition.h"
#include "Participant.h"
#include "RoleAssignment.h"
#include "LeastLoadedAssignment.h"
#include "RuleAssignment.h"
#include "AssignCommand.h"
#include "StartCommand.h"
#include "CompleteCommand.h"
#include "RejectCommand.h"
#include "CancelCommand.h"
#include "EscalateCommand.h"
#include "ApproveCommand.h"
#include "ProgressReport.h"

namespace tividy {

template <typename T>
class TividyFacade {
public:
    TividyFacade();
    ~TividyFacade();

    WorkflowDefinition<T>* constructSimpleWorkflow(const std::vector<T>& ids);
    WorkflowDefinition<T>* constructComplexWorkflow(const std::vector<T>& ids);

    void addParticipant(WorkflowDefinition<T>* workflow, Participant<T>* participant);
    WorkflowDefinition<T>* cloneWorkflow(const WorkflowDefinition<T>* workflow) const;
    void startWorkflow(WorkflowDefinition<T>* workflow);

    void assignWork(WorkflowDefinition<T>* workflow, const T& workId, const std::vector<Participant<T>*>& participants, int numAssignees = 0);
    void startWork(WorkflowDefinition<T>* workflow, const T& workId);
    void completeWork(WorkflowDefinition<T>* workflow, const T& workId);
    void rejectWork(WorkflowDefinition<T>* workflow, const T& workId);
    void cancelWork(WorkflowDefinition<T>* workflow, const T& workId);
    void escalateWork(WorkflowDefinition<T>* workflow, const T& workId, const std::vector<Participant<T>*>& participants);
    void approveWork(WorkflowDefinition<T>* workflow, const T& workId, const std::vector<Participant<T>*>& participants);

    bool undo(WorkflowDefinition<T>* workflow);
    bool isComplete(const WorkflowDefinition<T>* workflow) const;
    std::string getProgressReport(WorkflowDefinition<T>* workflow);

private:
    TividyFacade(const TividyFacade&) = delete;
    TividyFacade& operator=(const TividyFacade&) = delete;

    WorkflowBuilder<T>* builder;
};

} // namespace tividy

#endif