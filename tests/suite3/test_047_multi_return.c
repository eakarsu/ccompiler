int printf(const char *fmt, ...);

/* Function with many return paths based on comparisons */
int classify_int(int x) {
    if (x < -100) return -3;
    if (x < 0)    return -2;
    if (x == 0)   return 0;
    if (x <= 10)  return 1;
    if (x <= 100) return 2;
    return 3;
}

/* Return from nested if-else */
int fizzbuzz_code(int n) {
    if (n % 15 == 0) return 3;
    else if (n % 3 == 0) return 1;
    else if (n % 5 == 0) return 2;
    else return 0;
}

/* Return in switch */
int day_type(int day) {
    /* 0=Sun, 1=Mon, ..., 6=Sat */
    switch (day) {
        case 0: return 0; /* Sunday: weekend */
        case 1: return 1; /* Monday: weekday */
        case 2: return 1;
        case 3: return 1;
        case 4: return 1;
        case 5: return 1;
        case 6: return 0; /* Saturday: weekend */
        default: return -1;
    }
}

/* Return in switch with fall-through grouping */
int vowel_or_consonant(int c) {
    switch (c) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
        case 'A': case 'E': case 'I': case 'O': case 'U':
            return 1; /* vowel */
        default:
            return 0; /* consonant or other */
    }
}

/* Early return optimization: first match wins */
int first_divisor_above_one(int n) {
    int i;
    if (n <= 1) return n;
    for (i = 2; i <= n; i++) {
        if (n % i == 0) return i;
    }
    return n;
}

/* Multiple returns in nested conditions */
int triangle_type(int a, int b, int c) {
    /* -1=invalid, 0=scalene, 1=isosceles, 2=equilateral */
    if (a <= 0 || b <= 0 || c <= 0) return -1;
    if (a + b <= c || a + c <= b || b + c <= a) return -1;
    if (a == b && b == c) return 2;
    if (a == b || b == c || a == c) return 1;
    return 0;
}

/* Return based on bit pattern */
int count_set_bits(int n) {
    int count = 0;
    if (n < 0) return -1;
    while (n > 0) {
        if (n & 1) count++;
        n >>= 1;
    }
    return count;
}

/* Guard clause style: return early on error */
int safe_divide(int a, int b) {
    if (b == 0) return -999; /* error sentinel */
    return a / b;
}

/* Return from middle of computation */
int search_first_negative(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] < 0) return i;
    }
    return -1;
}

/* Multi-level returns in a parsing-like function */
int parse_digit(int c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/* Ternary chain as single return */
int sign3(int x) {
    return x > 0 ? 1 : (x < 0 ? -1 : 0);
}

/* Return from deeply nested if blocks */
int quadrant(int x, int y) {
    if (x == 0 || y == 0) return 0; /* on axis */
    if (x > 0) {
        if (y > 0) return 1;
        else return 4;
    } else {
        if (y > 0) return 2;
        else return 3;
    }
}

/* Sum with early termination */
int sum_until_negative(int *arr, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] < 0) return total;
        total += arr[i];
    }
    return total;
}

