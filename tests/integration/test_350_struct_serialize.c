int printf(const char *fmt, ...);
// EXPECT: cp1: pos=(10,20,30) rgb=(255,128,0)\ncp2: pos=(-5,15,-10) rgb=(0,255,64)\nserialized 12 ints\nbuffer: 10 20 30 255 128 0 -5 15 -10 0 255 64\nchecksum: 762\nrestored1: pos=(10,20,30) rgb=(255,128,0)\nrestored2: pos=(-5,15,-10) rgb=(0,255,64)\nmatch1: 1\nmatch2: 1\npoints serialized: 9 ints\npoints checksum: 45
// Test: struct serialization to int array

struct Point3D {
    int x;
    int y;
    int z;
};

struct ColorPoint {
    struct Point3D pos;
    int r;
    int g;
    int b;
};

int serialize_point(struct Point3D *p, int buf[], int offset) {
    buf[offset] = p->x;
    buf[offset + 1] = p->y;
    buf[offset + 2] = p->z;
    return offset + 3;
}

int deserialize_point(struct Point3D *p, int buf[], int offset) {
    p->x = buf[offset];
    p->y = buf[offset + 1];
    p->z = buf[offset + 2];
    return offset + 3;
}

int serialize_color_point(struct ColorPoint *cp, int buf[], int offset) {
    offset = serialize_point(&cp->pos, buf, offset);
    buf[offset] = cp->r;
    buf[offset + 1] = cp->g;
    buf[offset + 2] = cp->b;
    return offset + 3;
}

int deserialize_color_point(struct ColorPoint *cp, int buf[], int offset) {
    offset = deserialize_point(&cp->pos, buf, offset);
    cp->r = buf[offset];
    cp->g = buf[offset + 1];
    cp->b = buf[offset + 2];
    return offset + 3;
}

int checksum(int buf[], int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum = sum + buf[i];
    }
    return sum;
}

void print_point(struct Point3D *p) {
    printf("(%d,%d,%d)", p->x, p->y, p->z);
}

void print_color_point(struct ColorPoint *cp) {
    printf("pos=");
    print_point(&cp->pos);
    printf(" rgb=(%d,%d,%d)", cp->r, cp->g, cp->b);
}

int main(void) {
    struct ColorPoint cp1;
    cp1.pos.x = 10; cp1.pos.y = 20; cp1.pos.z = 30;
    cp1.r = 255; cp1.g = 128; cp1.b = 0;

    struct ColorPoint cp2;
    cp2.pos.x = -5; cp2.pos.y = 15; cp2.pos.z = -10;
    cp2.r = 0; cp2.g = 255; cp2.b = 64;

    printf("cp1: "); print_color_point(&cp1); printf("\n");
    printf("cp2: "); print_color_point(&cp2); printf("\n");

    int buffer[20];
    int offset = 0;
    offset = serialize_color_point(&cp1, buffer, offset);
    offset = serialize_color_point(&cp2, buffer, offset);
    printf("serialized %d ints\n", offset);

    printf("buffer: ");
    int i;
    for (i = 0; i < offset; i++) {
        if (i > 0) printf(" ");
        printf("%d", buffer[i]);
    }
    printf("\n");
    printf("checksum: %d\n", checksum(buffer, offset));

    struct ColorPoint restored1;
    struct ColorPoint restored2;
    int off2 = 0;
    off2 = deserialize_color_point(&restored1, buffer, off2);
    off2 = deserialize_color_point(&restored2, buffer, off2);

    printf("restored1: "); print_color_point(&restored1); printf("\n");
    printf("restored2: "); print_color_point(&restored2); printf("\n");

    int match1 = (cp1.pos.x == restored1.pos.x && cp1.pos.y == restored1.pos.y &&
                  cp1.pos.z == restored1.pos.z && cp1.r == restored1.r &&
                  cp1.g == restored1.g && cp1.b == restored1.b);
    int match2 = (cp2.pos.x == restored2.pos.x && cp2.pos.y == restored2.pos.y &&
                  cp2.pos.z == restored2.pos.z && cp2.r == restored2.r &&
                  cp2.g == restored2.g && cp2.b == restored2.b);
    printf("match1: %d\n", match1);
    printf("match2: %d\n", match2);

    struct Point3D points[3];
    points[0].x = 1; points[0].y = 2; points[0].z = 3;
    points[1].x = 4; points[1].y = 5; points[1].z = 6;
    points[2].x = 7; points[2].y = 8; points[2].z = 9;

    int pbuf[12];
    int poff = 0;
    for (i = 0; i < 3; i++) {
        poff = serialize_point(&points[i], pbuf, poff);
    }
    printf("points serialized: %d ints\n", poff);
    printf("points checksum: %d\n", checksum(pbuf, poff));

    return 0;
}
