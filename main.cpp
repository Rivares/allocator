#include "lib.hpp"




int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    uint8_t cnt = 10;
    std::cout << "std::allocator\n";
    //    - создание экземпляра std::map<int, int>
    std::map<int, int> test_0;

    //    - заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    while (cnt-- > 0)
    {
        test_0[cnt] = factorial(cnt);
    }

    //    - вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
    for (const auto& item : test_0)
    {   std::cout << item.first << " " << item.second << "\n";  }



    std::cout << "\n\nAllocator_1\n";
    //    - создание экземпляра std::map<int, int>
    std::map<int, int, std::less<int>, Allocator_1<std::pair<const int, int> >> test_1;

    //    - заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    cnt = 10;
    while (cnt-- > 0)
    {
        test_1[cnt] = factorial(cnt);
    }

    //    - вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
    for (const auto& item : test_1)
    {   std::cout << item.first << " " << item.second << "\n";  }




    std::cout << "\n\nAllocator_2\n";
    //    - создание экземпляра std::map<int, int>
    std::map<int, int, std::less<int>, Allocator_2<std::pair<const int, int> >> test_2;

    //    - заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    cnt = 15;
    while (cnt-- > 0)
    {
        test_1[cnt] = factorial(cnt);
    }

    //    - вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
    for (const auto& item : test_1)
    {   std::cout << item.first << " " << item.second << "\n";  }


    std::cout << "\n\nbetter_container<Allocator_2>\n";
    //    - создание экземпляра своего контейнера для хранения значений типа int
    better_container<int, 10, std::less<int>, Allocator_2<int>> test_3;

    //    - заполнение 10 элементами от 0 до 9
    size_t idx = 0;
    while(test_3.set(idx++, factorial(idx)))
    {}

    //    - вывод на экран всех значений хранящихся в контейнере
    test_3.printAllElems();



    return 0;
}


