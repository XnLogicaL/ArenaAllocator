# ArenaAllocator
An RAII/OOP-based arena allocator implementation.

This implementation prioritizes object-resource management, while retaining traditional allocation methods. It automatically cleans up emplaced objects when the allocator is destroyed.