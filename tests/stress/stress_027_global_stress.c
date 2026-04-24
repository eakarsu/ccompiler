int printf(const char *fmt, ...);

// Global variable stress: 30+ globals of different types, global arrays,
// global structs, pointers to globals, static globals, multi-function mutation.

// ---- Simple globals ----
int g_int = 42;
char g_char = 'A';
long g_long = 1000000;
unsigned int g_uint = 3000000000u;
int g_negative = -999;

// ---- Global arrays ----
int g_arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
char g_str[] = "Hello Global";
int g_zeros[5];
int g_big_arr[100];

// ---- Global structs ----
struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point tl;
    struct Point br;
};

struct Point g_origin = {0, 0};
struct Point g_p1 = {10, 20};
struct Rect g_rect = {{1, 2}, {3, 4}};

// ---- Global pointers ----
int *g_ptr_int = &g_int;
int *g_ptr_arr = g_arr;
struct Point *g_ptr_point = &g_p1;

// ---- Static globals ----
static int s_counter = 0;
static int s_history[20];
static int s_hist_idx = 0;

// ---- More globals for multi-function mutation ----
int g_accumulator = 0;
int g_multiplier = 1;
int g_flag = 0;
int g_result = 0;
int g_temp1 = 0;
int g_temp2 = 0;

// ---- Enum-initialized globals ----
enum Color { RED = 0, GREEN = 1, BLUE = 2 };
int g_color = GREEN;

// ---- Globals initialized with expressions ----
int g_ten = 10;

// ---- Additional globals to reach 30+ ----
int g_x1 = 1, g_x2 = 2, g_x3 = 3, g_x4 = 4, g_x5 = 5;
int g_x6 = 6, g_x7 = 7, g_x8 = 8, g_x9 = 9, g_x10 = 10;

// ---- Functions that modify globals ----
void increment_counter(void) {
    s_counter++;
    if (s_hist_idx < 20) {
        s_history[s_hist_idx++] = s_counter;
    }
}

void add_to_accumulator(int val) {
    g_accumulator += val;
}

void multiply_accumulator(void) {
    g_accumulator *= g_multiplier;
}

void set_flag(int val) {
    g_flag = val;
}

int get_flag(void) {
    return g_flag;
}

void swap_temps(void) {
    int tmp = g_temp1;
    g_temp1 = g_temp2;
    g_temp2 = tmp;
}

void fill_big_arr(void) {
    int i;
    for (i = 0; i < 100; i++) {
        g_big_arr[i] = i * i;
    }
}

int sum_big_arr(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 100; i++) {
        sum += g_big_arr[i];
    }
    return sum;
}

void modify_point(struct Point *p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}

void modify_rect(void) {
    g_rect.tl.x += 10;
    g_rect.tl.y += 10;
    g_rect.br.x += 10;
    g_rect.br.y += 10;
}

int sum_all_x(void) {
    return g_x1 + g_x2 + g_x3 + g_x4 + g_x5 +
           g_x6 + g_x7 + g_x8 + g_x9 + g_x10;
}

void double_all_x(void) {
    g_x1 *= 2; g_x2 *= 2; g_x3 *= 2; g_x4 *= 2; g_x5 *= 2;
    g_x6 *= 2; g_x7 *= 2; g_x8 *= 2; g_x9 *= 2; g_x10 *= 2;
}

// Function that reads and writes many globals
int complex_global_op(int input) {
    g_temp1 = input;
    g_temp2 = g_int + g_negative;  // 42 + (-999) = -957
    swap_temps();
    // Now g_temp1 = -957, g_temp2 = input
    g_result = g_temp1 + g_temp2;
    return g_result;
}

// Chain of functions modifying globals
void chain_step1(void) { g_accumulator = 10; }
void chain_step2(void) { g_multiplier = 3; multiply_accumulator(); }
void chain_step3(void) { add_to_accumulator(5); }
void chain_step4(void) { g_multiplier = 2; multiply_accumulator(); }

