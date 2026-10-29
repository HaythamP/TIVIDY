#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "Command.h"
#include "CommandHistory.h"
#include "StartCommand.h"
#include "CompleteCommand.h"
#include "RejectCommand.h"
#include "CancelCommand.h"
#include "WorkItem.h"
#include "ComponentNotFoundException.h"
#include "InvalidWorkflowException.h"

using namespace tividy;

TEST(CommandPatternTest, TargetValidationAndDescriptions) {
    EXPECT_THROW(StartCommand<int>(nullptr), ComponentNotFoundException);

    WorkItem<int> item(1, "DeployService");
    StartCommand<int> startCmd(&item);
    CompleteCommand<int> compCmd(&item);
    RejectCommand<int> rejCmd(&item);
    CancelCommand<int> canCmd(&item);

    EXPECT_EQ(startCmd.getDescription(), "Start DeployService");
    EXPECT_EQ(compCmd.getDescription(), "Complete DeployService");
    EXPECT_EQ(rejCmd.getDescription(), "Reject DeployService");
    EXPECT_EQ(canCmd.getDescription(), "Cancel DeployService");
}

TEST(CommandPatternTest, CommandHistoryExecutionAndUndo) {
    CommandHistory<int> history;
    EXPECT_THROW(history.run(nullptr), InvalidWorkflowException);
    EXPECT_FALSE(history.undoLast());

    WorkItem<int> item(1, "Task");
    item.release();

    std::vector<Participant<int>*> assignees;
    Participant<int> p(10, "Dev");
    assignees.push_back(&p);
    item.assignTo(assignees);

    EXPECT_EQ(item.getStateName(), "Assigned");

    // Execute StartCommand via Invoker
    history.run(new StartCommand<int>(&item));
    EXPECT_EQ(item.getStateName(), "InProgress");
    EXPECT_EQ(history.getDescriptions().size(), 1u);

    // Multi-level undo
    EXPECT_TRUE(history.undoLast());
    EXPECT_EQ(item.getStateName(), "Assigned");
    EXPECT_TRUE(history.getDescriptions().empty());
}