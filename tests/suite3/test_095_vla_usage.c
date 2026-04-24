int printf(const char *fmt, ...);

/* Variable Length Arrays: runtime-sized arrays on the stack */

/* VLA function parameter */
int vla_sum(int n, int arr[n]) {
    int s = 0, i;
    for (i = 0; i < n; i++) s += arr[i];
    return s;
}

/* Fill VLA in function */
void vla_fill(int n, int arr[n], int start) {
    int i;
    for (i = 0; i < n; i++) arr[i] = start + i;
}

/* VLA as local variable inside function */
int vla_local_sum(int n) {
    int arr[n];
    int i;
    for (i = 0; i < n; i++) arr[i] = i + 1;
    int s = 0;
    for (i = 0; i < n; i++) s += arr[i];
    return s;
}

/* Reverse a VLA in place */
void vla_reverse(int n, int arr[n]) {
    int i;
    for (i = 0; i < n / 2; i++) {
        int tmp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = tmp;
    }
}

/* VLA max element */
int vla_max(int n, int arr[n]) {
    int m = arr[0], i;
    for (i = 1; i < n; i++) if (arr[i] > m) m = arr[i];
    return m;
}

int main(void) {
    /* --- VLA size 5 --- */
    int n1 = 5;
    int vla1[n1];
    int i;
    for (i = 0; i < n1; i++) vla1[i] = i * 2;
    /* 0,2,4,6,8 */
    printf("%d\n", vla1[0]);    // EXPECT: 0
    printf("%d\n", vla1[1]);    // EXPECT: 2
    printf("%d\n", vla1[2]);    // EXPECT: 4
    printf("%d\n", vla1[3]);    // EXPECT: 6
    printf("%d\n", vla1[4]);    // EXPECT: 8

    /* --- VLA sum via vla_sum --- */
    printf("%d\n", vla_sum(n1, vla1));  // EXPECT: 20

    /* --- VLA size 1 --- */
    int n2 = 1;
    int vla2[n2];
    vla2[0] = 42;
    printf("%d\n", vla2[0]);    // EXPECT: 42
    printf("%d\n", vla_sum(1, vla2)); // EXPECT: 42

    /* --- VLA fill and check --- */
    int n3 = 6;
    int vla3[n3];
    vla_fill(n3, vla3, 10);
    /* 10,11,12,13,14,15 */
    printf("%d\n", vla3[0]);    // EXPECT: 10
    printf("%d\n", vla3[5]);    // EXPECT: 15
    printf("%d\n", vla_sum(n3, vla3)); // EXPECT: 75

    /* --- VLA local sum (1..n) --- */
    printf("%d\n", vla_local_sum(5));  // EXPECT: 15
    printf("%d\n", vla_local_sum(10)); // EXPECT: 55
    printf("%d\n", vla_local_sum(1));  // EXPECT: 1

    /* --- VLA reverse --- */
    int n4 = 5;
    int vla4[n4];
    for (i = 0; i < n4; i++) vla4[i] = (i + 1) * 10;
    /* Before: 10,20,30,40,50 */
    vla_reverse(n4, vla4);
    /* After: 50,40,30,20,10 */
    printf("%d\n", vla4[0]);    // EXPECT: 50
    printf("%d\n", vla4[1]);    // EXPECT: 40
    printf("%d\n", vla4[2]);    // EXPECT: 30
    printf("%d\n", vla4[3]);    // EXPECT: 20
    printf("%d\n", vla4[4]);    // EXPECT: 10

    /* --- sizeof VLA --- */
    int n5 = 7;
    int vla5[n5];
    int sz = (int)sizeof(vla5);
    printf("%d\n", sz);         // EXPECT: 28

    int n6 = 3;
    int vla6[n6];
    printf("%d\n", (int)sizeof(vla6));  // EXPECT: 12

    /* --- VLA max element --- */
    int n7 = 6;
    int vla7[n7];
    vla7[0] = 3; vla7[1] = 9; vla7[2] = 1;
    vla7[3] = 7; vla7[4] = 5; vla7[5] = 4;
    printf("%d\n", vla_max(n7, vla7)); // EXPECT: 9

    /* --- VLA with different runtime sizes in loop --- */
    int total = 0;
    int k;
    for (k = 1; k <= 4; k++) {
        int vla_loop[k];
        int j;
        for (j = 0; j < k; j++) vla_loop[j] = k;
        total += vla_sum(k, vla_loop);
        /* k=1: sum=1; k=2: sum=4; k=3: sum=9; k=4: sum=16 => total=30 */
    }
    printf("%d\n", total);      // EXPECT: 30

    /* --- VLA char array --- */
    int n8 = 8;
    char cvla[n8];
    for (i = 0; i < n8; i++) cvla[i] = (char)('A' + i);
    printf("%d\n", (int)sizeof(cvla)); // EXPECT: 8
    printf("%c\n", cvla[0]);    // EXPECT: A
    printf("%c\n", cvla[7]);    // EXPECT: H

    /* --- VLA nested: outer and inner --- */
    int outer = 3;
    int inner_size = 4;
    int outer_sums[3];
    int oi;
    for (oi = 0; oi < outer; oi++) {
        int inner_vla[inner_size];
        int j;
        for (j = 0; j < inner_size; j++) inner_vla[j] = oi * 10 + j;
        outer_sums[oi] = vla_sum(inner_size, inner_vla);
        /* oi=0: 0+1+2+3=6; oi=1: 10+11+12+13=46; oi=2: 20+21+22+23=86 */
    }
    printf("%d\n", outer_sums[0]); // EXPECT: 6
    printf("%d\n", outer_sums[1]); // EXPECT: 46
    printf("%d\n", outer_sums[2]); // EXPECT: 86

    /* --- VLA zero-init check (manually zeroed) --- */
    int n9 = 4;
    int vla9[n9];
    for (i = 0; i < n9; i++) vla9[i] = 0;
    int zsum = vla_sum(n9, vla9);
    printf("%d\n", zsum);       // EXPECT: 0

    return 0;
}
