#include <gtest/gtest.h>
#include <string>

#include "Mediator.h"
#include "WorkflowMediator.h"
#include "WorkItem.h"
#include "InvalidWorkflowException.h"

using namespace tividy;

TEST(MediatorPatternTest, ColleagueRegistrationAndSuccessorLookup) {
    WorkflowMediator<int> mediator;
    EXPECT_THROW(mediator.addColleague(nullptr), InvalidWorkflowException);

    WorkItem<int> step1(1, "Step1");
    WorkItem<int> step2(2, "Step2");
    WorkItem<int> unlinked(3, "Unlinked");

    mediator.addColleague(&step1);
    mediator.addColleague(&step2);

    EXPECT_EQ(mediator.findNext(&step1), &step2);
    EXPECT_EQ(mediator.findNext(&step2), nullptr);
    EXPECT_EQ(mediator.findNext(&unlinked), nullptr);
    EXPECT_EQ(mediator.findNext(nullptr), nullptr);
}