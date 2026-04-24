int printf(const char *fmt, ...);
// EXPECT: values: 10 20 30 40 50 \nmodified: 110 120 130 140 150\nsorted via ptrs: 10 20 30 40 50 \noriginal: 50 10 40 20 30 \nstr[0] = cat\nstr[1] = dog\nstr[2] = bird
// Test: array of pointers

int main(void) {
    // Array of int pointers
    int a = 10, b = 20, c = 30, d = 40, e = 50;
    int *ptrs[5];
    ptrs[0] = &a;
    ptrs[1] = &b;
    ptrs[2] = &c;
    ptrs[3] = &d;
    ptrs[4] = &e;

    int i;
    printf("values: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", *ptrs[i]);
    }
    printf("\n");

    // Modify through pointer array
    for (i = 0; i < 5; i++) {
        *ptrs[i] = *ptrs[i] + 100;
    }
    printf("modified: %d %d %d %d %d\n", a, b, c, d, e);

    // Sort pointers by pointed-to value (bubble sort)
    int vals[5];
    vals[0] = 50; vals[1] = 10; vals[2] = 40; vals[3] = 20; vals[4] = 30;
    int *sorted[5];
    for (i = 0; i < 5; i++) {
        sorted[i] = &vals[i];
    }

    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4 - i; j++) {
            if (*sorted[j] > *sorted[j + 1]) {
                int *tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }

    printf("sorted via ptrs: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", *sorted[i]);
    }
    printf("\n");

    // Original array unchanged
    printf("original: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", vals[i]);
    }
    printf("\n");

    // Array of char pointers (string table)
    char s1[4]; s1[0]='c'; s1[1]='a'; s1[2]='t'; s1[3]='\0';
    char s2[4]; s2[0]='d'; s2[1]='o'; s2[2]='g'; s2[3]='\0';
    char s3[5]; s3[0]='b'; s3[1]='i'; s3[2]='r'; s3[3]='d'; s3[4]='\0';

    char *strs[3];
    strs[0] = s1;
    strs[1] = s2;
    strs[2] = s3;

    for (i = 0; i < 3; i++) {
        printf("str[%d] = %s\n", i, strs[i]);
    }

    return 0;
}
