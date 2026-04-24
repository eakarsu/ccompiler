int printf(const char *fmt, ...);

struct Vec2 {
    int x;
    int y;
};

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Transform {
    struct Vec3 pos;
    struct Vec3 scale;
    int rotation;
};

struct Entity {
    int id;
    struct Transform transform;
    int active;
    char tag[12];
};

struct World {
    struct Entity entities[32];
    int count;
};

void copy_str(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int abs_val(int x) {
    if (x < 0) return 0 - x;
    return x;
}

int manhattan(struct Vec3 *a, struct Vec3 *b) {
    return abs_val(a->x - b->x) +
           abs_val(a->y - b->y) +
           abs_val(a->z - b->z);
}

int dot3(struct Vec3 *a, struct Vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

struct Vec3 add3(struct Vec3 a, struct Vec3 b) {
    struct Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

struct Vec3 scale3(struct Vec3 v, int s) {
    struct Vec3 r;
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
    return r;
}

int world_add(struct World *w, int id,
              int px, int py, int pz,
              int sx, int sy, int sz,
              int rot, const char *tag) {
    int idx = w->count;
    w->entities[idx].id = id;
    w->entities[idx].transform.pos.x = px;
    w->entities[idx].transform.pos.y = py;
    w->entities[idx].transform.pos.z = pz;
    w->entities[idx].transform.scale.x = sx;
    w->entities[idx].transform.scale.y = sy;
    w->entities[idx].transform.scale.z = sz;
    w->entities[idx].transform.rotation = rot;
    w->entities[idx].active = 1;
    copy_str(w->entities[idx].tag, tag);
    w->count = w->count + 1;
    return idx;
}

int count_active(struct World *w) {
    int c = 0;
    int i = 0;
    while (i < w->count) {
        if (w->entities[i].active) c = c + 1;
        i = i + 1;
    }
    return c;
}

int count_by_tag(struct World *w, const char *tag) {
    int c = 0;
    int i = 0;
    while (i < w->count) {
        if (w->entities[i].active &&
            str_eq(w->entities[i].tag, tag)) {
            c = c + 1;
        }
        i = i + 1;
    }
    return c;
}

typedef int (*ScoreFn)(struct Entity *);

int score_dist(struct Entity *e) {
    int x = e->transform.pos.x;
    int y = e->transform.pos.y;
    int z = e->transform.pos.z;
    return abs_val(x) + abs_val(y) + abs_val(z);
}

int score_volume(struct Entity *e) {
    int sx = e->transform.scale.x;
    int sy = e->transform.scale.y;
    int sz = e->transform.scale.z;
    return sx * sy * sz;
}

int apply_score(struct World *w, int idx, ScoreFn fn) {
    return fn(&w->entities[idx]);
}

int sum_scores(struct World *w, ScoreFn fn) {
    int s = 0;
    int i = 0;
    while (i < w->count) {
        if (w->entities[i].active) {
            s = s + fn(&w->entities[i]);
        }
        i = i + 1;
    }
    return s;
}

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int array_sum(int *arr, int n) {
    int s = 0;
    int i = 0;
    while (i < n) {
        s = s + arr[i];
        i = i + 1;
    }
    return s;
}

void bubble_sort(int *arr, int n) {
    int i = 0;
    while (i < n - 1) {
        int j = 0;
        while (j < n - 1 - i) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

int matrix_trace(int mat[4][4]) {
    return mat[0][0] + mat[1][1] + mat[2][2] + mat[3][3];
}

int main(void) {
    struct World w;
    w.count = 0;

    world_add(&w, 1, 10, 20, 30, 2, 3, 4, 0, "player");
    world_add(&w, 2, -5, 10, 0, 1, 1, 1, 90, "enemy");
    world_add(&w, 3, 0, 0, 0, 5, 5, 5, 0, "item");
    world_add(&w, 4, 100, 50, 25, 3, 2, 1, 45, "enemy");
    world_add(&w, 5, -10, -20, -30, 1, 1, 1, 180, "npc");

    // EXPECT: active=5
    printf("active=%d\n", count_active(&w));

    // EXPECT: enemies=2
    printf("enemies=%d\n", count_by_tag(&w, "enemy"));

    // EXPECT: players=1
    printf("players=%d\n", count_by_tag(&w, "player"));

    struct Vec3 a;
    a.x = 1; a.y = 2; a.z = 3;
    struct Vec3 b;
    b.x = 4; b.y = 5; b.z = 6;

    int d = dot3(&a, &b);
    // EXPECT: dot=32
    printf("dot=%d\n", d);

    struct Vec3 sum = add3(a, b);
    // EXPECT: sum=5_7_9
    printf("sum=%d_%d_%d\n", sum.x, sum.y, sum.z);

    struct Vec3 sc = scale3(a, 3);
    // EXPECT: scaled=3_6_9
    printf("scaled=%d_%d_%d\n", sc.x, sc.y, sc.z);

    int dist = manhattan(&a, &b);
    // EXPECT: manhattan=9
    printf("manhattan=%d\n", dist);

    int s1 = apply_score(&w, 0, score_dist);
    // EXPECT: player_dist=60
    printf("player_dist=%d\n", s1);

    int s2 = apply_score(&w, 2, score_volume);
    // EXPECT: item_vol=125
    printf("item_vol=%d\n", s2);

    int total_dist = sum_scores(&w, score_dist);
    // EXPECT: total_dist=310
    printf("total_dist=%d\n", total_dist);

    int total_vol = sum_scores(&w, score_volume);
    // EXPECT: total_vol=157
    printf("total_vol=%d\n", total_vol);

    // EXPECT: fib10=55
    printf("fib10=%d\n", fibonacci(10));

    int arr[8];
    arr[0] = 42; arr[1] = 17; arr[2] = 83; arr[3] = 5;
    arr[4] = 91; arr[5] = 33; arr[6] = 68; arr[7] = 11;
    bubble_sort(arr, 8);
    // EXPECT: sorted=5_11_17_33_42_68_83_91
    printf("sorted=%d_%d_%d_%d_%d_%d_%d_%d\n",
           arr[0], arr[1], arr[2], arr[3],
           arr[4], arr[5], arr[6], arr[7]);

    // EXPECT: arr_sum=350
    printf("arr_sum=%d\n", array_sum(arr, 8));

    int mat[4][4];
    int i = 0;
    while (i < 4) {
        int j = 0;
        while (j < 4) {
            mat[i][j] = i * 4 + j + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    // EXPECT: trace=34
    printf("trace=%d\n", matrix_trace(mat));

    {
        int x = 100;
        {
            int x = 200;
            {
                int x = 300;
                // EXPECT: inner_x=300
                printf("inner_x=%d\n", x);
            }
            // EXPECT: mid_x=200
            printf("mid_x=%d\n", x);
        }
        // EXPECT: outer_x=100
        printf("outer_x=%d\n", x);
    }

    w.entities[1].active = 0;
    // EXPECT: active_after=4
    printf("active_after=%d\n", count_active(&w));

    // EXPECT: enemies_after=1
    printf("enemies_after=%d\n", count_by_tag(&w, "enemy"));

    return 0;
}
