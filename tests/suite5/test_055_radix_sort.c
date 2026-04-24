int printf(const char *fmt, ...);

int get_digit(int num, int exp) {
    return (num / exp) % 10;
}

int get_max(int arr[], int n) {
    int max;
    int i;
    max = arr[0];
    i = 1;
    while (i < n) {
        if (arr[i] > max) {
            max = arr[i];
        }
        i = i + 1;
    }
    return max;
}

void counting_sort_by_digit(int arr[], int n, int exp) {
    int output[20];
    int count[10];
    int i;
    int digit;

    i = 0;
    while (i < 10) {
        count[i] = 0;
        i = i + 1;
    }

    i = 0;
    while (i < n) {
        digit = get_digit(arr[i], exp);
        count[digit] = count[digit] + 1;
        i = i + 1;
    }

    i = 1;
    while (i < 10) {
        count[i] = count[i] + count[i - 1];
        i = i + 1;
    }

    i = n - 1;
    while (i >= 0) {
        digit = get_digit(arr[i], exp);
        count[digit] = count[digit] - 1;
        output[count[digit]] = arr[i];
        i = i - 1;
    }

    i = 0;
    while (i < n) {
        arr[i] = output[i];
        i = i + 1;
    }
}

void radix_sort(int arr[], int n) {
    int max;
    int exp;
    max = get_max(arr, n);
    exp = 1;
    while (max / exp > 0) {
        counting_sort_by_digit(arr, n, exp);
        exp = exp * 10;
    }
}

int is_sorted(int arr[], int n) {
    int i;
    i = 0;
    while (i < n - 1) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

void print_array(int arr[], int n) {
    int i;
    i = 0;
    while (i < n) {
        printf("%d ", arr[i]);
        i = i + 1;
    }
}

int count_digits(int n) {
    int d;
    if (n == 0) return 1;
    d = 0;
    while (n > 0) {
        d = d + 1;
        n = n / 10;
    }
    return d;
}

int main(void) {
    int a[10];
    int b[8];
    int c[5];
    int d[6];

    a[0] = 170; a[1] = 45; a[2] = 75; a[3] = 90; a[4] = 802;
    a[5] = 24; a[6] = 2; a[7] = 66; a[8] = 501; a[9] = 310;
    radix_sort(a, 10);
    // EXPECT: rs10: 1
    printf("rs10: %d\n", is_sorted(a, 10));
    // EXPECT: 2 24 45 66 75 90 170 310 501 802 done
    print_array(a, 10);
    printf("done\n");

    b[0] = 9; b[1] = 8; b[2] = 7; b[3] = 6;
    b[4] = 5; b[5] = 4; b[6] = 3; b[7] = 2;
    radix_sort(b, 8);
    // EXPECT: single_dig: 1
    printf("single_dig: %d\n", is_sorted(b, 8));
    // EXPECT: 2 3 4 5 6 7 8 9 done
    print_array(b, 8);
    printf("done\n");

    c[0] = 1000; c[1] = 100; c[2] = 10; c[3] = 1; c[4] = 500;
    radix_sort(c, 5);
    // EXPECT: powers: 1
    printf("powers: %d\n", is_sorted(c, 5));
    // EXPECT: 1 10 100 500 1000 done
    print_array(c, 5);
    printf("done\n");

    d[0] = 55; d[1] = 55; d[2] = 11; d[3] = 11; d[4] = 33; d[5] = 33;
    radix_sort(d, 6);
    // EXPECT: dups: 1
    printf("dups: %d\n", is_sorted(d, 6));
    // EXPECT: 11 11 33 33 55 55 done
    print_array(d, 6);
    printf("done\n");

    // EXPECT: dig1: 5
    printf("dig1: %d\n", get_digit(12345, 1));
    // EXPECT: dig10: 4
    printf("dig10: %d\n", get_digit(12345, 10));
    // EXPECT: dig100: 3
    printf("dig100: %d\n", get_digit(12345, 100));
    // EXPECT: dig1000: 2
    printf("dig1000: %d\n", get_digit(12345, 1000));

    // EXPECT: nd0: 1
    printf("nd0: %d\n", count_digits(0));
    // EXPECT: nd5: 1
    printf("nd5: %d\n", count_digits(5));
    // EXPECT: nd99: 2
    printf("nd99: %d\n", count_digits(99));
    // EXPECT: nd1000: 4
    printf("nd1000: %d\n", count_digits(1000));

    a[0] = 999; a[1] = 1; a[2] = 500; a[3] = 250;
    a[4] = 750; a[5] = 125; a[6] = 625; a[7] = 375;
    a[8] = 875; a[9] = 50;
    radix_sort(a, 10);
    // EXPECT: mix: 1
    printf("mix: %d\n", is_sorted(a, 10));

    return 0;
}
