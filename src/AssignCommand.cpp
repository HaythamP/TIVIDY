#include "AssignCommand.h"
#include "InvalidHandlerChainException.h"
#include "InvalidAssignmentArgumentException.h"
#include <iostream>
#include <sstream>

namespace tividy {

namespace {

template <typename T>
std::string describeChain(const Handler<T>* head) {
    std::ostringstream text;
    for (const Handler<T>* h = head; h != nullptr; h = h->getNext()) {
        if (h != head) {
            text << " -> ";
        }
        Participant<T>* participant = h->getParticipant();
        text << (participant != nullptr ? participant->getName() : std::string("FallbackHandler"));
    }
    return text.str();
}

}

template <typename T>
AssignCommand<T>::AssignCommand(WorkComponent<T>* target,
                                const WorkflowDefinition<T>& workflow,
                                const std::vector<AssignmentStrategy<T>*>& strategies,
                                int numAssignees,
                                Handler<T>* escalationChain,
                                Handler<T>* approvalChain)
    : WorkCommand<T>(target),
      pool(workflow.getParticipants()),
      strategies(strategies),
      numAssignees(numAssignees),
      escalationChain(escalationChain),
      approvalChain(approvalChain),
      byStrategy(true) {}

template <typename T>
AssignCommand<T>::AssignCommand(WorkComponent<T>* target,
                                const std::vector<Participant<T>*>& participants,
                                int numAssignees,
                                Handler<T>* escalationChain,
                                Handler<T>* approvalChain)
    : WorkCommand<T>(target),
      pool(participants),
      numAssignees(numAssignees),
      escalationChain(escalationChain),
      approvalChain(approvalChain),
      byStrategy(false) {}

template <typename T>
AssignCommand<T>::~AssignCommand() {
    delete escalationChain;
    delete approvalChain;
}

template <typename T>
std::string AssignCommand<T>::getDescription() const {
    return "Assign " + this->target->getName();
}

template <typename T>
void AssignCommand<T>::perform() {
    if (byStrategy && strategies.empty()) {
        throw InvalidAssignmentArgumentException("Strategy assignment needs at least one strategy; to choose participants yourself, supply a participant list instead");
    }
    if (!byStrategy && pool.empty()) {
        throw InvalidAssignmentArgumentException("Direct assignment needs a non-empty list of participants");
    }

    bool needsEscalation = this->target->hasEscalation();
    bool needsApproval = this->target->hasApproval();

    if (needsEscalation && escalationChain == nullptr) {
        throw InvalidHandlerChainException(this->target->getName() + " requires an escalation chain");
    }
    if (!needsEscalation && escalationChain != nullptr) {
        throw InvalidHandlerChainException(this->target->getName() + " does not support escalation");
    }
    if (needsApproval && approvalChain == nullptr) {
        throw InvalidHandlerChainException(this->target->getName() + " requires an approval chain with at least one approver");
    }
    if (!needsApproval && approvalChain != nullptr) {
        throw InvalidHandlerChainException(this->target->getName() + " does not support approval");
    }

    std::vector<Participant<T>*> selected = this->target->assignWork(pool, strategies, numAssignees);
    this->target->assignTo(selected);

    if (needsEscalation) {
        std::string description = describeChain<T>(escalationChain);
        Handler<T>* chain = escalationChain;
        this->target->setEscalationChain(chain);
        escalationChain = nullptr;
        std::cout << "  Escalation chain: " << description << std::endl;
    }
    if (needsApproval) {
        std::string description = describeChain<T>(approvalChain);
        Handler<T>* chain = approvalChain;
        this->target->setApprovalChain(chain);
        approvalChain = nullptr;
        std::cout << "  Approval chain: " << description << std::endl;
    }
}

template class AssignCommand<int>;
template class AssignCommand<std::string>;

}
