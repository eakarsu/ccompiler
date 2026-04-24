int printf(const char *fmt, ...);
// EXPECT: rotate_left_8(0x01, 1) = 0x02\nrotate_left_8(0x01, 4) = 0x10\nrotate_left_8(0x01, 7) = 0x80\nrotate_left_8(0x80, 1) = 0x01\nrotate_left_8(0xAB, 4) = 0xBA\nrotate_right_8(0x01, 1) = 0x80\nrotate_right_8(0x80, 1) = 0x40\nrotate_right_8(0xAB, 4) = 0xBA\nrotations of 0xA5:\n  left 0: 0xA5 (4 bits)\n  left 1: 0x4B (4 bits)\n  left 2: 0x96 (4 bits)\n  left 3: 0x2D (4 bits)\n  left 4: 0x5A (4 bits)\n  left 5: 0xB4 (4 bits)\n  left 6: 0x69 (4 bits)\n  left 7: 0xD2 (4 bits)\n  left 8: 0xA5 (4 bits)\nfull rotation: 0x3C -> 0x3C (same=1)\nleft3+right3: 0xDE -> 0xDE (same=1)\nrotate_left_16(0x1234, 4) = 0x2341\nrotate_left_16(0x1234, 8) = 0x3412\nrotate_left_16(0x8001, 1) = 0x0003\noriginal: HI!7\nrotated:  42 4A 09 B9 \nrecovered: HI!7
// Test: bitwise rotation

int rotate_left_8(int val, int n) {
    int v = val & 0xFF;
    n = n & 7;
    return ((v << n) | (v >> (8 - n))) & 0xFF;
}

int rotate_right_8(int val, int n) {
    int v = val & 0xFF;
    n = n & 7;
    return ((v >> n) | (v << (8 - n))) & 0xFF;
}

int rotate_left_16(int val, int n) {
    int v = val & 0xFFFF;
    n = n & 15;
    return ((v << n) | (v >> (16 - n))) & 0xFFFF;
}

int main(void) {
    // Left rotation of 8-bit value
    printf("rotate_left_8(0x01, 1) = 0x%02X\n", rotate_left_8(0x01, 1));
    printf("rotate_left_8(0x01, 4) = 0x%02X\n", rotate_left_8(0x01, 4));
    printf("rotate_left_8(0x01, 7) = 0x%02X\n", rotate_left_8(0x01, 7));
    printf("rotate_left_8(0x80, 1) = 0x%02X\n", rotate_left_8(0x80, 1));
    printf("rotate_left_8(0xAB, 4) = 0x%02X\n", rotate_left_8(0xAB, 4));

    // Right rotation of 8-bit value
    printf("rotate_right_8(0x01, 1) = 0x%02X\n", rotate_right_8(0x01, 1));
    printf("rotate_right_8(0x80, 1) = 0x%02X\n", rotate_right_8(0x80, 1));
    printf("rotate_right_8(0xAB, 4) = 0x%02X\n", rotate_right_8(0xAB, 4));

    // Rotation preserves bit count
    int val = 0xA5;
    int i;
    printf("rotations of 0xA5:\n");
    for (i = 0; i <= 8; i++) {
        int rotated = rotate_left_8(val, i);
        // Count bits
        int bits = 0;
        int tmp = rotated;
        int j;
        for (j = 0; j < 8; j++) {
            bits = bits + (tmp & 1);
            tmp = tmp >> 1;
        }
        printf("  left %d: 0x%02X (%d bits)\n", i, rotated, bits);
    }

    // Full rotation returns original
    int orig = 0x3C;
    int full = rotate_left_8(orig, 8);
    printf("full rotation: 0x%02X -> 0x%02X (same=%d)\n",
           orig, full, orig == full);

    // Left and right cancel
    int test = 0xDE;
    int lr = rotate_right_8(rotate_left_8(test, 3), 3);
    printf("left3+right3: 0x%02X -> 0x%02X (same=%d)\n",
           test, lr, test == lr);

    // 16-bit rotation
    printf("rotate_left_16(0x1234, 4) = 0x%04X\n", rotate_left_16(0x1234, 4));
    printf("rotate_left_16(0x1234, 8) = 0x%04X\n", rotate_left_16(0x1234, 8));
    printf("rotate_left_16(0x8001, 1) = 0x%04X\n", rotate_left_16(0x8001, 1));

    // Rotation as simple cipher
    char msg[5];
    msg[0] = 'H'; msg[1] = 'I'; msg[2] = '!'; msg[3] = '7'; msg[4] = 0;
    int rot = 3;
    printf("original: ");
    for (i = 0; i < 4; i++) printf("%c", msg[i]);
    printf("\nrotated:  ");
    for (i = 0; i < 4; i++) printf("%02X ", rotate_left_8(msg[i], rot));
    printf("\nrecovered: ");
    for (i = 0; i < 4; i++) {
        int r = rotate_right_8(rotate_left_8(msg[i], rot), rot);
        printf("%c", r);
    }
    printf("\n");

    return 0;
}
