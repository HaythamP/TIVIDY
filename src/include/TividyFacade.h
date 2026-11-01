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

/**
 * @brief Facade: one simple entry point for the subsystem (builder, director, prototype, commands, visitors) so a GUI or main() does not have to know the individual classes. It is optional: every operation can also be done directly with the subsystem classes.
 *
 * It owns one WorkflowBuilder. Workflows it returns belong to the caller. Every command method looks the component up by id (ComponentNotFoundException if unknown), creates the matching command and executes it through the workflow's history, which prints what happens.
 * Assignment comes in two forms: assignWithStrategies() always selects from the workflow's entire participant pool; assignToParticipants() is used when no strategies are given and takes the participants explicitly. Escalation and approval participants become chains (the facade adds the FallbackHandler to escalation chains).
 */
template <typename T>
class TividyFacade {
public:
    /** @brief Creates a facade with its own builder. */
    TividyFacade();
    /** @brief Deletes the builder. */
    ~TividyFacade();

    /** @brief Builds the Power Outage template (5 ids). The caller owns the result. */
    WorkflowDefinition<T>* constructIncidentResponseWorkflow(const std::vector<T>& ids);
    /** @brief Builds the Tech Company template (7 ids). The caller owns the result. */
    WorkflowDefinition<T>* constructJobApplicationWorkflow(const std::vector<T>& ids);
    /** @brief Returns the builder for custom workflows: reset(), build, then getResult(). */
    WorkflowBuilder<T>& getBuilder();

    /** @brief Clones a pristine workflow (Prototype). The caller owns the clone. */
    WorkflowDefinition<T>* cloneWorkflow(const WorkflowDefinition<T>* workflow) const;
    /** @brief Releases the first step. */
    void startWorkflow(WorkflowDefinition<T>* workflow);

    /**
     * @brief Assigns from the workflow's whole participant pool using the strategies.
     *
     * @param workflow the running workflow whose whole participant pool is used.
     * @param workId id of the work item.
     * @param strategies applied in order (at least one).
     * @param numAssignees how many to pick, -1 for everyone left.
     * @param escalationParticipants levels of the escalation chain (needed for escalated items; a fallback is added; an empty list leaves only the fallback).
     * @param approvalParticipants approvers in order (needed for approved items).
     */
    void assignWithStrategies(WorkflowDefinition<T>* workflow, const T& workId,
                              const std::vector<AssignmentStrategy<T>*>& strategies,
                              int numAssignees = -1,
                              const std::vector<Participant<T>*>& escalationParticipants = std::vector<Participant<T>*>(),
                              const std::vector<Participant<T>*>& approvalParticipants = std::vector<Participant<T>*>());
    /**
     * @brief Assigns explicitly chosen participants (the no-strategy form).
     *
     * @param workflow the running workflow.
     * @param workId id of the work item.
     * @param participants the candidates.
     * @param numAssignees how many of them to take, -1 for all.
     * @param escalationParticipants levels of the escalation chain (the fallback is added).
     * @param approvalParticipants approvers in order.
     */
    void assignToParticipants(WorkflowDefinition<T>* workflow, const T& workId,
                              const std::vector<Participant<T>*>& participants,
                              int numAssignees = -1,
                              const std::vector<Participant<T>*>& escalationParticipants = std::vector<Participant<T>*>(),
                              const std::vector<Participant<T>*>& approvalParticipants = std::vector<Participant<T>*>());
    /** @brief Starts an item (or restarts one rejected from Approval). */
    void startWork(WorkflowDefinition<T>* workflow, const T& workId);
    /** @brief Completes an item. */
    void completeWork(WorkflowDefinition<T>* workflow, const T& workId);
    /** @brief Rejects an item; the workflow halts. */    
    void rejectWork(WorkflowDefinition<T>* workflow, const T& workId);
    /** @brief Cancels an item; the workflow halts. */    
    void cancelWork(WorkflowDefinition<T>* workflow, const T& workId);
    /** @brief Starts or advances the escalation chain. */
    void escalateWork(WorkflowDefinition<T>* workflow, const T& workId);
    /** @brief Starts or advances the approval chain. */    
    void approveWork(WorkflowDefinition<T>* workflow, const T& workId);

    /** @brief Returns the audit log of an audited item. */
    std::vector<std::string> getAuditLog(WorkflowDefinition<T>* workflow, const T& workId);
    /** @brief Adds a manual audit entry. */    
    void addAuditEntry(WorkflowDefinition<T>* workflow, const T& workId, const std::string& entry);
    /** @brief Edits the audit entry at a zero-based index. */
    void editAuditEntry(WorkflowDefinition<T>* workflow, const T& workId, std::size_t index, const std::string& entry);
    /** @brief Deletes the audit entry at a zero-based index. */
    void deleteAuditEntry(WorkflowDefinition<T>* workflow, const T& workId, std::size_t index);

    /** @brief Undoes the newest command if allowed; returns whether it did. */
    bool undo(WorkflowDefinition<T>* workflow);
    /** @brief True when the whole workflow is complete. */
    bool isComplete(const WorkflowDefinition<T>* workflow) const;
    /** @brief One-line summary produced by ProgressReport. */
    std::string getProgressReport(WorkflowDefinition<T>* workflow);
    /** @brief Indented tree text produced by TreePrinter. */
    std::string printWorkflow(WorkflowDefinition<T>* workflow);

private:
    TividyFacade(const TividyFacade&) = delete;
    TividyFacade& operator=(const TividyFacade&) = delete;

    /** @brief Finds a component or throws ComponentNotFoundException. */
    WorkComponent<T>* locate(WorkflowDefinition<T>* workflow, const T& workId) const;
    /** @brief Creates the handler chains from participant lists. */
    void buildChains(WorkComponent<T>* target, const std::vector<Participant<T>*>& escalationParticipants, const std::vector<Participant<T>*>& approvalParticipants, Handler<T>*& escalationChain, Handler<T>*& approvalChain) const;

    WorkflowBuilder<T>* builder;  ///< Owned builder.
};

}

#endif