int printf(const char *fmt, ...);

// Test: Array-pointer equivalence
// a[i] == *(a+i) == *(i+a) == i[a]

int main() {
    int arr[5];
    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;
    arr[3] = 400;
    arr[4] = 500;

    // Basic equivalence: a[i] vs *(a+i)
    // EXPECT: a[2] = 300
    printf("a[2] = %d\n", arr[2]);
    // EXPECT: *(a+2) = 300
    printf("*(a+2) = %d\n", *(arr + 2));

    // Commutative: *(a+i) == *(i+a)
    // EXPECT: *(2+a) = 300
    printf("*(2+a) = %d\n", *(2 + arr));

    // Subscript commutative: a[i] == i[a]
    // EXPECT: 2[a] = 300
    printf("2[a] = %d\n", 2[arr]);

    // Loop using subscript vs pointer deref
    int sum_sub = 0;
    int sum_ptr = 0;
    int i;
    for (i = 0; i < 5; i++) {
        sum_sub = sum_sub + arr[i];
        sum_ptr = sum_ptr + *(arr + i);
    }
    // EXPECT: sum subscript: 1500
    printf("sum subscript: %d\n", sum_sub);
    // EXPECT: sum pointer: 1500
    printf("sum pointer: %d\n", sum_ptr);

    // Pointer to array element, then subscript
    int *p = arr + 1;
    // EXPECT: p[0] = 200
    printf("p[0] = %d\n", p[0]);
    // EXPECT: p[1] = 300
    printf("p[1] = %d\n", p[1]);
    // EXPECT: p[-1] = 100
    printf("p[-1] = %d\n", p[-1]);

    // Negative index equivalence
    int *q = &arr[4];
    // EXPECT: q[-2] = 300
    printf("q[-2] = %d\n", q[-2]);
    // EXPECT: *(q-2) = 300
    printf("*(q-2) = %d\n", *(q - 2));

    // Char array equivalence
    char str[6];
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = 0;
    // EXPECT: str[1] as int: 101
    printf("str[1] as int: %d\n", (int)str[1]);
    // EXPECT: *(str+1) as int: 101
    printf("*(str+1) as int: %d\n", (int)*(str + 1));

    // Equivalence with write through pointer
    int data[4];
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    *(data + 0) = 11;
    *(data + 1) = 22;
    data[2] = 33;
    3[data] = 44;
    int total = 0;
    for (i = 0; i < 4; i++) {
        total = total + data[i];
    }
    // EXPECT: total: 110
    printf("total: %d\n", total);

    // Array decay to pointer
    int *decayed = arr;
    // EXPECT: decayed[0]: 100
    printf("decayed[0]: %d\n", decayed[0]);
    // EXPECT: decayed[4]: 500
    printf("decayed[4]: %d\n", decayed[4]);

    // Multi-step pointer arithmetic equivalence
    int *r = arr;
    r = r + 2;
    // EXPECT: r offset: 300
    printf("r offset: %d\n", *r);
    r = r + 1;
    // EXPECT: r offset2: 400
    printf("r offset2: %d\n", *r);
    r = r - 3;
    // EXPECT: r back: 100
    printf("r back: %d\n", *r);

    // EXPECT: equiv done
    printf("equiv done\n");

    return 0;
}
