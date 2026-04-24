int printf(const char *fmt, ...);

// XOR swap and XOR-based tricks

// XOR swap two values via pointers
void xor_swap(int *a, int *b) {
    if (a == b) return;
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

// XOR swap elements in an array
void xor_swap_arr(int *arr, int i, int j) {
    if (i == j) return;
    arr[i] = arr[i] ^ arr[j];
    arr[j] = arr[i] ^ arr[j];
    arr[i] = arr[i] ^ arr[j];
}

// Find the single element that appears once (all others appear twice)
int find_single(int *arr, int n) {
    int result = 0;
    int i = 0;
    while (i < n) {
        result = result ^ arr[i];
        i = i + 1;
    }
    return result;
}

// Find two missing numbers using XOR
// Given array contains 1..n except two values
// Returns via pointers
void find_two_missing(int *arr, int len, int n, int *m1, int *m2) {
    // XOR all array elements and all 1..n
    int xor_all = 0;
    int i = 0;
    while (i < len) {
        xor_all = xor_all ^ arr[i];
        i = i + 1;
    }
    i = 1;
    while (i <= n) {
        xor_all = xor_all ^ i;
        i = i + 1;
    }
    // xor_all = missing1 ^ missing2
    // Find a set bit to partition
    int set_bit = xor_all & (-xor_all);

    int group1 = 0;
    int group2 = 0;
    i = 0;
    while (i < len) {
        if (arr[i] & set_bit) {
            group1 = group1 ^ arr[i];
        } else {
            group2 = group2 ^ arr[i];
        }
        i = i + 1;
    }
    i = 1;
    while (i <= n) {
        if (i & set_bit) {
            group1 = group1 ^ i;
        } else {
            group2 = group2 ^ i;
        }
        i = i + 1;
    }
    if (group1 < group2) {
        *m1 = group1;
        *m2 = group2;
    } else {
        *m1 = group2;
        *m2 = group1;
    }
}

// XOR linked list simulation using array
// Each node stores prev_index ^ next_index
struct XorNode {
    int data;
    int xor_ptr;
};

void build_xor_list(struct XorNode *nodes, int *data, int n) {
    int i = 0;
    while (i < n) {
        nodes[i].data = data[i];
        int prev = -1;
        int next = -1;
        if (i > 0) prev = i - 1;
        if (i < n - 1) next = i + 1;
        // Use index+1 so 0 means null, actual indices are 1-based
        int p = (prev == -1) ? 0 : prev + 1;
        int nx = (next == -1) ? 0 : next + 1;
        nodes[i].xor_ptr = p ^ nx;
        i = i + 1;
    }
}

// Traverse XOR list forward, store results in output array
void traverse_xor_list(struct XorNode *nodes, int n, int *out) {
    if (n == 0) return;
    int prev_idx = 0;  // 0 = null (1-based)
    int curr_idx = 1;  // first node is index 1 (1-based)
    int count = 0;
    while (curr_idx != 0 && count < n) {
        int real = curr_idx - 1;
        out[count] = nodes[real].data;
        int next_idx = prev_idx ^ nodes[real].xor_ptr;
        prev_idx = curr_idx;
        curr_idx = next_idx;
        count = count + 1;
    }
}

// XOR-based simple encryption/decryption
void xor_encrypt(int *data, int len, int key) {
    int i = 0;
    while (i < len) {
        data[i] = data[i] ^ key;
        i = i + 1;
    }
}

int main() {
    // XOR swap
    int a = 42;
    int b = 99;
    xor_swap(&a, &b);
    printf("swap: %d %d\n", a, b);
    // EXPECT: swap: 99 42

    // swap same value
    int c = 7;
    int d = 7;
    xor_swap(&c, &d);
    printf("same: %d %d\n", c, d);
    // EXPECT: same: 7 7

    // swap with zero
    int e = 0;
    int f = 123;
    xor_swap(&e, &f);
    printf("zero: %d %d\n", e, f);
    // EXPECT: zero: 123 0

    // array swap
    int arr1[5];
    arr1[0] = 10; arr1[1] = 20; arr1[2] = 30; arr1[3] = 40; arr1[4] = 50;
    xor_swap_arr(arr1, 0, 4);
    xor_swap_arr(arr1, 1, 3);
    printf("rev: %d %d %d %d %d\n", arr1[0], arr1[1], arr1[2], arr1[3], arr1[4]);
    // EXPECT: rev: 50 40 30 20 10

    // find single
    int arr2[7];
    arr2[0] = 3; arr2[1] = 5; arr2[2] = 3; arr2[3] = 7;
    arr2[4] = 5; arr2[5] = 9; arr2[6] = 9;
    printf("single=%d\n", find_single(arr2, 7));
    // EXPECT: single=7

    int arr3[5];
    arr3[0] = 1; arr3[1] = 2; arr3[2] = 1; arr3[3] = 2; arr3[4] = 99;
    printf("single2=%d\n", find_single(arr3, 5));
    // EXPECT: single2=99

    // find two missing from 1..8
    // present: 1,2,4,5,7,8  missing: 3,6
    int arr4[6];
    arr4[0] = 1; arr4[1] = 2; arr4[2] = 4;
    arr4[3] = 5; arr4[4] = 7; arr4[5] = 8;
    int m1 = 0;
    int m2 = 0;
    find_two_missing(arr4, 6, 8, &m1, &m2);
    printf("missing: %d %d\n", m1, m2);
    // EXPECT: missing: 3 6

    // XOR list traversal
    struct XorNode nodes[5];
    int data[5];
    data[0] = 10; data[1] = 20; data[2] = 30; data[3] = 40; data[4] = 50;
    build_xor_list(nodes, data, 5);
    int out[5];
    traverse_xor_list(nodes, 5, out);
    printf("xlist: %d %d %d %d %d\n", out[0], out[1], out[2], out[3], out[4]);
    // EXPECT: xlist: 10 20 30 40 50

    // XOR encryption / decryption roundtrip
    int msg[4];
    msg[0] = 72; msg[1] = 101; msg[2] = 108; msg[3] = 111;
    int key = 42;
    xor_encrypt(msg, 4, key);
    printf("enc: %d %d %d %d\n", msg[0], msg[1], msg[2], msg[3]);
    // EXPECT: enc: 98 79 70 69
    xor_encrypt(msg, 4, key);
    printf("dec: %d %d %d %d\n", msg[0], msg[1], msg[2], msg[3]);
    // EXPECT: dec: 72 101 108 111

    printf("done\n");
    // EXPECT: done
    return 0;
}
