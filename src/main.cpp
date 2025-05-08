#include "arena.h"

struct Example {
    char one_byte;
    int integer;
};

int main(int, char **)
{
    ArenaAllocator allocator(8 * 1024);

    int *int_memory = allocator.alloc<int>();
    *int_memory = 6926;

    char *string = allocator.alloc_bytes(sizeof(char) * 14);
    std::memcpy(string, "Hello, world!\0", 14);

    Example *struct_memory = allocator.emplace<Example>();
    struct_memory->one_byte = 0xDE;
    struct_memory->integer = 0xBABE;

    return 0;
}
