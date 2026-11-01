#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "TividyFacade.h"
#include "WorkflowDefinition.h"
#include "Participant.h"
#include "RoleAssignment.h"
#include "ComponentNotFoundException.h"
#include "InvalidWorkflowException.h"

using namespace tividy;

TEST(FacadePatternTest, ManagesWorkflowLifecycleAndLocateGuards) {
    TividyFacade<int> facade;

    // Direct access to builder through facade
    facade.getBuilder().reset("FacadePipeline");
    facade.getBuilder().newWorkItem(1, "Task1").endWorkItem();
    WorkflowDefinition<int>* wf = facade.getBuilder().getResult();
    ASSERT_NE(wf, nullptr);

    // Guard on invalid component lookups through facade operations
    EXPECT_THROW(facade.startWork(wf, 999), ComponentNotFoundException);
    EXPECT_THROW(facade.completeWork(wf, 999), ComponentNotFoundException);
    EXPECT_THROW(facade.cancelWork(wf, 999), ComponentNotFoundException);

    // Facade lifecycle calls
    facade.startWorkflow(wf);
    EXPECT_FALSE(facade.isComplete(wf));

    Participant<int> p(10, "Tom", {"Engineer"});
    std::vector<Participant<int>*> pool = {&p};
    facade.assignToParticipants(wf, 1, pool);
    facade.startWork(wf, 1);
    facade.completeWork(wf, 1);

    EXPECT_TRUE(facade.isComplete(wf));

    // Facade report generators
    std::string report = facade.getProgressReport(wf);
    std::string tree = facade.printWorkflow(wf);
    EXPECT_FALSE(report.empty());
    EXPECT_FALSE(tree.empty());

    delete wf;
}

TEST(FacadePatternTest, ClonesPristineWorkflowViaFacade) {
    TividyFacade<int> facade;
    std::vector<int> ids = {1, 2, 3, 4, 5};
    WorkflowDefinition<int>* proto = facade.constructIncidentResponseWorkflow(ids);
    ASSERT_NE(proto, nullptr);

    WorkflowDefinition<int>* clone = facade.cloneWorkflow(proto);
    ASSERT_NE(clone, nullptr);
    EXPECT_NE(proto, clone);
    EXPECT_EQ(proto->getName(), clone->getName());

    delete proto;
    delete clone;
}

TEST(FacadePatternTest, NullWorkflowGuardsThrow) {
    TividyFacade<int> facade;
    EXPECT_THROW(facade.startWorkflow(nullptr), InvalidWorkflowException);
    EXPECT_THROW(facade.cloneWorkflow(nullptr), InvalidWorkflowException);
    EXPECT_THROW(facade.isComplete(nullptr), InvalidWorkflowException);
    EXPECT_THROW(facade.undo(nullptr), InvalidWorkflowException);
}