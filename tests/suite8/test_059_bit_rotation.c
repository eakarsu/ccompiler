int printf(const char *fmt, ...);

int rotate_left_8(int val, int n) {
    val = val & 255;
    n = n & 7;
    return ((val << n) | (val >> (8 - n))) & 255;
}

int rotate_right_8(int val, int n) {
    val = val & 255;
    n = n & 7;
    return ((val >> n) | (val << (8 - n))) & 255;
}

int rotate_left_16(int val, int n) {
    val = val & 65535;
    n = n & 15;
    return ((val << n) | (val >> (16 - n))) & 65535;
}

int rotate_right_16(int val, int n) {
    val = val & 65535;
    n = n & 15;
    return ((val >> n) | (val << (16 - n))) & 65535;
}

int circular_shift_register(int val, int bits, int steps) {
    int mask;
    int feedback;
    int i;
    mask = (1 << bits) - 1;
    val = val & mask;
    for (i = 0; i < steps; i = i + 1) {
        feedback = val & 1;
        val = val >> 1;
        if (feedback) {
            val = val | (1 << (bits - 1));
        }
    }
    return val;
}

int rotate_through_carry(int val, int carry_in, int bits) {
    int carry_out;
    int mask;
    mask = (1 << bits) - 1;
    val = val & mask;
    carry_out = (val >> (bits - 1)) & 1;
    val = ((val << 1) | carry_in) & mask;
    return (val << 1) | carry_out;
}

int find_rotation_distance(int a, int b, int bits) {
    int i;
    int rotated;
    int mask;
    mask = (1 << bits) - 1;
    a = a & mask;
    b = b & mask;
    for (i = 0; i < bits; i = i + 1) {
        rotated = ((a << i) | (a >> (bits - i))) & mask;
        if (rotated == b) return i;
    }
    return -1;
}

int barrel_shift_left(int val, int amount, int bits) {
    int mask;
    mask = (1 << bits) - 1;
    val = val & mask;
    amount = amount & (bits - 1);
    return ((val << amount) | (val >> (bits - amount))) & mask;
}

int barrel_shift_right(int val, int amount, int bits) {
    int mask;
    mask = (1 << bits) - 1;
    val = val & mask;
    amount = amount & (bits - 1);
    return ((val >> amount) | (val << (bits - amount))) & mask;
}

int main() {
    int val;
    int i;
    int rot;

    printf("=== 8-bit Rotate Left ===\n");
    // EXPECT: === 8-bit Rotate Left ===

    val = 1;
    for (i = 0; i <= 8; i = i + 1) {
        printf("rol8(%d,%d) = %d\n", val, i, rotate_left_8(val, i));
    }
    // EXPECT: rol8(1,0) = 1
    // EXPECT: rol8(1,1) = 2
    // EXPECT: rol8(1,2) = 4
    // EXPECT: rol8(1,3) = 8
    // EXPECT: rol8(1,4) = 16
    // EXPECT: rol8(1,5) = 32
    // EXPECT: rol8(1,6) = 64
    // EXPECT: rol8(1,7) = 128
    // EXPECT: rol8(1,8) = 1

    printf("=== 8-bit Rotate Right ===\n");
    // EXPECT: === 8-bit Rotate Right ===

    val = 128;
    for (i = 0; i <= 8; i = i + 1) {
        printf("ror8(%d,%d) = %d\n", val, i, rotate_right_8(val, i));
    }
    // EXPECT: ror8(128,0) = 128
    // EXPECT: ror8(128,1) = 64
    // EXPECT: ror8(128,2) = 32
    // EXPECT: ror8(128,3) = 16
    // EXPECT: ror8(128,4) = 8
    // EXPECT: ror8(128,5) = 4
    // EXPECT: ror8(128,6) = 2
    // EXPECT: ror8(128,7) = 1
    // EXPECT: ror8(128,8) = 128

    printf("=== Rotate Roundtrip ===\n");
    // EXPECT: === Rotate Roundtrip ===

    val = 170;
    rot = rotate_left_8(val, 3);
    rot = rotate_right_8(rot, 3);
    printf("170 -> rol3 -> ror3 = %d\n", rot);
    // EXPECT: 170 -> rol3 -> ror3 = 170

    val = 85;
    rot = rotate_right_8(val, 5);
    rot = rotate_left_8(rot, 5);
    printf("85 -> ror5 -> rol5 = %d\n", rot);
    // EXPECT: 85 -> ror5 -> rol5 = 85

    printf("=== 16-bit Rotate ===\n");
    // EXPECT: === 16-bit Rotate ===

    printf("rol16(%d,%d) = %d\n", 1, 8, rotate_left_16(1, 8));
    // EXPECT: rol16(1,8) = 256
    printf("rol16(%d,%d) = %d\n", 1, 15, rotate_left_16(1, 15));
    // EXPECT: rol16(1,15) = 32768
    printf("ror16(%d,%d) = %d\n", 256, 8, rotate_right_16(256, 8));
    // EXPECT: ror16(256,8) = 1

    printf("=== Circular Shift Register ===\n");
    // EXPECT: === Circular Shift Register ===

    val = 1;
    for (i = 0; i <= 8; i = i + 1) {
        printf("csr(%d,%d,%d) = %d\n", 1, 4, i, circular_shift_register(1, 4, i));
    }
    // EXPECT: csr(1,4,0) = 1
    // EXPECT: csr(1,4,1) = 8
    // EXPECT: csr(1,4,2) = 4
    // EXPECT: csr(1,4,3) = 2
    // EXPECT: csr(1,4,4) = 1
    // EXPECT: csr(1,4,5) = 8
    // EXPECT: csr(1,4,6) = 4
    // EXPECT: csr(1,4,7) = 2
    // EXPECT: csr(1,4,8) = 1

    printf("=== Rotation Distance ===\n");
    // EXPECT: === Rotation Distance ===

    printf("rotdist(1,2,8) = %d\n", find_rotation_distance(1, 2, 8));
    // EXPECT: rotdist(1,2,8) = 1
    printf("rotdist(1,128,8) = %d\n", find_rotation_distance(1, 128, 8));
    // EXPECT: rotdist(1,128,8) = 7
    printf("rotdist(3,6,8) = %d\n", find_rotation_distance(3, 6, 8));
    // EXPECT: rotdist(3,6,8) = 1
    printf("rotdist(1,3,8) = %d\n", find_rotation_distance(1, 3, 8));
    // EXPECT: rotdist(1,3,8) = -1

    printf("=== Barrel Shift ===\n");
    // EXPECT: === Barrel Shift ===

    printf("bsl(15,2,8) = %d\n", barrel_shift_left(15, 2, 8));
    // EXPECT: bsl(15,2,8) = 60
    printf("bsr(60,2,8) = %d\n", barrel_shift_right(60, 2, 8));
    // EXPECT: bsr(60,2,8) = 15
    printf("bsl(255,4,8) = %d\n", barrel_shift_left(255, 4, 8));
    // EXPECT: bsl(255,4,8) = 255

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
