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



    Allocator() = default;

    Allocator(const Allocator& other) noexcept
    {}

//    template<class U>
//    constexpr Allocator(const Allocator <U>&) noexcept {}

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
        using other = Allocator<U>;
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
std::size_t Allocator<T>::maxSize = 11;

template <typename T>
std::size_t Allocator<T>::currCnt = 0;

template<typename T, typename U>
constexpr bool operator == (const Allocator<T>&, const Allocator<U>&) noexcept
{ return true; }


template<typename T, typename U>
constexpr bool operator != (const Allocator<T>&, const Allocator<U>&) noexcept
{ return false; }



//___________________________________________________________________________________________


//template<typename Allocator>
//class PoolAllocator // stateless
//{
//public:

//    using pointer = typename Allocator::value_type*;
////    using const_pointer = const typename Allocator::value_type*;


//    static pointer allocate(Allocator& alloc, std::size_t n)
//    {
//        return alloc.allocate(n);
//    }

//    static void deallocate(Allocator& alloc, pointer p, std::size_t n) noexcept
//    {
//        alloc.deallocate(p, n);
//    }

////    template <typename Tp, typename... Args, typename = std::enable_if<has_construct<Allocator, Tp*, Args...>::value> >
////    static void construct(Allocator& alloc, Tp* p, Args&&... args)
////    {
////        alloc.construct(p, std::forward<Args>(args)...);
////    }

////    template <typename Tp, typename... Args, typename = void, typename = std::enable_if<!has_construct<Allocator, Tp*, Args...>::value> >
////    static void construct(Allocator&, Tp* p, Args&&... args)
////    {
////        ::new ((void*)p) Tp(std::forward<Args>(args)...);
////    }

//    template<typename U>
//    struct rebind
//    {
//        using other = PoolAllocator<U>;
//    };

//    void setLimitElems(std::size_t limit)
//    {   maxSize = limit;    }

//private:
//    static std::size_t maxSize; // 10 + 1
//    Allocator alloc;
//};

//template<typename Allocator>
//std::size_t PoolAllocator<Allocator>::maxSize = 11;



template<typename T,
         int capacity,
         typename Comp,
         typename Allocator = std::allocator<T> >
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
        typename Allocator::template rebind<T>::other allocBlock;
        allocBlock.setLimitElems(capacity);
        data = allocBlock.allocate(capacity);

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

        typename Allocator::template rebind<T>::other allocBlock;
        allocBlock.construct(data + idx, item);

//        std::cout << "Address: " << &(*(data + idx)) << " Data: " << *(data + idx) << "\n";

        ++size;
        if (size == capacity)
        {   return false;   }

        return true;
    }

    void printAllElems()
    {
        for (std::size_t i = 0; i < size; ++i)
        {
            std::cout << *data++ << "\n";
        }
    }


private:
    std::size_t size = 0;
    pointer data = nullptr;
};



