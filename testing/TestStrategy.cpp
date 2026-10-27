#include <gtest/gtest.h>
#include "Participant.h"
#include "RoleAssignment.h"
#include "LeastLoadedAssignment.h"
#include <vector>

using namespace tividy;

TEST(StrategyPatternTest, RoleAssignmentSelectsMatchingRole) {
    Participant<std::string> dev("p-1", "Alice", "Developer");
    Participant<std::string> manager("p-2", "Bob", "Manager");

    std::vector<Participant<std::string>*> candidates = {&manager, &dev};
    RoleAssignment<std::string> strategy("Developer");

    Participant<std::string>* assigned = strategy.assign(candidates);
    ASSERT_NE(assigned, nullptr);
    EXPECT_EQ(assigned->getId(), "p-1");
    EXPECT_EQ(assigned->getRole(), "Developer");
}

TEST(StrategyPatternTest, LeastLoadedAssignmentPicksLowestWorkload) {
    Participant<std::string> p1("p-1", "Alice", "Developer");
    Participant<std::string> p2("p-2", "Bob", "Developer");

    p1.setWorkload(5);
    p2.setWorkload(2);

    std::vector<Participant<std::string>*> candidates = {&p1, &p2};
    LeastLoadedAssignment<std::string> strategy;

    Participant<std::string>* assigned = strategy.assign(candidates);
    ASSERT_NE(assigned, nullptr);
    EXPECT_EQ(assigned->getId(), "p-2");
}