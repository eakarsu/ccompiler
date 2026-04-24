int printf(const char *fmt, ...);
// EXPECT: === Unsigned Operations Tests ===
// EXPECT: uint_add: 100+200=300
// EXPECT: uint_sub: 200-50=150
// EXPECT: uint_mul: 1000*42=42000
// EXPECT: uint_div: 1000/3=333
// EXPECT: uint_mod: 1000%3=1
// EXPECT: uchar_max: 255
// EXPECT: uchar_wrap: 0
// EXPECT: uchar_wrap2: 255
// EXPECT: ushort_max: 65535
// EXPECT: ushort_wrap: 0
// EXPECT: uint_max: 4294967295
// EXPECT: uint_wrap: 0
// EXPECT: uchar_add: 200+100=44
// EXPECT: uchar_mul: 16*16=0
// EXPECT: ushort_add: 60000+10000=4464
// EXPECT: bit_pattern: 0xAA=170
// EXPECT: bit_pattern2: 0x55=85
// EXPECT: and_pattern: 0xFF & 0x0F=15
// EXPECT: or_pattern: 0xF0 | 0x0F=255
// EXPECT: xor_pattern: 0xFF ^ 0xAA=85
// EXPECT: not_pattern: ~0=4294967295
// EXPECT: shift_left: 1<<10=1024
// EXPECT: shift_right: 1024>>5=32
// EXPECT: unsigned_div_large: 3000000000/3=1000000000
// EXPECT: unsigned_mod_large: 3000000001%3=1
// EXPECT: cast_int_to_uint: -1 as uint=4294967295
// EXPECT: cast_uint_to_int: 4294967295 as int=-1
// EXPECT: cast_uchar_to_int: 255 as int=255
// EXPECT: cast_int_to_uchar: 256 as uchar=0
// EXPECT: cast_int_to_uchar2: -1 as uchar=255
// EXPECT: compare_uint: 3000000000>1000000000=1
// EXPECT: compare_mixed_pos: 10u==10=1
// EXPECT: uchar_arith: 128+127=255
// EXPECT: uint_chain: 10*20*30=6000
// EXPECT: complex_unsigned: (300*400)/100+50=1250
// uint_add: 100+200=300
// uint_sub: 200-50=150
// uint_mul: 1000*42=42000
// uint_div: 1000/3=333
// uint_mod: 1000%3=1
// uchar_max: 255
// uchar_wrap: 0
// uchar_wrap2: 255
// ushort_max: 65535
// ushort_wrap: 0
// uint_max: 4294967295
// uint_wrap: 0
// uchar_add: 200+100=44
// uchar_mul: 16*16=0
// ushort_add: 60000+10000=4464
// bit_pattern: 0xAA=170
// bit_pattern2: 0x55=85
// and_pattern: 0xFF & 0x0F=15
// or_pattern: 0xF0 | 0x0F=255
// xor_pattern: 0xFF ^ 0xAA=85
// not_pattern: ~0=4294967295
// shift_left: 1<<10=1024
// shift_right: 1024>>5=32
// unsigned_div_large: 3000000000/3=1000000000
// unsigned_mod_large: 3000000001%3=1
// cast_int_to_uint: -1 as uint=4294967295
// cast_uint_to_int: 4294967295 as int=-1
// cast_uchar_to_int: 255 as int=255
// cast_int_to_uchar: 256 as uchar=0
// cast_int_to_uchar2: -1 as uchar=255
// compare_uint: 3000000000>1000000000=1
// compare_mixed_pos: 10u==10=1
// uchar_arith: 128+127=255
// uint_chain: 10*20*30=6000
// complex_unsigned: (300*400)/100+50=1250

