#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "WorkIterator.h"
#include "DepthFirstIterator.h"
#include "WorkItem.h"
#include "WorkGroup.h"

using namespace tividy;

TEST(IteratorPatternTest, DepthFirstTraversalOrder) {
    WorkGroup<int> root(1, "Root");
    WorkItem<int>* child1 = new WorkItem<int>(2, "Child1");
    WorkItem<int>* child2 = new WorkItem<int>(3, "Child2");
    root.add(child1);
    root.add(child2);

    std::vector<WorkComponent<int>*> roots;
    roots.push_back(&root);

    DepthFirstIterator<int> it(roots);

    ASSERT_FALSE(it.isDone());
    EXPECT_EQ(it.getCurrent()->getId(), 1);

    it.next();
    ASSERT_FALSE(it.isDone());
    EXPECT_EQ(it.getCurrent()->getId(), 2);

    it.next();
    ASSERT_FALSE(it.isDone());
    EXPECT_EQ(it.getCurrent()->getId(), 3);

    it.next();
    EXPECT_TRUE(it.isDone());
    EXPECT_EQ(it.getCurrent(), nullptr);

    it.first();
    EXPECT_FALSE(it.isDone());
    EXPECT_EQ(it.getCurrent()->getId(), 1);
}