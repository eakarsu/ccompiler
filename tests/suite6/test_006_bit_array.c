int printf(const char *fmt, ...);

void bit_set(int arr[], int idx) {
    int word = idx / 32;
    int bit = idx % 32;
    arr[word] = arr[word] | (1 << bit);
}

void bit_clear(int arr[], int idx) {
    int word = idx / 32;
    int bit = idx % 32;
    arr[word] = arr[word] & ~(1 << bit);
}

int bit_get(int arr[], int idx) {
    int word = idx / 32;
    int bit = idx % 32;
    return (arr[word] >> bit) & 1;
}

void bit_toggle(int arr[], int idx) {
    int word = idx / 32;
    int bit = idx % 32;
    arr[word] = arr[word] ^ (1 << bit);
}

int popcount_word(int x) {
    int count = 0;
    while (x) {
        count = count + (x & 1);
        x = (x >> 1) & 0x7FFFFFFF;
    }
    return count;
}

int bit_count_set(int arr[], int words) {
    int count = 0;
    int i = 0;
    while (i < words) {
        count = count + popcount_word(arr[i]);
        i = i + 1;
    }
    return count;
}

void bit_and(int result[], int a[], int b[], int words) {
    int i = 0;
    while (i < words) {
        result[i] = a[i] & b[i];
        i = i + 1;
    }
}

void bit_or(int result[], int a[], int b[], int words) {
    int i = 0;
    while (i < words) {
        result[i] = a[i] | b[i];
        i = i + 1;
    }
}

void bit_xor(int result[], int a[], int b[], int words) {
    int i = 0;
    while (i < words) {
        result[i] = a[i] ^ b[i];
        i = i + 1;
    }
}

void bit_not(int result[], int a[], int words) {
    int i = 0;
    while (i < words) {
        result[i] = ~a[i];
        i = i + 1;
    }
}

int bit_find_first_set(int arr[], int words) {
    int i = 0;
    while (i < words) {
        if (arr[i] != 0) {
            int bit = 0;
            int w = arr[i];
            while ((w & 1) == 0) {
                bit = bit + 1;
                w = (w >> 1) & 0x7FFFFFFF;
            }
            return i * 32 + bit;
        }
        i = i + 1;
    }
    return -1;
}

int bit_find_first_clear(int arr[], int words) {
    int i = 0;
    while (i < words) {
        if (arr[i] != -1) {
            int bit = 0;
            int w = arr[i];
            while (w & 1) {
                bit = bit + 1;
                w = (w >> 1) & 0x7FFFFFFF;
            }
            return i * 32 + bit;
        }
        i = i + 1;
    }
    return -1;
}

void bit_set_range(int arr[], int lo, int hi) {
    int i = lo;
    while (i <= hi) {
        bit_set(arr, i);
        i = i + 1;
    }
}

int bit_all_zero(int arr[], int words) {
    int i = 0;
    while (i < words) {
        if (arr[i] != 0) return 0;
        i = i + 1;
    }
    return 1;
}

int main(void) {
    int bits[4];
    bits[0] = 0; bits[1] = 0; bits[2] = 0; bits[3] = 0;

    // EXPECT: all_zero = 1
    printf("all_zero = %d\n", bit_all_zero(bits, 4));

    bit_set(bits, 0);
    bit_set(bits, 5);
    bit_set(bits, 31);
    bit_set(bits, 32);
    bit_set(bits, 63);

    // EXPECT: bit[0] = 1
    printf("bit[0] = %d\n", bit_get(bits, 0));
    // EXPECT: bit[5] = 1
    printf("bit[5] = %d\n", bit_get(bits, 5));
    // EXPECT: bit[31] = 1
    printf("bit[31] = %d\n", bit_get(bits, 31));
    // EXPECT: bit[32] = 1
    printf("bit[32] = %d\n", bit_get(bits, 32));
    // EXPECT: bit[63] = 1
    printf("bit[63] = %d\n", bit_get(bits, 63));
    // EXPECT: bit[1] = 0
    printf("bit[1] = %d\n", bit_get(bits, 1));

    // EXPECT: count = 5
    printf("count = %d\n", bit_count_set(bits, 4));

    bit_clear(bits, 5);
    // EXPECT: after clear bit[5] = 0
    printf("after clear bit[5] = %d\n", bit_get(bits, 5));
    // EXPECT: count = 4
    printf("count = %d\n", bit_count_set(bits, 4));

    bit_toggle(bits, 10);
    // EXPECT: after toggle bit[10] = 1
    printf("after toggle bit[10] = %d\n", bit_get(bits, 10));
    bit_toggle(bits, 10);
    // EXPECT: after toggle2 bit[10] = 0
    printf("after toggle2 bit[10] = %d\n", bit_get(bits, 10));

    // EXPECT: first_set = 0
    printf("first_set = %d\n", bit_find_first_set(bits, 4));

    // EXPECT: all_zero = 0
    printf("all_zero = %d\n", bit_all_zero(bits, 4));

    int a[2]; a[0] = 0xFF; a[1] = 0;
    int b[2]; b[0] = 0x0F; b[1] = 0xFF;
    int r[2]; r[0] = 0; r[1] = 0;

    bit_and(r, a, b, 2);
    // EXPECT: and[0] = 15
    printf("and[0] = %d\n", r[0]);
    // EXPECT: and[1] = 0
    printf("and[1] = %d\n", r[1]);

    bit_or(r, a, b, 2);
    // EXPECT: or[0] = 255
    printf("or[0] = %d\n", r[0]);
    // EXPECT: or[1] = 255
    printf("or[1] = %d\n", r[1]);

    bit_xor(r, a, b, 2);
    // EXPECT: xor[0] = 240
    printf("xor[0] = %d\n", r[0]);

    int c[2]; c[0] = 0; c[1] = 0;
    bit_set_range(c, 4, 11);
    // EXPECT: range count = 8
    printf("range count = %d\n", bit_count_set(c, 2));
    // EXPECT: range bit[3] = 0
    printf("range bit[3] = %d\n", bit_get(c, 3));
    // EXPECT: range bit[4] = 1
    printf("range bit[4] = %d\n", bit_get(c, 4));
    // EXPECT: range bit[11] = 1
    printf("range bit[11] = %d\n", bit_get(c, 11));
    // EXPECT: range bit[12] = 0
    printf("range bit[12] = %d\n", bit_get(c, 12));

    return 0;
}
