int printf(const char *fmt, ...);

char pool[4096];
int pool_offset;

void pool_init(void) {
    pool_offset = 0;
    int i;
    for (i = 0; i < 4096; i = i + 1) {
        pool[i] = 0;
    }
}

char *pool_alloc(int size) {
    /* Align to 8 bytes */
    int aligned = (size + 7) & ~7;
    if (pool_offset + aligned > 4096) {
        return (char *)0;
    }
    char *ptr = &pool[pool_offset];
    pool_offset = pool_offset + aligned;
    return ptr;
}

int pool_used(void) {
    return pool_offset;
}

int pool_remaining(void) {
    return 4096 - pool_offset;
}

struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point top_left;
    struct Point bottom_right;
};

struct Node {
    int value;
    int next_offset;  /* offset into pool, -1 means null */
};

int rect_area(struct Rect *r) {
    int w = r->bottom_right.x - r->top_left.x;
    int h = r->bottom_right.y - r->top_left.y;
    if (w < 0) w = -w;
    if (h < 0) h = -h;
    return w * h;
}

int rect_perimeter(struct Rect *r) {
    int w = r->bottom_right.x - r->top_left.x;
    int h = r->bottom_right.y - r->top_left.y;
    if (w < 0) w = -w;
    if (h < 0) h = -h;
    return 2 * (w + h);
}

int main(void) {
    pool_init();

    // EXPECT: Pool used initially: 0
    printf("Pool used initially: %d\n", pool_used());
    // EXPECT: Pool remaining: 4096
    printf("Pool remaining: %d\n", pool_remaining());

    /* Allocate a Point (8 bytes, already aligned) */
    struct Point *p1 = (struct Point *)pool_alloc(sizeof(struct Point));
    p1->x = 10;
    p1->y = 20;
    // EXPECT: Point1: 10 20
    printf("Point1: %d %d\n", p1->x, p1->y);

    /* sizeof(struct Point) is 8 on most systems, aligned to 8 */
    // EXPECT: Used after Point: 8
    printf("Used after Point: %d\n", pool_used());

    /* Allocate a Rect */
    struct Rect *r1 = (struct Rect *)pool_alloc(sizeof(struct Rect));
    r1->top_left.x = 0;
    r1->top_left.y = 0;
    r1->bottom_right.x = 5;
    r1->bottom_right.y = 3;
    // EXPECT: Rect area: 15
    printf("Rect area: %d\n", rect_area(r1));
    // EXPECT: Rect perimeter: 16
    printf("Rect perimeter: %d\n", rect_perimeter(r1));

    /* sizeof(struct Rect) = 16, aligned to 8 = 16. Total = 8 + 16 = 24 */
    // EXPECT: Used after Rect: 24
    printf("Used after Rect: %d\n", pool_used());

    /* Allocate multiple Points */
    struct Point *points[5];
    int i;
    for (i = 0; i < 5; i = i + 1) {
        points[i] = (struct Point *)pool_alloc(sizeof(struct Point));
        points[i]->x = i * 10;
        points[i]->y = i * 10 + 5;
    }
    // 24 + 5*8 = 64
    // EXPECT: Used after 5 Points: 64
    printf("Used after 5 Points: %d\n", pool_used());

    // Verify the points
    // EXPECT: points[0]: 0 5
    printf("points[0]: %d %d\n", points[0]->x, points[0]->y);
    // EXPECT: points[2]: 20 25
    printf("points[2]: %d %d\n", points[2]->x, points[2]->y);
    // EXPECT: points[4]: 40 45
    printf("points[4]: %d %d\n", points[4]->x, points[4]->y);

    /* Allocate a linked list of Nodes in the pool */
    int node_offsets[4];
    for (i = 0; i < 4; i = i + 1) {
        int before = pool_offset;
        struct Node *n = (struct Node *)pool_alloc(sizeof(struct Node));
        node_offsets[i] = before;
        n->value = (i + 1) * 100;
        n->next_offset = -1;
    }
    /* Link them together */
    for (i = 0; i < 3; i = i + 1) {
        struct Node *n = (struct Node *)&pool[node_offsets[i]];
        n->next_offset = node_offsets[i + 1];
    }

    /* Walk the list */
    int sum = 0;
    int count = 0;
    int offset = node_offsets[0];
    while (offset != -1) {
        struct Node *n = (struct Node *)&pool[offset];
        sum = sum + n->value;
        count = count + 1;
        offset = n->next_offset;
    }
    // values: 100 + 200 + 300 + 400 = 1000
    // EXPECT: List sum: 1000
    printf("List sum: %d\n", sum);
    // EXPECT: List count: 4
    printf("List count: %d\n", count);

    /* Allocate a second Rect */
    struct Rect *r2 = (struct Rect *)pool_alloc(sizeof(struct Rect));
    r2->top_left.x = -3;
    r2->top_left.y = -2;
    r2->bottom_right.x = 7;
    r2->bottom_right.y = 8;
    // area = 10 * 10 = 100
    // EXPECT: Rect2 area: 100
    printf("Rect2 area: %d\n", rect_area(r2));
    // perimeter = 2*(10+10) = 40
    // EXPECT: Rect2 perimeter: 40
    printf("Rect2 perimeter: %d\n", rect_perimeter(r2));

    // EXPECT: Pool remaining: 3984
    // 64 + 4*8 + 16 = 64 + 32 + 16 = 112, remaining = 4096 - 112 = 3984
    printf("Pool remaining: %d\n", pool_remaining());

    /* Test allocation of a large block */
    char *big = pool_alloc(3000);
    int big_ok = (big != (char *)0) ? 1 : 0;
    // EXPECT: Big alloc ok: 1
    printf("Big alloc ok: %d\n", big_ok);

    /* Try to allocate more than remaining - should fail */
    char *fail = pool_alloc(2000);
    int fail_ok = (fail == (char *)0) ? 1 : 0;
    // EXPECT: Overflow alloc null: 1
    printf("Overflow alloc null: %d\n", fail_ok);

    /* Reset and reuse pool */
    pool_init();
    // EXPECT: After reset used: 0
    printf("After reset used: %d\n", pool_used());

    struct Point *fresh = (struct Point *)pool_alloc(sizeof(struct Point));
    fresh->x = 99;
    fresh->y = 88;
    // EXPECT: Fresh point: 99 88
    printf("Fresh point: %d %d\n", fresh->x, fresh->y);

    return 0;
}
