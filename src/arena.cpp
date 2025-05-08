#include "arena.h"

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

void* ArenaAllocator::alloc_bytes(size_t num_bytes, size_t alignment)
{
    size_t remaining = m_size - static_cast<size_t>(m_offset - m_buffer);
    void* ptr = static_cast<void*>(m_offset);
    void* aligned = std::align(alignment, num_bytes, ptr, remaining);
    if (!aligned) {
        throw std::bad_alloc();
    }
    m_offset = static_cast<std::byte*>(aligned) + num_bytes;
    return aligned;
}
