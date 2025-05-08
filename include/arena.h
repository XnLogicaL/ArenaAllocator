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
#include <memory>

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

    void *alloc_bytes(size_t num_bytes, size_t alignment = alignof(std::max_align_t));

    template<typename T>
    T *alloc()
    {
        size_t remaining_num_bytes = m_size - static_cast<size_t>(m_offset - m_buffer);
        void *pointer = static_cast<void *>(m_offset);
        void *aligned = std::align(alignof(T), sizeof(T), pointer, remaining_num_bytes);
        if (aligned == nullptr) {
            throw std::bad_alloc();
        }

        m_offset = static_cast<std::byte *>(aligned) + sizeof(T);
        return static_cast<T *>(aligned);
    }

    template<typename T, typename... Args>
        requires std::is_destructible_v<T> && std::is_constructible_v<T, Args...>
    T *emplace(Args &&...args)
    {
        const auto allocated_memory = alloc<T>();
        T *obj = new (allocated_memory) T{std::forward<Args>(args)...};
        register_destructor<T>(obj);
        return obj;
    }

private:
    size_t m_size;
    std::byte *m_buffer;
    std::byte *m_offset;
    std::unordered_map<void *, Destructor> m_destructor_map;

    template<typename T>
    void register_destructor(T *ptr)
    {
        void *void_ptr = reinterpret_cast<void *>(ptr);
        m_destructor_map[void_ptr] = [](void *obj_ptr) {
            T *t_ptr = reinterpret_cast<T *>(obj_ptr);
            t_ptr->~T();
        };
    }
};

#endif
