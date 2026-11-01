#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "WorkItemState.h"
#include "CreatedState.h"
#include "AvailableState.h"
#include "AssignedState.h"
#include "InProgressState.h"
#include "ApprovalState.h"
#include "EscalatedState.h"
#include "CompletedState.h"
#include "RejectedState.h"
#include "CancelledState.h"
#include "InvalidTransitionException.h"
#include "WorkItem.h"
#include "Participant.h"

using namespace tividy;

TEST(StatePatternTest, InitialQueryProperties) {
    CreatedState<int> created;
    EXPECT_EQ(created.getName(), "Created");
    EXPECT_FALSE(created.isComplete());
    EXPECT_FALSE(created.isAvailable());
    EXPECT_FALSE(created.isTerminal());

    AvailableState<int> available;
    EXPECT_EQ(available.getName(), "Available");
    EXPECT_FALSE(available.isComplete());
    EXPECT_TRUE(available.isAvailable());
    EXPECT_FALSE(available.isTerminal());

    AssignedState<int> assigned;
    EXPECT_EQ(assigned.getName(), "Assigned");
    EXPECT_FALSE(assigned.isComplete());
    EXPECT_FALSE(assigned.isAvailable());
    EXPECT_FALSE(assigned.isTerminal());

    InProgressState<int> inProgress;
    EXPECT_EQ(inProgress.getName(), "InProgress");
    EXPECT_FALSE(inProgress.isComplete());
    EXPECT_FALSE(inProgress.isAvailable());
    EXPECT_FALSE(inProgress.isTerminal());

    ApprovalState<int> approval;
    EXPECT_EQ(approval.getName(), "Approval");
    EXPECT_FALSE(approval.isComplete());
    EXPECT_FALSE(approval.isAvailable());
    EXPECT_FALSE(approval.isTerminal());

    EscalatedState<int> escalated;
    EXPECT_EQ(escalated.getName(), "Escalated");
    EXPECT_FALSE(escalated.isComplete());
    EXPECT_FALSE(escalated.isAvailable());
    EXPECT_FALSE(escalated.isTerminal());

    CompletedState<int> completed;
    EXPECT_EQ(completed.getName(), "Completed");
    EXPECT_TRUE(completed.isComplete());
    EXPECT_FALSE(completed.isAvailable());
    EXPECT_TRUE(completed.isTerminal());

    RejectedState<int> rejectedTerminal(false);
    EXPECT_EQ(rejectedTerminal.getName(), "Rejected");
    EXPECT_TRUE(rejectedTerminal.isTerminal());
    EXPECT_FALSE(rejectedTerminal.isRestartable());

    RejectedState<int> rejectedRestartable(true);
    EXPECT_EQ(rejectedRestartable.getName(), "Rejected");
    EXPECT_TRUE(rejectedRestartable.isTerminal());
    EXPECT_TRUE(rejectedRestartable.isRestartable());

    CancelledState<int> cancelled;
    EXPECT_EQ(cancelled.getName(), "Cancelled");
    EXPECT_FALSE(cancelled.isComplete());
    EXPECT_FALSE(cancelled.isAvailable());
    EXPECT_TRUE(cancelled.isTerminal());
}

TEST(StatePatternTest, PolymorphicCopyAndIdentity) {
    WorkItemState<int>* s1 = new InProgressState<int>();
    WorkItemState<int>* s2 = s1->copy();

    EXPECT_NE(s1, s2);
    EXPECT_EQ(s1->getName(), s2->getName());

    WorkItemState<int>* r1 = new RejectedState<int>(true);
    WorkItemState<int>* r2 = r1->copy();
    EXPECT_TRUE(dynamic_cast<RejectedState<int>*>(r2)->isRestartable());

    delete s1;
    delete s2;
    delete r1;
    delete r2;
}

TEST(StatePatternTest, DefaultStateTransitionGuardsThrow) {
    CreatedState<int> state;
    WorkItem<int> dummy(1, "DummyItem");

    EXPECT_THROW(state.start(dummy), InvalidTransitionException);
    EXPECT_THROW(state.complete(dummy), InvalidTransitionException);
    EXPECT_THROW(state.reject(dummy), InvalidTransitionException);
    EXPECT_THROW(state.escalate(dummy), InvalidTransitionException);
    EXPECT_THROW(state.approve(dummy), InvalidTransitionException);
}