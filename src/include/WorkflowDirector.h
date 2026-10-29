#ifndef WORKFLOWDIRECTOR_H
#define WORKFLOWDIRECTOR_H

#include <vector>
#include "Builder.h"

namespace tividy {

/**
 * @brief Director of the Builder pattern: knows the recipes of the two concrete scenarios and drives any Builder.
 *
 * Incident Response (Power Outage), 5 ids: Dispatch Team, Investigation (audited), Containment, Repair (audited, escalation, approval), Restore.
 * Job Application (Tech Company), 7 ids: group Initial Screening (Application Review and Reference Check, both audited), group Interview (Technical and Behavioural Interview, both audited), Final Decision (approval).
 * Ids are consumed in the order the components are listed (a group id before its children).
 */
template <typename T>
class WorkflowDirector {
public:
    /** @brief Creates a director. */
    WorkflowDirector();

    /**
     * @brief Builds the Power Outage workflow; use the concrete builder's getResult() afterwards.
     *
     * @throws InvalidWorkflowException unless exactly 5 ids are given.
     */
    void constructIncidentResponseWorkflow(Builder<T>& builder, const std::vector<T>& ids) const;
    /**
     * @brief Builds the Tech Company hiring workflow.
     *
     * @throws InvalidWorkflowException unless exactly 7 ids are given.
     */
    void constructJobApplicationWorkflow(Builder<T>& builder, const std::vector<T>& ids) const;

private:
    WorkflowDirector(const WorkflowDirector&) = delete;
    WorkflowDirector& operator=(const WorkflowDirector&) = delete;
};

}

#endif