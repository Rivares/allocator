#define GOOGLE_TEST_MODULE test_main_google

#include "test_main_google.hpp"

#include <gtest/gtest.h>

#include <charconv>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <array>
#include <set>

TEST(TestGroupName, Subtest_1)
{
//    std::ifstream file("ip_filter.tsv");
//    if (file.is_open())
//    {
//        std::multiset<std::array<int, cntDigits>, std::greater<>> ip_pool;

//        std::string buff = "";
//        while (std::getline(file, buff))
//        {
//            std::array<int, cntDigits> ip_add {0, 0, 0, 0};
//            std::string_view ip_addr = buff.substr(0, buff.find_first_of('\t'));

//            std::string_view ip_part = ip_addr.substr(0, ip_addr.find_first_of('.'));
//            std::from_chars(ip_part.data(), ip_part.data() + ip_part.size(), ip_add[0]);
//            ip_addr.remove_prefix(ip_addr.find_first_of('.') + 1);

//            ip_part = ip_addr.substr(0, ip_addr.find_first_of('.'));
//            std::from_chars(ip_part.data(), ip_part.data() + ip_part.size(), ip_add[1]);
//            ip_addr.remove_prefix(ip_addr.find_first_of('.') + 1);

//            ip_part = ip_addr.substr(0, ip_addr.find_first_of('.'));
//            std::from_chars(ip_part.data(), ip_part.data() + ip_part.size(), ip_add[2]);
//            ip_addr.remove_prefix(ip_addr.find_first_of('.') + 1);

//            ip_part = ip_addr.substr(0, ip_addr.find_first_of('.'));
//            std::from_chars(ip_part.data(), ip_part.data() + ip_part.size(), ip_add[3]);

//            ip_pool.insert(ip_add);
//        }

//        // Reverse lexicographically sort
//        for (auto addr = ip_pool.begin(); addr != ip_pool.end(); ++addr)
//        {
//            ASSERT_TRUE((*addr).at(0) >= (*(++addr)).at(0));
//        }
//    }
//    else
//    {  ASSERT_TRUE(false); }
}
