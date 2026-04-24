int printf(const char *fmt, ...);

int round_down(int x, int m) {
    if (m <= 0) return x;
    if (x >= 0) {
        return (x / m) * m;
    }
    return ((x - m + 1) / m) * m;
}

int round_up(int x, int m) {
    if (m <= 0) return x;
    if (x >= 0) {
        return ((x + m - 1) / m) * m;
    }
    return (x / m) * m;
}

int ceil_div(int a, int b) {
    if (b == 0) return 0;
    if ((a > 0 && b > 0) || (a < 0 && b < 0)) {
        int abs_a = a;
        int abs_b = b;
        if (abs_a < 0) abs_a = -abs_a;
        if (abs_b < 0) abs_b = -abs_b;
        return (abs_a + abs_b - 1) / abs_b;
    }
    return a / b;
}

int floor_div(int a, int b) {
    if (b == 0) return 0;
    int q = a / b;
    int r = a % b;
    if (r != 0 && ((a < 0 && b > 0) || (a > 0 && b < 0))) {
        q = q - 1;
    }
    return q;
}

int round_nearest(int x, int m) {
    if (m <= 0) return x;
    int abs_x = x;
    if (abs_x < 0) abs_x = -abs_x;
    int lo = (abs_x / m) * m;
    int hi = lo + m;
    int result;
    if (abs_x - lo <= hi - abs_x) {
        result = lo;
    } else {
        result = hi;
    }
    if (x < 0) return -result;
    return result;
}

int align_up(int addr, int alignment) {
    return round_up(addr, alignment);
}

int pages_needed(int bytes, int page_size) {
    return ceil_div(bytes, page_size);
}

int chunks_needed(int total, int chunk_size) {
    return ceil_div(total, chunk_size);
}

int round_to_power_of_two(int x) {
    int p = 1;
    while (p < x) {
        p = p * 2;
    }
    return p;
}

int snap_to_grid(int val, int grid) {
    return round_nearest(val, grid);
}

int midpoint(int a, int b) {
    return a + (b - a) / 2;
}

int div_toward_zero(int a, int b) {
    return a / b;
}

int main(void) {
    // EXPECT: round_down(17,5)=15
    printf("round_down(17,5)=%d\n", round_down(17, 5));

    // EXPECT: round_down(20,5)=20
    printf("round_down(20,5)=%d\n", round_down(20, 5));

    // EXPECT: round_down(-17,5)=-20
    printf("round_down(-17,5)=%d\n", round_down(-17, 5));

    // EXPECT: round_up(17,5)=20
    printf("round_up(17,5)=%d\n", round_up(17, 5));

    // EXPECT: round_up(20,5)=20
    printf("round_up(20,5)=%d\n", round_up(20, 5));

    // EXPECT: round_up(-17,5)=-15
    printf("round_up(-17,5)=%d\n", round_up(-17, 5));

    // EXPECT: ceil_div(10,3)=4
    printf("ceil_div(10,3)=%d\n", ceil_div(10, 3));

    // EXPECT: ceil_div(9,3)=3
    printf("ceil_div(9,3)=%d\n", ceil_div(9, 3));

    // EXPECT: ceil_div(-10,3)=-3
    printf("ceil_div(-10,3)=%d\n", ceil_div(-10, 3));

    // EXPECT: floor_div(10,3)=3
    printf("floor_div(10,3)=%d\n", floor_div(10, 3));

    // EXPECT: floor_div(-10,3)=-4
    printf("floor_div(-10,3)=%d\n", floor_div(-10, 3));

    // EXPECT: floor_div(10,-3)=-4
    printf("floor_div(10,-3)=%d\n", floor_div(10, -3));

    // EXPECT: round_nearest(17,5)=15
    printf("round_nearest(17,5)=%d\n", round_nearest(17, 5));

    // EXPECT: round_nearest(18,5)=20
    printf("round_nearest(18,5)=%d\n", round_nearest(18, 5));

    // EXPECT: round_nearest(-17,5)=-15
    printf("round_nearest(-17,5)=%d\n", round_nearest(-17, 5));

    // EXPECT: align_up(100,16)=112
    printf("align_up(100,16)=%d\n", align_up(100, 16));

    // EXPECT: align_up(128,16)=128
    printf("align_up(128,16)=%d\n", align_up(128, 16));

    // EXPECT: pages_needed(5000,4096)=2
    printf("pages_needed(5000,4096)=%d\n", pages_needed(5000, 4096));

    // EXPECT: pages_needed(4096,4096)=1
    printf("pages_needed(4096,4096)=%d\n", pages_needed(4096, 4096));

    // EXPECT: chunks_needed(100,7)=15
    printf("chunks_needed(100,7)=%d\n", chunks_needed(100, 7));

    // EXPECT: round_pow2(5)=8
    printf("round_pow2(5)=%d\n", round_to_power_of_two(5));

    // EXPECT: round_pow2(16)=16
    printf("round_pow2(16)=%d\n", round_to_power_of_two(16));

    // EXPECT: snap_to_grid(47,10)=50
    printf("snap_to_grid(47,10)=%d\n", snap_to_grid(47, 10));

    // EXPECT: midpoint(10,20)=15
    printf("midpoint(10,20)=%d\n", midpoint(10, 20));

    // EXPECT: div_toward_zero(-7,2)=-3
    printf("div_toward_zero(-7,2)=%d\n", div_toward_zero(-7, 2));

    return 0;
}
