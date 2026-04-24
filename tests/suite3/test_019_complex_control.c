int printf(const char *fmt, ...);

/* test_019_complex_control.c
   Tests: Mix of if/for/while/switch/goto, state machines, menu dispatch,
   complex loop exit patterns.
*/

/* State machine: simple tokenizer counting words/numbers/spaces */
void count_tokens(const char *s, int *words, int *nums, int *spaces) {
    enum { ST_SPACE, ST_WORD, ST_NUM } state = ST_SPACE;
    *words = 0; *nums = 0; *spaces = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        int c = s[i];
        switch (state) {
            case ST_SPACE:
                if (c == ' ') { (*spaces)++; }
                else if (c >= '0' && c <= '9') { state = ST_NUM; }
                else { state = ST_WORD; }
                break;
            case ST_WORD:
                if (c == ' ') { (*words)++; state = ST_SPACE; (*spaces)++; }
                else if (c >= '0' && c <= '9') { state = ST_NUM; }
                break;
            case ST_NUM:
                if (c == ' ') { (*nums)++; state = ST_SPACE; (*spaces)++; }
                else if (!(c >= '0' && c <= '9')) { state = ST_WORD; }
                break;
        }
    }
    /* flush last token */
    if (state == ST_WORD) (*words)++;
    if (state == ST_NUM) (*nums)++;
}

/* Menu dispatch using switch + function pointers simulation */
int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }
int op_div(int a, int b) { return b != 0 ? a / b : -1; }

int dispatch(int op, int a, int b) {
    switch (op) {
        case 0: return op_add(a, b);
        case 1: return op_sub(a, b);
        case 2: return op_mul(a, b);
        case 3: return op_div(a, b);
        default: return -999;
    }
}

/* Complex loop exit: find two numbers in [1,n] that sum to target */
int find_pair(int n, int target, int *out_a, int *out_b) {
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            if (i + j == target) {
                *out_a = i;
                *out_b = j;
                return 1;
            }
        }
    }
    return 0;
}

/* Mix: compute number of primes up to n using nested if+for+goto */
int count_primes(int n) {
    int count = 0;
    for (int i = 2; i <= n; i++) {
        int is_prime = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) { is_prime = 0; break; }
        }
        if (is_prime) count++;
    }
    return count;
}
/* count_primes(10) = 4 (2,3,5,7) */
/* count_primes(20) = 8 (2,3,5,7,11,13,17,19) */

/* State machine: run-length decode length */
int run_length_count(const char *s) {
    int total = 0;
    for (int i = 0; s[i] != '\0'; ) {
        int count = 0;
        int c = s[i];
        while (s[i] == c && s[i] != '\0') { count++; i++; }
        total += count;
    }
    return total;
}
/* just counts total characters (sanity check for the loop pattern) */

/* Complex: 3-way partition: return number of iterations until sorted */
int bubble_passes(int arr[], int n) {
    int passes = 0;
    int swapped;
    do {
        swapped = 0;
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                int t = arr[i]; arr[i] = arr[i+1]; arr[i+1] = t;
                swapped = 1;
            }
        }
        passes++;
    } while (swapped);
    return passes;
}

/* Goto-based retry with complex conditions */
int search_with_goto(int arr[], int n, int target) {
    int i = 0;
next:
    if (i >= n) return -1;
    if (arr[i] == target) return i;
    i++;
    goto next;
}

/* Nested switch inside for with break/continue */
int count_special(void) {
    int total = 0;
    for (int i = 0; i < 20; i++) {
        int type;
        if (i % 6 == 0) type = 0;
        else if (i % 3 == 0) type = 1;
        else if (i % 2 == 0) type = 2;
        else type = 3;

        switch (type) {
            case 0: total += 10; break;
            case 1: total += 3; break;
            case 2: continue; /* skips the total+1 below */
            case 3: break;
        }
        total += 1;
    }
    return total;
    /* i=0:type0 total+=10+1=11
       i=1:type3 total+=1=12
       i=2:type2 continue -> skip total+1 -> 12
       i=3:type1 total+=3+1=16
       i=4:type2 continue -> 16
       i=5:type3 total+=1=17
       i=6:type0 total+=10+1=28
       i=7:type3 total+=1=29
       i=8:type2 continue -> 29
       i=9:type1 total+=3+1=33
       i=10:type2 continue -> 33
       i=11:type3 total+=1=34
       i=12:type0 total+=10+1=45
       i=13:type3 total+=1=46
       i=14:type2 continue -> 46
       i=15:type1 total+=3+1=50
       i=16:type2 continue -> 50
       i=17:type3 total+=1=51
       i=18:type0 total+=10+1=62
       i=19:type3 total+=1=63 */
}

int main(void) {
    int words, nums, spaces;
    count_tokens("hello 42 world", &words, &nums, &spaces);
    printf("%d %d %d\n", words, nums, spaces); // EXPECT: 2 1 2

    count_tokens("abc", &words, &nums, &spaces);
    printf("%d %d %d\n", words, nums, spaces); // EXPECT: 1 0 0

    printf("%d\n", dispatch(0, 10, 3));  // EXPECT: 13
    printf("%d\n", dispatch(1, 10, 3));  // EXPECT: 7
    printf("%d\n", dispatch(2, 10, 3));  // EXPECT: 30
    printf("%d\n", dispatch(3, 10, 3));  // EXPECT: 3
    printf("%d\n", dispatch(9, 10, 3));  // EXPECT: -999

    int a, b;
    printf("%d\n", find_pair(10, 11, &a, &b)); // EXPECT: 1
    printf("%d %d\n", a, b);                   // EXPECT: 1 10

    printf("%d\n", find_pair(5, 20, &a, &b)); // EXPECT: 0

    printf("%d\n", count_primes(10)); // EXPECT: 4
    printf("%d\n", count_primes(20)); // EXPECT: 8
    printf("%d\n", count_primes(2));  // EXPECT: 1

    printf("%d\n", run_length_count("aaabbc")); // EXPECT: 6

    int arr1[5] = {5, 3, 1, 4, 2};
    printf("%d\n", bubble_passes(arr1, 5)); // EXPECT: 4

    int arr2[4] = {1, 2, 3, 4};
    printf("%d\n", bubble_passes(arr2, 4)); // EXPECT: 1

    int arr3[6] = {10, 20, 5, 15, 7, 12};
    int idx = search_with_goto(arr3, 6, 15);
    printf("%d\n", idx); // EXPECT: 3
    idx = search_with_goto(arr3, 6, 99);
    printf("%d\n", idx); // EXPECT: -1

    printf("%d\n", count_special()); // EXPECT: 63

    return 0;
}
