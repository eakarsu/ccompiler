int printf(const char *fmt, ...);
// EXPECT: === Bitwise Operations Tests ===
// EXPECT: and_basic: 0xFF & 0x0F=15
// EXPECT: and_zero: 0xFF & 0x00=0
// EXPECT: and_self: 0xAB & 0xAB=171
// EXPECT: or_basic: 0xF0 | 0x0F=255
// EXPECT: or_zero: 0xAB | 0x00=171
// EXPECT: or_self: 0xAB | 0xAB=171
// EXPECT: xor_basic: 0xFF ^ 0x0F=240
// EXPECT: xor_self: 0xAB ^ 0xAB=0
// EXPECT: xor_zero: 0xAB ^ 0x00=171
// EXPECT: not_zero: ~0=-1
// EXPECT: not_one: ~1=-2
// EXPECT: not_ff: ~255=-256
// EXPECT: shl_1: 1<<0=1
// EXPECT: shl_2: 1<<1=2
// EXPECT: shl_3: 1<<4=16
// EXPECT: shl_4: 1<<10=1024
// EXPECT: shl_5: 1<<31=-2147483648
// EXPECT: shr_1: 16>>1=8
// EXPECT: shr_2: 1024>>5=32
// EXPECT: shr_3: 256>>8=1
// EXPECT: shr_unsigned: 0x80000000u>>31=1
// EXPECT: mask_low4: 0xABCD & 0x000F=13
// EXPECT: mask_high4: 0xABCD >> 12=10
// EXPECT: mask_byte0: 0x12345678 & 0xFF=120
// EXPECT: mask_byte1: (0x12345678>>8) & 0xFF=86
// EXPECT: set_bit3: 0 | (1<<3)=8
// EXPECT: clear_bit3: 0xFF & ~(1<<3)=247
// EXPECT: toggle_bit3: 0xF0 ^ (1<<3)=248
// EXPECT: toggle_bit3b: 0xF8 ^ (1<<3)=240
// EXPECT: test_bit: (0xAA >> 7) & 1=1
// EXPECT: test_bit2: (0xAA >> 6) & 1=0
// EXPECT: bitcount_0: bits(0)=0
// EXPECT: bitcount_1: bits(1)=1
// EXPECT: bitcount_ff: bits(255)=8
// EXPECT: bitcount_aa: bits(170)=4
// EXPECT: swap_xor: a=20, b=10
// EXPECT: rotate_left: rotl(0xABCD0001,4)=3167748122
// EXPECT: flags_set: flags=7
// EXPECT: flags_check: has_read=1, has_write=1, has_exec=1, has_admin=0
// EXPECT: flags_clear: flags after clear write=5
// EXPECT: chain_and_or: (0xF0 & 0xFF) | 0x0F=255
// EXPECT: chain_xor: 0xAA ^ 0x55 ^ 0xFF=0
// EXPECT: double_not: ~~42=42
// EXPECT: shift_chain: (1<<8)-1=255
// and_basic: 0xFF & 0x0F=15
// and_zero: 0xFF & 0x00=0
// and_self: 0xAB & 0xAB=171
// or_basic: 0xF0 | 0x0F=255
// or_zero: 0xAB | 0x00=171
// or_self: 0xAB | 0xAB=171
// xor_basic: 0xFF ^ 0x0F=240
// xor_self: 0xAB ^ 0xAB=0
// xor_zero: 0xAB ^ 0x00=171
// not_zero: ~0=-1
// not_one: ~1=-2
// not_ff: ~255=-256
// shl_1: 1<<0=1
// shl_2: 1<<1=2
// shl_3: 1<<4=16
// shl_4: 1<<10=1024
// shl_5: 1<<31=-2147483648
// shr_1: 16>>1=8
// shr_2: 1024>>5=32
// shr_3: 256>>8=1
// shr_unsigned: 0x80000000u>>31=1
// mask_low4: 0xABCD & 0x000F=13
// mask_high4: 0xABCD >> 12=10
// mask_byte0: 0x12345678 & 0xFF=120
// mask_byte1: (0x12345678>>8) & 0xFF=86
// set_bit3: 0 | (1<<3)=8
// clear_bit3: 0xFF & ~(1<<3)=247
// toggle_bit3: 0xF0 ^ (1<<3)=248
// toggle_bit3b: 0xF8 ^ (1<<3)=240
// test_bit: (0xAA >> 7) & 1=1
// test_bit2: (0xAA >> 6) & 1=0
// bitcount_0: bits(0)=0
// bitcount_1: bits(1)=1
// bitcount_ff: bits(255)=8
// bitcount_aa: bits(170)=4
// swap_xor: a=20, b=10
// rotate_left: rotl(0xABCD0001,4)=3167748122
// flags_set: flags=7
// flags_check: has_read=1, has_write=1, has_exec=1, has_admin=0
// flags_clear: flags after clear write=5
// chain_and_or: (0xF0 & 0xFF) | 0x0F=255
// chain_xor: 0xAA ^ 0x55 ^ 0xFF=0
// double_not: ~~42=42
// shift_chain: (1<<8)-1=255

int popcount(int x) {
    int count = 0;
    while (x) {
        count = count + (x & 1);
        x = (unsigned int)x >> 1;
    }
    return count;
}

