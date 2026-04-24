int printf(const char *fmt, ...);

/* ===== Grand Finale: Comprehensive C Feature Test ===== */

/* Enums */
enum Color { RED, GREEN, BLUE, YELLOW };
enum Direction { NORTH, EAST, SOUTH, WEST };

/* Forward-declared structs */
struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point origin;
    int width;
    int height;
};

struct Node {
    int value;
    int left_idx;
    int right_idx;
};

struct Registry {
    char names[8][16];
    int values[8];
    int count;
};

/* String utilities */
int str_len(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) i = i + 1;
    return i;
}

void str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

/* Geometry functions */
int point_distance_sq(struct Point *a, struct Point *b) {
    int dx;
    int dy;
    dx = a->x - b->x;
    dy = a->y - b->y;
    return dx * dx + dy * dy;
}

int rect_area(struct Rect *r) {
    return r->width * r->height;
}

int rect_perimeter(struct Rect *r) {
    return 2 * (r->width + r->height);
}

int rect_contains_point(struct Rect *r, struct Point *p) {
    return p->x >= r->origin.x && p->x < r->origin.x + r->width &&
           p->y >= r->origin.y && p->y < r->origin.y + r->height;
}

/* Recursive functions */
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int power(int base, int exp) {
    if (exp == 0) return 1;
    return base * power(base, exp - 1);
}

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

/* BST in array */
int bst_nodes_count;

void bst_init(struct Node *tree) {
    int i;
    bst_nodes_count = 0;
    i = 0;
    while (i < 15) {
        tree[i].value = 0;
        tree[i].left_idx = -1;
        tree[i].right_idx = -1;
        i = i + 1;
    }
}

int bst_insert(struct Node *tree, int root, int value) {
    int new_idx;
    if (bst_nodes_count == 0) {
        tree[0].value = value;
        bst_nodes_count = 1;
        return 0;
    }
    if (value < tree[root].value) {
        if (tree[root].left_idx == -1) {
            new_idx = bst_nodes_count;
            bst_nodes_count = bst_nodes_count + 1;
            tree[new_idx].value = value;
            tree[new_idx].left_idx = -1;
            tree[new_idx].right_idx = -1;
            tree[root].left_idx = new_idx;
            return new_idx;
        }
        return bst_insert(tree, tree[root].left_idx, value);
    } else {
        if (tree[root].right_idx == -1) {
            new_idx = bst_nodes_count;
            bst_nodes_count = bst_nodes_count + 1;
            tree[new_idx].value = value;
            tree[new_idx].left_idx = -1;
            tree[new_idx].right_idx = -1;
            tree[root].right_idx = new_idx;
            return new_idx;
        }
        return bst_insert(tree, tree[root].right_idx, value);
    }
}

int bst_find(struct Node *tree, int root, int value) {
    if (root == -1) return 0;
    if (tree[root].value == value) return 1;
    if (value < tree[root].value) return bst_find(tree, tree[root].left_idx, value);
    return bst_find(tree, tree[root].right_idx, value);
}

int bst_min(struct Node *tree, int root) {
    if (tree[root].left_idx == -1) return tree[root].value;
    return bst_min(tree, tree[root].left_idx);
}

int bst_max(struct Node *tree, int root) {
    if (tree[root].right_idx == -1) return tree[root].value;
    return bst_max(tree, tree[root].right_idx);
}

/* Function pointer support */
int apply_op(int (*op)(int, int), int a, int b) {
    return op(a, b);
}

int add_fn(int a, int b) { return a + b; }
int sub_fn(int a, int b) { return a - b; }
int mul_fn(int a, int b) { return a * b; }

/* Registry (name-value store) */
void reg_init(struct Registry *r) {
    r->count = 0;
}

int reg_set(struct Registry *r, char *name, int value) {
    int i;
    i = 0;
    while (i < r->count) {
        if (str_eq(r->names[i], name)) {
            r->values[i] = value;
            return i;
        }
        i = i + 1;
    }
    if (r->count >= 8) return -1;
    str_copy(r->names[r->count], name);
    r->values[r->count] = value;
    r->count = r->count + 1;
    return r->count - 1;
}

int reg_get(struct Registry *r, char *name) {
    int i;
    i = 0;
    while (i < r->count) {
        if (str_eq(r->names[i], name)) return r->values[i];
        i = i + 1;
    }
    return -1;
}

