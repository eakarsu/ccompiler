int printf(const char *fmt, ...);
// EXPECT: b0:x=  0,y=0|step1:x=3,y=4|step5:x=15,y=20|accel:x=30,y=15,vx=10,vy=5|proj:h= 55,t=19,r=95|proj2:h=210,t=39,r=312|proj3:h=15,r=27|coll01= 1|coll02= 0|coll12= 0|tot_coll= 1|ke(0)=25|ke(1)=2|total_ke= 27|mom_x= 11|mom_y= 8|dist01= 14|1d_init:p0=  0,p1=10|1d_after3:p0=15,v0=5,p1=1,v1=-3|bbox:-10,20,-15,30|isqrt:0,1,2,3,4,5,10,12|approach:t=22,x0=   22,x1=28,coll= 1|1d_uneq:v0=  5,v1=15|end

/* Integer physics simulation */

struct Body {
    int x;
    int y;
    int vx;
    int vy;
    int ax;
    int ay;
    int mass;
    int radius;
    int active;
};

struct Body bodies[16];
int body_count;
int sim_time;

/* 1D particles for collision */
struct Particle1D {
    int pos;
    int vel;
    int mass;
};

struct Particle1D particles[16];
int particle_count;

void init_physics(void) {
    int i;
    body_count = 0;
    sim_time = 0;
    particle_count = 0;
    i = 0;
    while (i < 16) {
        bodies[i].x = 0;
        bodies[i].y = 0;
        bodies[i].vx = 0;
        bodies[i].vy = 0;
        bodies[i].ax = 0;
        bodies[i].ay = 0;
        bodies[i].mass = 1;
        bodies[i].radius = 1;
        bodies[i].active = 0;
        i = i + 1;
    }
}

int create_body(int x, int y, int vx, int vy, int mass, int radius) {
    int idx;
    idx = body_count;
    if (idx >= 16) return -1;
    bodies[idx].x = x;
    bodies[idx].y = y;
    bodies[idx].vx = vx;
    bodies[idx].vy = vy;
    bodies[idx].ax = 0;
    bodies[idx].ay = 0;
    bodies[idx].mass = mass;
    bodies[idx].radius = radius;
    bodies[idx].active = 1;
    body_count = body_count + 1;
    return idx;
}

void set_accel(int idx, int ax, int ay) {
    bodies[idx].ax = ax;
    bodies[idx].ay = ay;
}

/* Update position and velocity for one time step */
void step_body(int idx) {
    if (!bodies[idx].active) return;
    /* v = v + a */
    bodies[idx].vx = bodies[idx].vx + bodies[idx].ax;
    bodies[idx].vy = bodies[idx].vy + bodies[idx].ay;
    /* x = x + v */
    bodies[idx].x = bodies[idx].x + bodies[idx].vx;
    bodies[idx].y = bodies[idx].y + bodies[idx].vy;
}

void step_all(void) {
    int i;
    i = 0;
    while (i < body_count) {
        step_body(i);
        i = i + 1;
    }
    sim_time = sim_time + 1;
}

/* Run n steps */
void run_steps(int n) {
    int i;
    i = 0;
    while (i < n) {
        step_all();
        i = i + 1;
    }
}

/* Projectile motion: integer approximation of gravity */
/* Launch at angle (represented as vx, vy) with gravity = -1 per step */
int projectile_max_height(int vy0) {
    /* Max height when vy becomes 0 */
    /* h = vy0*(vy0+1)/2 approximately */
    int h;
    int vy;
    h = 0;
    vy = vy0;
    while (vy > 0) {
        h = h + vy;
        vy = vy - 1; /* gravity */
    }
    return h;
}

int projectile_flight_time(int vy0) {
    /* Time until y returns to 0 or below */
    int y;
    int vy;
    int t;
    y = 0;
    vy = vy0;
    t = 0;
    do {
        vy = vy - 1;
        y = y + vy;
        t = t + 1;
    } while (y > 0);
    return t;
}

int projectile_range(int vx0, int vy0) {
    /* Horizontal distance when projectile lands */
    int x;
    int y;
    int vx;
    int vy;
    x = 0;
    y = 0;
    vx = vx0;
    vy = vy0;
    do {
        vy = vy - 1;
        x = x + vx;
        y = y + vy;
    } while (y > 0);
    return x;
}

/* Distance squared between two bodies */
int dist_sq(int i, int j) {
    int dx;
    int dy;
    dx = bodies[i].x - bodies[j].x;
    dy = bodies[i].y - bodies[j].y;
    return dx * dx + dy * dy;
}

/* Check collision between two bodies (circles) */
int check_collision(int i, int j) {
    int d2;
    int r_sum;
    d2 = dist_sq(i, j);
    r_sum = bodies[i].radius + bodies[j].radius;
    return (d2 <= r_sum * r_sum);
}

