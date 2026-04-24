int printf(const char *fmt, ...);
// EXPECT: === test_two_large_arrays ===\na[0]=0 b[0]=0\na[2500]=5000 b[2500]=7500\na[4999]=9998 b[4999]=14997\na[4095]=8190 b[4095]=12285\na[4096]=8192 b[4096]=12288\nsum_a=24995000 sum_b=37492500\ntwo_arrays: PASS\n=== test_large_with_struct ===\nbig[0]=1 big[3999]=4000 big[7999]=8000\nbig[4095]=4096 big[4096]=4097\nbig[7000]=7001\nsum_big=32004000\npad: 100 200 300 400\nlarge_with_struct: PASS\n=== test_boundary_access ===\narr[4094]=2049 arr[4095]=2048\narr[4096]=6144 arr[4097]=6145\narr[8190]=4097 arr[8191]=4096\nsum_gray=43865612\nboundary: PASS\nAll large stack frame tests passed!

// ---------------------------------------------------------------------------
// Stress test: large stack frames on ARM64
//
// ARM64 str/ldr instructions with immediate offsets are limited to 32760
// bytes (unsigned offset mode for 64-bit loads). The compiler must handle
// stack frame offsets beyond this limit using scratch registers, add-based
// addressing, or similar techniques.
//
// This test creates multiple functions with large stack-allocated arrays
// that push frame sizes well beyond the 32760-byte threshold.
// ---------------------------------------------------------------------------

struct padding {
    int x;
    int y;
    int z;
    int w;
    int extra[16];
};

// ---------------------------------------------------------------------------
// Test 1: Two arrays of 5000 ints each (40000 bytes total on the stack).
//
// With two arrays of 20000 bytes each, the second array's elements are
// accessed at offsets 20000-39996 from the base of the first array.
// When measured from the stack pointer, these offsets easily exceed 32760.
// ---------------------------------------------------------------------------
int test_two_large_arrays(void) {
    int a[5000];
    int b[5000];
    int i;
    int ok;
    int sum_a;
    int sum_b;

    // Fill both arrays with simple linear patterns
    for (i = 0; i < 5000; i++) {
        a[i] = i * 2;
    }
    for (i = 0; i < 5000; i++) {
        b[i] = i * 3;
    }

    printf("=== test_two_large_arrays ===\n");

    // Spot-check: beginning, middle, end
    printf("a[0]=%d b[0]=%d\n", a[0], b[0]);
    printf("a[2500]=%d b[2500]=%d\n", a[2500], b[2500]);
    printf("a[4999]=%d b[4999]=%d\n", a[4999], b[4999]);

    // Spot-check: near the 16380-byte offset boundary (index 4095)
    printf("a[4095]=%d b[4095]=%d\n", a[4095], b[4095]);
    printf("a[4096]=%d b[4096]=%d\n", a[4096], b[4096]);

    // Full checksum over all elements
    // sum(i*2, i=0..4999) = 2 * (4999 * 5000 / 2) = 24995000
    // sum(i*3, i=0..4999) = 3 * (4999 * 5000 / 2) = 37492500
    sum_a = 0;
    sum_b = 0;
    for (i = 0; i < 5000; i++) {
        sum_a = sum_a + a[i];
        sum_b = sum_b + b[i];
    }
    printf("sum_a=%d sum_b=%d\n", sum_a, sum_b);

    // Validate all results
    ok = 1;
    if (sum_a != 24995000) ok = 0;
    if (sum_b != 37492500) ok = 0;
    if (a[0] != 0) ok = 0;
    if (b[0] != 0) ok = 0;
    if (a[2500] != 5000) ok = 0;
    if (b[2500] != 7500) ok = 0;
    if (a[4999] != 9998) ok = 0;
    if (b[4999] != 14997) ok = 0;
    if (a[4095] != 8190) ok = 0;
    if (b[4095] != 12285) ok = 0;
    if (a[4096] != 8192) ok = 0;
    if (b[4096] != 12288) ok = 0;

    if (ok) {
        printf("two_arrays: PASS\n");
    } else {
        printf("two_arrays: FAIL\n");
    }
    return ok;
}

