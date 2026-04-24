int printf(const char *fmt, ...);
// EXPECT: arr[0]=0 arr[1]=10 arr[2]=20 arr[3]=30 arr[4]=40
// sum=100

void fill_array(int n) {
    int arr[n];
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i * 10;
    }
    printf("arr[0]=%d arr[1]=%d arr[2]=%d arr[3]=%d arr[4]=%d\n",
           arr[0], arr[1], arr[2], arr[3], arr[4]);

    int sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    printf("sum=%d\n", sum);
}

int main(void) {
    fill_array(5);
    return 0;
}
