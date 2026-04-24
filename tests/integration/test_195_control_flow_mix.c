int printf(const char *fmt, ...);
// EXPECT: Prime table:\n 1   2*  3*  4   5* \n 6   7*  8   9  10  \n11* 12  13* 14  15  \n16  17* 18  19* 20  \n21  22  23* 24  25  \n26  27  28  29* 30  \n31* 32  33  34  35  \n36  37* 38  39  40  \n41* 42  43* 44  45  \n46  47* 48  49  50  \nCollatz steps:\n1:0 2:1 3:7 4:2 5:5 6:8 7:16 8:3 9:19 10:6 11:14 12:9 13:9 14:17 15:17 16:4 17:12 18:20 19:20 20:7 \nInsertion sort trace:\nstep 1: 3 5 1 4 2 6 \nstep 2: 1 3 5 4 2 6 \nstep 3: 1 3 4 5 2 6 \nstep 4: 1 2 3 4 5 6 \nstep 5: 1 2 3 4 5 6 \nNumber properties:\n1:odd,dsum=1\n2:even,prime,dsum=2\n3:odd,prime,dsum=3\n4:even,dsum=4\n5:odd,prime,dsum=5\n6:even,dsum=6\n7:odd,prime,dsum=7\n8:even,dsum=8\n9:odd,dsum=9\n10:even,dsum=1\n11:odd,prime,dsum=2\n12:even,dsum=3\n13:odd,prime,dsum=4\n14:even,dsum=5\n15:odd,dsum=6\nConvergent search:\nfound 42 in 7 steps
int is_prime(int n) {
    if (n < 2) return 0;
    if (n < 4) return 1;
    if (n % 2 == 0) return 0;
    int i = 3;
    while (i * i <= n) {
        if (n % i == 0) return 0;
        i = i + 2;
    }
    return 1;
}

int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        steps++;
    }
    return steps;
}

int main(void) {
    int i;
    int j;

    printf("Prime table:\n");
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 5; j++) {
            int n = i * 5 + j + 1;
            if (is_prime(n)) {
                printf("%2d* ", n);
            } else {
                printf("%2d  ", n);
            }
        }
        printf("\n");
    }

    printf("Collatz steps:\n");
    for (i = 1; i <= 20; i++) {
        printf("%d:%d ", i, collatz_steps(i));
    }
    printf("\n");

    printf("Insertion sort trace:\n");
    int arr[6];
    arr[0] = 5; arr[1] = 3; arr[2] = 1; arr[3] = 4; arr[4] = 2; arr[5] = 6;
    for (i = 1; i < 6; i++) {
        int key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
        printf("step %d: ", i);
        int k;
        for (k = 0; k < 6; k++) printf("%d ", arr[k]);
        printf("\n");
    }

    printf("Number properties:\n");
    for (i = 1; i <= 15; i++) {
        printf("%d:", i);
        if (i % 2 == 0) printf("even");
        else printf("odd");
        if (is_prime(i)) printf(",prime");
        int sum = 0;
        int n = i;
        do {
            sum = sum + n % 10;
            n = n / 10;
        } while (n > 0);
        printf(",dsum=%d", sum);
        printf("\n");
    }

    printf("Convergent search:\n");
    int target = 42;
    int lo = 1;
    int hi = 100;
    int steps = 0;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (mid < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
        steps++;
    }
    printf("found %d in %d steps\n", lo, steps);

    return 0;
}
