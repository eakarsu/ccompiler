int printf(const char *fmt, ...);

void xor_swap(int *a, int *b) {
    if (a == b) return;
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

int find_missing(int arr[], int n) {
    int xor_all = 0;
    int xor_arr = 0;
    int i = 0;
    while (i <= n) {
        xor_all = xor_all ^ i;
        i = i + 1;
    }
    i = 0;
    while (i < n) {
        xor_arr = xor_arr ^ arr[i];
        i = i + 1;
    }
    return xor_all ^ xor_arr;
}

int find_duplicate(int arr[], int n) {
    int xor_all = 0;
    int xor_arr = 0;
    int i = 1;
    while (i < n) {
        xor_all = xor_all ^ i;
        i = i + 1;
    }
    i = 0;
    while (i < n) {
        xor_arr = xor_arr ^ arr[i];
        i = i + 1;
    }
    return xor_all ^ xor_arr;
}

int xor_checksum(int arr[], int n) {
    int cs = 0;
    int i = 0;
    while (i < n) {
        cs = cs ^ arr[i];
        i = i + 1;
    }
    return cs;
}

int xor_encrypt_byte(int data, int key) {
    return data ^ key;
}

int xor_in_range(int lo, int hi) {
    int result = 0;
    int i = lo;
    while (i <= hi) {
        result = result ^ i;
        i = i + 1;
    }
    return result;
}

int find_odd_occurrence(int arr[], int n) {
    int result = 0;
    int i = 0;
    while (i < n) {
        result = result ^ arr[i];
        i = i + 1;
    }
    return result;
}

int xor_equal_check(int a, int b) {
    return (a ^ b) == 0;
}

int sign_different(int a, int b) {
    return (a ^ b) < 0;
}

int toggle_case_letter(int ch) {
    return ch ^ 32;
}

int xor_linked_encode(int prev_addr, int next_addr) {
    return prev_addr ^ next_addr;
}

int xor_nibble_swap(int x) {
    int lo = x & 0x0F;
    int hi = (x >> 4) & 0x0F;
    int xor_val = lo ^ hi;
    lo = lo ^ xor_val;
    hi = hi ^ xor_val;
    return (hi << 4) | lo;
}

int conditional_negate(int val, int negate_flag) {
    int mask = 0;
    if (negate_flag) mask = -1;
    return (val ^ mask) - mask;
}

int main(void) {
    int a = 42;
    int b = 99;
    xor_swap(&a, &b);
    // EXPECT: after swap: a=99, b=42
    printf("after swap: a=%d, b=%d\n", a, b);

    int x = 10;
    int y = 10;
    xor_swap(&x, &y);
    // EXPECT: same_swap: x=10, y=10
    printf("same_swap: x=%d, y=%d\n", x, y);

    int arr1[4];
    arr1[0] = 0; arr1[1] = 1; arr1[2] = 3; arr1[3] = 4;
    // EXPECT: missing = 2
    printf("missing = %d\n", find_missing(arr1, 4));

    int arr2[3];
    arr2[0] = 0; arr2[1] = 2; arr2[2] = 1;
    // EXPECT: missing2 = 3
    printf("missing2 = %d\n", find_missing(arr2, 3));

    int arr3[5];
    arr3[0] = 1; arr3[1] = 3; arr3[2] = 4; arr3[3] = 2; arr3[4] = 1;
    // EXPECT: duplicate = 1
    printf("duplicate = %d\n", find_duplicate(arr3, 5));

    int arr4[4];
    arr4[0] = 10; arr4[1] = 20; arr4[2] = 30; arr4[3] = 40;
    // EXPECT: checksum = 40
    printf("checksum = %d\n", xor_checksum(arr4, 4));

    int enc = xor_encrypt_byte(65, 0xFF);
    int dec = xor_encrypt_byte(enc, 0xFF);
    // EXPECT: encrypt/decrypt: 65
    printf("encrypt/decrypt: %d\n", dec);

    // EXPECT: xor_range(1,4) = 4
    printf("xor_range(1,4) = %d\n", xor_in_range(1, 4));
    // EXPECT: xor_range(1,7) = 0
    printf("xor_range(1,7) = %d\n", xor_in_range(1, 7));

    int arr5[7];
    arr5[0] = 2; arr5[1] = 3; arr5[2] = 5; arr5[3] = 3;
    arr5[4] = 2; arr5[5] = 5; arr5[6] = 7;
    // EXPECT: odd_occur = 7
    printf("odd_occur = %d\n", find_odd_occurrence(arr5, 7));

    // EXPECT: equal(5,5) = 1
    printf("equal(5,5) = %d\n", xor_equal_check(5, 5));
    // EXPECT: equal(5,3) = 0
    printf("equal(5,3) = %d\n", xor_equal_check(5, 3));

    // EXPECT: sign_diff(5,-3) = 1
    printf("sign_diff(5,-3) = %d\n", sign_different(5, -3));
    // EXPECT: sign_diff(5,3) = 0
    printf("sign_diff(5,3) = %d\n", sign_different(5, 3));

    // EXPECT: toggle_A = 97
    printf("toggle_A = %d\n", toggle_case_letter(65));
    // EXPECT: toggle_a = 65
    printf("toggle_a = %d\n", toggle_case_letter(97));

    // EXPECT: nibble_swap(0xAB) = 186
    printf("nibble_swap(0xAB) = %d\n", xor_nibble_swap(0xAB));

    // EXPECT: cond_neg(5, 1) = -5
    printf("cond_neg(5, 1) = %d\n", conditional_negate(5, 1));
    // EXPECT: cond_neg(5, 0) = 5
    printf("cond_neg(5, 0) = %d\n", conditional_negate(5, 0));

    // EXPECT: xor_link(100, 200) = 172
    printf("xor_link(100, 200) = %d\n", xor_linked_encode(100, 200));

    return 0;
}
