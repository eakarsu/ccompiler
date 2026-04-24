int printf(const char *fmt, ...);
// EXPECT: MIN(10,20) = 10\nMAX(10,20) = 20\nMIN3(10,20,15) = 10\nMAX3(10,20,15) = 20\nMEDIAN3(10,20,15) = 15\nBefore swap: x=10 y=20\nAfter swap: x=20 y=10\nSorted: 1 7 12 23 34 45 56 89 \nMin = 1\nMax = 89\nMedian-ish = 34\nMIN3(100,200,150) = 100\nMAX3(100,200,150) = 200\nMEDIAN3(100,200,150) = 150
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SWAP(a, b, tmp) do { tmp = a; a = b; b = tmp; } while(0)
#define MIN3(a, b, c) MIN(MIN(a, b), c)
#define MAX3(a, b, c) MAX(MAX(a, b), c)
#define MEDIAN3(a, b, c) MAX(MIN(a, b), MIN(MAX(a, b), c))

int main(void) {
    int x = 10;
    int y = 20;
    int z = 15;
    int tmp;

    printf("MIN(%d,%d) = %d\n", x, y, MIN(x, y));
    printf("MAX(%d,%d) = %d\n", x, y, MAX(x, y));
    printf("MIN3(%d,%d,%d) = %d\n", x, y, z, MIN3(x, y, z));
    printf("MAX3(%d,%d,%d) = %d\n", x, y, z, MAX3(x, y, z));
    printf("MEDIAN3(%d,%d,%d) = %d\n", x, y, z, MEDIAN3(x, y, z));

    printf("Before swap: x=%d y=%d\n", x, y);
    SWAP(x, y, tmp);
    printf("After swap: x=%d y=%d\n", x, y);
    SWAP(x, y, tmp);

    int arr[8];
    arr[0] = 34; arr[1] = 12; arr[2] = 45; arr[3] = 7;
    arr[4] = 23; arr[5] = 56; arr[6] = 1; arr[7] = 89;
    int n = 8;
    int i;
    int j;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                SWAP(arr[j], arr[j + 1], tmp);
            }
        }
    }
    printf("Sorted: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("Min = %d\n", arr[0]);
    printf("Max = %d\n", arr[n - 1]);
    printf("Median-ish = %d\n", arr[n / 2]);

    int a = 100;
    int b = 200;
    int c = 150;
    printf("MIN3(%d,%d,%d) = %d\n", a, b, c, MIN3(a, b, c));
    printf("MAX3(%d,%d,%d) = %d\n", a, b, c, MAX3(a, b, c));
    printf("MEDIAN3(%d,%d,%d) = %d\n", a, b, c, MEDIAN3(a, b, c));

    return 0;
}
