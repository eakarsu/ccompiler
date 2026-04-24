int printf(const char *fmt, ...);

typedef int Coord;
typedef Coord Distance;
typedef int (*MapFunc)(int);
typedef int (*ReduceFunc)(int, int);

typedef struct {
    Coord x;
    Coord y;
} Vec2;

typedef struct {
    Vec2 position;
    Vec2 velocity;
    int mass;
} Particle;

typedef Particle *ParticleRef;

typedef struct {
    int key;
    int value;
} KVPair;

typedef struct {
    KVPair entries[8];
    int size;
} SmallMap;

typedef SmallMap *SmallMapPtr;

int identity(int x) { return x; }
int triple(int x) { return x * 3; }
int inc(int x) { return x + 1; }

int sum_reduce(int a, int b) { return a + b; }
int max_reduce(int a, int b) { if (a > b) return a; return b; }

void map_array(int *arr, int n, MapFunc f) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = f(arr[i]);
    }
}

int reduce_array(int *arr, int n, ReduceFunc f) {
    int result = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        result = f(result, arr[i]);
    }
    return result;
}

void test_map_reduce() {
    int data[5];
    data[0] = 1; data[1] = 2; data[2] = 3; data[3] = 4; data[4] = 5;

    map_array(data, 5, triple);
    // EXPECT: tripled: 3 6 9 12 15
    printf("tripled: %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4]);

    int total = reduce_array(data, 5, sum_reduce);
    // EXPECT: sum=45
    printf("sum=%d\n", total);

    int mx = reduce_array(data, 5, max_reduce);
    // EXPECT: max=15
    printf("max=%d\n", mx);

    map_array(data, 5, inc);
    // EXPECT: incremented: 4 7 10 13 16
    printf("incremented: %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4]);
}

Distance vec2_manhattan(Vec2 a, Vec2 b) {
    Distance dx = a.x - b.x;
    Distance dy = a.y - b.y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

void particle_step(ParticleRef p) {
    p->position.x = p->position.x + p->velocity.x;
    p->position.y = p->position.y + p->velocity.y;
}

void test_particle() {
    Particle p;
    p.position.x = 0;
    p.position.y = 0;
    p.velocity.x = 3;
    p.velocity.y = -2;
    p.mass = 10;

    ParticleRef pr = &p;
    int step;
    for (step = 0; step < 5; step++) {
        particle_step(pr);
    }
    // EXPECT: particle pos(15,-10) mass=10
    printf("particle pos(%d,%d) mass=%d\n", p.position.x, p.position.y, p.mass);

    Vec2 origin;
    origin.x = 0;
    origin.y = 0;
    Distance d = vec2_manhattan(p.position, origin);
    // EXPECT: distance=25
    printf("distance=%d\n", d);
}

void smallmap_init(SmallMapPtr m) {
    m->size = 0;
}

void smallmap_put(SmallMapPtr m, int key, int value) {
    int i;
    for (i = 0; i < m->size; i++) {
        if (m->entries[i].key == key) {
            m->entries[i].value = value;
            return;
        }
    }
    if (m->size < 8) {
        m->entries[m->size].key = key;
        m->entries[m->size].value = value;
        m->size = m->size + 1;
    }
}

int smallmap_get(SmallMapPtr m, int key, int default_val) {
    int i;
    for (i = 0; i < m->size; i++) {
        if (m->entries[i].key == key) {
            return m->entries[i].value;
        }
    }
    return default_val;
}

int smallmap_has(SmallMapPtr m, int key) {
    int i;
    for (i = 0; i < m->size; i++) {
        if (m->entries[i].key == key) return 1;
    }
    return 0;
}

void test_smallmap() {
    SmallMap m;
    smallmap_init(&m);

    smallmap_put(&m, 1, 100);
    smallmap_put(&m, 2, 200);
    smallmap_put(&m, 3, 300);

    // EXPECT: map size=3
    printf("map size=%d\n", m.size);
    // EXPECT: get(1)=100
    printf("get(1)=%d\n", smallmap_get(&m, 1, -1));
    // EXPECT: get(2)=200
    printf("get(2)=%d\n", smallmap_get(&m, 2, -1));
    // EXPECT: get(3)=300
    printf("get(3)=%d\n", smallmap_get(&m, 3, -1));
    // EXPECT: get(4)=-1
    printf("get(4)=%d\n", smallmap_get(&m, 4, -1));

    smallmap_put(&m, 2, 999);
    // EXPECT: updated get(2)=999
    printf("updated get(2)=%d\n", smallmap_get(&m, 2, -1));
    // EXPECT: has(1)=1 has(5)=0
    printf("has(1)=%d has(5)=%d\n", smallmap_has(&m, 1), smallmap_has(&m, 5));
}

typedef struct {
    MapFunc transform;
    char *name;
} NamedTransform;

void test_named_transforms() {
    NamedTransform transforms[3];
    transforms[0].transform = identity;
    transforms[0].name = "identity";
    transforms[1].transform = triple;
    transforms[1].name = "triple";
    transforms[2].transform = inc;
    transforms[2].name = "inc";

    int val = 10;
    int i;
    // EXPECT: identity(10)=10
    // EXPECT: triple(10)=30
    // EXPECT: inc(10)=11
    for (i = 0; i < 3; i++) {
        int result = transforms[i].transform(val);
        printf("%s(%d)=%d\n", transforms[i].name, val, result);
    }
}

void test_chain_transforms() {
    MapFunc chain[4];
    chain[0] = inc;
    chain[1] = triple;
    chain[2] = inc;
    chain[3] = triple;

    int val = 1;
    int i;
    for (i = 0; i < 4; i++) {
        val = chain[i](val);
    }
    // EXPECT: chained=21
    printf("chained=%d\n", val);
}

int main() {
    test_map_reduce();
    test_particle();
    test_smallmap();
    test_named_transforms();
    test_chain_transforms();
    // EXPECT: done
    printf("done\n");
    return 0;
}
