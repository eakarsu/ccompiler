int printf(const char *fmt, ...);

// Test: Pointer difference and subtraction between pointers
// Tests ptrdiff with int arrays, char arrays, and struct arrays

struct Block {
    int x;
    int y;
    int z;
};

int main() {
    // Pointer difference with int array
    int arr[10];
    int *p1 = &arr[0];
    int *p2 = &arr[5];
    int diff1 = p2 - p1;
    // EXPECT: ptr diff int: 5
    printf("ptr diff int: %d\n", diff1);

    int *p3 = &arr[9];
    int diff2 = p3 - p1;
    // EXPECT: ptr diff int end: 9
    printf("ptr diff int end: %d\n", diff2);

    // Negative pointer difference
    int diff3 = p1 - p3;
    // EXPECT: ptr diff negative: -9
    printf("ptr diff negative: %d\n", diff3);

    // Pointer difference with char array
    char str[20];
    char *c1 = &str[0];
    char *c2 = &str[13];
    int diff4 = c2 - c1;
    // EXPECT: ptr diff char: 13
    printf("ptr diff char: %d\n", diff4);

    // Pointer arithmetic: advance and measure
    int data[8];
    int *start = data;
    int *end = data;
    int i;
    for (i = 0; i < 8; i++) {
        end = end + 1;
    }
    int diff5 = end - start;
    // EXPECT: ptr advance diff: 8
    printf("ptr advance diff: %d\n", diff5);

    // Pointer difference with struct array
    struct Block blocks[5];
    struct Block *bp1 = &blocks[1];
    struct Block *bp2 = &blocks[4];
    int diff6 = bp2 - bp1;
    // EXPECT: ptr diff struct: 3
    printf("ptr diff struct: %d\n", diff6);

    // Pointer subtraction after arithmetic
    int nums[10];
    int *pa = nums + 2;
    int *pb = nums + 7;
    int diff7 = pb - pa;
    // EXPECT: ptr arith diff: 5
    printf("ptr arith diff: %d\n", diff7);

    // Zero difference
    int *same1 = &arr[3];
    int *same2 = &arr[3];
    int diff8 = same2 - same1;
    // EXPECT: ptr diff zero: 0
    printf("ptr diff zero: %d\n", diff8);

    // Pointer comparison using subtraction
    int vals[6];
    int *lo = &vals[1];
    int *hi = &vals[4];
    int is_before = (lo - hi) < 0;
    // EXPECT: lo before hi: 1
    printf("lo before hi: %d\n", is_before);

    // Walk array with pointer difference tracking
    int walk[5];
    walk[0] = 10;
    walk[1] = 20;
    walk[2] = 30;
    walk[3] = 40;
    walk[4] = 50;
    int *wp = walk;
    int sum = 0;
    while (wp - walk < 5) {
        sum = sum + *wp;
        wp = wp + 1;
    }
    // EXPECT: walk sum: 150
    printf("walk sum: %d\n", sum);

    // Pointer difference in loop condition
    int count = 0;
    int buf[7];
    int *bstart = buf;
    int *bend = buf + 7;
    int *bcur = bstart;
    while (bcur - bstart < bend - bstart) {
        count = count + 1;
        bcur = bcur + 1;
    }
    // EXPECT: loop count: 7
    printf("loop count: %d\n", count);

    // EXPECT: done
    printf("done\n");

    return 0;
}
