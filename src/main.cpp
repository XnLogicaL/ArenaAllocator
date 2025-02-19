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

    Example *struct_memory = allocator.emplace<Example>();
    struct_memory->one_byte = 0xDE;
    struct_memory->integer = 0xBABE;

    return 0;
}
