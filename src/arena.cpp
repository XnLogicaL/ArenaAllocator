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
