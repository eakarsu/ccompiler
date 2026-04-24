int printf(const char *fmt, ...);
// EXPECT: === McCarthy 91 ===\nM(85)=91\nM(90)=91\nM(95)=91\nM(100)=91\nM(105)=95\n=== Tak ===\ntak(7,4,2)=4\ntak(10,5,1)=5\ntak(6,4,2)=3\n=== Digital operations ===\ndsum(12345)=15\ndsum(9999)=36\ndigits(12345)=5\ndigits(1)=1\ndigits(1000000)=7\nrev(12345)=54321\nrev(1000)=1\npalindrome(12321)=1\npalindrome(12345)=0\npalindrome(1001)=1\n=== Array recursion ===\nsum=39\nmax=9\nfind(7)=3\nfind(1)=0\nfind(15)=7\nfind(6)=-1\n=== Ackermann bounded ===\nack(0,0)=1\nack(1,1)=3\nack(2,2)=7\nack(3,3)=61
// Test 249: Mixed recursion patterns

int ackermann_bounded(int m, int n, int limit) {
    if (limit <= 0) return -1;
    if (m == 0) return n + 1;
    if (n == 0) return ackermann_bounded(m - 1, 1, limit - 1);
    int inner = ackermann_bounded(m, n - 1, limit - 1);
    if (inner < 0) return -1;
    return ackermann_bounded(m - 1, inner, limit - 1);
}

int mccarthy91(int n) {
    if (n > 100) return n - 10;
    return mccarthy91(mccarthy91(n + 11));
}

int tak(int x, int y, int z) {
    if (x <= y) return z;
    return tak(tak(x - 1, y, z), tak(y - 1, z, x), tak(z - 1, x, y));
}

int flatten(int n) {
    if (n < 10) return n;
    int last = n - (n / 10) * 10;
    return flatten(n / 10) + last;
}

int digital_sum(int n) {
    if (n < 0) n = -n;
    if (n < 10) return n;
    return (n - (n / 10) * 10) + digital_sum(n / 10);
}

int count_digits(int n) {
    if (n < 0) n = -n;
    if (n < 10) return 1;
    return 1 + count_digits(n / 10);
}

int reverse_number(int n, int acc) {
    if (n == 0) return acc;
    return reverse_number(n / 10, acc * 10 + n - (n / 10) * 10);
}

int is_palindrome_num(int n) {
    if (n < 0) return 0;
    return n == reverse_number(n, 0);
}

int sum_array_recursive(int *arr, int n) {
    if (n <= 0) return 0;
    return arr[n - 1] + sum_array_recursive(arr, n - 1);
}

int max_array_recursive(int *arr, int n) {
    if (n == 1) return arr[0];
    int sub_max = max_array_recursive(arr, n - 1);
    if (arr[n - 1] > sub_max) return arr[n - 1];
    return sub_max;
}

int binary_search_recursive(int *arr, int lo, int hi, int target) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] > target) return binary_search_recursive(arr, lo, mid - 1, target);
    return binary_search_recursive(arr, mid + 1, hi, target);
}

int main(void) {
    printf("=== McCarthy 91 ===\n");
    int i = 85;
    while (i <= 105) {
        printf("M(%d)=%d\n", i, mccarthy91(i));
        i = i + 5;
    }

    printf("=== Tak ===\n");
    printf("tak(7,4,2)=%d\n", tak(7, 4, 2));
    printf("tak(10,5,1)=%d\n", tak(10, 5, 1));
    printf("tak(6,4,2)=%d\n", tak(6, 4, 2));

    printf("=== Digital operations ===\n");
    printf("dsum(12345)=%d\n", digital_sum(12345));
    printf("dsum(9999)=%d\n", digital_sum(9999));
    printf("digits(12345)=%d\n", count_digits(12345));
    printf("digits(1)=%d\n", count_digits(1));
    printf("digits(1000000)=%d\n", count_digits(1000000));
    printf("rev(12345)=%d\n", reverse_number(12345, 0));
    printf("rev(1000)=%d\n", reverse_number(1000, 0));
    printf("palindrome(12321)=%d\n", is_palindrome_num(12321));
    printf("palindrome(12345)=%d\n", is_palindrome_num(12345));
    printf("palindrome(1001)=%d\n", is_palindrome_num(1001));

    printf("=== Array recursion ===\n");
    int arr[8];
    arr[0] = 5; arr[1] = 2; arr[2] = 8; arr[3] = 1;
    arr[4] = 9; arr[5] = 3; arr[6] = 7; arr[7] = 4;
    printf("sum=%d\n", sum_array_recursive(arr, 8));
    printf("max=%d\n", max_array_recursive(arr, 8));

    int sorted[8];
    sorted[0] = 1; sorted[1] = 3; sorted[2] = 5; sorted[3] = 7;
    sorted[4] = 9; sorted[5] = 11; sorted[6] = 13; sorted[7] = 15;
    printf("find(7)=%d\n", binary_search_recursive(sorted, 0, 7, 7));
    printf("find(1)=%d\n", binary_search_recursive(sorted, 0, 7, 1));
    printf("find(15)=%d\n", binary_search_recursive(sorted, 0, 7, 15));
    printf("find(6)=%d\n", binary_search_recursive(sorted, 0, 7, 6));

    printf("=== Ackermann bounded ===\n");
    printf("ack(0,0)=%d\n", ackermann_bounded(0, 0, 100));
    printf("ack(1,1)=%d\n", ackermann_bounded(1, 1, 100));
    printf("ack(2,2)=%d\n", ackermann_bounded(2, 2, 100));
    printf("ack(3,3)=%d\n", ackermann_bounded(3, 3, 10000));

    return 0;
}
