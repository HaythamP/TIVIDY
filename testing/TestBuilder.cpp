#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "Builder.h"
#include "WorkflowBuilder.h"
#include "WorkflowDirector.h"
#include "WorkflowDefinition.h"
#include "InvalidWorkflowException.h"
#include "DuplicateIdException.h"

using namespace tividy;

TEST(BuilderPatternTest, BuilderLifecycleAndStateValidation) {
    WorkflowBuilder<int> builder;
    EXPECT_THROW(builder.getResult(), InvalidWorkflowException);
    EXPECT_THROW(builder.newWorkItem(1, "Task"), InvalidWorkflowException);

    builder.reset("Simple");
    builder.newWorkItem(1, "Task1").endWorkItem();
    EXPECT_THROW(builder.newWorkItem(1, "DuplicateTask"), DuplicateIdException);

    WorkflowDefinition<int>* wf = builder.getResult();
    ASSERT_NE(wf, nullptr);
    EXPECT_EQ(wf->getName(), "Simple");
    delete wf;
}

TEST(DirectorPatternTest, DirectorEnforcesIdCount) {
    WorkflowDirector<int> director;
    WorkflowBuilder<int> builder;

    // Power Outage requires exactly 5 IDs
    std::vector<int> badIds = {1, 2, 3};
    EXPECT_THROW(director.constructIncidentResponseWorkflow(builder, badIds), InvalidWorkflowException);

    std::vector<int> validIds = {1, 2, 3, 4, 5};
    director.constructIncidentResponseWorkflow(builder, validIds);
    WorkflowDefinition<int>* ir = builder.getResult();
    ASSERT_NE(ir, nullptr);
    EXPECT_EQ(ir->getName(), "Incident Response (Power Outage)");
    delete ir;
}