int main(void) {
    /* classify_int */
    printf("%d\n", classify_int(-200));  // EXPECT: -3
    printf("%d\n", classify_int(-50));   // EXPECT: -2
    printf("%d\n", classify_int(0));     // EXPECT: 0
    printf("%d\n", classify_int(5));     // EXPECT: 1
    printf("%d\n", classify_int(50));    // EXPECT: 2
    printf("%d\n", classify_int(200));   // EXPECT: 3

    /* fizzbuzz codes */
    printf("%d\n", fizzbuzz_code(1));    // EXPECT: 0
    printf("%d\n", fizzbuzz_code(3));    // EXPECT: 1
    printf("%d\n", fizzbuzz_code(5));    // EXPECT: 2
    printf("%d\n", fizzbuzz_code(15));   // EXPECT: 3
    printf("%d\n", fizzbuzz_code(30));   // EXPECT: 3

    /* day_type */
    printf("%d\n", day_type(0));    // EXPECT: 0
    printf("%d\n", day_type(1));    // EXPECT: 1
    printf("%d\n", day_type(3));    // EXPECT: 1
    printf("%d\n", day_type(6));    // EXPECT: 0
    printf("%d\n", day_type(7));    // EXPECT: -1

    /* vowel_or_consonant */
    printf("%d\n", vowel_or_consonant('a'));  // EXPECT: 1
    printf("%d\n", vowel_or_consonant('E'));  // EXPECT: 1
    printf("%d\n", vowel_or_consonant('b'));  // EXPECT: 0
    printf("%d\n", vowel_or_consonant('z'));  // EXPECT: 0

    /* first_divisor_above_one (= smallest prime factor) */
    printf("%d\n", first_divisor_above_one(1));   // EXPECT: 1
    printf("%d\n", first_divisor_above_one(7));   // EXPECT: 7
    printf("%d\n", first_divisor_above_one(12));  // EXPECT: 2
    printf("%d\n", first_divisor_above_one(15));  // EXPECT: 3
    printf("%d\n", first_divisor_above_one(49));  // EXPECT: 7

    /* triangle_type */
    printf("%d\n", triangle_type(3, 4, 5));    // EXPECT: 0
    printf("%d\n", triangle_type(5, 5, 3));    // EXPECT: 1
    printf("%d\n", triangle_type(4, 4, 4));    // EXPECT: 2
    printf("%d\n", triangle_type(1, 2, 10));   // EXPECT: -1
    printf("%d\n", triangle_type(0, 1, 1));    // EXPECT: -1

    /* count_set_bits */
    printf("%d\n", count_set_bits(0));     // EXPECT: 0
    printf("%d\n", count_set_bits(1));     // EXPECT: 1
    printf("%d\n", count_set_bits(7));     // EXPECT: 3
    printf("%d\n", count_set_bits(255));   // EXPECT: 8
    printf("%d\n", count_set_bits(256));   // EXPECT: 1
    printf("%d\n", count_set_bits(-1));    // EXPECT: -1

    /* safe_divide */
    printf("%d\n", safe_divide(10, 2));    // EXPECT: 5
    printf("%d\n", safe_divide(7, 0));     // EXPECT: -999

    /* search_first_negative */
    int arr1[5]; arr1[0]=3; arr1[1]=1; arr1[2]=-2; arr1[3]=4; arr1[4]=5;
    printf("%d\n", search_first_negative(arr1, 5));   // EXPECT: 2
    int arr2[3]; arr2[0]=1; arr2[1]=2; arr2[2]=3;
    printf("%d\n", search_first_negative(arr2, 3));   // EXPECT: -1

    /* parse_digit */
    printf("%d\n", parse_digit('0'));   // EXPECT: 0
    printf("%d\n", parse_digit('9'));   // EXPECT: 9
    printf("%d\n", parse_digit('a'));   // EXPECT: 10
    printf("%d\n", parse_digit('f'));   // EXPECT: 15
    printf("%d\n", parse_digit('A'));   // EXPECT: 10
    printf("%d\n", parse_digit('z'));   // EXPECT: -1

    /* sign3 */
    printf("%d\n", sign3(5));    // EXPECT: 1
    printf("%d\n", sign3(-3));   // EXPECT: -1
    printf("%d\n", sign3(0));    // EXPECT: 0

    /* quadrant */
    printf("%d\n", quadrant(1, 1));    // EXPECT: 1
    printf("%d\n", quadrant(-1, 1));   // EXPECT: 2
    printf("%d\n", quadrant(-1, -1));  // EXPECT: 3
    printf("%d\n", quadrant(1, -1));   // EXPECT: 4
    printf("%d\n", quadrant(0, 5));    // EXPECT: 0

    /* sum_until_negative */
    int arr3[6]; arr3[0]=1; arr3[1]=2; arr3[2]=3; arr3[3]=-1; arr3[4]=4; arr3[5]=5;
    printf("%d\n", sum_until_negative(arr3, 6));  // EXPECT: 6
    int arr4[3]; arr4[0]=10; arr4[1]=20; arr4[2]=30;
    printf("%d\n", sum_until_negative(arr4, 3));  // EXPECT: 60

    return 0;
}
