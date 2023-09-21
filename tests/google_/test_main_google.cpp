#define GOOGLE_TEST_MODULE test_main_google

#include "test_main_google.hpp"

#include <gtest/gtest.h>


TEST(TestGroupName, Subtest_1)
{
    std::vector<int, Allocator<int>> test_0(1);
    ASSERT_TRUE(test_0.size() == 1);
}
