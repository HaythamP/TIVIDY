#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "AssignmentStrategy.h"
#include "RoleAssignment.h"
#include "LeastLoadedAssignment.h"
#include "RuleAssignment.h"
#include "Participant.h"
#include "InvalidAssignmentArgumentException.h"

using namespace tividy;

TEST(StrategyPatternTest, RoleAssignmentFiltersByRole) {
    EXPECT_THROW(RoleAssignment<int>(""), InvalidAssignmentArgumentException);

    RoleAssignment<int> roleStrategy("dev");
    EXPECT_FALSE(roleStrategy.overridesSelection());

    Participant<int> p1(1, "Alice", {"dev"});
    Participant<int> p2(2, "Bob", {"ops"});

    std::vector<Participant<int>*> candidates;
    candidates.push_back(&p1);
    candidates.push_back(&p2);

    roleStrategy.filter(candidates);
    ASSERT_EQ(candidates.size(), 1u);
    EXPECT_EQ(candidates[0]->getId(), 1);
}

TEST(StrategyPatternTest, LeastLoadedAssignmentSortsByAscendingWorkload) {
    LeastLoadedAssignment<int> strategy;
    EXPECT_FALSE(strategy.overridesSelection());

    Participant<int> p1(1, "Alice", {"dev"});
    Participant<int> p2(2, "Bob", {"ops"});
    Participant<int> p3(3, "Carol", {"dev"});

    p1.adjustWorkload(5);
    p2.adjustWorkload(1);
    p3.adjustWorkload(3);

    std::vector<Participant<int>*> candidates;
    candidates.push_back(&p1);
    candidates.push_back(&p2);
    candidates.push_back(&p3);

    strategy.filter(candidates);
    ASSERT_EQ(candidates.size(), 3u);
    EXPECT_EQ(candidates[0]->getId(), 2); // 1
    EXPECT_EQ(candidates[1]->getId(), 3); // 3
    EXPECT_EQ(candidates[2]->getId(), 1); // 5
}

TEST(StrategyPatternTest, RuleAssignmentOverridesSelectionAndTruncates) {
    RuleAssignment<int> strategy;
    EXPECT_TRUE(strategy.overridesSelection());

    Participant<int> p1(1, "Alice", {"dev"});
    Participant<int> p2(2, "Bob", {"ops"});
    Participant<int> p3(3, "Carol", {"dev"});

    std::vector<Participant<int>*> candidates;
    candidates.push_back(&p1);
    candidates.push_back(&p2);
    candidates.push_back(&p3);

    strategy.filter(candidates);
    EXPECT_EQ(candidates.size(), 3u);

    strategy.select(candidates, 2);
    EXPECT_EQ(candidates.size(), 2u);
}