int main(void) {
    // Basic globals
    printf("%d\n", g_int);
    // EXPECT: 42
    printf("%c\n", g_char);
    // EXPECT: A
    printf("%ld\n", g_long);
    // EXPECT: 1000000
    printf("%d\n", g_negative);
    // EXPECT: -999

    // Global array
    int arr_sum = 0;
    int i;
    for (i = 0; i < 10; i++) arr_sum += g_arr[i];
    printf("%d\n", arr_sum);
    // EXPECT: 45

    // Global string
    printf("%s\n", g_str);
    // EXPECT: Hello Global

    // Zero-initialized global array
    int zero_sum = 0;
    for (i = 0; i < 5; i++) zero_sum += g_zeros[i];
    printf("%d\n", zero_sum);
    // EXPECT: 0

    // Global struct
    printf("%d %d\n", g_origin.x, g_origin.y);
    // EXPECT: 0 0
    printf("%d %d\n", g_p1.x, g_p1.y);
    // EXPECT: 10 20

    // Global rect (nested struct)
    printf("%d %d %d %d\n", g_rect.tl.x, g_rect.tl.y, g_rect.br.x, g_rect.br.y);
    // EXPECT: 1 2 3 4

    // Global pointer to int
    printf("%d\n", *g_ptr_int);
    // EXPECT: 42

    // Global pointer to array
    printf("%d\n", g_ptr_arr[3]);
    // EXPECT: 3

    // Global pointer to struct
    printf("%d %d\n", g_ptr_point->x, g_ptr_point->y);
    // EXPECT: 10 20

    // Static counter
    for (i = 0; i < 10; i++) increment_counter();
    printf("%d\n", s_counter);
    // EXPECT: 10
    printf("%d %d %d\n", s_history[0], s_history[4], s_history[9]);
    // EXPECT: 1 5 10

    // Accumulator operations
    g_accumulator = 0;
    for (i = 1; i <= 10; i++) add_to_accumulator(i);
    printf("%d\n", g_accumulator);
    // EXPECT: 55

    // Flag set/get
    set_flag(77);
    printf("%d\n", get_flag());
    // EXPECT: 77

    // Swap temps
    g_temp1 = 111;
    g_temp2 = 222;
    swap_temps();
    printf("%d %d\n", g_temp1, g_temp2);
    // EXPECT: 222 111

    // Big array
    fill_big_arr();
    // sum of i*i for i=0..99 = 99*100*199/6 = 328350
    printf("%d\n", sum_big_arr());
    // EXPECT: 328350

    // Modify global struct via pointer
    modify_point(&g_p1, 5, 10);
    printf("%d %d\n", g_p1.x, g_p1.y);
    // EXPECT: 15 30

    // Modify global rect
    modify_rect();
    printf("%d %d %d %d\n", g_rect.tl.x, g_rect.tl.y, g_rect.br.x, g_rect.br.y);
    // EXPECT: 11 12 13 14

    // Sum all x globals: 1+2+...+10 = 55
    printf("%d\n", sum_all_x());
    // EXPECT: 55

    // Double and resum: 2+4+...+20 = 110
    double_all_x();
    printf("%d\n", sum_all_x());
    // EXPECT: 110

    // Complex global op: input=100, result = -957 + 100 = -857
    printf("%d\n", complex_global_op(100));
    // EXPECT: -857

    // Chain: step1 sets acc=10, step2 mult by 3 => 30, step3 add 5 => 35, step4 mult by 2 => 70
    chain_step1();
    chain_step2();
    chain_step3();
    chain_step4();
    printf("%d\n", g_accumulator);
    // EXPECT: 70

    // Enum global
    printf("%d\n", g_color);
    // EXPECT: 1

    // Unsigned global
    if (g_uint > 2000000000u) {
        printf("unsigned ok\n");
    } else {
        printf("unsigned fail\n");
    }
    // EXPECT: unsigned ok

    printf("ALL PASSED\n");
    // EXPECT: ALL PASSED
    return 0;
}