unsigned int rotl(unsigned int val, int n) {
    return (val << n) | (val >> (32 - n));
}

int main(void) {
    int a, b, r;
    unsigned int ua;

    printf("=== Bitwise Operations Tests ===\n");

    // AND operations
    printf("and_basic: 0xFF & 0x0F=%d\n", 0xFF & 0x0F);
    printf("and_zero: 0xFF & 0x00=%d\n", 0xFF & 0x00);
    printf("and_self: 0xAB & 0xAB=%d\n", 0xAB & 0xAB);

    // OR operations
    printf("or_basic: 0xF0 | 0x0F=%d\n", 0xF0 | 0x0F);
    printf("or_zero: 0xAB | 0x00=%d\n", 0xAB | 0x00);
    printf("or_self: 0xAB | 0xAB=%d\n", 0xAB | 0xAB);

    // XOR operations
    printf("xor_basic: 0xFF ^ 0x0F=%d\n", 0xFF ^ 0x0F);
    printf("xor_self: 0xAB ^ 0xAB=%d\n", 0xAB ^ 0xAB);
    printf("xor_zero: 0xAB ^ 0x00=%d\n", 0xAB ^ 0x00);

    // NOT operations
    printf("not_zero: ~0=%d\n", ~0);
    printf("not_one: ~1=%d\n", ~1);
    printf("not_ff: ~255=%d\n", ~255);

    // Left shifts
    printf("shl_1: 1<<0=%d\n", 1 << 0);
    printf("shl_2: 1<<1=%d\n", 1 << 1);
    printf("shl_3: 1<<4=%d\n", 1 << 4);
    printf("shl_4: 1<<10=%d\n", 1 << 10);
    printf("shl_5: 1<<31=%d\n", 1 << 31);

    // Right shifts
    printf("shr_1: 16>>1=%d\n", 16 >> 1);
    printf("shr_2: 1024>>5=%d\n", 1024 >> 5);
    printf("shr_3: 256>>8=%d\n", 256 >> 8);
    ua = 0x80000000u;
    printf("shr_unsigned: 0x80000000u>>31=%u\n", ua >> 31);

    // Masking
    a = 0xABCD;
    printf("mask_low4: 0xABCD & 0x000F=%d\n", a & 0x000F);
    printf("mask_high4: 0xABCD >> 12=%d\n", a >> 12);
    a = 0x12345678;
    printf("mask_byte0: 0x12345678 & 0xFF=%d\n", a & 0xFF);
    printf("mask_byte1: (0x12345678>>8) & 0xFF=%d\n", (a >> 8) & 0xFF);

    // Set, clear, toggle bits
    a = 0;
    r = a | (1 << 3);
    printf("set_bit3: 0 | (1<<3)=%d\n", r);
    a = 0xFF;
    r = a & ~(1 << 3);
    printf("clear_bit3: 0xFF & ~(1<<3)=%d\n", r);
    a = 0xF0;
    r = a ^ (1 << 3);
    printf("toggle_bit3: 0xF0 ^ (1<<3)=%d\n", r);
    a = 0xF8;
    r = a ^ (1 << 3);
    printf("toggle_bit3b: 0xF8 ^ (1<<3)=%d\n", r);

    // Test individual bits
    a = 0xAA;
    printf("test_bit: (0xAA >> 7) & 1=%d\n", (a >> 7) & 1);
    printf("test_bit2: (0xAA >> 6) & 1=%d\n", (a >> 6) & 1);

    // Popcount
    printf("bitcount_0: bits(0)=%d\n", popcount(0));
    printf("bitcount_1: bits(1)=%d\n", popcount(1));
    printf("bitcount_ff: bits(255)=%d\n", popcount(255));
    printf("bitcount_aa: bits(170)=%d\n", popcount(170));

    // XOR swap
    a = 10; b = 20;
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
    printf("swap_xor: a=%d, b=%d\n", a, b);

    // Rotate left
    ua = 0xABCD0001u;
    printf("rotate_left: rotl(0xABCD0001,4)=%u\n", rotl(ua, 4));

    // Flag manipulation
    int flags = 0;
    int FLAG_READ = 1;
    int FLAG_WRITE = 2;
    int FLAG_EXEC = 4;
    int FLAG_ADMIN = 8;
    flags = flags | FLAG_READ | FLAG_WRITE | FLAG_EXEC;
    printf("flags_set: flags=%d\n", flags);

    printf("flags_check: has_read=%d, has_write=%d, has_exec=%d, has_admin=%d\n",
           (flags & FLAG_READ) != 0,
           (flags & FLAG_WRITE) != 0,
           (flags & FLAG_EXEC) != 0,
           (flags & FLAG_ADMIN) != 0);

    flags = flags & ~FLAG_WRITE;
    printf("flags_clear: flags after clear write=%d\n", flags);

    // Chained bitwise
    printf("chain_and_or: (0xF0 & 0xFF) | 0x0F=%d\n", (0xF0 & 0xFF) | 0x0F);
    printf("chain_xor: 0xAA ^ 0x55 ^ 0xFF=%d\n", 0xAA ^ 0x55 ^ 0xFF);

    // Double NOT
    printf("double_not: ~~42=%d\n", ~~42);

    // Shift chain
    printf("shift_chain: (1<<8)-1=%d\n", (1 << 8) - 1);

    return 0;
}
