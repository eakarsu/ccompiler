int printf(const char *fmt, ...);
// EXPECT: 1:0\n2:1\n3:3\n4:4\n5:8\n6:10\n7:1\n8:1\n9:0\n10:1\n11:14\n12:8\n13:11\n14:8\n15:1\n16:1\n17:0\n18:1\n19:0\n20:1\n21:0\n22:3\n23:7\n24:2\n25:3\n26:0\n27:20\n28:10\n29:99\n30:0\n31:3\n32:0\n33:1\n34:1\n35:0\n36:1\n37:0\n38:8\n39:6\n40:13\n41:160\n42:3\n43:2\n44:0\n45:5\n46:5\n47:0\n48:1\n49:0\n50:0\n51:8\n52:4\n53:1\n54:1\n55:0\n56:4\n57:8\n58:8\n59:1\n60:32\n61:0\n62:1\n63:3\n64:2\n65:7\n66:15\n67:42\n68:70\n69:50\n70:3
// =============================================================================
// TEST 024: Bit Manipulation Tricks
// =============================================================================
// Tests various bit manipulation operations: counting set bits, reversing
// bits, checking power of 2, swap without temp, finding missing number,
// bit parity, and more.
// =============================================================================

// -----------------------------------------------------------------------------
// Count set bits (popcount) - Brian Kernighan's method
// -----------------------------------------------------------------------------
int count_set_bits(int n) {
    int count;
    count = 0;
    // Work with positive representation
    if (n < 0) {
        // For negative, just count bits of the positive part
        // This is simplified to avoid unsigned issues
        n = -n;
    }
    while (n > 0) {
        n = n & (n - 1); // clear lowest set bit
        count = count + 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Count set bits (simple loop version)
// Works only for positive numbers
// -----------------------------------------------------------------------------
int count_bits_simple(int n) {
    int count;
    count = 0;
    while (n > 0) {
        if (n & 1) count = count + 1;
        n = n >> 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Get bit at position (0-indexed from right)
// -----------------------------------------------------------------------------
int get_bit(int n, int pos) {
    return (n >> pos) & 1;
}

// -----------------------------------------------------------------------------
// Set bit at position
// -----------------------------------------------------------------------------
int set_bit(int n, int pos) {
    return n | (1 << pos);
}

// -----------------------------------------------------------------------------
// Clear bit at position
// -----------------------------------------------------------------------------
int clear_bit(int n, int pos) {
    return n & (~(1 << pos));
}

// -----------------------------------------------------------------------------
// Toggle bit at position
// -----------------------------------------------------------------------------
int toggle_bit(int n, int pos) {
    return n ^ (1 << pos);
}

// -----------------------------------------------------------------------------
// Check if number is power of 2
// -----------------------------------------------------------------------------
int is_power_of_2(int n) {
    if (n <= 0) return 0;
    if ((n & (n - 1)) == 0) return 1;
    return 0;
}

// -----------------------------------------------------------------------------
// Find the highest set bit position (0-indexed)
// Returns -1 for 0
// -----------------------------------------------------------------------------
int highest_bit(int n) {
    int pos;
    if (n <= 0) return -1;
    pos = 0;
    while (n > 1) {
        n = n >> 1;
        pos = pos + 1;
    }
    return pos;
}

// -----------------------------------------------------------------------------
// Find the lowest set bit position (0-indexed)
// Returns -1 for 0
// -----------------------------------------------------------------------------
int lowest_bit(int n) {
    int pos;
    if (n == 0) return -1;
    if (n < 0) n = -n;
    pos = 0;
    while ((n & 1) == 0) {
        n = n >> 1;
        pos = pos + 1;
    }
    return pos;
}

// -----------------------------------------------------------------------------
// Swap two values without temp variable using XOR
// Returns the swapped pair via array
// -----------------------------------------------------------------------------
int swap_result[2];

void swap_xor(int a, int b) {
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
    swap_result[0] = a;
    swap_result[1] = b;
}

// -----------------------------------------------------------------------------
// Find missing number from 0 to n (one number missing)
// Uses XOR property: a ^ a = 0
// -----------------------------------------------------------------------------
int missing_arr[100];

int find_missing(int n) {
    int xor_all;
    int xor_arr;
    int i;
    xor_all = 0;
    for (i = 0; i <= n; i = i + 1) {
        xor_all = xor_all ^ i;
    }
    xor_arr = 0;
    for (i = 0; i < n; i = i + 1) {
        xor_arr = xor_arr ^ missing_arr[i];
    }
    return xor_all ^ xor_arr;
}

// -----------------------------------------------------------------------------
// Bit parity: 1 if odd number of set bits, 0 if even
// -----------------------------------------------------------------------------
int bit_parity(int n) {
    int p;
    if (n < 0) n = -n;
    p = 0;
    while (n > 0) {
        p = p ^ (n & 1);
        n = n >> 1;
    }
    return p;
}

// -----------------------------------------------------------------------------
// Reverse bits of a number (within given bit width)
// -----------------------------------------------------------------------------
int reverse_bits(int n, int width) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < width; i = i + 1) {
        result = result << 1;
        result = result | (n & 1);
        n = n >> 1;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Count trailing zeros
// -----------------------------------------------------------------------------
int count_trailing_zeros(int n) {
    int count;
    if (n == 0) return 32;
    count = 0;
    while ((n & 1) == 0) {
        count = count + 1;
        n = n >> 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Count leading zeros (for positive ints up to 31 bits)
// -----------------------------------------------------------------------------
int count_leading_zeros(int n) {
    int count;
    int bits;
    if (n == 0) return 31;
    count = 0;
    bits = 30; // check from bit 30 down
    while (bits >= 0) {
        if ((n >> bits) & 1) return count;
        count = count + 1;
        bits = bits - 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Absolute value using bit tricks
// For 32-bit ints
// -----------------------------------------------------------------------------
int bit_abs(int n) {
    int mask;
    mask = n >> 31;
    return (n ^ mask) - mask;
}

// -----------------------------------------------------------------------------
// Min of two numbers using bit tricks
// -----------------------------------------------------------------------------
int bit_min(int a, int b) {
    if (a < b) return a;
    return b;
}

// -----------------------------------------------------------------------------
// Max of two numbers
// -----------------------------------------------------------------------------
int bit_max(int a, int b) {
    if (a > b) return a;
    return b;
}

// -----------------------------------------------------------------------------
// Check if two numbers have opposite signs
// -----------------------------------------------------------------------------
int opposite_signs(int a, int b) {
    if ((a ^ b) < 0) return 1;
    return 0;
}

// -----------------------------------------------------------------------------
// Turn off the rightmost set bit
// -----------------------------------------------------------------------------
int turn_off_rightmost(int n) {
    return n & (n - 1);
}

// -----------------------------------------------------------------------------
// Isolate the rightmost set bit
// -----------------------------------------------------------------------------
int isolate_rightmost(int n) {
    return n & (-n);
}

// -----------------------------------------------------------------------------
// Check if number has alternating bit pattern
// e.g., 5 = 101, 10 = 1010
// -----------------------------------------------------------------------------
int has_alternating_bits(int n) {
    int prev;
    int curr;
    if (n <= 0) return 0;
    prev = n & 1;
    n = n >> 1;
    while (n > 0) {
        curr = n & 1;
        if (curr == prev) return 0;
        prev = curr;
        n = n >> 1;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Count bits to flip to convert a to b
// (Hamming distance)
// -----------------------------------------------------------------------------
int bits_to_flip(int a, int b) {
    int diff;
    diff = a ^ b;
    return count_bits_simple(diff);
}

// -----------------------------------------------------------------------------
// Next power of 2 >= n
// -----------------------------------------------------------------------------
int next_power_of_2(int n) {
    int p;
    if (n <= 1) return 1;
    p = 1;
    while (p < n) {
        p = p << 1;
    }
    return p;
}

// -----------------------------------------------------------------------------
// Is bit palindrome within given width
// -----------------------------------------------------------------------------
int is_bit_palindrome(int n, int width) {
    int rev;
    rev = reverse_bits(n, width);
    return (n == rev) ? 1 : 0;
}

// -----------------------------------------------------------------------------
// Gray code: convert binary to Gray code
// -----------------------------------------------------------------------------
int to_gray(int n) {
    return n ^ (n >> 1);
}

// -----------------------------------------------------------------------------
// Gray code: convert Gray code back to binary
// -----------------------------------------------------------------------------
int from_gray(int gray) {
    int n;
    n = gray;
    gray = gray >> 1;
    while (gray > 0) {
        n = n ^ gray;
        gray = gray >> 1;
    }
    return n;
}

// -----------------------------------------------------------------------------
// Multiply by 7 using bit shifts
// n * 7 = n * 8 - n = (n << 3) - n
// -----------------------------------------------------------------------------
int multiply_by_7(int n) {
    return (n << 3) - n;
}

// -----------------------------------------------------------------------------
// Divide by 2 using shift
// -----------------------------------------------------------------------------
int divide_by_2(int n) {
    return n >> 1;
}

// -----------------------------------------------------------------------------
// Check if bit at position is set
// -----------------------------------------------------------------------------
int is_bit_set(int n, int pos) {
    return (n >> pos) & 1;
}

// =============================================================================
// Main test function
// =============================================================================
int main(void) {
    int test_num;
    int v;
    int i;

    test_num = 0;

    // ---- Count set bits ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_set_bits(0)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_set_bits(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_set_bits(7)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_set_bits(15)); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_set_bits(255)); // 8

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_bits_simple(1023)); // 10

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_bits_simple(256)); // 1

    // ---- Get/Set/Clear/Toggle bit ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, get_bit(10, 1)); // 1 (10=1010)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, get_bit(10, 2)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, get_bit(10, 3)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, set_bit(10, 2)); // 14 (1110)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, clear_bit(10, 1)); // 8 (1000)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, toggle_bit(10, 0)); // 11 (1011)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, toggle_bit(10, 1)); // 8 (1000)

    // ---- Power of 2 checks ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_power_of_2(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_power_of_2(2)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_power_of_2(3)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_power_of_2(16)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_power_of_2(0)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_power_of_2(1024)); // 1

    // ---- Highest/lowest bit ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, highest_bit(1)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, highest_bit(8)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, highest_bit(255)); // 7

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lowest_bit(12)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lowest_bit(8)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, lowest_bit(1)); // 0

    // ---- Swap without temp ----
    swap_xor(10, 20);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, swap_result[0]); // 20

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, swap_result[1]); // 10

    swap_xor(0, 99);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, swap_result[0]); // 99

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, swap_result[1]); // 0

    // ---- Find missing number ----
    // Array [0,1,2,4,5] - missing 3
    missing_arr[0] = 0; missing_arr[1] = 1; missing_arr[2] = 2;
    missing_arr[3] = 4; missing_arr[4] = 5;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, find_missing(5)); // 3

    // Array [1,2,3,4] - missing 0
    missing_arr[0] = 1; missing_arr[1] = 2; missing_arr[2] = 3; missing_arr[3] = 4;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, find_missing(4)); // 0

    // Array [0] - missing 1
    missing_arr[0] = 0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, find_missing(1)); // 1

    // ---- Bit parity ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, bit_parity(7)); // 1 (3 bits = odd)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, bit_parity(15)); // 0 (4 bits = even)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, bit_parity(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, bit_parity(0)); // 0

    // ---- Reverse bits ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, reverse_bits(1, 4)); // 8 (0001 -> 1000)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, reverse_bits(6, 4)); // 6 (0110 -> 0110)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, reverse_bits(11, 4)); // 13 (1011 -> 1101)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, reverse_bits(5, 8)); // 160 (00000101 -> 10100000)

    // ---- Trailing/leading zeros ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_trailing_zeros(8)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_trailing_zeros(12)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_trailing_zeros(1)); // 0

    // ---- Bit abs ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, bit_abs(5)); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, bit_abs(-5)); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, bit_abs(0)); // 0

    // ---- Opposite signs ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, opposite_signs(5, -3)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, opposite_signs(5, 3)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, opposite_signs(-5, -3)); // 0

    // ---- Turn off / isolate rightmost ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, turn_off_rightmost(12)); // 8 (1100 -> 1000)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, isolate_rightmost(12)); // 4 (1100 -> 0100)

    // ---- Alternating bits ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_alternating_bits(5)); // 1 (101)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_alternating_bits(10)); // 1 (1010)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_alternating_bits(7)); // 0 (111)

    // ---- Hamming distance ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, bits_to_flip(10, 20)); // bits differ

    // ---- Next power of 2 ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, next_power_of_2(5)); // 8

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, next_power_of_2(8)); // 8

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, next_power_of_2(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, next_power_of_2(17)); // 32

    // ---- Gray code ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, to_gray(0)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, to_gray(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, to_gray(2)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, to_gray(3)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, from_gray(to_gray(7))); // 7

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, from_gray(to_gray(15))); // 15

    // ---- Multiply/divide by shifts ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, multiply_by_7(6)); // 42

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, multiply_by_7(10)); // 70

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, divide_by_2(100)); // 50

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, divide_by_2(7)); // 3

    return 0;
}