/* Array sorting and searching */
void bubble_sort(int *arr, int n) {
    int i;
    int j;
    int tmp;
    i = 0;
    while (i < n - 1) {
        j = 0;
        while (j < n - 1 - i) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

int binary_search(int *arr, int n, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n - 1;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

/* Direction helper using enum */
int move_x(int dir) {
    if (dir == EAST) return 1;
    if (dir == WEST) return -1;
    return 0;
}

int move_y(int dir) {
    if (dir == NORTH) return 1;
    if (dir == SOUTH) return -1;
    return 0;
}

char *color_name(int c) {
    if (c == RED) return "red";
    if (c == GREEN) return "green";
    if (c == BLUE) return "blue";
    if (c == YELLOW) return "yellow";
    return "unknown";
}

int main(void) {
    struct Point p1;
    struct Point p2;
    struct Rect rect;
    struct Node tree[15];
    struct Registry reg;
    int arr[10];
    int result;
    int (*op)(int, int);
    int i;

    /* === Geometry === */
    p1.x = 3; p1.y = 4;
    p2.x = 0; p2.y = 0;

    /* Distance squared: 9 + 16 = 25 */
    // EXPECT: 25
    printf("%d\n", point_distance_sq(&p1, &p2));

    rect.origin.x = 0; rect.origin.y = 0;
    rect.width = 10; rect.height = 5;

    // EXPECT: 50
    printf("%d\n", rect_area(&rect));
    // EXPECT: 30
    printf("%d\n", rect_perimeter(&rect));

    /* Point (3,4) inside rect (0,0,10,5) */
    // EXPECT: 1
    printf("%d\n", rect_contains_point(&rect, &p1));

    p2.x = 15; p2.y = 3;
    // EXPECT: 0
    printf("%d\n", rect_contains_point(&rect, &p2));

    /* === Recursion === */
    // EXPECT: 55
    printf("%d\n", fibonacci(10));

    // EXPECT: 120
    printf("%d\n", factorial(5));

    // EXPECT: 32
    printf("%d\n", power(2, 5));

    // EXPECT: 6
    printf("%d\n", gcd(54, 24));

    /* === BST === */
    bst_init(tree);
    bst_insert(tree, 0, 50);
    bst_insert(tree, 0, 30);
    bst_insert(tree, 0, 70);
    bst_insert(tree, 0, 20);
    bst_insert(tree, 0, 40);
    bst_insert(tree, 0, 60);
    bst_insert(tree, 0, 80);

    // EXPECT: 7
    printf("%d\n", bst_nodes_count);

    // EXPECT: 1
    printf("%d\n", bst_find(tree, 0, 40));
    // EXPECT: 0
    printf("%d\n", bst_find(tree, 0, 45));
    // EXPECT: 20
    printf("%d\n", bst_min(tree, 0));
    // EXPECT: 80
    printf("%d\n", bst_max(tree, 0));

    /* === Function Pointers === */
    op = add_fn;
    // EXPECT: 15
    printf("%d\n", apply_op(op, 7, 8));

    op = sub_fn;
    // EXPECT: 3
    printf("%d\n", apply_op(op, 10, 7));

    op = mul_fn;
    // EXPECT: 42
    printf("%d\n", apply_op(op, 6, 7));

    /* === Enums === */
    // EXPECT: red
    printf("%s\n", color_name(RED));
    // EXPECT: blue
    printf("%s\n", color_name(BLUE));

    /* Walk a path: NORTH, EAST, EAST, SOUTH */
    {
        int px;
        int py;
        int path[4];
        path[0] = NORTH; path[1] = EAST; path[2] = EAST; path[3] = SOUTH;
        px = 0; py = 0;
        i = 0;
        while (i < 4) {
            px = px + move_x(path[i]);
            py = py + move_y(path[i]);
            i = i + 1;
        }
        // EXPECT: 2
        printf("%d\n", px);
        // EXPECT: 0
        printf("%d\n", py);
    }

    /* === Registry === */
    reg_init(&reg);
    reg_set(&reg, "width", 640);
    reg_set(&reg, "height", 480);
    reg_set(&reg, "depth", 32);

    // EXPECT: 640
    printf("%d\n", reg_get(&reg, "width"));
    // EXPECT: 480
    printf("%d\n", reg_get(&reg, "height"));
    // EXPECT: -1
    printf("%d\n", reg_get(&reg, "missing"));

    /* Update existing */
    reg_set(&reg, "width", 1920);
    // EXPECT: 1920
    printf("%d\n", reg_get(&reg, "width"));

    /* === Array sort + binary search === */
    arr[0] = 64; arr[1] = 25; arr[2] = 12; arr[3] = 22; arr[4] = 11;
    arr[5] = 90; arr[6] = 45; arr[7] = 37; arr[8] = 78; arr[9] = 55;

    bubble_sort(arr, 10);
    /* Sorted: 11,12,22,25,37,45,55,64,78,90 */
    // EXPECT: 11
    printf("%d\n", arr[0]);
    // EXPECT: 90
    printf("%d\n", arr[9]);

    result = binary_search(arr, 10, 45);
    // EXPECT: 5
    printf("%d\n", result);

    result = binary_search(arr, 10, 99);
    // EXPECT: -1
    printf("%d\n", result);

    /* === Scoping test === */
    {
        int x;
        x = 100;
        {
            int x;
            x = 200;
            // EXPECT: 200
            printf("%d\n", x);
        }
        // EXPECT: 100
        printf("%d\n", x);
    }

    /* === Pointer arithmetic === */
    {
        int data[5];
        int *p;
        data[0] = 10; data[1] = 20; data[2] = 30; data[3] = 40; data[4] = 50;
        p = data;
        // EXPECT: 10
        printf("%d\n", *p);
        p = p + 2;
        // EXPECT: 30
        printf("%d\n", *p);
        *(p + 1) = 99;
        // EXPECT: 99
        printf("%d\n", data[3]);
    }

    /* === Final computation: combine everything === */
    {
        int fib_val;
        int fact_val;
        int bst_val;
        fib_val = fibonacci(7);
        fact_val = factorial(4);
        bst_val = bst_min(tree, 0);
        /* 13 + 24 + 20 = 57 */
        // EXPECT: 57
        printf("%d\n", fib_val + fact_val + bst_val);
    }

    return 0;
}
