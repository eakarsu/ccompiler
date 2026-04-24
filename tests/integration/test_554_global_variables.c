int printf(const char *fmt, ...);
// EXPECT: After recording 5 values:\nCounter: 5\nTotal: 215\nAverage: 43\nMin: 8\nMax: 93\nAfter 2 more:\nCounter: 7\nTotal: 318\nAverage: 45\nMin: 3\nMax: 100\nValues: 42 17 93 8 55 100 3 \nAfter reset:\nCounter: 0\n100 / 5 = 20\nError: division by zero\n42 / 7 = 6\nOperations: 3\nLast error: 0

int g_counter = 0;
int g_total = 0;
int g_min = 999999;
int g_max = -999999;
int g_values[20];
int g_num_values = 0;

void record_value(int val) {
    g_values[g_num_values] = val;
    g_num_values++;
    g_counter++;
    g_total = g_total + val;
    if (val < g_min) g_min = val;
    if (val > g_max) g_max = val;
}

int get_average(void) {
    if (g_num_values == 0) return 0;
    return g_total / g_num_values;
}

void reset_stats(void) {
    g_counter = 0;
    g_total = 0;
    g_min = 999999;
    g_max = -999999;
    g_num_values = 0;
}

void print_stats(void) {
    printf("Counter: %d\n", g_counter);
    printf("Total: %d\n", g_total);
    printf("Average: %d\n", get_average());
    printf("Min: %d\n", g_min);
    printf("Max: %d\n", g_max);
}

int g_error_code = 0;
int g_operations = 0;

void safe_divide(int a, int b) {
    g_operations++;
    if (b == 0) {
        g_error_code = 1;
        printf("Error: division by zero\n");
        return;
    }
    g_error_code = 0;
    printf("%d / %d = %d\n", a, b, a / b);
}

int main(void) {
    record_value(42);
    record_value(17);
    record_value(93);
    record_value(8);
    record_value(55);
    printf("After recording 5 values:\n");
    print_stats();

    record_value(100);
    record_value(3);
    printf("After 2 more:\n");
    print_stats();

    printf("Values: ");
    int i;
    for (i = 0; i < g_num_values; i++) {
        printf("%d ", g_values[i]);
    }
    printf("\n");

    reset_stats();
    printf("After reset:\n");
    printf("Counter: %d\n", g_counter);

    safe_divide(100, 5);
    safe_divide(100, 0);
    safe_divide(42, 7);
    printf("Operations: %d\n", g_operations);
    printf("Last error: %d\n", g_error_code);

    return 0;
}
