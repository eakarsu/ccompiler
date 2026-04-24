int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Vec2;

typedef struct {
    int values[4];
} IntQuad;

typedef struct {
    Vec2 position;
    Vec2 velocity;
    int mass;
} Particle;

typedef struct {
    IntQuad row0;
    IntQuad row1;
} Matrix2x4;

typedef struct {
    Particle particles[4];
    int count;
} ParticleSystem;

Vec2 vec2_new(int x, int y) {
    Vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

Vec2 vec2_add(Vec2 a, Vec2 b) {
    Vec2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

IntQuad quad_new(int a, int b, int c, int d) {
    IntQuad q;
    q.values[0] = a;
    q.values[1] = b;
    q.values[2] = c;
    q.values[3] = d;
    return q;
}

int quad_sum(IntQuad *q) {
    return q->values[0] + q->values[1] + q->values[2] + q->values[3];
}

int quad_max(IntQuad *q) {
    int m = q->values[0];
    int i;
    for (i = 1; i < 4; i = i + 1) {
        if (q->values[i] > m) m = q->values[i];
    }
    return m;
}

void particle_init(Particle *p, int px, int py, int vx, int vy, int m) {
    p->position = vec2_new(px, py);
    p->velocity = vec2_new(vx, vy);
    p->mass = m;
}

void particle_step(Particle *p) {
    p->position = vec2_add(p->position, p->velocity);
}

int particle_energy(Particle *p) {
    int vx = p->velocity.x;
    int vy = p->velocity.y;
    return p->mass * (vx * vx + vy * vy) / 2;
}

void system_init(ParticleSystem *sys) {
    sys->count = 0;
}

int system_add(ParticleSystem *sys, int px, int py, int vx, int vy, int m) {
    if (sys->count >= 4) return 0;
    particle_init(&sys->particles[sys->count], px, py, vx, vy, m);
    sys->count = sys->count + 1;
    return 1;
}

void system_step(ParticleSystem *sys) {
    int i;
    for (i = 0; i < sys->count; i = i + 1) {
        particle_step(&sys->particles[i]);
    }
}

int system_total_energy(ParticleSystem *sys) {
    int total = 0;
    int i;
    for (i = 0; i < sys->count; i = i + 1) {
        total = total + particle_energy(&sys->particles[i]);
    }
    return total;
}

int system_total_mass(ParticleSystem *sys) {
    int total = 0;
    int i;
    for (i = 0; i < sys->count; i = i + 1) {
        total = total + sys->particles[i].mass;
    }
    return total;
}

Matrix2x4 matrix_new(IntQuad r0, IntQuad r1) {
    Matrix2x4 m;
    m.row0 = r0;
    m.row1 = r1;
    return m;
}

int matrix_element(Matrix2x4 *m, int row, int col) {
    if (row == 0) return m->row0.values[col];
    return m->row1.values[col];
}

int matrix_sum(Matrix2x4 *m) {
    return quad_sum(&m->row0) + quad_sum(&m->row1);
}

int main(void) {
    Vec2 v1 = vec2_new(3, 4);
    // EXPECT: v1: 3 4
    printf("v1: %d %d\n", v1.x, v1.y);

    Vec2 v2 = vec2_new(1, 2);
    Vec2 v3 = vec2_add(v1, v2);
    // EXPECT: v1+v2: 4 6
    printf("v1+v2: %d %d\n", v3.x, v3.y);

    IntQuad q = quad_new(10, 20, 30, 40);
    // EXPECT: quad sum: 100
    printf("quad sum: %d\n", quad_sum(&q));

    // EXPECT: quad max: 40
    printf("quad max: %d\n", quad_max(&q));

    // EXPECT: quad[2]: 30
    printf("quad[2]: %d\n", q.values[2]);

    Particle p;
    particle_init(&p, 0, 0, 2, 3, 10);
    // EXPECT: p pos: 0 0
    printf("p pos: %d %d\n", p.position.x, p.position.y);

    // EXPECT: p vel: 2 3
    printf("p vel: %d %d\n", p.velocity.x, p.velocity.y);

    particle_step(&p);
    // EXPECT: p after step: 2 3
    printf("p after step: %d %d\n", p.position.x, p.position.y);

    particle_step(&p);
    // EXPECT: p after step2: 4 6
    printf("p after step2: %d %d\n", p.position.x, p.position.y);

    // EXPECT: p energy: 65
    printf("p energy: %d\n", particle_energy(&p));

    ParticleSystem sys;
    system_init(&sys);
    system_add(&sys, 0, 0, 1, 0, 5);
    system_add(&sys, 10, 10, 0, 1, 3);
    system_add(&sys, 5, 5, 1, 1, 2);
    // EXPECT: sys count: 3
    printf("sys count: %d\n", sys.count);

    // EXPECT: sys total mass: 10
    printf("sys total mass: %d\n", system_total_mass(&sys));

    // EXPECT: sys total energy: 5
    printf("sys total energy: %d\n", system_total_energy(&sys));

    system_step(&sys);
    // EXPECT: p0 after sys step: 1 0
    printf("p0 after sys step: %d %d\n", sys.particles[0].position.x, sys.particles[0].position.y);

    // EXPECT: p1 after sys step: 10 11
    printf("p1 after sys step: %d %d\n", sys.particles[1].position.x, sys.particles[1].position.y);

    IntQuad r0 = quad_new(1, 2, 3, 4);
    IntQuad r1 = quad_new(5, 6, 7, 8);
    Matrix2x4 mat = matrix_new(r0, r1);
    // EXPECT: mat[0][0]: 1
    printf("mat[0][0]: %d\n", matrix_element(&mat, 0, 0));

    // EXPECT: mat[1][2]: 7
    printf("mat[1][2]: %d\n", matrix_element(&mat, 1, 2));

    // EXPECT: mat sum: 36
    printf("mat sum: %d\n", matrix_sum(&mat));

    // EXPECT: mat row0 max: 4
    printf("mat row0 max: %d\n", quad_max(&mat.row0));

    // EXPECT: mat row1 max: 8
    printf("mat row1 max: %d\n", quad_max(&mat.row1));

    return 0;
}
