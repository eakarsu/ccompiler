int printf(const char *fmt, ...);
// EXPECT: Alloc 4 bytes at offset 0\nRead int: 42\nAlloc 16 bytes at offset 8\nRead string: hello\nAlloc 10 bytes at offset 24\nBytes: ABCDEFGHIJ\nAllocations: 3\nTotal allocated: 40\nUsed: 40/256\nAfter reset, offset: 0\nAlloc 200 at offset 0\nTry alloc 100: -1
// Simple bump allocator simulation

struct BumpAllocator {
    char memory[256];
    int offset;
    int capacity;
    int alloc_count;
    int total_allocated;
};

void bump_init(struct BumpAllocator *a) {
    int i;
    for (i = 0; i < 256; i++) a->memory[i] = 0;
    a->offset = 0;
    a->capacity = 256;
    a->alloc_count = 0;
    a->total_allocated = 0;
}

int bump_alloc(struct BumpAllocator *a, int size) {
    int aligned;
    int result;
    aligned = (size + 7) & ~7;
    if (a->offset + aligned > a->capacity) return -1;
    result = a->offset;
    a->offset += aligned;
    a->alloc_count++;
    a->total_allocated += aligned;
    return result;
}

void bump_write_int(struct BumpAllocator *a, int offset, int value) {
    a->memory[offset] = value & 0xFF;
    a->memory[offset + 1] = (value >> 8) & 0xFF;
    a->memory[offset + 2] = (value >> 16) & 0xFF;
    a->memory[offset + 3] = (value >> 24) & 0xFF;
}

int bump_read_int(struct BumpAllocator *a, int offset) {
    int v;
    v = (a->memory[offset] & 0xFF);
    v = v | ((a->memory[offset + 1] & 0xFF) << 8);
    v = v | ((a->memory[offset + 2] & 0xFF) << 16);
    v = v | ((a->memory[offset + 3] & 0xFF) << 24);
    return v;
}

void bump_write_str(struct BumpAllocator *a, int offset, char *s) {
    int i = 0;
    while (s[i] != 0) {
        a->memory[offset + i] = s[i];
        i++;
    }
    a->memory[offset + i] = 0;
}

void bump_read_str(struct BumpAllocator *a, int offset, char *buf) {
    int i = 0;
    while (a->memory[offset + i] != 0) {
        buf[i] = a->memory[offset + i];
        i++;
    }
    buf[i] = 0;
}

void bump_reset(struct BumpAllocator *a) {
    a->offset = 0;
    a->alloc_count = 0;
    a->total_allocated = 0;
}

int main(void) {
    struct BumpAllocator alloc;
    int p1;
    int p2;
    int p3;
    int p4;
    int i;
    char buf[32];

    bump_init(&alloc);

    p1 = bump_alloc(&alloc, 4);
    printf("Alloc 4 bytes at offset %d\n", p1);
    bump_write_int(&alloc, p1, 42);
    printf("Read int: %d\n", bump_read_int(&alloc, p1));

    p2 = bump_alloc(&alloc, 16);
    printf("Alloc 16 bytes at offset %d\n", p2);
    bump_write_str(&alloc, p2, "hello");
    bump_read_str(&alloc, p2, buf);
    printf("Read string: %s\n", buf);

    p3 = bump_alloc(&alloc, 10);
    printf("Alloc 10 bytes at offset %d\n", p3);
    for (i = 0; i < 10; i++) {
        alloc.memory[p3 + i] = 'A' + i;
    }
    printf("Bytes: ");
    for (i = 0; i < 10; i++) {
        printf("%c", alloc.memory[p3 + i]);
    }
    printf("\n");

    printf("Allocations: %d\n", alloc.alloc_count);
    printf("Total allocated: %d\n", alloc.total_allocated);
    printf("Used: %d/%d\n", alloc.offset, alloc.capacity);

    bump_reset(&alloc);
    printf("After reset, offset: %d\n", alloc.offset);

    p4 = bump_alloc(&alloc, 200);
    printf("Alloc 200 at offset %d\n", p4);
    printf("Try alloc 100: %d\n", bump_alloc(&alloc, 100));

    return 0;
}
