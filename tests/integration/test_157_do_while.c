int printf(const char *fmt, ...);
// EXPECT: Digit counts:\ndigits(0)=1\ndigits(5)=1\ndigits(42)=2\ndigits(999)=3\ndigits(10000)=5\ndigits(-123)=3\nReversed:\nrev(0)=0\nrev(5)=5\nrev(42)=24\nrev(999)=999\nrev(10000)=1\nrev(-123)=-321\nDigit sums:\ndsum(0)=0\ndsum(5)=5\ndsum(42)=6\ndsum(999)=27\ndsum(10000)=1\ndsum(-123)=6\nGCD tests:\ngcd(12,8)=4\ngcd(100,75)=25\ngcd(17,13)=1\ngcd(48,36)=12\nPowers of 2 to reach 1000: steps=10 n=1024
int count_digits(int n) {
    int count = 0;
    if (n < 0) n = -n;
    do {
        count++;
        n = n / 10;
    } while (n > 0);
    return count;
}

int reverse_num(int n) {
    int rev = 0;
    int neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    do {
        rev = rev * 10 + n % 10;
        n = n / 10;
    } while (n > 0);
    if (neg) rev = -rev;
    return rev;
}

int sum_digits(int n) {
    int sum = 0;
    if (n < 0) n = -n;
    do {
        sum = sum + n % 10;
        n = n / 10;
    } while (n > 0);
    return sum;
}

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    do {
        int temp = b;
        b = a % b;
        a = temp;
    } while (b != 0);
    return a;
}

int main(void) {
    printf("Digit counts:\n");
    int nums[6];
    nums[0] = 0;
    nums[1] = 5;
    nums[2] = 42;
    nums[3] = 999;
    nums[4] = 10000;
    nums[5] = -123;
    int i;
    for (i = 0; i < 6; i++) {
        printf("digits(%d)=%d\n", nums[i], count_digits(nums[i]));
    }

    printf("Reversed:\n");
    for (i = 0; i < 6; i++) {
        printf("rev(%d)=%d\n", nums[i], reverse_num(nums[i]));
    }

    printf("Digit sums:\n");
    for (i = 0; i < 6; i++) {
        printf("dsum(%d)=%d\n", nums[i], sum_digits(nums[i]));
    }

    printf("GCD tests:\n");
    printf("gcd(12,8)=%d\n", gcd(12, 8));
    printf("gcd(100,75)=%d\n", gcd(100, 75));
    printf("gcd(17,13)=%d\n", gcd(17, 13));
    printf("gcd(48,36)=%d\n", gcd(48, 36));

    int n = 1;
    int steps = 0;
    do {
        n = n * 2;
        steps++;
    } while (n < 1000);
    printf("Powers of 2 to reach 1000: steps=%d n=%d\n", steps, n);

    return 0;
}
