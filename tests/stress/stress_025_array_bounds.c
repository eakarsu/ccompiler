int printf(const char *fmt, ...);

// Array access patterns: first/last of large arrays, pointer to last, reverse traversal,
// stride access, interleaved reads, pointer copy, array of arrays.

int g_arr[200];

void fill_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i * 3 + 1;
    }
}

int sum_array(int *arr, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

int test_first_last(void) {
    int arr[100];
    fill_array(arr, 100);
    // arr[0] = 0*3+1 = 1, arr[99] = 99*3+1 = 298
    return arr[0] * 1000 + arr[99];
}

int test_pointer_to_last(void) {
    int arr[100];
    fill_array(arr, 100);
    int *last = &arr[99];
    int *first = &arr[0];
    // Pointer difference
    long diff = last - first;  // should be 99
    return (int)diff * 10 + *last % 10;
    // *last = 298, 298%10 = 8 => 99*10+8 = 998
}

int test_reverse_traversal(void) {
    int arr[50];
    int i;
    for (i = 0; i < 50; i++) arr[i] = i;

    // Sum in reverse
    int sum = 0;
    for (i = 49; i >= 0; i--) {
        sum += arr[i];
    }
    // 0+1+...+49 = 1225
    return sum;
}

int test_stride_access(void) {
    int arr[100];
    int i;
    for (i = 0; i < 100; i++) arr[i] = i;

    // Sum every other element (stride 2)
    int sum = 0;
    for (i = 0; i < 100; i += 2) {
        sum += arr[i];
    }
    // 0+2+4+...+98 = 2*(0+1+2+...+49) = 2*1225 = 2450
    return sum;
}

int test_interleaved_read(void) {
    int a[50], b[50];
    int i;
    for (i = 0; i < 50; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    // Read alternating from a and b
    int sum = 0;
    for (i = 0; i < 50; i++) {
        if (i % 2 == 0) {
            sum += a[i];
        } else {
            sum += b[i];
        }
    }
    // even i: 0+2+4+...+48 = 2*12*25 = 600. Wait, let me recalc.
    // even indices: 0,2,4,...,48 -> a[i] = i, sum = 0+2+4+...+48 = 2*(0+1+2+...+24) = 2*300 = 600
    // odd indices: 1,3,5,...,49 -> b[i] = i*2, sum = 2+6+10+...+98 = 2*(1+3+5+...+49) = 2*625 = 1250
    // total = 600 + 1250 = 1850
    return sum;
}

int test_pointer_copy(void) {
    int src[30], dst[30];
    int i;
    for (i = 0; i < 30; i++) src[i] = i * i;

    // Copy via pointer loop
    int *sp = src;
    int *dp = dst;
    for (i = 0; i < 30; i++) {
        *dp++ = *sp++;
    }

    // Verify
    int ok = 1;
    for (i = 0; i < 30; i++) {
        if (dst[i] != i * i) ok = 0;
    }

    // dst[29] = 29*29 = 841
    return ok * 1000 + dst[29];
}

int test_array_of_arrays(void) {
    int arr[5][10];
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 10; j++) {
            arr[i][j] = i * 100 + j;
        }
    }

    // Access corners
    int tl = arr[0][0];      // 0
    int tr = arr[0][9];      // 9
    int bl = arr[4][0];      // 400
    int br = arr[4][9];      // 409
    return tl + tr + bl + br; // 0 + 9 + 400 + 409 = 818
}

int test_global_array(void) {
    int i;
    for (i = 0; i < 200; i++) {
        g_arr[i] = i;
    }
    // Sum first 10 and last 10
    int sum = 0;
    for (i = 0; i < 10; i++) sum += g_arr[i];
    for (i = 190; i < 200; i++) sum += g_arr[i];
    // first 10: 0+1+...+9 = 45
    // last 10: 190+191+...+199 = 1945
    // total = 1990
    return sum;
}

int test_negative_index_via_pointer(void) {
    int arr[20];
    int i;
    for (i = 0; i < 20; i++) arr[i] = i * 5;

    // Point to middle, access with negative offset
    int *mid = &arr[10];
    int v_neg3 = *(mid - 3);  // arr[7] = 35
    int v_neg10 = *(mid - 10); // arr[0] = 0
    int v_pos5 = *(mid + 5);  // arr[15] = 75
    return v_neg3 + v_neg10 + v_pos5; // 35 + 0 + 75 = 110
}

int test_array_as_param(int arr[], int n) {
    int max = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

int test_char_array(void) {
    char msg[20];
    msg[0] = 'H';
    msg[1] = 'i';
    msg[2] = '!';
    msg[3] = '\0';

    // String length via pointer
    int len = 0;
    char *p = msg;
    while (*p) { len++; p++; }

    // Sum of chars
    int sum = msg[0] + msg[1] + msg[2];
    // 'H'=72, 'i'=105, '!'=33 => 210

    return len * 1000 + sum;
}

int test_overlapping_copy(void) {
    int arr[20];
    int i;
    for (i = 0; i < 20; i++) arr[i] = i;

    // Copy forward: arr[5..14] = arr[0..9]
    for (i = 0; i < 10; i++) {
        arr[i + 5] = arr[i];
    }
    // arr: 0,1,2,3,4, 0,1,2,3,4, 0,1,2,3,4, 15,16,17,18,19
    // Wait, overlapping: arr[5]=arr[0]=0, arr[6]=arr[1]=1, ..., arr[9]=arr[4]=4
    // arr[10]=arr[5]=0 (already overwritten!), arr[11]=arr[6]=1, ...
    // arr[10]=0, arr[11]=1, arr[12]=2, arr[13]=3, arr[14]=4
    return arr[5] * 100 + arr[10] * 10 + arr[14];
    // 0*100 + 0*10 + 4 = 4
}

int test_large_local_array(void) {
    int arr[500];
    int i;
    for (i = 0; i < 500; i++) {
        arr[i] = i % 7;
    }
    // Sum: each full cycle of 7 has sum 0+1+2+3+4+5+6=21
    // 500/7 = 71 full cycles, 500%7 = 3 remainder (0+1+2=3)
    // Total = 71*21 + 3 = 1491 + 3 = 1494
    int sum = 0;
    for (i = 0; i < 500; i++) sum += arr[i];
    return sum;
}

int main(void) {
    printf("%d\n", test_first_last());
    // EXPECT: 1298

    printf("%d\n", test_pointer_to_last());
    // EXPECT: 998

    printf("%d\n", test_reverse_traversal());
    // EXPECT: 1225

    printf("%d\n", test_stride_access());
    // EXPECT: 2450

    printf("%d\n", test_interleaved_read());
    // EXPECT: 1850

    printf("%d\n", test_pointer_copy());
    // EXPECT: 1841

    printf("%d\n", test_array_of_arrays());
    // EXPECT: 818

    printf("%d\n", test_global_array());
    // EXPECT: 1990

    printf("%d\n", test_negative_index_via_pointer());
    // EXPECT: 110

    int arr[] = {3, 7, 2, 9, 1, 8, 4, 6, 5, 0};
    printf("%d\n", test_array_as_param(arr, 10));
    // EXPECT: 9

    printf("%d\n", test_char_array());
    // EXPECT: 3210

    printf("%d\n", test_overlapping_copy());
    // EXPECT: 4

    printf("%d\n", test_large_local_array());
    // EXPECT: 1494

    printf("ALL PASSED\n");
    // EXPECT: ALL PASSED
    return 0;
}
