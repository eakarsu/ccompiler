int printf(const char *fmt, ...);

int my_abs(int x) {
    if (x < 0) return -x;
    return x;
}

int abs_diff(int a, int b) {
    if (a > b) return a - b;
    return b - a;
}

int manhattan_2d(int x1, int y1, int x2, int y2) {
    return abs_diff(x1, x2) + abs_diff(y1, y2);
}

int chebyshev_2d(int x1, int y1, int x2, int y2) {
    int dx = abs_diff(x1, x2);
    int dy = abs_diff(y1, y2);
    if (dx > dy) return dx;
    return dy;
}

int manhattan_3d(int x1, int y1, int z1, int x2, int y2, int z2) {
    return abs_diff(x1, x2) + abs_diff(y1, y2) + abs_diff(z1, z2);
}

int chebyshev_3d(int x1, int y1, int z1, int x2, int y2, int z2) {
    int dx = abs_diff(x1, x2);
    int dy = abs_diff(y1, y2);
    int dz = abs_diff(z1, z2);
    int m = dx;
    if (dy > m) m = dy;
    if (dz > m) m = dz;
    return m;
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int min_abs(int a, int b) {
    int aa = my_abs(a);
    int ab = my_abs(b);
    if (aa < ab) return a;
    return b;
}

int max_abs(int a, int b) {
    int aa = my_abs(a);
    int ab = my_abs(b);
    if (aa > ab) return a;
    return b;
}

int closest_to_zero(int arr[], int n) {
    int best = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (my_abs(arr[i]) < my_abs(best)) {
            best = arr[i];
        }
    }
    return best;
}

int farthest_from_zero(int arr[], int n) {
    int best = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (my_abs(arr[i]) > my_abs(best)) {
            best = arr[i];
        }
    }
    return best;
}

int sum_abs(int arr[], int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + my_abs(arr[i]);
    }
    return sum;
}

int max_abs_diff(int arr[], int n) {
    int i;
    int j;
    int mx = 0;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int d = abs_diff(arr[i], arr[j]);
            if (d > mx) mx = d;
        }
    }
    return mx;
}

int main(void) {
    // EXPECT: abs(5)=5
    printf("abs(5)=%d\n", my_abs(5));

    // EXPECT: abs(-5)=5
    printf("abs(-5)=%d\n", my_abs(-5));

    // EXPECT: abs(0)=0
    printf("abs(0)=%d\n", my_abs(0));

    // EXPECT: abs_diff(10,3)=7
    printf("abs_diff(10,3)=%d\n", abs_diff(10, 3));

    // EXPECT: abs_diff(3,10)=7
    printf("abs_diff(3,10)=%d\n", abs_diff(3, 10));

    // EXPECT: manhattan(0,0,3,4)=7
    printf("manhattan(0,0,3,4)=%d\n", manhattan_2d(0, 0, 3, 4));

    // EXPECT: manhattan(-1,-1,2,3)=7
    printf("manhattan(-1,-1,2,3)=%d\n", manhattan_2d(-1, -1, 2, 3));

    // EXPECT: chebyshev(0,0,3,4)=4
    printf("chebyshev(0,0,3,4)=%d\n", chebyshev_2d(0, 0, 3, 4));

    // EXPECT: chebyshev(0,0,5,2)=5
    printf("chebyshev(0,0,5,2)=%d\n", chebyshev_2d(0, 0, 5, 2));

    // EXPECT: manhattan3d(0,0,0,1,2,3)=6
    printf("manhattan3d(0,0,0,1,2,3)=%d\n", manhattan_3d(0, 0, 0, 1, 2, 3));

    // EXPECT: chebyshev3d(0,0,0,1,5,3)=5
    printf("chebyshev3d(0,0,0,1,5,3)=%d\n", chebyshev_3d(0, 0, 0, 1, 5, 3));

    // EXPECT: sign(42)=1
    printf("sign(42)=%d\n", sign(42));

    // EXPECT: sign(-7)=-1
    printf("sign(-7)=%d\n", sign(-7));

    // EXPECT: sign(0)=0
    printf("sign(0)=%d\n", sign(0));

    // EXPECT: min_abs(3,-5)=3
    printf("min_abs(3,-5)=%d\n", min_abs(3, -5));

    // EXPECT: max_abs(3,-5)=-5
    printf("max_abs(3,-5)=%d\n", max_abs(3, -5));

    int arr1[5];
    arr1[0] = -10;
    arr1[1] = 3;
    arr1[2] = -1;
    arr1[3] = 7;
    arr1[4] = -5;

    // EXPECT: closest_to_zero=-1
    printf("closest_to_zero=%d\n", closest_to_zero(arr1, 5));

    // EXPECT: farthest_from_zero=-10
    printf("farthest_from_zero=%d\n", farthest_from_zero(arr1, 5));

    // EXPECT: sum_abs=26
    printf("sum_abs=%d\n", sum_abs(arr1, 5));

    // EXPECT: max_abs_diff=17
    printf("max_abs_diff=%d\n", max_abs_diff(arr1, 5));

    return 0;
}
