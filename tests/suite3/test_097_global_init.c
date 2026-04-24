int printf(const char *fmt, ...);

/* Global variable initialization: int, array, struct, string, pointer, array-of-structs */

/* --- Global int and expressions --- */
int g_zero = 0;
int g_pos  = 42;
int g_neg  = -17;
int g_expr = 3 * 4 + 5;       /* 17 */
int g_big  = 1000000;

/* --- Global char --- */
char g_char = 'Z';
char g_char2 = 65;             /* 'A' */

/* --- Global array --- */
int g_arr[5] = {10, 20, 30, 40, 50};
int g_arr_partial[5] = {1, 2};  /* rest zero */
int g_arr_neg[3] = {-1, -2, -3};

/* --- Global string (pointer to string literal) --- */
const char *g_str = "global_string";

/* --- Global struct --- */
typedef struct {
    int x;
    int y;
} GPoint;

GPoint g_pt = {7, 11};
GPoint g_pt2 = {.x = 100, .y = 200};

/* --- Global array of structs --- */
GPoint g_pts[4] = {
    {1, 2},
    {3, 4},
    {5, 6},
    {7, 8}
};

/* --- Global nested struct --- */
typedef struct {
    GPoint tl;
    GPoint br;
} GRect;

GRect g_rect = {{0, 0}, {10, 20}};

/* --- Global pointer to global int --- */
int g_target = 55;
int *g_ptr = &g_target;        /* pointer initialized to address of global */

/* --- Global accumulator (mutable globals) --- */
int g_counter = 0;

/* --- Global array with negative values --- */
int g_neg_arr[5] = {-10, -20, -30, -40, -50};

/* --- Global char array --- */
char g_letters[4] = {'A', 'B', 'C', 'D'};

/* --- Global with constant expression --- */
int g_shift = 1 << 4;          /* 16 */
int g_mask  = 0xFF;            /* 255 */
int g_bits  = (1 << 3) | (1 << 1); /* 8|2=10 */

/* Functions that use and modify globals */
void increment_counter(void) { g_counter++; }
int get_counter(void) { return g_counter; }

int sum_g_arr(void) {
    int s = 0, i;
    for (i = 0; i < 5; i++) s += g_arr[i];
    return s;
}

int sum_g_pts_x(void) {
    int s = 0, i;
    for (i = 0; i < 4; i++) s += g_pts[i].x;
    return s;
}

int sum_g_pts_y(void) {
    int s = 0, i;
    for (i = 0; i < 4; i++) s += g_pts[i].y;
    return s;
}

int main(void) {
    /* --- Basic global int --- */
    printf("%d\n", g_zero);       // EXPECT: 0
    printf("%d\n", g_pos);        // EXPECT: 42
    printf("%d\n", g_neg);        // EXPECT: -17
    printf("%d\n", g_expr);       // EXPECT: 17
    printf("%d\n", g_big);        // EXPECT: 1000000

    /* --- Global char --- */
    printf("%c\n", g_char);       // EXPECT: Z
    printf("%d\n", (int)g_char2); // EXPECT: 65

    /* --- Global array --- */
    printf("%d\n", g_arr[0]);     // EXPECT: 10
    printf("%d\n", g_arr[2]);     // EXPECT: 30
    printf("%d\n", g_arr[4]);     // EXPECT: 50
    printf("%d\n", sum_g_arr());  // EXPECT: 150

    /* --- Partial init: rest should be zero --- */
    printf("%d\n", g_arr_partial[0]); // EXPECT: 1
    printf("%d\n", g_arr_partial[1]); // EXPECT: 2
    printf("%d\n", g_arr_partial[2]); // EXPECT: 0
    printf("%d\n", g_arr_partial[4]); // EXPECT: 0

    /* --- Negative global array --- */
    printf("%d\n", g_arr_neg[0]); // EXPECT: -1
    printf("%d\n", g_arr_neg[1]); // EXPECT: -2
    printf("%d\n", g_arr_neg[2]); // EXPECT: -3

    /* --- Global string --- */
    printf("%s\n", g_str);        // EXPECT: global_string

    /* --- Global struct --- */
    printf("%d\n", g_pt.x);      // EXPECT: 7
    printf("%d\n", g_pt.y);      // EXPECT: 11
    printf("%d\n", g_pt2.x);     // EXPECT: 100
    printf("%d\n", g_pt2.y);     // EXPECT: 200

    /* --- Global array of structs --- */
    printf("%d\n", g_pts[0].x);  // EXPECT: 1
    printf("%d\n", g_pts[1].y);  // EXPECT: 4
    printf("%d\n", g_pts[3].x);  // EXPECT: 7
    printf("%d\n", g_pts[3].y);  // EXPECT: 8
    printf("%d\n", sum_g_pts_x()); // EXPECT: 16
    printf("%d\n", sum_g_pts_y()); // EXPECT: 20

    /* --- Global nested struct --- */
    printf("%d\n", g_rect.tl.x); // EXPECT: 0
    printf("%d\n", g_rect.tl.y); // EXPECT: 0
    printf("%d\n", g_rect.br.x); // EXPECT: 10
    printf("%d\n", g_rect.br.y); // EXPECT: 20

    /* --- Global pointer to global --- */
    printf("%d\n", *g_ptr);      // EXPECT: 55
    *g_ptr = 77;
    printf("%d\n", g_target);    // EXPECT: 77
    printf("%d\n", *g_ptr);      // EXPECT: 77

    /* --- Global counter via functions --- */
    printf("%d\n", get_counter()); // EXPECT: 0
    increment_counter();
    increment_counter();
    increment_counter();
    printf("%d\n", get_counter()); // EXPECT: 3

    /* --- Global negative array sum --- */
    int i, ns = 0;
    for (i = 0; i < 5; i++) ns += g_neg_arr[i];
    printf("%d\n", ns);          // EXPECT: -150

    /* --- Global char array --- */
    printf("%c\n", g_letters[0]); // EXPECT: A
    printf("%c\n", g_letters[3]); // EXPECT: D

    /* --- Global constant expressions --- */
    printf("%d\n", g_shift);     // EXPECT: 16
    printf("%d\n", g_mask);      // EXPECT: 255
    printf("%d\n", g_bits);      // EXPECT: 10

    /* --- Modify global struct --- */
    g_pt.x += 3;
    g_pt.y *= 2;
    printf("%d\n", g_pt.x);      // EXPECT: 10
    printf("%d\n", g_pt.y);      // EXPECT: 22

    return 0;
}
