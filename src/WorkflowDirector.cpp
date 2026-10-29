#include "WorkflowDirector.h"
#include "InvalidWorkflowException.h"
#include <string>

namespace tividy {

template <typename T>
WorkflowDirector<T>::WorkflowDirector() {}

template <typename T>
void WorkflowDirector<T>::constructIncidentResponseWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 5) {
        throw InvalidWorkflowException("The Incident Response workflow requires exactly 5 ids");
    }
    builder.reset("Incident Response (Power Outage)");
    builder.newWorkItem(ids[0], "Dispatch Team").endWorkItem();
    builder.newWorkItem(ids[1], "Investigation").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Containment").endWorkItem();
    builder.newWorkItem(ids[3], "Repair").withAudit().withEscalation().withApproval().endWorkItem();
    builder.newWorkItem(ids[4], "Restore").endWorkItem();
}

template <typename T>
void WorkflowDirector<T>::constructJobApplicationWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 7) {
        throw InvalidWorkflowException("The Job Application workflow requires exactly 7 ids");
    }
    builder.reset("Job Application (Tech Company)");

    builder.beginGroup(ids[0], "Initial Screening");
    builder.newWorkItem(ids[1], "Application Review").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Reference Check").withAudit().endWorkItem();
    builder.endGroup();

    builder.beginGroup(ids[3], "Interview");
    builder.newWorkItem(ids[4], "Technical Interview").withAudit().endWorkItem();
    builder.newWorkItem(ids[5], "Behavioural Interview").withAudit().endWorkItem();
    builder.endGroup();

    builder.newWorkItem(ids[6], "Final Decision").withApproval().endWorkItem();
}

template class WorkflowDirector<int>;
template class WorkflowDirector<std::string>;

}