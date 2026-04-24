int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
    int z;
};

struct Color {
    int r;
    int g;
    int b;
    int a;
};

struct Particle {
    struct Point pos;
    struct Point vel;
    struct Color col;
    int mass;
    int age;
};

void serialize_point(struct Point *p, int *buf, int offset) {
    buf[offset] = p->x;
    buf[offset + 1] = p->y;
    buf[offset + 2] = p->z;
}

void deserialize_point(struct Point *p, int *buf, int offset) {
    p->x = buf[offset];
    p->y = buf[offset + 1];
    p->z = buf[offset + 2];
}

void serialize_color(struct Color *c, int *buf, int offset) {
    buf[offset] = c->r;
    buf[offset + 1] = c->g;
    buf[offset + 2] = c->b;
    buf[offset + 3] = c->a;
}

void deserialize_color(struct Color *c, int *buf, int offset) {
    c->r = buf[offset];
    c->g = buf[offset + 1];
    c->b = buf[offset + 2];
    c->a = buf[offset + 3];
}

int serialize_particle(struct Particle *p, int *buf) {
    serialize_point(&p->pos, buf, 0);
    serialize_point(&p->vel, buf, 3);
    serialize_color(&p->col, buf, 6);
    buf[10] = p->mass;
    buf[11] = p->age;
    return 12;
}

int deserialize_particle(struct Particle *p, int *buf) {
    deserialize_point(&p->pos, buf, 0);
    deserialize_point(&p->vel, buf, 3);
    deserialize_color(&p->col, buf, 6);
    p->mass = buf[10];
    p->age = buf[11];
    return 12;
}

int compute_checksum(int *buf, int len) {
    int sum = 0;
    int i = 0;
    while (i < len) {
        sum = sum + buf[i] * (i + 1);
        i = i + 1;
    }
    return sum;
}

int compare_particles(struct Particle *a, struct Particle *b) {
    if (a->pos.x != b->pos.x) return 0;
    if (a->pos.y != b->pos.y) return 0;
    if (a->pos.z != b->pos.z) return 0;
    if (a->vel.x != b->vel.x) return 0;
    if (a->vel.y != b->vel.y) return 0;
    if (a->vel.z != b->vel.z) return 0;
    if (a->col.r != b->col.r) return 0;
    if (a->col.g != b->col.g) return 0;
    if (a->col.b != b->col.b) return 0;
    if (a->col.a != b->col.a) return 0;
    if (a->mass != b->mass) return 0;
    if (a->age != b->age) return 0;
    return 1;
}

int main(void) {
    struct Particle p1;
    p1.pos.x = 10;
    p1.pos.y = 20;
    p1.pos.z = 30;
    p1.vel.x = -1;
    p1.vel.y = 2;
    p1.vel.z = -3;
    p1.col.r = 255;
    p1.col.g = 128;
    p1.col.b = 64;
    p1.col.a = 200;
    p1.mass = 50;
    p1.age = 100;

    int buf[12];
    int len = serialize_particle(&p1, buf);
    // EXPECT: serialized_len=12
    printf("serialized_len=%d\n", len);

    // EXPECT: buf0=10
    printf("buf0=%d\n", buf[0]);
    // EXPECT: buf1=20
    printf("buf1=%d\n", buf[1]);
    // EXPECT: buf2=30
    printf("buf2=%d\n", buf[2]);
    // EXPECT: buf3=-1
    printf("buf3=%d\n", buf[3]);
    // EXPECT: buf6=255
    printf("buf6=%d\n", buf[6]);
    // EXPECT: buf10=50
    printf("buf10=%d\n", buf[10]);
    // EXPECT: buf11=100
    printf("buf11=%d\n", buf[11]);

    int cksum = compute_checksum(buf, 12);
    // EXPECT: checksum=7263
    printf("checksum=%d\n", cksum);

    struct Particle p2;
    deserialize_particle(&p2, buf);
    // EXPECT: equal=1
    printf("equal=%d\n", compare_particles(&p1, &p2));

    // EXPECT: p2_x=10
    printf("p2_x=%d\n", p2.pos.x);
    // EXPECT: p2_vy=2
    printf("p2_vy=%d\n", p2.vel.y);
    // EXPECT: p2_r=255
    printf("p2_r=%d\n", p2.col.r);
    // EXPECT: p2_mass=50
    printf("p2_mass=%d\n", p2.mass);

    buf[5] = 99;
    struct Particle p3;
    deserialize_particle(&p3, buf);
    // EXPECT: modified_equal=0
    printf("modified_equal=%d\n", compare_particles(&p1, &p3));
    // EXPECT: p3_vz=99
    printf("p3_vz=%d\n", p3.vel.z);

    int buf2[12];
    serialize_particle(&p3, buf2);
    int cksum2 = compute_checksum(buf2, 12);
    // EXPECT: checksum2=7875
    printf("checksum2=%d\n", cksum2);

    struct Particle parr[3];
    int bigbuf[36];
    int idx = 0;
    while (idx < 3) {
        parr[idx].pos.x = idx * 10;
        parr[idx].pos.y = idx * 20;
        parr[idx].pos.z = idx * 30;
        parr[idx].vel.x = idx;
        parr[idx].vel.y = idx;
        parr[idx].vel.z = idx;
        parr[idx].col.r = idx * 50;
        parr[idx].col.g = idx * 40;
        parr[idx].col.b = idx * 30;
        parr[idx].col.a = idx * 20;
        parr[idx].mass = idx + 1;
        parr[idx].age = idx * 5;
        serialize_particle(&parr[idx], bigbuf + idx * 12);
        idx = idx + 1;
    }
    int total_cksum = 0;
    idx = 0;
    while (idx < 3) {
        total_cksum = total_cksum + compute_checksum(
            bigbuf + idx * 12, 12);
        idx = idx + 1;
    }
    // EXPECT: total_cksum=4131
    printf("total_cksum=%d\n", total_cksum);

    return 0;
}
