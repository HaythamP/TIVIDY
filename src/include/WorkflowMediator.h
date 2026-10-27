#ifndef TIVIDY_WORKFLOWMEDIATOR_H
#define TIVIDY_WORKFLOWMEDIATOR_H

#include "Mediator.h"

namespace tividy {

// MEDIATOR: ConcreteMediator. Coordinates one workflow.
// OWNED by the WorkflowDefinition; components hold non-owning pointers to it.
//
// notify(sender):  (sender = the WorkItem that just completed)
//   1. group = sender's parent
//   2. if group exists (sender is inside a WorkGroup):
//        if !group->isComplete()  -> do nothing (the check is only done on groups)
//        else treat the group as the component that completed and repeat from step 1
//   3. if there is no parent (top-level step): next = its next step;
//        if next exists -> next->release()  (WorkItem: Created -> Available;
//                                            WorkGroup: releases all its children)
// Example: WG1{WI1,WI2}, WI3, WI4
//   WI2 completes -> WG1 not complete -> nothing
//   WI1 completes -> WG1 complete -> WI3 released
//   WI3 completes -> WI4 released
//
// canRevertNext / revertNext use the same lookup to find the component that was released
// because of 'sender': it may only be reverted to Created if every WorkItem in it is
// Created, Available or Assigned (never InProgress or Complete).
template <typename IdType>
class WorkflowMediator : public Mediator<IdType> {
public:
    explicit WorkflowMediator(const std::vector<Participant<IdType>*>* participants);   // non-owning

    void notify(WorkComponent<IdType>* sender) override;
    bool canRevertNext(WorkComponent<IdType>* sender) const override;
    void revertNext(WorkComponent<IdType>* sender) override;
    const std::vector<Participant<IdType>*>& getParticipants() const override;

private:
    // The component that was (or would be) released because 'sender' is complete, or null.
    WorkComponent<IdType>* findNext(WorkComponent<IdType>* sender) const;

    const std::vector<Participant<IdType>*>* participants;     // non-owning (owned by the definition)
};

} // namespace tividy

#endif