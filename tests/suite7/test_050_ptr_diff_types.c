int printf(const char *fmt, ...);

struct Big {
    int a;
    int b;
    int c;
};

int main(void) {
    // char pointer arithmetic: stride of 1
    char carr[8];
    int i;
    for (i = 0; i < 8; i++) {
        carr[i] = (char)(i * 10);
    }

    char *cp = carr;
    // EXPECT: cp[0] = 0
    printf("cp[0] = %d\n", (int)*cp);

    cp = cp + 1;
    // EXPECT: cp[1] = 10
    printf("cp[1] = %d\n", (int)*cp);

    cp = cp + 3;
    // EXPECT: cp[4] = 40
    printf("cp[4] = %d\n", (int)*cp);

    // int pointer arithmetic: stride of 4
    int iarr[6];
    for (i = 0; i < 6; i++) {
        iarr[i] = (i + 1) * 100;
    }

    int *ip = iarr;
    // EXPECT: ip[0] = 100
    printf("ip[0] = %d\n", *ip);

    ip = ip + 1;
    // EXPECT: ip[1] = 200
    printf("ip[1] = %d\n", *ip);

    ip = ip + 2;
    // EXPECT: ip[3] = 400
    printf("ip[3] = %d\n", *ip);

    // Pointer difference for char: 1 byte apart
    char *c1 = &carr[0];
    char *c2 = &carr[5];
    // EXPECT: char_diff = 5
    printf("char_diff = %d\n", (int)(c2 - c1));

    // Pointer difference for int: measured in ints, not bytes
    int *i1 = &iarr[0];
    int *i2 = &iarr[4];
    // EXPECT: int_diff = 4
    printf("int_diff = %d\n", (int)(i2 - i1));

    // byte distance vs element distance
    char *byte_i1 = (char *)i1;
    char *byte_i2 = (char *)i2;
    // EXPECT: byte_distance = 16
    printf("byte_distance = %d\n", (int)(byte_i2 - byte_i1));

    // struct pointer arithmetic: stride of sizeof(struct)
    struct Big barr[3];
    barr[0].a = 1; barr[0].b = 2; barr[0].c = 3;
    barr[1].a = 4; barr[1].b = 5; barr[1].c = 6;
    barr[2].a = 7; barr[2].b = 8; barr[2].c = 9;

    struct Big *bp = barr;
    // EXPECT: bp[0].a = 1
    printf("bp[0].a = %d\n", bp->a);

    bp = bp + 1;
    // EXPECT: bp[1].a = 4
    printf("bp[1].a = %d\n", bp->a);

    // EXPECT: bp[1].c = 6
    printf("bp[1].c = %d\n", bp->c);

    bp = bp + 1;
    // EXPECT: bp[2].b = 8
    printf("bp[2].b = %d\n", bp->b);

    // Struct pointer difference
    struct Big *s1 = &barr[0];
    struct Big *s2 = &barr[2];
    // EXPECT: struct_diff = 2
    printf("struct_diff = %d\n", (int)(s2 - s1));

    // byte distance between struct elements
    char *sb1 = (char *)s1;
    char *sb2 = (char *)s2;
    // Each struct Big is 12 bytes, distance should be 24
    // EXPECT: struct_byte_dist = 24
    printf("struct_byte_dist = %d\n", (int)(sb2 - sb1));

    // Cast int array to char*, step through int bytes
    int test = 0x01020304;
    char *tb = (char *)&test;
    // Little-endian: byte 0 = 0x04 = 4
    // EXPECT: int_byte0 = 4
    printf("int_byte0 = %d\n", (int)tb[0]);

    // EXPECT: int_byte3 = 1
    printf("int_byte3 = %d\n", (int)tb[3]);

    // Incrementing char* vs int* over same memory
    int shared[2];
    shared[0] = 0x0A0B0C0D;
    shared[1] = 0x01020304;

    int *si = shared;
    char *sc = (char *)shared;

    // int ptr + 1 jumps 4 bytes
    // EXPECT: int_step = 16909060
    printf("int_step = %d\n", *(si + 1));

    // char ptr + 4 also reaches next int
    int *recast = (int *)(sc + 4);
    // EXPECT: char_step_to_int = 16909060
    printf("char_step_to_int = %d\n", *recast);

    // char ptr + 1 reads next byte of first int
    // 0x0A0B0C0D little endian: bytes are 0D, 0C, 0B, 0A
    // EXPECT: next_byte = 12
    printf("next_byte = %d\n", (int)*(sc + 1));

    // Pointer walk comparison: 5 ints vs 20 chars cover same space
    int walk[5];
    for (i = 0; i < 5; i++) walk[i] = i + 1;

    int *iwalk = walk;
    char *cwalk = (char *)walk;

    int isum = 0;
    for (i = 0; i < 5; i++) {
        isum = isum + *(iwalk + i);
    }
    // EXPECT: int_walk_sum = 15
    printf("int_walk_sum = %d\n", isum);

    // Read first byte of each int in the array (little-endian low byte)
    int bsum = 0;
    for (i = 0; i < 5; i++) {
        bsum = bsum + (int)*(cwalk + i * 4);
    }
    // low bytes of 1,2,3,4,5 = 1,2,3,4,5
    // EXPECT: byte_walk_sum = 15
    printf("byte_walk_sum = %d\n", bsum);

    return 0;
}
