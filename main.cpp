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
    //    - заполнение 10 элементами от 0 до 9
    //    - создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором,
    //    ограниченным 10 элементами
    //    - заполнение 10 элементами от 0 до 9
    //    - вывод на экран всех значений, хранящихся в контейнере

    return 0;
}


