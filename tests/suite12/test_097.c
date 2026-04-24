int printf(const char *fmt, ...);

// Struct containing arrays containing structs

struct Point {
    int x;
    int y;
};

struct Triangle {
    struct Point vertices[3];
    int id;
};

struct Mesh {
    struct Triangle triangles[4];
    int count;
    int total_area_x2;
};

void init_point(struct Point *p, int x, int y) {
    p->x = x;
    p->y = y;
}

void init_triangle(struct Triangle *t, int id, int x0, int y0, int x1, int y1, int x2, int y2) {
    t->id = id;
    t->vertices[0].x = x0;
    t->vertices[0].y = y0;
    t->vertices[1].x = x1;
    t->vertices[1].y = y1;
    t->vertices[2].x = x2;
    t->vertices[2].y = y2;
}

// 2x area using cross product: |(x1-x0)(y2-y0) - (x2-x0)(y1-y0)|
int triangle_area_x2(struct Triangle *t) {
    int ax;
    int ay;
    int bx;
    int by;
    int cross;
    ax = t->vertices[1].x - t->vertices[0].x;
    ay = t->vertices[1].y - t->vertices[0].y;
    bx = t->vertices[2].x - t->vertices[0].x;
    by = t->vertices[2].y - t->vertices[0].y;
    cross = ax * by - bx * ay;
    if (cross < 0) cross = -cross;
    return cross;
}

void compute_mesh_area(struct Mesh *m) {
    int i;
    m->total_area_x2 = 0;
    for (i = 0; i < m->count; i = i + 1) {
        m->total_area_x2 = m->total_area_x2 + triangle_area_x2(&m->triangles[i]);
    }
}

struct Record {
    int values[8];
    int count;
};

struct Table {
    struct Record rows[5];
    int num_rows;
};

void init_record(struct Record *r, int start, int cnt) {
    int i;
    r->count = cnt;
    for (i = 0; i < cnt; i = i + 1) {
        r->values[i] = start + i;
    }
}

int sum_record(struct Record *r) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < r->count; i = i + 1) {
        s = s + r->values[i];
    }
    return s;
}

int sum_table(struct Table *t) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < t->num_rows; i = i + 1) {
        s = s + sum_record(&t->rows[i]);
    }
    return s;
}

int max_in_table(struct Table *t) {
    int i;
    int j;
    int mx;
    mx = t->rows[0].values[0];
    for (i = 0; i < t->num_rows; i = i + 1) {
        for (j = 0; j < t->rows[i].count; j = j + 1) {
            if (t->rows[i].values[j] > mx) {
                mx = t->rows[i].values[j];
            }
        }
    }
    return mx;
}

struct Matrix {
    int data[4][4];
    int rows;
    int cols;
};

void init_identity(struct Matrix *m, int n) {
    int i;
    int j;
    m->rows = n;
    m->cols = n;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (i == j) {
                m->data[i][j] = 1;
            } else {
                m->data[i][j] = 0;
            }
        }
    }
}

int trace(struct Matrix *m) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < m->rows; i = i + 1) {
        s = s + m->data[i][i];
    }
    return s;
}

int main() {
    struct Mesh mesh;
    struct Table table;
    struct Matrix mat;

    // Triangle tests
    init_triangle(&mesh.triangles[0], 0, 0, 0, 4, 0, 0, 3);
    init_triangle(&mesh.triangles[1], 1, 0, 0, 6, 0, 0, 4);
    init_triangle(&mesh.triangles[2], 2, 1, 1, 5, 1, 1, 5);
    init_triangle(&mesh.triangles[3], 3, 0, 0, 10, 0, 0, 10);
    mesh.count = 4;

    // area_x2: |4*3 - 0*0| = 12
    printf("%d\n", triangle_area_x2(&mesh.triangles[0])); // EXPECT: 12
    // area_x2: |6*4 - 0*0| = 24
    printf("%d\n", triangle_area_x2(&mesh.triangles[1])); // EXPECT: 24
    // area_x2: |4*4 - 0*0| = 16
    printf("%d\n", triangle_area_x2(&mesh.triangles[2])); // EXPECT: 16
    // area_x2: |10*10 - 0*0| = 100
    printf("%d\n", triangle_area_x2(&mesh.triangles[3])); // EXPECT: 100

    compute_mesh_area(&mesh);
    printf("%d\n", mesh.total_area_x2); // EXPECT: 152

    // Vertex access
    printf("%d\n", mesh.triangles[2].vertices[1].x); // EXPECT: 5
    printf("%d\n", mesh.triangles[3].vertices[2].y); // EXPECT: 10
    printf("%d\n", mesh.triangles[0].id); // EXPECT: 0
    printf("%d\n", mesh.triangles[3].id); // EXPECT: 3

    // Table tests
    table.num_rows = 5;
    init_record(&table.rows[0], 1, 3);
    init_record(&table.rows[1], 10, 5);
    init_record(&table.rows[2], 100, 2);
    init_record(&table.rows[3], 50, 4);
    init_record(&table.rows[4], 1, 8);

    // sum row 0: 1+2+3 = 6
    printf("%d\n", sum_record(&table.rows[0])); // EXPECT: 6
    // sum row 1: 10+11+12+13+14 = 60
    printf("%d\n", sum_record(&table.rows[1])); // EXPECT: 60
    // sum row 2: 100+101 = 201
    printf("%d\n", sum_record(&table.rows[2])); // EXPECT: 201
    // sum row 3: 50+51+52+53 = 206
    printf("%d\n", sum_record(&table.rows[3])); // EXPECT: 206
    // sum row 4: 1+2+3+4+5+6+7+8 = 36
    printf("%d\n", sum_record(&table.rows[4])); // EXPECT: 36

    // total sum: 6+60+201+206+36 = 509
    printf("%d\n", sum_table(&table)); // EXPECT: 509

    // max in table: 101
    printf("%d\n", max_in_table(&table)); // EXPECT: 101

    // Matrix tests
    init_identity(&mat, 4);
    printf("%d\n", trace(&mat)); // EXPECT: 4
    printf("%d\n", mat.data[0][0]); // EXPECT: 1
    printf("%d\n", mat.data[0][1]); // EXPECT: 0
    printf("%d\n", mat.data[3][3]); // EXPECT: 1

    mat.data[1][2] = 7;
    mat.data[2][1] = 3;
    printf("%d\n", mat.data[1][2]); // EXPECT: 7
    printf("%d\n", mat.data[2][1]); // EXPECT: 3

    printf("%s\n", "done"); // EXPECT: done
    return 0;
}
