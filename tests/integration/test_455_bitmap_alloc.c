int printf(const char *fmt, ...);
// EXPECT: Free blocks: 128\nAllocated: 0, 1, 2\nFree blocks: 125\nAfter freeing 1, free: 126\nRealloc: 1\nContiguous 5 at: 3\nFree blocks: 120\nBit 0: 1\nBit 10: 0\nAfter free contiguous, free: 125\nDone

// Bitmap-based memory allocator simulation

struct BitmapAlloc {
    int bitmap[4];
    int block_size;
    int total_blocks;
};

void ba_init(struct BitmapAlloc *ba) {
    ba->block_size = 32;
    ba->total_blocks = 128;
    int i = 0;
    while (i < 4) {
        ba->bitmap[i] = 0;
        i = i + 1;
    }
}

int ba_get_bit(struct BitmapAlloc *ba, int block) {
    int word = block / 32;
    int bit = block % 32;
    return (ba->bitmap[word] >> bit) & 1;
}

void ba_set_bit(struct BitmapAlloc *ba, int block) {
    int word = block / 32;
    int bit = block % 32;
    ba->bitmap[word] = ba->bitmap[word] | (1 << bit);
}

void ba_clear_bit(struct BitmapAlloc *ba, int block) {
    int word = block / 32;
    int bit = block % 32;
    ba->bitmap[word] = ba->bitmap[word] & ~(1 << bit);
}

int ba_alloc(struct BitmapAlloc *ba) {
    int i = 0;
    while (i < ba->total_blocks) {
        if (!ba_get_bit(ba, i)) {
            ba_set_bit(ba, i);
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int ba_alloc_contiguous(struct BitmapAlloc *ba, int count) {
    int i = 0;
    while (i <= ba->total_blocks - count) {
        int j = 0;
        int found = 1;
        while (j < count) {
            if (ba_get_bit(ba, i + j)) {
                found = 0;
                break;
            }
            j = j + 1;
        }
        if (found) {
            j = 0;
            while (j < count) {
                ba_set_bit(ba, i + j);
                j = j + 1;
            }
            return i;
        }
        i = i + 1;
    }
    return -1;
}

void ba_free(struct BitmapAlloc *ba, int block) {
    ba_clear_bit(ba, block);
}

int ba_count_free(struct BitmapAlloc *ba) {
    int count = 0;
    int i = 0;
    while (i < ba->total_blocks) {
        if (!ba_get_bit(ba, i)) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int main(void) {
    struct BitmapAlloc ba;
    ba_init(&ba);

    printf("Free blocks: %d\n", ba_count_free(&ba));

    int a = ba_alloc(&ba);
    int b = ba_alloc(&ba);
    int c = ba_alloc(&ba);
    printf("Allocated: %d, %d, %d\n", a, b, c);
    printf("Free blocks: %d\n", ba_count_free(&ba));

    ba_free(&ba, b);
    printf("After freeing %d, free: %d\n", b, ba_count_free(&ba));

    int d = ba_alloc(&ba);
    printf("Realloc: %d\n", d);

    // Contiguous allocation
    int blk = ba_alloc_contiguous(&ba, 5);
    printf("Contiguous 5 at: %d\n", blk);
    printf("Free blocks: %d\n", ba_count_free(&ba));

    // Check bits
    printf("Bit 0: %d\n", ba_get_bit(&ba, 0));
    printf("Bit 10: %d\n", ba_get_bit(&ba, 10));

    // Free contiguous
    int i = blk;
    while (i < blk + 5) {
        ba_free(&ba, i);
        i = i + 1;
    }
    printf("After free contiguous, free: %d\n", ba_count_free(&ba));

    printf("Done\n");
    return 0;
}
