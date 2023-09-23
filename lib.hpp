#pragma once

#include <functional>
#include <charconv>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <limits>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>


//1. Аллокатор должен освобождать всю память самостоятельно.
//2. Аллокатор работает с фиксированным количеством элементов. Попытку выделить большее число
//элементов считать ошибкой.

//Опционально:
//1. Реализовать расширяемость аллокатора. При попытке выделить число элементов,
//которое превышает текущее зарезервированное количество, аллокатор расширяет
//зарезервированную память.

//2. Реализовать поэлементное освобождение.

//3. Реализовать свой контейнер, который по аналогии с контейнерами stl параметризуется
//аллокатором. Контейнер должен иметь две возможности - добавить новый элемент и обойти
//контейнер в одном направлении.

//4. Реализовать совместимость с контейнерами stl – итераторы, вспомогательные
//методы size, empty и т.д.



template<typename T>
class Allocator // stateless
{
public:

    using value_type = T;
    using pointer = T*;

    static int currCnt;
    static constexpr std::size_t poolSize = 11; // 10 + 1

    Allocator() = default;

    Allocator(const Allocator& other) noexcept
    {}

//    template<class U>
//    constexpr Allocator(const Allocator <U>&) noexcept {}

    [[nodiscard]] pointer allocate(std::size_t cnt)
    {
        if (cnt > (std::numeric_limits<std::size_t>::max() / sizeof(T)))
        {   throw std::bad_array_new_length();  }

        if ((currCnt + 1) < poolSize)
        {
            if (auto ptr = static_cast<T*>( std::malloc(cnt * sizeof(T)) )) // ::operator new(cnt * sizeof(T))
            {
                report(ptr, cnt);
                ++currCnt;
                return ptr;
            }
        }

        throw std::bad_alloc();
    }

    void deallocate(pointer ptr, [[maybe_unused]] std::size_t cnt) noexcept
    {
        report(ptr, cnt, false);
        --currCnt;
        std::free(ptr); // ::operator delete(p);
    }


private:
    void report(pointer ptr, std::size_t cnt, bool action = true) const
    {
        std::cout << ((action) ? "Alloc:\t\t" : "Dealloc:\t") << std::hex << std::showbase
                  << reinterpret_cast<void*>(ptr)  << std::dec << " | "
                  << sizeof(T) * cnt << "\n";
    }

};

template <typename T>
int Allocator<T>::currCnt = 0;

template<typename T, typename U>
constexpr bool operator == (const Allocator<T>&, const Allocator<U>&) noexcept
{ return true; }


template<typename T, typename U>
constexpr bool operator != (const Allocator<T>&, const Allocator<U>&) noexcept
{ return false; }




