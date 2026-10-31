#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "WorkItem.h"
#include "EscalatedWork.h"
#include "LevelHandler.h"
#include "FallbackHandler.h"
#include "AssignCommand.h"
#include "EscalateCommand.h"
#include "Participant.h"
#include "AssignmentStrategy.h"

using namespace tividy;

TEST(CommandDecoratorCorTest, CommandDrivesEscalationChain) {
    Participant<int> worker(1, "Worker",{"Dev"});
    Participant<int> lead(2, "TeamLead",{"Lead"});

    std::vector<Participant<int>*> pool;
    pool.push_back(&worker);
    std::vector<AssignmentStrategy<int>*> noStrategies;

    // Build decorator wrapping a raw WorkItem
    WorkComponent<int>* leaf = new WorkItem<int>(10, "EscalationTask");
    WorkComponent<int>* decorated = new EscalatedWork<int>(leaf);

    // Build Chain: LevelHandler -> FallbackHandler
    Handler<int>* chain = new LevelHandler<int>(&lead, 0);
    chain->setNext(new FallbackHandler<int>());

    // Target must be Available before assigning
    decorated->release();

    // Execute AssignCommand directly
    AssignCommand<int> assignCmd(decorated, pool, 1, chain, nullptr);
    assignCmd.execute();
    EXPECT_EQ(decorated->getStateName(), "Assigned");

    // Start work to reach InProgress
    decorated->start();
    EXPECT_EQ(decorated->getStateName(), "InProgress");

    // Execute EscalateCommand directly
    EscalateCommand<int> escCmd(decorated);
    escCmd.execute();

    EXPECT_EQ(decorated->getStateName(), "Escalated");
    EXPECT_EQ(decorated->getEscalationContact(), &lead);

    delete decorated; // Frees inner leaf and attached handler chain
}