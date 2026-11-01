#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "WorkGroup.h"
#include "WorkItem.h"
#include "WorkflowMediator.h"
#include "Participant.h"

using namespace tividy;

TEST(MediatorStateCompositeTest, CompletionTriggersNextStepRelease) {
    WorkflowMediator<int> mediator;

    WorkItem<int>* step1 = new WorkItem<int>(1, "Step1");
    WorkItem<int>* step2 = new WorkItem<int>(2, "Step2");

    // Register top-level colleagues with the mediator
    mediator.addColleague(step1);
    mediator.addColleague(step2);
    step1->setMediator(&mediator);
    step2->setMediator(&mediator);

    // Start step1
    step1->release();
    EXPECT_TRUE(step1->isAvailable());
    EXPECT_FALSE(step2->isAvailable());

    // Move step1: Available -> Assigned -> InProgress -> Completed
    Participant<int> user(1, "Dev", {"developer"});
    std::vector<Participant<int>*> assignees;
    assignees.push_back(&user);

    step1->assignTo(assignees);
    step1->start();
    EXPECT_EQ(step1->getStateName(), "InProgress");

    // Completing step 1 triggers mediator->notify(this)
    step1->complete();
    EXPECT_TRUE(step1->isComplete());

    // Mediator automatically calls release() on step 2
    EXPECT_TRUE(step2->isAvailable());
    EXPECT_EQ(step2->getStateName(), "Available");

    delete step1;
    delete step2;
}