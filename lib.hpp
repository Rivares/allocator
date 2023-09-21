#pragma once

#include <functional>
#include <charconv>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <array>
#include <set>

static constexpr size_t cntDigits = 4;

//struct ModeComp
//{
//    bool operator()(const std::array<int, cntDigits> lhs, const std::array<int, cntDigits> rhs) const
//    {
//        if (lhs.at(0) == rhs.at(0))
//        {
//            if (lhs.at(1) == rhs.at(1))
//            {
//                if (lhs.at(2) == rhs.at(2))
//                {
//                    if (lhs.at(3) == rhs.at(3))
//                    {
//                        return true;
//                    }
//                    else if (lhs.at(3) > rhs.at(3))
//                    {   return true;    }
//                    else
//                    {   return false;   }
//                }
//                else if (lhs.at(2) > rhs.at(2))
//                {   return true;    }
//                else
//                {   return false;   }
//            }
//            else if (lhs.at(1) > rhs.at(1))
//            {   return true;    }
//            else
//            {   return false;   }
//        }
//        else if (lhs.at(0) > rhs.at(0))
//        {   return true;    }
//        else
//        {   return false;   }
//    }
//};


void outWithFilter(std::multiset<std::array<int, cntDigits>, std::greater<>> ip_pool,
                   std::function<bool(int, int, int, int)> filter)
{
    for (const auto& addr : ip_pool)
    {
        if (filter(addr.at(0), addr.at(1), addr.at(2), addr.at(3)))
        {
            std::cout << addr.at(0) << '.'
                      << addr.at(1) << '.'
                      << addr.at(2) << '.'
                      << addr.at(3) << '\n';
        }
    }
}
void pluginAutoLambda()
{
    std::ifstream file("ip_filter.tsv");
    if (file.is_open())
    {
        try
        {
            std::multiset<std::array<int, cntDigits>, std::greater<>> ip_pool;

            std::string buff = "";
            while (std::getline(file, buff))
            {
                std::array<int, cntDigits> ip_add {0, 0, 0, 0};
                std::string_view ip_addr = buff.substr(0, buff.find_first_of('\t'));

                std::string_view ip_part;
                for (auto& item : ip_add)
                {
                    ip_part = ip_addr.substr(0, ip_addr.find_first_of('.'));
                    std::from_chars(ip_part.data(), ip_part.data() + ip_part.size(), item);
                    ip_addr.remove_prefix(ip_addr.find_first_of('.') + 1);
                }

                ip_pool.insert(ip_add);
            }


            // Reverse lexicographically sort
            outWithFilter(
                            ip_pool,
                            [] (int ip_1, int ip_2, int ip_3, int ip_4)
                            {    return true;   }
                        );

            // Filter by first byte and output . ip = filter(1)
            outWithFilter(
                            ip_pool,
                            [] (int ip_1, int ip_2, int ip_3, int ip_4)
                            {    return (ip_1 != 1) ? false : true;   }
                        );

            // Filter by first and second bytes and output . ip = filter(46, 70)
            outWithFilter(
                            ip_pool,
                            [] (int ip_1, int ip_2, int ip_3, int ip_4)
                            {    return ( (ip_1 != 46) || (ip_2 != 70) ) ? false : true;   }
                        );


             // Filter by any byte and output . ip = filter_any(46)
            outWithFilter(
                            ip_pool,
                            [] (int ip_1, int ip_2, int ip_3, int ip_4)
                            {    return (
                                            (ip_1 != 46)
                                             && (ip_2 != 46)
                                             && (ip_3 != 46)
                                             && (ip_4 != 46)
                                         ) ? false : true;
                            }
                        );


        }
        catch (const std::exception& except)
        {
            std::cerr << except.what() << std::endl;
        }
    }
}