// ---------------------------------------------------------------------------
// Test 2: One very large array (8000 ints = 32000 bytes) combined with
// a struct to push total frame size beyond 32760 bytes.
//
// The struct adds 80 bytes (4 ints + 16-int array), and we also add
// additional local int variables. Total frame: ~32000 + 80 + 16+ = 32096+
// bytes, right around the critical boundary.
// ---------------------------------------------------------------------------
int test_large_with_struct(void) {
    int big[8000];
    struct padding pad;
    int i;
    int ok;
    int sum_big;
    int extra1;
    int extra2;
    int extra3;
    int extra4;

    // Initialize struct
    pad.x = 100;
    pad.y = 200;
    pad.z = 300;
    pad.w = 400;
    for (i = 0; i < 16; i++) {
        pad.extra[i] = i * 10;
    }

    // Extra local variables to increase frame size
    extra1 = 1111;
    extra2 = 2222;
    extra3 = 3333;
    extra4 = 4444;

    // Fill the large array: big[i] = i + 1
    for (i = 0; i < 8000; i++) {
        big[i] = i + 1;
    }

    printf("=== test_large_with_struct ===\n");

    // Spot-check positions
    printf("big[0]=%d big[3999]=%d big[7999]=%d\n",
           big[0], big[3999], big[7999]);

    // Near offset boundary
    printf("big[4095]=%d big[4096]=%d\n", big[4095], big[4096]);

    // Deep into the array
    printf("big[7000]=%d\n", big[7000]);

    // Full checksum: sum(1..8000) = 8000 * 8001 / 2 = 32004000
    sum_big = 0;
    for (i = 0; i < 8000; i++) {
        sum_big = sum_big + big[i];
    }
    printf("sum_big=%d\n", sum_big);

    // Verify struct survived the large array operations
    printf("pad: %d %d %d %d\n", pad.x, pad.y, pad.z, pad.w);

    // Validate everything
    ok = 1;
    if (big[0] != 1) ok = 0;
    if (big[3999] != 4000) ok = 0;
    if (big[7999] != 8000) ok = 0;
    if (big[4095] != 4096) ok = 0;
    if (big[4096] != 4097) ok = 0;
    if (big[7000] != 7001) ok = 0;
    if (sum_big != 32004000) ok = 0;
    if (pad.x != 100) ok = 0;
    if (pad.y != 200) ok = 0;
    if (pad.z != 300) ok = 0;
    if (pad.w != 400) ok = 0;
    if (pad.extra[0] != 0) ok = 0;
    if (pad.extra[15] != 150) ok = 0;
    if (extra1 != 1111) ok = 0;
    if (extra2 != 2222) ok = 0;
    if (extra3 != 3333) ok = 0;
    if (extra4 != 4444) ok = 0;

    if (ok) {
        printf("large_with_struct: PASS\n");
    } else {
        printf("large_with_struct: FAIL\n");
    }
    return ok;
}

// ---------------------------------------------------------------------------
// Test 3: Boundary access with Gray code pattern.
//
// Array of 9000 ints (36000 bytes). The pattern arr[i] = i ^ (i >> 1)
// produces Gray codes, giving non-trivial bit patterns at every index.
// We specifically test indices near:
//   - 4095/4096 (byte offsets 16380/16384 -- ARM64 12-bit immediate limit)
//   - 8190/8191 (byte offsets 32760/32764 -- ARM64 str/ldr offset limit)
// ---------------------------------------------------------------------------
int test_boundary_access(void) {
    int arr[9000];
    int i;
    int ok;
    int sum;

    // Fill with Gray code pattern: arr[i] = i XOR (i >> 1)
    for (i = 0; i < 9000; i++) {
        arr[i] = i ^ (i >> 1);
    }

    printf("=== test_boundary_access ===\n");

    // Indices 4094-4097: near the 12-bit immediate boundary
    // 4094 ^ 2047 = 2049, 4095 ^ 2047 = 2048
    // 4096 ^ 2048 = 6144, 4097 ^ 2048 = 6145
    printf("arr[4094]=%d arr[4095]=%d\n", arr[4094], arr[4095]);
    printf("arr[4096]=%d arr[4097]=%d\n", arr[4096], arr[4097]);

    // Indices 8190-8191: byte offsets 32760/32764 -- right at the
    // ARM64 unsigned immediate offset limit for str/ldr
    // 8190 ^ 4095 = 4097, 8191 ^ 4095 = 4096
    printf("arr[8190]=%d arr[8191]=%d\n", arr[8190], arr[8191]);

    // Full checksum over all 9000 elements
    // Pre-computed: sum of (i ^ (i>>1)) for i=0..8999 = 43865612
    sum = 0;
    for (i = 0; i < 9000; i++) {
        sum = sum + arr[i];
    }
    printf("sum_gray=%d\n", sum);

    // Validate
    ok = 1;
    if (arr[0] != 0) ok = 0;
    if (arr[1] != 1) ok = 0;
    if (arr[2] != 3) ok = 0;
    if (arr[3] != 2) ok = 0;
    if (arr[4094] != 2049) ok = 0;
    if (arr[4095] != 2048) ok = 0;
    if (arr[4096] != 6144) ok = 0;
    if (arr[4097] != 6145) ok = 0;
    if (arr[8190] != 4097) ok = 0;
    if (arr[8191] != 4096) ok = 0;
    if (sum != 43865612) ok = 0;

    if (ok) {
        printf("boundary: PASS\n");
    } else {
        printf("boundary: FAIL\n");
    }
    return ok;
}

// ---------------------------------------------------------------------------
// main: run all tests and report overall result
// ---------------------------------------------------------------------------
int main(void) {
    int pass;
    pass = 1;

    if (!test_two_large_arrays()) pass = 0;
    if (!test_large_with_struct()) pass = 0;
    if (!test_boundary_access()) pass = 0;

    if (pass) {
        printf("All large stack frame tests passed!\n");
    } else {
        printf("Some large stack frame tests FAILED!\n");
    }
    return 0;
}
