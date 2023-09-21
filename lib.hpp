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

template<typename T>
class Allocator // stateless
{
public:

    using value_type = T;

    Allocator() = default;

    Allocator(const Allocator& other) noexcept
    {}

//    template<class U>
//    constexpr Allocator(const Allocator <U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t cnt)
    {
        if (cnt > (std::numeric_limits<std::size_t>::max() / sizeof(T)))
        {   throw std::bad_array_new_length();  }

        if (auto ptr = static_cast<T*>( std::malloc(cnt * sizeof(T)) ))
        {
            report(ptr, cnt);
            return ptr;
        }

        throw std::bad_alloc();
    }

    void deallocate(T* ptr, [[maybe_unused]] std::size_t cnt) noexcept
    {
        report(ptr, cnt, false);
        std::free(ptr);
    }

private:
    void report(T* ptr, std::size_t cnt, bool action = true) const
    {
        std::cout << ((action) ? "Alloc:\t\t" : "Dealloc:\t") << std::hex << std::showbase
                  << reinterpret_cast<void*>(ptr)  << std::dec << " | "
                  << sizeof(T) * cnt << "\n";
    }

};



template<typename T, typename U>
constexpr bool operator == (const Allocator<T>&, const Allocator<U>&) noexcept
{ return true; }


template<typename T, typename U>
constexpr bool operator != (const Allocator<T>&, const Allocator<U>&) noexcept
{ return false; }




