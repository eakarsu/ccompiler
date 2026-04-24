int printf(const char *fmt, ...);

/* Level 1: innermost */
struct Vec2 {
    int x;
    int y;
};

/* Level 2: contains Vec2 */
struct Segment {
    struct Vec2 start;
    struct Vec2 end;
    int weight;
};

/* Level 3: contains Segment (3 levels deep) */
struct Graph {
    struct Segment edge1;
    struct Segment edge2;
    int node_count;
};

/* Flat struct for testing assignment */
struct Color {
    int r;
    int g;
    int b;
};

struct Pixel {
    struct Vec2  pos;
    struct Color col;
};

struct Frame {
    struct Pixel top_left;
    struct Pixel bot_right;
    int          id;
};

/* Helper: sum of two Vec2 coordinates */
int vec2_sum(struct Vec2 v) {
    return v.x + v.y;
}

/* Helper: length squared of a segment (Manhattan approx) */
int seg_span(struct Segment s) {
    int dx = s.end.x - s.start.x;
    int dy = s.end.y - s.start.y;
    return dx * dx + dy * dy;
}

/* Modify nested member via pointer */
void move_start(struct Segment *s, int dx, int dy) {
    s->start.x += dx;
    s->start.y += dy;
}

int main(void) {
    /* --- Level-2 nested struct --- */
    struct Segment seg;
    seg.start.x = 1;
    seg.start.y = 2;
    seg.end.x   = 4;
    seg.end.y   = 6;
    seg.weight  = 10;

    printf("%d\n", seg.start.x);            // EXPECT: 1
    printf("%d\n", seg.start.y);            // EXPECT: 2
    printf("%d\n", seg.end.x);              // EXPECT: 4
    printf("%d\n", seg.end.y);              // EXPECT: 6
    printf("%d\n", seg.weight);             // EXPECT: 10

    /* Access nested member with chained . */
    printf("%d\n", vec2_sum(seg.start));    // EXPECT: 3
    printf("%d\n", vec2_sum(seg.end));      // EXPECT: 10

    /* seg_span: dx=3, dy=4 => 9+16=25 */
    printf("%d\n", seg_span(seg));          // EXPECT: 25

    /* --- Initialise with nested braces --- */
    struct Segment seg2 = {{0, 0}, {3, 4}, 5};
    printf("%d\n", seg2.start.x);           // EXPECT: 0
    printf("%d\n", seg2.start.y);           // EXPECT: 0
    printf("%d\n", seg2.end.x);             // EXPECT: 3
    printf("%d\n", seg2.end.y);             // EXPECT: 4
    printf("%d\n", seg2.weight);            // EXPECT: 5
    /* seg_span: 9+16=25 */
    printf("%d\n", seg_span(seg2));         // EXPECT: 25

    /* --- Modify nested via pointer (move_start) --- */
    move_start(&seg2, 2, 3);
    printf("%d\n", seg2.start.x);           // EXPECT: 2
    printf("%d\n", seg2.start.y);           // EXPECT: 3

    /* --- Level-3: Graph --- */
    struct Graph g;
    g.edge1.start.x  = 0;
    g.edge1.start.y  = 0;
    g.edge1.end.x    = 5;
    g.edge1.end.y    = 5;
    g.edge1.weight   = 7;
    g.edge2.start.x  = 10;
    g.edge2.start.y  = 10;
    g.edge2.end.x    = 13;
    g.edge2.end.y    = 14;
    g.edge2.weight   = 3;
    g.node_count     = 4;

    printf("%d\n", g.edge1.start.x);        // EXPECT: 0
    printf("%d\n", g.edge1.end.y);          // EXPECT: 5
    printf("%d\n", g.edge1.weight);         // EXPECT: 7
    printf("%d\n", g.edge2.start.x);        // EXPECT: 10
    printf("%d\n", g.edge2.end.y);          // EXPECT: 14
    printf("%d\n", g.edge2.weight);         // EXPECT: 3
    printf("%d\n", g.node_count);           // EXPECT: 4

    /* Level-3 span: dx=3,dy=4 => 25 */
    printf("%d\n", seg_span(g.edge2));       // EXPECT: 25

    /* --- Struct assignment copies all levels --- */
    struct Graph g2 = g;
    g2.edge1.start.x = 99;
    g2.node_count    = 100;
    printf("%d\n", g.edge1.start.x);        // EXPECT: 0
    printf("%d\n", g.node_count);           // EXPECT: 4
    printf("%d\n", g2.edge1.start.x);       // EXPECT: 99
    printf("%d\n", g2.node_count);          // EXPECT: 100

    /* --- Frame (3-level: Frame -> Pixel -> Vec2/Color) --- */
    struct Frame fr;
    fr.top_left.pos.x   = 0;
    fr.top_left.pos.y   = 0;
    fr.top_left.col.r   = 255;
    fr.top_left.col.g   = 0;
    fr.top_left.col.b   = 0;
    fr.bot_right.pos.x  = 640;
    fr.bot_right.pos.y  = 480;
    fr.bot_right.col.r  = 0;
    fr.bot_right.col.g  = 255;
    fr.bot_right.col.b  = 0;
    fr.id               = 42;

    printf("%d\n", fr.top_left.pos.x);      // EXPECT: 0
    printf("%d\n", fr.top_left.col.r);      // EXPECT: 255
    printf("%d\n", fr.bot_right.pos.x);     // EXPECT: 640
    printf("%d\n", fr.bot_right.pos.y);     // EXPECT: 480
    printf("%d\n", fr.bot_right.col.g);     // EXPECT: 255
    printf("%d\n", fr.id);                  // EXPECT: 42

    /* Nested init with braces */
    struct Pixel px = {{10, 20}, {128, 64, 32}};
    printf("%d\n", px.pos.x);               // EXPECT: 10
    printf("%d\n", px.pos.y);               // EXPECT: 20
    printf("%d\n", px.col.r);               // EXPECT: 128
    printf("%d\n", px.col.g);               // EXPECT: 64
    printf("%d\n", px.col.b);               // EXPECT: 32

    return 0;
}
