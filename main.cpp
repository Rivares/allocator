#include "lib.hpp"

[[nodiscard]] int factorial (const int& num)
{
    if (num == 0)
    {   return 1;   }

    int factorial = 1;

    for (size_t i = 1; i <= num; ++i)
    {   factorial *= i;   }

    return factorial;
}

template<int num>
struct fact
{
    static const int value = num * fact<num - 1>::value;
};

template<>
struct fact<0>
{
    static const int value = 1;
};


int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    uint8_t cnt = 10;

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




    //    - создание экземпляра std::map<int, int>
    std::map<int, int, std::less<int>, Allocator<std::pair<const int, int> >> test_1;

    //    - заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    cnt = 10;
    while (cnt-- > 0)
    {
        test_1[cnt] = factorial(cnt);
    }

    //    - вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
    for (const auto& item : test_1)
    {   std::cout << item.first << " " << item.second << "\n";  }





    //    - создание экземпляра своего контейнера для хранения значений типа int
    better_container<int, 10, std::less<int>, Allocator<int>> test_3;

    //    - заполнение 10 элементами от 0 до 9
    size_t idx = 0;
    while(test_3.set(idx++, factorial(idx)))
    {}

    //    - вывод на экран всех значений хранящихся в контейнере
    test_3.printAllElems();



    return 0;
}