/* Count all collisions among active bodies */
int count_collisions(void) {
    int cnt;
    int i;
    int j;
    cnt = 0;
    i = 0;
    while (i < body_count) {
        if (!bodies[i].active) { i = i + 1; continue; }
        j = i + 1;
        while (j < body_count) {
            if (!bodies[j].active) { j = j + 1; continue; }
            if (check_collision(i, j)) {
                cnt = cnt + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return cnt;
}

/* Kinetic energy (integer approx): 1/2 * m * v^2 -> m * (vx^2 + vy^2) / 2 */
int kinetic_energy(int idx) {
    int v2;
    v2 = bodies[idx].vx * bodies[idx].vx + bodies[idx].vy * bodies[idx].vy;
    return bodies[idx].mass * v2 / 2;
}

int total_kinetic_energy(void) {
    int s;
    int i;
    s = 0;
    i = 0;
    while (i < body_count) {
        if (bodies[i].active) {
            s = s + kinetic_energy(i);
        }
        i = i + 1;
    }
    return s;
}

/* Momentum: m * v */
int total_momentum_x(void) {
    int s;
    int i;
    s = 0;
    i = 0;
    while (i < body_count) {
        if (bodies[i].active) {
            s = s + bodies[i].mass * bodies[i].vx;
        }
        i = i + 1;
    }
    return s;
}

int total_momentum_y(void) {
    int s;
    int i;
    s = 0;
    i = 0;
    while (i < body_count) {
        if (bodies[i].active) {
            s = s + bodies[i].mass * bodies[i].vy;
        }
        i = i + 1;
    }
    return s;
}

/* 1D elastic collision: swap velocities for equal mass */
void collide_1d(int i, int j) {
    int tmp;
    /* For equal mass, just swap velocities */
    if (particles[i].mass == particles[j].mass) {
        tmp = particles[i].vel;
        particles[i].vel = particles[j].vel;
        particles[j].vel = tmp;
    } else {
        /* General case: v1' = (m1-m2)v1 + 2m2v2 / (m1+m2) */
        int m1;
        int m2;
        int v1;
        int v2;
        int nv1;
        int nv2;
        m1 = particles[i].mass;
        m2 = particles[j].mass;
        v1 = particles[i].vel;
        v2 = particles[j].vel;
        nv1 = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2);
        nv2 = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2);
        particles[i].vel = nv1;
        particles[j].vel = nv2;
    }
}

/* Check if two 1D particles overlap */
int overlap_1d(int i, int j) {
    int diff;
    diff = particles[i].pos - particles[j].pos;
    if (diff < 0) diff = 0 - diff;
    return (diff <= 1); /* within 1 unit */
}

/* Step 1D particles */
void step_1d(void) {
    int i;
    int j;
    /* Move */
    i = 0;
    while (i < particle_count) {
        particles[i].pos = particles[i].pos + particles[i].vel;
        i = i + 1;
    }
    /* Check collisions */
    i = 0;
    while (i < particle_count) {
        j = i + 1;
        while (j < particle_count) {
            if (overlap_1d(i, j)) {
                collide_1d(i, j);
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

/* Integer sqrt approximation */
int isqrt(int n) {
    int x;
    int x1;
    if (n <= 0) return 0;
    x = n;
    x1 = (x + 1) / 2;
    while (x1 < x) {
        x = x1;
        x1 = (x + n / x) / 2;
    }
    return x;
}

/* Distance (integer approx) */
int distance(int i, int j) {
    return isqrt(dist_sq(i, j));
}

/* Bounding box of all active bodies */
int bbox_min_x(void) {
    int m;
    int i;
    int first;
    first = 1;
    m = 0;
    i = 0;
    while (i < body_count) {
        if (bodies[i].active) {
            if (first) { m = bodies[i].x; first = 0; }
            else if (bodies[i].x < m) m = bodies[i].x;
        }
        i = i + 1;
    }
    return m;
}

int bbox_max_x(void) {
    int m;
    int i;
    int first;
    first = 1;
    m = 0;
    i = 0;
    while (i < body_count) {
        if (bodies[i].active) {
            if (first) { m = bodies[i].x; first = 0; }
            else if (bodies[i].x > m) m = bodies[i].x;
        }
        i = i + 1;
    }
    return m;
}

int bbox_min_y(void) {
    int m;
    int i;
    int first;
    first = 1;
    m = 0;
    i = 0;
    while (i < body_count) {
        if (bodies[i].active) {
            if (first) { m = bodies[i].y; first = 0; }
            else if (bodies[i].y < m) m = bodies[i].y;
        }
        i = i + 1;
    }
    return m;
}

int bbox_max_y(void) {
    int m;
    int i;
    int first;
    first = 1;
    m = 0;
    i = 0;
    while (i < body_count) {
        if (bodies[i].active) {
            if (first) { m = bodies[i].y; first = 0; }
            else if (bodies[i].y > m) m = bodies[i].y;
        }
        i = i + 1;
    }
    return m;
}

int main(void) {
    int b0;
    int b1;
    int b2;
    int b3;
    int i;

    init_physics();

    /* Test 1: Create and step a body */
    b0 = create_body(0, 0, 3, 4, 10, 2);
    printf("%c%c%c%c%c%c%c", 'b', '0', ':', 'x', '=', ' ', ' ');
    printf("%d", bodies[b0].x);
    printf("%c%c", ',', 'y');
    printf("%c", '=');
    printf("%d", bodies[b0].y);
    step_body(b0);
    printf("%c%c%c%c%c%c%c%c", '|', 's', 't', 'e', 'p', '1', ':', 'x');
    printf("%c", '=');
    printf("%d", bodies[b0].x);
    printf("%c%c%c", ',', 'y', '=');
    printf("%d", bodies[b0].y);

    /* Step 4 more times */
    run_steps(4);
    printf("%c%c%c%c%c%c%c%c", '|', 's', 't', 'e', 'p', '5', ':', 'x');
    printf("%c", '=');
    printf("%d", bodies[b0].x);
    printf("%c%c%c", ',', 'y', '=');
    printf("%d", bodies[b0].y);

    /* Test 2: Body with acceleration */
    init_physics();
    b0 = create_body(0, 0, 0, 0, 5, 1);
    set_accel(b0, 2, 1);
    run_steps(5);
    printf("%c%c%c%c%c%c%c", '|', 'a', 'c', 'c', 'e', 'l', ':');
    printf("%c%c", 'x', '=');
    printf("%d", bodies[b0].x);
    printf("%c%c%c", ',', 'y', '=');
    printf("%d", bodies[b0].y);
    printf("%c%c%c%c", ',', 'v', 'x', '=');
    printf("%d", bodies[b0].vx);
    printf("%c%c%c%c", ',', 'v', 'y', '=');
    printf("%d", bodies[b0].vy);

    /* Test 3: Projectile motion */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'p', 'r', 'o', 'j', ':', 'h', '=', ' ');
    printf("%d", projectile_max_height(10));
    printf("%c%c", ',', 't');
    printf("%c", '=');
    printf("%d", projectile_flight_time(10));
    printf("%c%c", ',', 'r');
    printf("%c", '=');
    printf("%d", projectile_range(5, 10));

    /* Projectile with different params */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'p', 'r', 'o', 'j', '2', ':', 'h', '=');
    printf("%d", projectile_max_height(20));
    printf("%c%c", ',', 't');
    printf("%c", '=');
    printf("%d", projectile_flight_time(20));
    printf("%c%c", ',', 'r');
    printf("%c", '=');
    printf("%d", projectile_range(8, 20));

    /* Small projectile */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'p', 'r', 'o', 'j', '3', ':', 'h', '=');
    printf("%d", projectile_max_height(5));
    printf("%c%c", ',', 'r');
    printf("%c", '=');
    printf("%d", projectile_range(3, 5));

    /* Test 4: Collision detection */
    init_physics();
    b0 = create_body(0, 0, 0, 0, 1, 5);
    b1 = create_body(3, 0, 0, 0, 1, 5);
    b2 = create_body(100, 100, 0, 0, 1, 5);
    printf("%c%c%c%c%c%c%c%c%c", '|', 'c', 'o', 'l', 'l', '0', '1', '=', ' ');
    printf("%d", check_collision(b0, b1));
    printf("%c%c%c%c%c%c%c%c%c", '|', 'c', 'o', 'l', 'l', '0', '2', '=', ' ');
    printf("%d", check_collision(b0, b2));
    printf("%c%c%c%c%c%c%c%c%c", '|', 'c', 'o', 'l', 'l', '1', '2', '=', ' ');
    printf("%d", check_collision(b1, b2));

    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 't', 'o', 't', '_', 'c', 'o', 'l', 'l', '=', ' ');
    printf("%d", count_collisions());

    /* Test 5: Kinetic energy */
    init_physics();
    b0 = create_body(0, 0, 3, 4, 2, 1);
    b1 = create_body(10, 10, 1, 0, 5, 1);
    printf("%c%c%c%c%c%c%c", '|', 'k', 'e', '(', '0', ')', '=');
    printf("%d", kinetic_energy(b0));
    printf("%c%c%c%c%c%c%c", '|', 'k', 'e', '(', '1', ')', '=');
    printf("%d", kinetic_energy(b1));
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 't', 'o', 't', 'a', 'l', '_', 'k', 'e', '=', ' ');
    printf("%d", total_kinetic_energy());

    /* Test 6: Momentum */
    printf("%c%c%c%c%c%c%c%c", '|', 'm', 'o', 'm', '_', 'x', '=', ' ');
    printf("%d", total_momentum_x());
    printf("%c%c%c%c%c%c%c%c", '|', 'm', 'o', 'm', '_', 'y', '=', ' ');
    printf("%d", total_momentum_y());

    /* Test 7: Distance */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'd', 'i', 's', 't', '0', '1', '=', ' ');
    printf("%d", distance(b0, b1));

    /* Test 8: 1D collisions */
    particles[0].pos = 0; particles[0].vel = 5; particles[0].mass = 1;
    particles[1].pos = 10; particles[1].vel = -3; particles[1].mass = 1;
    particle_count = 2;
    printf("%c%c%c%c%c%c%c%c", '|', '1', 'd', '_', 'i', 'n', 'i', 't');
    printf("%c%c%c%c%c%c", ':', 'p', '0', '=', ' ', ' ');
    printf("%d", particles[0].pos);
    printf("%c%c%c%c", ',', 'p', '1', '=');
    printf("%d", particles[1].pos);

    /* Run a few steps */
    i = 0;
    while (i < 3) {
        step_1d();
        i = i + 1;
    }
    printf("%c%c%c%c%c%c%c%c%c%c", '|', '1', 'd', '_', 'a', 'f', 't', 'e', 'r', '3');
    printf("%c%c%c%c", ':', 'p', '0', '=');
    printf("%d", particles[0].pos);
    printf("%c%c%c", ',', 'v', '0');
    printf("%c", '=');
    printf("%d", particles[0].vel);
    printf("%c%c%c%c", ',', 'p', '1', '=');
    printf("%d", particles[1].pos);
    printf("%c%c%c%c", ',', 'v', '1', '=');
    printf("%d", particles[1].vel);

    /* Test 9: Bounding box */
    init_physics();
    create_body(-10, 5, 0, 0, 1, 1);
    create_body(20, -15, 0, 0, 1, 1);
    create_body(5, 30, 0, 0, 1, 1);
    printf("%c%c%c%c%c%c", '|', 'b', 'b', 'o', 'x', ':');
    printf("%d", bbox_min_x());
    printf("%c", ',');
    printf("%d", bbox_max_x());
    printf("%c", ',');
    printf("%d", bbox_min_y());
    printf("%c", ',');
    printf("%d", bbox_max_y());

    /* Test 10: isqrt */
    printf("%c%c%c%c%c%c", '|', 'i', 's', 'q', 'r', 't');
    printf("%c", ':');
    printf("%d", isqrt(0));
    printf("%c", ',');
    printf("%d", isqrt(1));
    printf("%c", ',');
    printf("%d", isqrt(4));
    printf("%c", ',');
    printf("%d", isqrt(9));
    printf("%c", ',');
    printf("%d", isqrt(16));
    printf("%c", ',');
    printf("%d", isqrt(25));
    printf("%c", ',');
    printf("%d", isqrt(100));
    printf("%c", ',');
    printf("%d", isqrt(144));

    /* Test 11: Multiple bodies moving */
    init_physics();
    b0 = create_body(0, 0, 1, 0, 1, 3);
    b1 = create_body(50, 0, -1, 0, 1, 3);
    /* Move until they collide or 30 steps */
    i = 0;
    while (i < 30) {
        step_all();
        if (check_collision(b0, b1)) {
            break;
        }
        i = i + 1;
    }
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'a', 'p', 'p', 'r', 'o', 'a', 'c', 'h', ':', 't');
    printf("%c", '=');
    printf("%d", sim_time);
    printf("%c%c%c%c%c%c%c", ',', 'x', '0', '=', ' ', ' ', ' ');
    printf("%d", bodies[b0].x);
    printf("%c%c%c", ',', 'x', '1');
    printf("%c", '=');
    printf("%d", bodies[b1].x);
    printf("%c%c%c%c%c%c%c", ',', 'c', 'o', 'l', 'l', '=', ' ');
    printf("%d", check_collision(b0, b1));

    /* 1D unequal mass collision */
    particles[0].pos = 0; particles[0].vel = 10; particles[0].mass = 3;
    particles[1].pos = 50; particles[1].vel = 0; particles[1].mass = 1;
    particle_count = 2;
    i = 0;
    while (i < 10) {
        step_1d();
        i = i + 1;
    }
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", '|', '1', 'd', '_', 'u', 'n', 'e', 'q', ':', 'v', '0', '=', ' ', ' ');
    printf("%d", particles[0].vel);
    printf("%c%c%c", ',', 'v', '1');
    printf("%c", '=');
    printf("%d", particles[1].vel);

    printf("%c%c%c%c", '|', 'e', 'n', 'd');
    printf("%c", '\n');

    return 0;
}
