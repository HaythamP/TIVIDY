#ifndef ASSIGNCOMMAND_H
#define ASSIGNCOMMAND_H

#include <vector>
#include "WorkCommand.h"
#include "Participant.h"
#include "AssignmentStrategy.h"
#include "Handler.h"
#include "WorkflowDefinition.h"

namespace tividy {

/**
 * @brief ConcreteCommand: assigns participants to a work item and installs its handler chains.
 *
 * Two construction forms exist. Strategy form: the candidates are always the entire participant pool of the given workflow and the strategies pick from it (no separate participant list). Direct form: no strategies; the caller supplies the participants (and optionally numAssignees to take the first few).
 * Chains: items with EscalatedWork need an escalation chain (LevelHandlers followed by a FallbackHandler); items with ApprovedWork need an approval chain with at least one approver. Chains are never filtered by strategies. The command owns the chains until they are installed, after which the decorator owns them; an undo removes them again.
 */
template <typename T>
class AssignCommand : public WorkCommand<T> {
public:
    /**
     * @brief Strategy form: selects from the whole participant pool of workflow using the strategies.
     *
     * @param target the work item to assign (a WorkItem, possibly decorated).
     * @param workflow its participant pool is the candidate set.
     * @param strategies applied in order; at least one is required.
     * @param numAssignees how many to select, -1 for everyone left.
     * @param escalationChain owned until installed.
     * @param approvalChain owned until installed.
     */
    AssignCommand(WorkComponent<T>* target,
                  const WorkflowDefinition<T>& workflow,
                  const std::vector<AssignmentStrategy<T>*>& strategies,
                  int numAssignees = -1,
                  Handler<T>* escalationChain = nullptr,
                  Handler<T>* approvalChain = nullptr);
    /**
     * @brief Direct form: assigns from an explicit participant list (no strategies).
     *
     * @param target the work item to assign.
     * @param participants the explicitly chosen candidates.
     * @param numAssignees how many of the list to take, -1 for all.
     * @param escalationChain owned until installed; required for escalated items.
     * @param approvalChain owned until installed; required for approved items.
     */
    AssignCommand(WorkComponent<T>* target,
                  const std::vector<Participant<T>*>& participants,
                  int numAssignees = -1,
                  Handler<T>* escalationChain = nullptr,
                  Handler<T>* approvalChain = nullptr);
    /** @brief Deletes chains that were never installed. */
    ~AssignCommand();

    /** @brief Returns "Assign" plus the item name. */
    std::string getDescription() const override;

protected:
    /**
     * @brief Validates the arguments and chains, selects, assigns and installs the chains.
     *
     * @throws InvalidAssignmentArgumentException, InsufficientParticipantsException, InvalidHandlerChainException.
     */
    void perform() override;

private:
    std::vector<Participant<T>*> pool;  ///< Candidate participants (non-owning).
    std::vector<AssignmentStrategy<T>*> strategies;  ///< Caller-owned strategies (strategy form only).
    int numAssignees;  ///< Requested count, -1 for all.
    Handler<T>* escalationChain;  ///< Owned until installed.
    Handler<T>* approvalChain;  ///< Owned until installed.
    bool byStrategy;  ///< True for the strategy form.
};

}

#endif
