#define GOOGLE_TEST_MODULE test_main_google

#include "test_main_google.hpp"

#include <gtest/gtest.h>



TEST(TestGroupName, Subtest_1)
{
    uint8_t cnt = 10;

    std::map<int, int, std::less<int>, Allocator_1<std::pair<const int, int> >> test_1;

    while (cnt-- > 0)
    {
        test_1[cnt] = factorial(cnt);
    }
    ASSERT_TRUE(test_1.size() == 10);
}

TEST(TestGroupName, Subtest_2)
{
    uint8_t cnt = 15;

    std::map<int, int, std::less<int>, Allocator_2<std::pair<const int, int> >> test_2;

    while (cnt-- > 0)
    {
        test_2[cnt] = factorial(cnt);
    }
    ASSERT_TRUE(test_2.size() == 15);
}

TEST(TestGroupName, Subtest_3)
{
    size_t idx = 0;

    better_container<int, 10, std::less<int>, Allocator_2<int>> test_3;

    while(test_3.set(idx++, factorial(idx)))
    {}
    ASSERT_TRUE(test_3.size() == 10);
}
