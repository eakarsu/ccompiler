int printf(const char *fmt, ...);
// EXPECT: Modulo by powers of 2:\n  123 % 2 = 1 (mask: 1)\n  123 % 4 = 3 (mask: 3)\n  123 % 8 = 3 (mask: 3)\n  123 % 16 = 11 (mask: 11)\n  123 % 32 = 27 (mask: 27)\n  123 % 64 = 59 (mask: 59)\n  123 % 128 = 123 (mask: 123)\nmask mod matches: 1\nring buffer (size 8):\n  index  0 -> slot 0\n  index  1 -> slot 1\n  index  2 -> slot 2\n  index  3 -> slot 3\n  index  4 -> slot 4\n  index  5 -> slot 5\n  index  6 -> slot 6\n  index  7 -> slot 7\n  index  8 -> slot 0\n  index  9 -> slot 1\n  index 10 -> slot 2\n  index 11 -> slot 3\n  index 12 -> slot 4\n  index 13 -> slot 5\n  index 14 -> slot 6\n  index 15 -> slot 7\nhash buckets (table size 16):\n  key 42 -> bucket 6\n  key 100 -> bucket 12\n  key 255 -> bucket 1\n  key 1024 -> bucket 0\n  key 7 -> bucket 7\n  key 999 -> bucket 7\nalignment checks:\n  0: align4=1 align8=1 align16=1\n  4: align4=1 align8=0 align16=0\n  7: align4=0 align8=0 align16=0\n  16: align4=1 align8=1 align16=1\n  100: align4=1 align8=0 align16=0\neven/odd: 0101010101
// Test: modulo with masks

int is_power_of_2(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// Fast modulo for power-of-2 divisors
int fast_mod(int val, int divisor) {
    return val & (divisor - 1);
}

int main(void) {
    // Modulo by powers of 2 using mask
    int i;
    printf("Modulo by powers of 2:\n");
    int val = 123;
    printf("  %d %% 2 = %d (mask: %d)\n", val, val % 2, fast_mod(val, 2));
    printf("  %d %% 4 = %d (mask: %d)\n", val, val % 4, fast_mod(val, 4));
    printf("  %d %% 8 = %d (mask: %d)\n", val, val % 8, fast_mod(val, 8));
    printf("  %d %% 16 = %d (mask: %d)\n", val, val % 16, fast_mod(val, 16));
    printf("  %d %% 32 = %d (mask: %d)\n", val, val % 32, fast_mod(val, 32));
    printf("  %d %% 64 = %d (mask: %d)\n", val, val % 64, fast_mod(val, 64));
    printf("  %d %% 128 = %d (mask: %d)\n", val, val % 128, fast_mod(val, 128));

    // Verify match for many values
    int all_match = 1;
    int power;
    for (power = 1; power <= 1024; power = power << 1) {
        for (i = 0; i < 100; i++) {
            if ((i % power) != fast_mod(i, power)) {
                all_match = 0;
            }
        }
    }
    printf("mask mod matches: %d\n", all_match);

    // Ring buffer index wrapping
    int buf_size = 8;  // must be power of 2
    int mask = buf_size - 1;
    printf("ring buffer (size %d):\n", buf_size);
    for (i = 0; i < 16; i++) {
        printf("  index %2d -> slot %d\n", i, i & mask);
    }

    // Hash table bucket selection
    int table_size = 16;
    int hash_mask = table_size - 1;
    int keys[6];
    keys[0] = 42; keys[1] = 100; keys[2] = 255;
    keys[3] = 1024; keys[4] = 7; keys[5] = 999;
    printf("hash buckets (table size %d):\n", table_size);
    for (i = 0; i < 6; i++) {
        int hash = keys[i] * 2654435761;  // Knuth's multiplicative hash
        if (hash < 0) hash = -hash;
        int bucket = hash & hash_mask;
        printf("  key %d -> bucket %d\n", keys[i], bucket);
    }

    // Alignment check using modulo
    printf("alignment checks:\n");
    int addrs[5];
    addrs[0] = 0; addrs[1] = 4; addrs[2] = 7; addrs[3] = 16; addrs[4] = 100;
    for (i = 0; i < 5; i++) {
        printf("  %d: align4=%d align8=%d align16=%d\n",
               addrs[i],
               (addrs[i] & 3) == 0,
               (addrs[i] & 7) == 0,
               (addrs[i] & 15) == 0);
    }

    // Even/odd using mod 2 mask
    printf("even/odd: ");
    for (i = 0; i < 10; i++) {
        printf("%d", i & 1);
    }
    printf("\n");

    return 0;
}
