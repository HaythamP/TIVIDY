#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "WorkComponent.h"
#include "WorkGroup.h"
#include "WorkItem.h"
#include "UnsupportedOperationException.h"
#include "InvalidWorkflowException.h"

using namespace tividy;

TEST(CompositePatternTest, TreeHierarchyAndItemAggregation){
    WorkGroup<int> root(1, "RootGroup");
    WorkGroup<int>* subGroup = new WorkGroup<int>(2, "SubGroup");
    WorkItem<int>* leaf1 = new WorkItem<int>(10, "Task1");
    WorkItem<int>* leaf2 = new WorkItem<int>(20, "Task2");

    subGroup->add(leaf1);
    root.add(subGroup);
    root.add(leaf2);

    EXPECT_EQ(leaf1->getParent(), subGroup);
    EXPECT_EQ(subGroup->getParent(), &root);
    EXPECT_EQ(leaf2->getParent(), &root);

}