int main(void) {
    unsigned int ua, ub, ur;
    unsigned char uca, ucb, ucr;
    unsigned short usa, usb, usr;
    int sa;

    printf("=== Unsigned Operations Tests ===\n");

    // Basic unsigned int operations
    ua = 100; ub = 200;
    printf("uint_add: 100+200=%u\n", ua + ub);
    ua = 200; ub = 50;
    printf("uint_sub: 200-50=%u\n", ua - ub);
    ua = 1000; ub = 42;
    printf("uint_mul: 1000*42=%u\n", ua * ub);
    ua = 1000; ub = 3;
    printf("uint_div: 1000/3=%u\n", ua / ub);
    printf("uint_mod: 1000%%3=%u\n", ua % ub);

    // Max values and wrapping
    uca = 255;
    printf("uchar_max: %u\n", (unsigned int)uca);
    uca = 255;
    ucb = uca + 1;
    printf("uchar_wrap: %u\n", (unsigned int)ucb);
    uca = 0;
    ucb = uca - 1;
    printf("uchar_wrap2: %u\n", (unsigned int)ucb);

    usa = 65535;
    printf("ushort_max: %u\n", (unsigned int)usa);
    usb = usa + 1;
    printf("ushort_wrap: %u\n", (unsigned int)usb);

    ua = 4294967295u;
    printf("uint_max: %u\n", ua);
    ur = ua + 1;
    printf("uint_wrap: %u\n", ur);

    // Unsigned char overflow arithmetic
    uca = 200; ucb = 100;
    ucr = uca + ucb;  // 300 mod 256 = 44
    printf("uchar_add: 200+100=%u\n", (unsigned int)ucr);

    uca = 16; ucb = 16;
    ucr = uca * ucb;  // 256 mod 256 = 0
    printf("uchar_mul: 16*16=%u\n", (unsigned int)ucr);

    // Unsigned short overflow
    usa = 60000; usb = 10000;
    usr = usa + usb;  // 70000 mod 65536 = 4464
    printf("ushort_add: 60000+10000=%u\n", (unsigned int)usr);

    // Bit patterns
    ua = 0xAA;
    printf("bit_pattern: 0xAA=%u\n", ua);
    ua = 0x55;
    printf("bit_pattern2: 0x55=%u\n", ua);

    // Bitwise ops on unsigned
    ua = 0xFF; ub = 0x0F;
    printf("and_pattern: 0xFF & 0x0F=%u\n", ua & ub);
    ua = 0xF0; ub = 0x0F;
    printf("or_pattern: 0xF0 | 0x0F=%u\n", ua | ub);
    ua = 0xFF; ub = 0xAA;
    printf("xor_pattern: 0xFF ^ 0xAA=%u\n", ua ^ ub);
    ua = 0;
    printf("not_pattern: ~0=%u\n", ~ua);

    // Shifts on unsigned
    ua = 1;
    printf("shift_left: 1<<10=%u\n", ua << 10);
    ua = 1024;
    printf("shift_right: 1024>>5=%u\n", ua >> 5);

    // Large unsigned values (> INT_MAX)
    ua = 3000000000u;
    printf("unsigned_div_large: 3000000000/3=%u\n", ua / 3);
    ua = 3000000001u;
    printf("unsigned_mod_large: 3000000001%%3=%u\n", ua % 3);

    // Casting between signed and unsigned
    sa = -1;
    ua = (unsigned int)sa;
    printf("cast_int_to_uint: -1 as uint=%u\n", ua);

    ua = 4294967295u;
    sa = (int)ua;
    printf("cast_uint_to_int: 4294967295 as int=%d\n", sa);

    uca = 255;
    sa = (int)uca;
    printf("cast_uchar_to_int: 255 as int=%d\n", sa);

    sa = 256;
    uca = (unsigned char)sa;
    printf("cast_int_to_uchar: 256 as uchar=%u\n", (unsigned int)uca);

    sa = -1;
    uca = (unsigned char)sa;
    printf("cast_int_to_uchar2: -1 as uchar=%u\n", (unsigned int)uca);

    // Unsigned comparisons
    ua = 3000000000u; ub = 1000000000u;
    printf("compare_uint: 3000000000>1000000000=%d\n", ua > ub);

    ua = 10;
    printf("compare_mixed_pos: 10u==10=%d\n", ua == 10);

    // Unsigned char chain
    uca = 128; ucb = 127;
    ucr = uca + ucb;
    printf("uchar_arith: 128+127=%u\n", (unsigned int)ucr);

    // Unsigned chain
    ua = 10; ub = 20;
    ur = ua * ub * 30;
    printf("uint_chain: 10*20*30=%u\n", ur);

    // Complex unsigned expression
    ua = 300; ub = 400;
    ur = (ua * ub) / 100 + 50;
    printf("complex_unsigned: (300*400)/100+50=%u\n", ur);

    return 0;
}
