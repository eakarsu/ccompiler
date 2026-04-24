int printf(const char *fmt, ...);
// EXPECT: p1=(3,4)\np1+p2=(10,6)\np1*3=(9,12)\n2*p1+3*p2=(27,14)\n10/3: val=3 ok=1\n10/0: val=0 ok=0\nmin=1 max=9\nsum=28 count=6 avg=4
// Test 221: Functions returning structs

struct Point {
    int x;
    int y;
};

struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

struct Point add_points(struct Point a, struct Point b) {
    struct Point r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

struct Point scale_point(struct Point p, int s) {
    struct Point r;
    r.x = p.x * s;
    r.y = p.y * s;
    return r;
}

struct Result {
    int value;
    int ok;
};

struct Result safe_divide(int a, int b) {
    struct Result r;
    if (b == 0) {
        r.value = 0;
        r.ok = 0;
    } else {
        r.value = a / b;
        r.ok = 1;
    }
    return r;
}

struct MinMax {
    int min;
    int max;
};

struct MinMax find_minmax(int *arr, int n) {
    struct MinMax mm;
    mm.min = arr[0];
    mm.max = arr[0];
    int i = 1;
    while (i < n) {
        if (arr[i] < mm.min) mm.min = arr[i];
        if (arr[i] > mm.max) mm.max = arr[i];
        i = i + 1;
    }
    return mm;
}

struct Stats {
    int sum;
    int count;
    int avg;
};

struct Stats compute_stats(int *arr, int n) {
    struct Stats s;
    s.sum = 0;
    s.count = n;
    int i = 0;
    while (i < n) {
        s.sum = s.sum + arr[i];
        i = i + 1;
    }
    s.avg = s.sum / n;
    return s;
}

int main(void) {
    struct Point p1 = make_point(3, 4);
    printf("p1=(%d,%d)\n", p1.x, p1.y);

    struct Point p2 = make_point(7, 2);
    struct Point p3 = add_points(p1, p2);
    printf("p1+p2=(%d,%d)\n", p3.x, p3.y);

    struct Point p4 = scale_point(p1, 3);
    printf("p1*3=(%d,%d)\n", p4.x, p4.y);

    struct Point p5 = add_points(scale_point(p1, 2), scale_point(p2, 3));
    printf("2*p1+3*p2=(%d,%d)\n", p5.x, p5.y);

    struct Result r1 = safe_divide(10, 3);
    printf("10/3: val=%d ok=%d\n", r1.value, r1.ok);
    struct Result r2 = safe_divide(10, 0);
    printf("10/0: val=%d ok=%d\n", r2.value, r2.ok);

    int arr[6];
    arr[0] = 5; arr[1] = 2; arr[2] = 8; arr[3] = 1; arr[4] = 9; arr[5] = 3;
    struct MinMax mm = find_minmax(arr, 6);
    printf("min=%d max=%d\n", mm.min, mm.max);

    struct Stats st = compute_stats(arr, 6);
    printf("sum=%d count=%d avg=%d\n", st.sum, st.count, st.avg);

    return 0;
}
