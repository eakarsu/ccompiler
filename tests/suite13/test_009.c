int printf(const char *fmt, ...);

// Monotonic stack: next greater element and related problems

struct Stack {
    int data[64];
    int top;
};

void stack_init(struct Stack *s) {
    s->top = -1;
}

int stack_empty(struct Stack *s) {
    return s->top == -1;
}

void stack_push(struct Stack *s, int val) {
    s->top = s->top + 1;
    s->data[s->top] = val;
}

int stack_pop(struct Stack *s) {
    int val = s->data[s->top];
    s->top = s->top - 1;
    return val;
}

int stack_peek(struct Stack *s) {
    return s->data[s->top];
}

// Next greater element to the right for each element
// result[i] = next element greater than arr[i] to its right, or -1
void next_greater_right(int *arr, int n, int *result) {
    struct Stack stk;
    stack_init(&stk);
    int i = n - 1;
    while (i >= 0) {
        while (!stack_empty(&stk) && stack_peek(&stk) <= arr[i]) {
            stack_pop(&stk);
        }
        if (stack_empty(&stk)) {
            result[i] = -1;
        } else {
            result[i] = stack_peek(&stk);
        }
        stack_push(&stk, arr[i]);
        i = i - 1;
    }
}

// Next smaller element to the right
void next_smaller_right(int *arr, int n, int *result) {
    struct Stack stk;
    stack_init(&stk);
    int i = n - 1;
    while (i >= 0) {
        while (!stack_empty(&stk) && stack_peek(&stk) >= arr[i]) {
            stack_pop(&stk);
        }
        if (stack_empty(&stk)) {
            result[i] = -1;
        } else {
            result[i] = stack_peek(&stk);
        }
        stack_push(&stk, arr[i]);
        i = i - 1;
    }
}

// Previous greater element to the left
void prev_greater_left(int *arr, int n, int *result) {
    struct Stack stk;
    stack_init(&stk);
    int i = 0;
    while (i < n) {
        while (!stack_empty(&stk) && stack_peek(&stk) <= arr[i]) {
            stack_pop(&stk);
        }
        if (stack_empty(&stk)) {
            result[i] = -1;
        } else {
            result[i] = stack_peek(&stk);
        }
        stack_push(&stk, arr[i]);
        i = i + 1;
    }
}

// Largest rectangle in histogram using monotonic stack
// Heights array, returns area of largest rectangle
int largest_rect_histogram(int *heights, int n) {
    struct Stack stk;
    stack_init(&stk);
    int max_area = 0;
    int i = 0;
    while (i <= n) {
        int h;
        if (i == n) {
            h = 0;
        } else {
            h = heights[i];
        }
        while (!stack_empty(&stk) && heights[stack_peek(&stk)] > h) {
            int top = stack_pop(&stk);
            int width;
            if (stack_empty(&stk)) {
                width = i;
            } else {
                width = i - stack_peek(&stk) - 1;
            }
            int area = heights[top] * width;
            if (area > max_area) max_area = area;
        }
        stack_push(&stk, i);
        i = i + 1;
    }
    return max_area;
}

// Stock span problem: for each day, count consecutive days before
// (including itself) where price was <= today's price
void stock_span(int *prices, int n, int *spans) {
    struct Stack stk;  // stack of indices
    stack_init(&stk);
    int i = 0;
    while (i < n) {
        while (!stack_empty(&stk) && prices[stack_peek(&stk)] <= prices[i]) {
            stack_pop(&stk);
        }
        if (stack_empty(&stk)) {
            spans[i] = i + 1;
        } else {
            spans[i] = i - stack_peek(&stk);
        }
        stack_push(&stk, i);
        i = i + 1;
    }
}

// Trapping rain water using monotonic stack
int trap_water(int *height, int n) {
    struct Stack stk;
    stack_init(&stk);
    int water = 0;
    int i = 0;
    while (i < n) {
        while (!stack_empty(&stk) && height[i] > height[stack_peek(&stk)]) {
            int top = stack_pop(&stk);
            if (stack_empty(&stk)) {
                // break out of while - no left boundary
                i = i; // no-op to avoid empty block
            } else {
                int left = stack_peek(&stk);
                int min_h;
                if (height[left] < height[i]) {
                    min_h = height[left];
                } else {
                    min_h = height[i];
                }
                int w = i - left - 1;
                water = water + (min_h - height[top]) * w;
            }
        }
        stack_push(&stk, i);
        i = i + 1;
    }
    return water;
}

