#ifndef WORKFLOWBUILDER_H
#define WORKFLOWBUILDER_H

#include <string>
#include <vector>
#include "WorkflowDefinition.h"
#include "WorkGroup.h"
#include "AssignmentStrategy.h"
#include "Participant.h"

namespace tividy {

template <typename T>
class WorkflowBuilder {
public:
    WorkflowBuilder();
    ~WorkflowBuilder();

    WorkflowBuilder& reset(const std::string& workflowName);
    WorkflowBuilder& addParticipant(Participant<T>* participant);

    WorkflowBuilder& beginGroup(const T& id, const std::string& name);
    WorkflowBuilder& endGroup();

    WorkflowBuilder& newWorkItem(const T& id, const std::string& name);
    WorkflowBuilder& withStrategy(AssignmentStrategy<T>* strategy);
    WorkflowBuilder& withAudit();
    WorkflowBuilder& withApproval();
    WorkflowBuilder& withEscalation();
    WorkflowBuilder& endWorkItem();

    WorkflowDefinition<T>* getResult();

private:
    WorkflowBuilder(const WorkflowBuilder&) = delete;
    WorkflowBuilder& operator=(const WorkflowBuilder&) = delete;

    WorkflowDefinition<T>* workflow;
    WorkComponent<T>* pending;
    std::vector<WorkGroup<T>*> openGroups;
};

} // namespace tividy

#endif