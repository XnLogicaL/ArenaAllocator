/*
    An arena allocator implementation that uses RAII and object-oriented allocation.

    Author(s): @XnLogicaL
    License: MIT
*/

#ifndef _ARENA_H
#define _ARENA_H

#include <cstddef>
#include <functional>
#include <unordered_map>
#include <utility>

class ArenaAllocator {
public:
    using Destructor = std::function<void(void *)>;

    explicit ArenaAllocator(const size_t max_size)
        : m_size(max_size)
        , m_buffer(new std::byte[max_size])
        , m_offset(m_buffer)
    {
    }

    ArenaAllocator(const ArenaAllocator &) = delete;
    ArenaAllocator &operator=(const ArenaAllocator &) = delete;

    ArenaAllocator(ArenaAllocator &&other) noexcept
        : m_size(std::exchange(other.m_size, 0))
        , m_buffer(std::exchange(other.m_buffer, nullptr))
        , m_offset(std::exchange(other.m_offset, nullptr))
        , m_destructor_map(std::move(other.m_destructor_map))
    {
    }

    ArenaAllocator &operator=(ArenaAllocator &&other) noexcept;
    ~ArenaAllocator();

    template<typename T>
    T *alloc();

    template<typename T, typename... Args>
        requires std::is_destructible_v<T> && std::is_constructible_v<T, Args...>
    T *emplace(Args &&...args);

private:
    size_t m_size;
    std::byte *m_buffer;
    std::byte *m_offset;
    std::unordered_map<void *, Destructor> m_destructor_map;

    template<typename T>
    void register_destructor(T *ptr);
};

#endif
