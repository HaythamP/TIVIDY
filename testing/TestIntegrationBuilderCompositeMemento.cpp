#include <gtest/gtest.h>
#include <string>

#include "WorkflowBuilder.h"
#include "WorkflowDefinition.h"
#include "WorkGroup.h"
#include "WorkItem.h"
#include "WorkMemento.h"

using namespace tividy;

TEST(BuilderCompositeMementoTest, BuildHierarchyAndRestoreSnapshot) {
    WorkflowBuilder<int> builder;
    builder.reset("Pipeline");

    builder.beginGroup(100, "GroupA");
        builder.newWorkItem(1, "Task1").endWorkItem();
    builder.endGroup();
    builder.newWorkItem(2, "Task2").endWorkItem();

    WorkflowDefinition<int>* wf = builder.getResult();
    ASSERT_NE(wf, nullptr);

    WorkComponent<int>* item1 = wf->find(1);
    ASSERT_NE(item1, nullptr);
    EXPECT_EQ(item1->getStateName(), "Created");

    // Capture initial state memento
    WorkMemento<int>* snapshot = item1->createMemento();
    ASSERT_NE(snapshot, nullptr);

    // Mutate state through release
    item1->release();
    EXPECT_EQ(item1->getStateName(), "Available");

    // Restore back to Created state via memento
    item1->restore(*snapshot);
    EXPECT_EQ(item1->getStateName(), "Created");

    delete snapshot;
    delete wf;
}