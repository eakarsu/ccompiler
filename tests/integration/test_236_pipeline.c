int printf(const char *fmt, ...);
// EXPECT: pipeline1(5)=22\npipeline1(0)=12\npipeline1(10)=32\npipeline2(3)=10\npipeline2(7)=50\nbefore: 1 2 3 4 5\nafter pipe1: 14 16 18 20 22\nafter pipe2: 2 5 10 17 26\npipe_sum1=90\npipe_max1=22\npipe_sum2=60\npipe_max2=26\npipe3(-5)=16\npipe3(3)=10
// Test 236: Function composition patterns - pipeline

typedef int (*transform)(int);

int double_val(int x) { return x * 2; }
int triple_val(int x) { return x * 3; }
int add_one(int x) { return x + 1; }
int add_ten(int x) { return x + 10; }
int square(int x) { return x * x; }
int negate(int x) { return -x; }
int abs_val(int x) { if (x < 0) return -x; return x; }

int pipeline(int input, transform *steps, int n_steps) {
    int result = input;
    int i = 0;
    while (i < n_steps) {
        result = steps[i](result);
        i = i + 1;
    }
    return result;
}

void pipeline_array(int *arr, int n, transform *steps, int n_steps) {
    int i = 0;
    while (i < n) {
        arr[i] = pipeline(arr[i], steps, n_steps);
        i = i + 1;
    }
}

int pipeline_sum(int *arr, int n, transform *steps, int n_steps) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        sum = sum + pipeline(arr[i], steps, n_steps);
        i = i + 1;
    }
    return sum;
}

int pipeline_max(int *arr, int n, transform *steps, int n_steps) {
    int mx = pipeline(arr[0], steps, n_steps);
    int i = 1;
    while (i < n) {
        int val = pipeline(arr[i], steps, n_steps);
        if (val > mx) mx = val;
        i = i + 1;
    }
    return mx;
}

void print_arr(int *arr, int n) {
    int i = 0;
    while (i < n) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
        i = i + 1;
    }
    printf("\n");
}

int main(void) {
    transform pipe1[3];
    pipe1[0] = add_one;
    pipe1[1] = double_val;
    pipe1[2] = add_ten;

    printf("pipeline1(5)=%d\n", pipeline(5, pipe1, 3));
    printf("pipeline1(0)=%d\n", pipeline(0, pipe1, 3));
    printf("pipeline1(10)=%d\n", pipeline(10, pipe1, 3));

    transform pipe2[2];
    pipe2[0] = square;
    pipe2[1] = add_one;
    printf("pipeline2(3)=%d\n", pipeline(3, pipe2, 2));
    printf("pipeline2(7)=%d\n", pipeline(7, pipe2, 2));

    int arr[5];
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    printf("before: "); print_arr(arr, 5);
    pipeline_array(arr, 5, pipe1, 3);
    printf("after pipe1: "); print_arr(arr, 5);

    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    pipeline_array(arr, 5, pipe2, 2);
    printf("after pipe2: "); print_arr(arr, 5);

    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    printf("pipe_sum1=%d\n", pipeline_sum(arr, 5, pipe1, 3));
    printf("pipe_max1=%d\n", pipeline_max(arr, 5, pipe1, 3));
    printf("pipe_sum2=%d\n", pipeline_sum(arr, 5, pipe2, 2));
    printf("pipe_max2=%d\n", pipeline_max(arr, 5, pipe2, 2));

    transform pipe3[4];
    pipe3[0] = negate;
    pipe3[1] = abs_val;
    pipe3[2] = triple_val;
    pipe3[3] = add_one;
    printf("pipe3(-5)=%d\n", pipeline(-5, pipe3, 4));
    printf("pipe3(3)=%d\n", pipeline(3, pipe3, 4));

    return 0;
}
