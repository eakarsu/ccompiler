int printf(const char *fmt, ...);
// EXPECT: Two pointers converge:\ni=0 j=10 sum=10\ni=1 j=9 sum=10\ni=2 j=8 sum=10\ni=3 j=7 sum=10\ni=4 j=6 sum=10\nTriple step:\n0 1 0\n1 0 1\n0 1 2\n1 2 3\n2 3 6\n3 6 11\n6 11 20\n11 20 37\nPaired iteration:\n1*50=50\n2*40=80\n3*30=90\n4*20=80\n5*10=50\nMulti-accumulator:\ni=1 sum=1 prod=1\ni=2 sum=3 prod=2\ni=3 sum=6 prod=6\ni=4 sum=10 prod=24\ni=5 sum=15 prod=120\ni=6 sum=21 prod=720\nIndex pair sums:\ndata[0]+data[5]=1+11=12\ndata[1]+data[4]=3+9=12\ndata[2]+data[3]=5+7=12
int main(void) {
    int i;
    int j;
    int sum;

    printf("Two pointers converge:\n");
    i = 0;
    j = 10;
    while (i < j) {
        printf("i=%d j=%d sum=%d\n", i, j, i + j);
        i++;
        j--;
    }

    printf("Triple step:\n");
    int a = 0;
    int b = 1;
    int c = 0;
    for (i = 0; i < 8; i++) {
        printf("%d %d %d\n", a, b, c);
        int temp = a + b + c;
        a = b;
        b = c;
        c = temp;
    }

    printf("Paired iteration:\n");
    int arr1[5];
    int arr2[5];
    for (i = 0; i < 5; i++) {
        arr1[i] = i + 1;
        arr2[i] = (i + 1) * 10;
    }
    i = 0;
    j = 4;
    while (i < 5 && j >= 0) {
        printf("%d*%d=%d\n", arr1[i], arr2[j], arr1[i] * arr2[j]);
        i++;
        j--;
    }

    printf("Multi-accumulator:\n");
    sum = 0;
    int product = 1;
    for (i = 1; i <= 6; i++) {
        sum = sum + i;
        product = product * i;
        printf("i=%d sum=%d prod=%d\n", i, sum, product);
    }

    printf("Index pair sums:\n");
    int data[6];
    for (i = 0; i < 6; i++) {
        data[i] = i * 2 + 1;
    }
    for (i = 0; i < 6; i++) {
        for (j = i + 1; j < 6; j++) {
            if (data[i] + data[j] == 12) {
                printf("data[%d]+data[%d]=%d+%d=12\n", i, j, data[i], data[j]);
            }
        }
    }

    return 0;
}
