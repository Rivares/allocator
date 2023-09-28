#pragma once

#include <functional>
#include <charconv>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <limits>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>


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




//1. Аллокатор должен освобождать всю память самостоятельно.
//2. Аллокатор работает с фиксированным количеством элементов. Попытку выделить большее число
//элементов считать ошибкой.

template<typename T>
class Allocator_1
{
public:

    using value_type = T;
    using pointer = T*;

    Allocator_1() {}

    template <class U>
    Allocator_1(const Allocator_1 <U>& a) noexcept
    {}

    [[nodiscard]] pointer allocate(std::size_t cnt)
    {
        if (currCnt + cnt > capacity)
        {   throw std::bad_alloc(); }

        int cur = currCnt;
        currCnt += cnt;

        auto resultPtr = reinterpret_cast<pointer>(data) + cur;
        report(resultPtr, cnt);
        return resultPtr;
    }

    void deallocate(pointer ptr, [[maybe_unused]] std::size_t cnt) noexcept
    {
        report(ptr, cnt);
    }


private:
    static constexpr int capacity = 11 * sizeof(T);
    static int currCnt;
    static uint8_t data[capacity];

    void report(pointer ptr, std::size_t cnt, bool action = true) const
    {
        std::cout << ((action) ? "Alloc:\t\t" : "Dealloc:\t") << std::hex << std::showbase
                  << reinterpret_cast<void*>(ptr)  << std::dec << " | "
                  << sizeof(T) * cnt << "\n";
    }

};


template <typename T>
int Allocator_1<T>::currCnt = 0;

template <typename T>
uint8_t Allocator_1<T>::data[capacity];

template<typename T, typename U>
constexpr bool operator == (const Allocator_1<T>&, const Allocator_1<U>&) noexcept
{ return true; }


template<typename T, typename U>
constexpr bool operator != (const Allocator_1<T>&, const Allocator_1<U>&) noexcept
{ return false; }



//___________________________________________________________________________________________



//Опционально:
//1. Реализовать расширяемость аллокатора. При попытке выделить число элементов,
//которое превышает текущее зарезервированное количество, аллокатор расширяет
//зарезервированную память.
//2. Реализовать поэлементное освобождение.

template<typename T>
class Allocator_2 // stateless
{
public:

    using value_type = T;
    using pointer = T*;


    Allocator_2() = default;

    Allocator_2(const Allocator_2& other) noexcept
    {}


    [[nodiscard]] pointer allocate(std::size_t cnt)
    {
        if (cnt > (std::numeric_limits<std::size_t>::max() / sizeof(T)))
        {   throw std::bad_array_new_length();  }

        if ((currCnt + 1) < maxSize)
        {
            if (auto ptr = static_cast<pointer>( std::malloc(cnt * sizeof(T)) )) // ::operator new(cnt * sizeof(T))
            {
                report(ptr, cnt);
                ++currCnt;
                return ptr;
            }
        }
        else
        {
            setLimitElems(maxSize * 2);

            if (auto ptr = static_cast<pointer>( std::malloc(cnt * sizeof(T)) )) // ::operator new(cnt * sizeof(T))
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

    // For non-STL contatiners
    template <typename Up, typename... Args>
    void construct(Up* p, Args&& ... args)
    {
//        std::cout << "construct\t" << *p << "\t";

        ::new ((void*)p) Up(std::forward<Args>(args)...);

//        std::cout << *p << "\n";
    }

    void destroy(pointer p)
    {
//        std::cout << "destroy\n";
        p->~T();
    }


    void setLimitElems(std::size_t limit)
    {   maxSize = limit;    }


    template<typename U>
    struct rebind
    {
        using other = Allocator_2<U>;
    };

private:
    static std::size_t maxSize; // 10 + 1
    static std::size_t currCnt;

    void report(pointer ptr, std::size_t cnt, bool action = true) const
    {
        std::cout << ((action) ? "Alloc:\t\t" : "Dealloc:\t") << std::hex << std::showbase
                  << reinterpret_cast<void*>(ptr)  << std::dec << " | "
                  << sizeof(T) * cnt << "\n";
    }

};

template <typename T>
std::size_t Allocator_2<T>::maxSize = 11;

template <typename T>
std::size_t Allocator_2<T>::currCnt = 0;

template<typename T, typename U>
constexpr bool operator == (const Allocator_2<T>&, const Allocator_2<U>&) noexcept
{ return true; }


template<typename T, typename U>
constexpr bool operator != (const Allocator_2<T>&, const Allocator_2<U>&) noexcept
{ return false; }



//___________________________________________________________________________________________



//3. Реализовать свой контейнер, который по аналогии с контейнерами stl параметризуется
//аллокатором. Контейнер должен иметь две возможности - добавить новый элемент и обойти
//контейнер в одном направлении.



template<typename T,
         int capacity,
         typename Comp,
         typename Allocator_2 = std::allocator<T> >
class better_container
{
public:

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    better_container() noexcept
    {
        typename Allocator_2::template rebind<T>::other allocBlock;
        allocBlock.setLimitElems(capacity);
        m_data = allocBlock.allocate(capacity);

//        std::cout << "Alloc Data B: " << &(*data) << " Alloc Data B: " << &(*(data + capacity)) << "\n";
    }
    better_container(const better_container& other) noexcept
    {}
    ~better_container() noexcept
    {
    }

    bool set(size_t idx, const_reference item)
    {
        if (idx >= capacity)
        {
            throw std::out_of_range("better_container<T>::set() : index is out of range");
            return false;
        }

        typename Allocator_2::template rebind<T>::other allocBlock;
        allocBlock.construct(m_data + idx, item);

//        std::cout << "Address: " << &(*(data + idx)) << " Data: " << *(data + idx) << "\n";

        ++m_size;
        if (m_size == capacity)
        {   return false;   }

        return true;
    }

    void printAllElems() const
    {
        for (std::size_t i = 0; i < m_size; ++i)
        {
            std::cout << *(m_data + i) << "\n";
        }
    }

    //4. Реализовать совместимость с контейнерами stl – итераторы, вспомогательные
    //методы size, empty и т.д.

    std::size_t size() const
    {
        return m_size;
    }

    bool empty() const
    {
        return (m_size > 0)? false : true;
    }

    //iterators
    //...

private:
    std::size_t m_size = 0;
    pointer m_data = nullptr;
};



