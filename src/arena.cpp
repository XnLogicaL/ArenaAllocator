#include "arena.h"
#include <memory>

ArenaAllocator &ArenaAllocator::operator=(ArenaAllocator &&other) noexcept
{
    std::swap(m_size, other.m_size);
    std::swap(m_buffer, other.m_buffer);
    std::swap(m_offset, other.m_offset);
    std::swap(m_destructor_map, other.m_destructor_map);
    return *this;
}

ArenaAllocator::~ArenaAllocator()
{
    for (const auto &[ptr, destructor] : m_destructor_map) {
        destructor(ptr);
    }

    delete[] m_buffer;
}

template<typename T>
T *ArenaAllocator::alloc()
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
T *ArenaAllocator::emplace(Args &&...args)
{
    const auto allocated_memory = alloc<T>();
    T *obj = new (allocated_memory) T{std::forward<Args>(args)...};
    register_destructor<T>(obj);
    return obj;
}

template<typename T>
void ArenaAllocator::register_destructor(T *ptr)
{
    void *void_ptr = reinterpret_cast<void *>(ptr);
    m_destructor_map[void_ptr] = [](void *obj_ptr) {
        T *t_ptr = reinterpret_cast<T *>(obj_ptr);
        t_ptr->~T();
    };
}
