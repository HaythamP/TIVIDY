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

    EXPECT_EQ(root.find(10), leaf1);
    EXPECT_EQ(root.find(20), leaf2);
    EXPECT_EQ(root.find(2), subGroup);
    EXPECT_EQ(root.find(999), nullptr);

    std::vector<WorkComponent<int>*> items = root.getItems();
    EXPECT_EQ(items.size(), 2u);

    std::vector<WorkComponent<int>*> children = root.getChildren();
    EXPECT_EQ(children.size(), 2u);
}

TEST(CompositePatternTest, GroupDisallowsLeafOperations) {
    WorkGroup<int> group(1, "Group");
    EXPECT_THROW(group.start(), UnsupportedOperationException);
    EXPECT_THROW(group.complete(), UnsupportedOperationException);
    EXPECT_THROW(group.cancel(), UnsupportedOperationException);
    EXPECT_THROW(group.add(nullptr), InvalidWorkflowException);
}