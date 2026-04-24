int printf(const char *fmt, ...);

// 20-variable function with complex control flow

int mega_compute(int a, int b, int c, int d) {
    int e;
    int f;
    int g;
    int h;
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int o;
    int p;
    int q;
    int r;
    int s;
    int t;

    e = a + b;
    f = c - d;
    g = e * f;
    h = 0;

    if (g > 0) {
        i = g / 2;
        j = g % 3;
    } else {
        i = -g;
        j = (-g) % 5;
    }

    k = 0;
    l = 1;
    m = i;
    while (m > 0) {
        k = k + l;
        l = l + 1;
        m = m - 1;
    }

    n = 0;
    for (o = 0; o < j + 3; o = o + 1) {
        n = n + o * o;
    }

    p = a;
    q = b;
    r = 0;
    while (p > 0 && q > 0) {
        if (p > q) {
            p = p - q;
        } else {
            q = q - p;
        }
        r = r + 1;
    }

    s = e + f + i + j + k + n + r;
    t = 0;
    if (s > 100) {
        t = s - 100;
    } else if (s > 50) {
        t = s * 2;
    } else if (s > 0) {
        t = s + 10;
    } else {
        t = -s;
    }

    return t;
}

int chain_locals(int seed) {
    int a; int b; int c; int d; int e;
    int f; int g; int h; int i; int j;
    int k; int l; int m; int n; int o;
    int p; int q; int r; int s; int t;

    a = seed;
    b = a * 2 + 1;
    c = b - a + 3;
    d = c * c;
    e = d / 3;
    f = e + a;
    g = f % 7;
    h = g + b;
    i = h * 2;
    j = i - d;
    k = j + e;
    l = k % 11;
    m = l * l;
    n = m + g;
    o = n - f;
    p = o * 2;
    q = p + l;
    r = q % 13;
    s = r + m;
    t = s - n;

    return t;
}

int multi_loop(int n) {
    int a; int b; int c; int d;
    int sum;
    int i; int j;

    sum = 0;
    a = 1;
    b = 1;

    for (i = 0; i < n; i = i + 1) {
        c = a + b;
        d = c * 2;
        a = b;
        b = c;

        for (j = 0; j < d % 5; j = j + 1) {
            sum = sum + 1;
        }

        if (c > 100) {
            a = 1;
            b = 1;
        }
    }

    return sum;
}

int nested_conditionals(int a, int b, int c, int d) {
    int result;
    result = 0;

    if (a > 0) {
        if (b > 0) {
            if (c > 0) {
                if (d > 0) {
                    result = a + b + c + d;
                } else {
                    result = a + b + c - d;
                }
            } else {
                if (d > 0) {
                    result = a + b - c + d;
                } else {
                    result = a + b;
                }
            }
        } else {
            if (c > 0) {
                result = a - b + c;
            } else {
                result = a - b;
            }
        }
    } else {
        if (b > 0) {
            result = b;
        } else {
            result = 0;
        }
    }

    return result;
}

int do_while_chain(int start) {
    int x;
    int count;
    int phase;

    x = start;
    count = 0;
    phase = 0;

    do {
        if (phase == 0) {
            x = x + 3;
            if (x > 20) phase = 1;
        } else if (phase == 1) {
            x = x - 5;
            if (x < 10) phase = 2;
        } else {
            x = x * 2;
            if (x > 50) phase = 3;
        }
        count = count + 1;
    } while (phase < 3 && count < 100);

    return count;
}

int main() {
    // mega_compute(10, 6, 8, 3):
    // e=16, f=5, g=80 > 0 => i=40, j=80%3=2
    // k: sum 1..40 = 820, l ends at 41
    // n: j+3=5, sum o*o for o=0..4 = 0+1+4+9+16 = 30
    // gcd loop: p=10,q=6: 10>6 p=4,r=1; 6>4 q=2,r=2; 4>2 p=2,r=3; 2==2 q=0,r=4 (wait q-p=0)
    // Actually: p=2,q=2 => p not > q, so q=q-p=0, r=4
    // s = 16+5+40+2+820+30+4 = 917
    // s > 100 => t = 917-100 = 817
    printf("%d\n", mega_compute(10, 6, 8, 3)); // EXPECT: 817

    // mega_compute(1, 1, 1, 5):
    // e=2, f=-4, g=-8 <= 0 => i=8, j=8%5=3
    // k: sum 1..8 = 36
    // n: j+3=6, sum o*o for o=0..5 = 0+1+4+9+16+25=55
    // gcd: p=1,q=1 => q=q-p=0, r=1
    // s = 2+(-4)+8+3+36+55+1 = 101
    // s > 100 => t = 101-100 = 1
    printf("%d\n", mega_compute(1, 1, 1, 5)); // EXPECT: 1

    // chain_locals(5):
    // a=5, b=11, c=11-5+3=9, d=81, e=27, f=32, g=32%7=4
    // h=4+11=15, i=30, j=30-81=-51, k=-51+27=-24, l=-24%11=-2
    // m=4, n=4+4=8, o=8-32=-24, p=-48, q=-48+(-2)=-50, r=-50%13=-11
    // s=-11+4=-7, t=-7-8=-15
    printf("%d\n", chain_locals(5)); // EXPECT: -15

    // chain_locals(10):
    // a=10, b=21, c=21-10+3=14, d=196, e=65, f=75, g=75%7=5
    // h=5+21=26, i=52, j=52-196=-144, k=-144+65=-79, l=-79%11=-2
    // m=4, n=4+5=9, o=9-75=-66, p=-132, q=-132+(-2)=-134, r=-134%13=-4
    // s=-4+4=0, t=0-9=-9
    printf("%d\n", chain_locals(10)); // EXPECT: -9

    // multi_loop(10)
    printf("%d\n", multi_loop(10)); // EXPECT: 18

    // multi_loop(20)
    printf("%d\n", multi_loop(20)); // EXPECT: 36

    // nested_conditionals
    printf("%d\n", nested_conditionals(1, 2, 3, 4));    // EXPECT: 10
    printf("%d\n", nested_conditionals(1, 2, 3, -4));   // EXPECT: 10
    printf("%d\n", nested_conditionals(1, 2, -3, 4));   // EXPECT: 10
    printf("%d\n", nested_conditionals(1, 2, -3, -4));  // EXPECT: 3
    printf("%d\n", nested_conditionals(1, -2, 3, 0));   // EXPECT: 6
    printf("%d\n", nested_conditionals(1, -2, -3, 0));  // EXPECT: 3
    printf("%d\n", nested_conditionals(-1, 2, 0, 0));   // EXPECT: 2
    printf("%d\n", nested_conditionals(-1, -2, 0, 0));  // EXPECT: 0

    // do_while_chain
    printf("%d\n", do_while_chain(1));  // EXPECT: 13
    printf("%d\n", do_while_chain(10)); // EXPECT: 10

    printf("%s\n", "done"); // EXPECT: done
    return 0;
}
