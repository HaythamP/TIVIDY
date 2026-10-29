#ifndef WORKFLOWMEDIATOR_H
#define WORKFLOWMEDIATOR_H

#include <vector>
#include "Mediator.h"

namespace tividy {

/**
 * @brief ConcreteMediator: sequences the top-level steps of one workflow.
 *
 * When an item completes it climbs to the item's top-level step; if that step (an item or a group) is complete it releases the next step, otherwise it does nothing (a parallel sibling is still running). Rejected and Cancelled items never complete, so progression halts. Colleagues are non-owning.
 */
template <typename T>
class WorkflowMediator : public Mediator<T> {
public:
    WorkflowMediator();

    /** @brief Releases the next step when the sender's step is complete and prints what happened. */
    void notify(WorkComponent<T>* sender) override;
    /** @brief Returns the step after the sender's step. */
    WorkComponent<T>* findNext(WorkComponent<T>* sender) const override;
    /** @brief Registers a top-level step in order. */
    void addColleague(WorkComponent<T>* colleague) override;

private:
    WorkflowMediator(const WorkflowMediator&) = delete;
    WorkflowMediator& operator=(const WorkflowMediator&) = delete;

    /** @brief Index of the top-level step containing the sender, or -1. */
    int indexOfStep(WorkComponent<T>* sender) const;

    std::vector<WorkComponent<T>*> colleagues;  ///< Ordered top-level steps (non-owning).
};

}

#endif