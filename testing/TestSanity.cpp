#include <gtest/gtest.h>

TEST(SanityTest, GoogleTestActive) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
}