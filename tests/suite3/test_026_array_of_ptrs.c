int printf(const char *fmt, ...);

/* test_026_array_of_ptrs.c
   Tests: array of pointers (char *arr[]), pointer to arrays,
          array of string literals, sorting a pointer array (selection sort)
*/

int my_strlen(const char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

int my_strcmp(const char *a, const char *b) {
    int i = 0;
    while (a[i] && a[i] == b[i]) i++;
    return (int)(unsigned char)a[i] - (int)(unsigned char)b[i];
}

/* selection sort on array of char pointers */
void sort_strs(const char **arr, int n) {
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (my_strcmp(arr[j], arr[min_idx]) < 0)
                min_idx = j;
        }
        if (min_idx != i) {
            const char *tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}

/* selection sort on array of int pointers by pointed-to value */
void sort_int_ptrs(int **arr, int n) {
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (*arr[j] < *arr[min_idx])
                min_idx = j;
        }
        if (min_idx != i) {
            int *tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}

int main(void) {
    /* array of string literal pointers */
    const char *words[5] = {"banana", "apple", "cherry", "date", "elderberry"};

    printf("%s\n", words[0]);
    // EXPECT: banana
    printf("%s\n", words[1]);
    // EXPECT: apple
    printf("%s\n", words[4]);
    // EXPECT: elderberry

    /* lengths via pointer array */
    printf("%d\n", my_strlen(words[0]));
    // EXPECT: 6
    printf("%d\n", my_strlen(words[1]));
    // EXPECT: 5
    printf("%d\n", my_strlen(words[2]));
    // EXPECT: 6

    /* sort string pointer array */
    sort_strs(words, 5);
    /* sorted: apple, banana, cherry, date, elderberry */
    printf("%s\n", words[0]);
    // EXPECT: apple
    printf("%s\n", words[1]);
    // EXPECT: banana
    printf("%s\n", words[2]);
    // EXPECT: cherry
    printf("%s\n", words[3]);
    // EXPECT: date
    printf("%s\n", words[4]);
    // EXPECT: elderberry

    /* array of int pointers pointing to separate ints */
    int v0 = 30, v1 = 10, v2 = 50, v3 = 20, v4 = 40;
    int *iptrs[5] = {&v0, &v1, &v2, &v3, &v4};

    printf("%d\n", *iptrs[0]);
    // EXPECT: 30
    printf("%d\n", *iptrs[1]);
    // EXPECT: 10

    /* sort by pointed-to value */
    sort_int_ptrs(iptrs, 5);
    /* sorted values: 10, 20, 30, 40, 50 */
    printf("%d\n", *iptrs[0]);
    // EXPECT: 10
    printf("%d\n", *iptrs[1]);
    // EXPECT: 20
    printf("%d\n", *iptrs[2]);
    // EXPECT: 30
    printf("%d\n", *iptrs[3]);
    // EXPECT: 40
    printf("%d\n", *iptrs[4]);
    // EXPECT: 50

    /* modifying through pointer in array: changes original var */
    *iptrs[2] = 99;
    printf("%d\n", v0);
    // EXPECT: 99

    /* array of pointers into a single int array (every other element) */
    int data[8] = {2, 4, 6, 8, 10, 12, 14, 16};
    int *evens[4];
    int i;
    for (i = 0; i < 4; i++) evens[i] = &data[i * 2];

    printf("%d\n", *evens[0]);
    // EXPECT: 2
    printf("%d\n", *evens[1]);
    // EXPECT: 6
    printf("%d\n", *evens[2]);
    // EXPECT: 10
    printf("%d\n", *evens[3]);
    // EXPECT: 14

    /* array of pointers, pointer arithmetic on each */
    printf("%d\n", *(evens[1] + 1));
    // EXPECT: 8
    printf("%d\n", *(evens[2] + 1));
    // EXPECT: 12

    /* null-terminated array of string pointers */
    const char *fruits[5];
    fruits[0] = "mango";
    fruits[1] = "kiwi";
    fruits[2] = "pear";
    fruits[3] = "fig";
    fruits[4] = 0;

    int count = 0;
    const char **fp = fruits;
    while (*fp) {
        count++;
        fp++;
    }
    printf("%d\n", count);
    // EXPECT: 4

    /* reassigning pointer in array */
    const char *ptrs[3] = {"one", "two", "three"};
    ptrs[1] = "TWO";
    printf("%s\n", ptrs[0]);
    // EXPECT: one
    printf("%s\n", ptrs[1]);
    // EXPECT: TWO
    printf("%s\n", ptrs[2]);
    // EXPECT: three

    /* pointer to first pointer in array of pointers */
    const char **pp = &ptrs[0];
    printf("%s\n", *pp);
    // EXPECT: one
    printf("%s\n", *(pp + 1));
    // EXPECT: TWO
    printf("%s\n", *(pp + 2));
    // EXPECT: three

    /* compare adjacent strings in sorted array */
    if (my_strcmp(words[0], words[1]) < 0) printf("sorted\n");
    // EXPECT: sorted

    return 0;
}