int main() {
    // Next greater element right
    int arr1[6];
    arr1[0] = 4; arr1[1] = 5; arr1[2] = 2; arr1[3] = 25; arr1[4] = 7; arr1[5] = 8;
    int res1[6];
    next_greater_right(arr1, 6, res1);
    // arr:    4  5  2  25  7  8
    // NGR:    5  25 25 -1  8  -1
    printf("%d %d %d %d %d %d\n", res1[0], res1[1], res1[2], res1[3], res1[4], res1[5]);
    // EXPECT: 5 25 25 -1 8 -1

    // Next smaller element right
    int arr2[5];
    arr2[0] = 3; arr2[1] = 7; arr2[2] = 1; arr2[3] = 5; arr2[4] = 2;
    int res2[5];
    next_smaller_right(arr2, 5, res2);
    // arr:    3  7  1  5  2
    // NSR:    1  1  -1 2  -1
    printf("%d %d %d %d %d\n", res2[0], res2[1], res2[2], res2[3], res2[4]);
    // EXPECT: 1 1 -1 2 -1

    // Previous greater element left
    int arr3[6];
    arr3[0] = 10; arr3[1] = 4; arr3[2] = 2; arr3[3] = 20; arr3[4] = 40; arr3[5] = 12;
    int res3[6];
    prev_greater_left(arr3, 6, res3);
    // arr:    10  4   2  20  40  12
    // PGL:    -1  10  4  -1  -1  40
    printf("%d %d %d %d %d %d\n", res3[0], res3[1], res3[2], res3[3], res3[4], res3[5]);
    // EXPECT: -1 10 4 -1 -1 40

    // Largest rectangle in histogram
    int h1[6];
    h1[0] = 2; h1[1] = 1; h1[2] = 5; h1[3] = 6; h1[4] = 2; h1[5] = 3;
    printf("%d\n", largest_rect_histogram(h1, 6));
    // EXPECT: 10

    int h2[4];
    h2[0] = 4; h2[1] = 4; h2[2] = 4; h2[3] = 4;
    printf("%d\n", largest_rect_histogram(h2, 4));
    // EXPECT: 16

    int h3[5];
    h3[0] = 1; h3[1] = 2; h3[2] = 3; h3[3] = 4; h3[4] = 5;
    printf("%d\n", largest_rect_histogram(h3, 5));
    // EXPECT: 9

    // Stock span
    int prices[7];
    prices[0] = 100; prices[1] = 80; prices[2] = 60;
    prices[3] = 70; prices[4] = 60; prices[5] = 75; prices[6] = 85;
    int spans[7];
    stock_span(prices, 7, spans);
    // day0: 100 => 1
    // day1: 80 <= 100? no => 1
    // day2: 60 <= 80? no => 1
    // day3: 70 > 60, pop idx2. 70 <= 80? no => 3-1=2
    // day4: 60 <= 70? no => 1
    // day5: 75 > 60, pop idx4. 75 > 70, pop idx3. 75 <= 80? no => 5-1=4
    // day6: 85 > 75, pop idx5. 85 > 80, pop idx1. 85 <= 100? no => 6-0=6
    printf("%d %d %d %d %d %d %d\n",
        spans[0], spans[1], spans[2], spans[3], spans[4], spans[5], spans[6]);
    // EXPECT: 1 1 1 2 1 4 6

    // Trap water
    int tw[12];
    tw[0] = 0; tw[1] = 1; tw[2] = 0; tw[3] = 2;
    tw[4] = 1; tw[5] = 0; tw[6] = 1; tw[7] = 3;
    tw[8] = 2; tw[9] = 1; tw[10] = 2; tw[11] = 1;
    printf("%d\n", trap_water(tw, 12));
    // EXPECT: 6

    int tw2[6];
    tw2[0] = 4; tw2[1] = 2; tw2[2] = 0; tw2[3] = 3; tw2[4] = 2; tw2[5] = 5;
    printf("%d\n", trap_water(tw2, 6));
    // EXPECT: 9

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
