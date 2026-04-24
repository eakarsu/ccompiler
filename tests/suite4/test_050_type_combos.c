int printf(const char *fmt, ...);

enum Shape {
    SHAPE_CIRCLE,
    SHAPE_RECT,
    SHAPE_TRIANGLE
};

typedef enum Shape ShapeType;

typedef enum {
    PRIORITY_LOW = 0,
    PRIORITY_MED = 1,
    PRIORITY_HIGH = 2
} Priority;

union IntOrChar {
    int i;
    char c;
};

typedef union IntOrChar Value;

struct Task {
    int id;
    Priority priority;
    int done;
};

typedef struct Task Task;
typedef Task *TaskPtr;

struct ShapeData {
    ShapeType type;
    int param1;
    int param2;
};

typedef struct ShapeData ShapeData;

typedef int (*Comparator)(int, int);

int compare_asc(int a, int b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int compare_desc(int a, int b) {
    if (a > b) return -1;
    if (a < b) return 1;
    return 0;
}

int test_typedef_enum(void) {
    ShapeType s = SHAPE_RECT;
    return s; // 1
}

int test_typedef_enum_switch(void) {
    ShapeType s = SHAPE_TRIANGLE;
    switch (s) {
        case SHAPE_CIRCLE:   return 10;
        case SHAPE_RECT:     return 20;
        case SHAPE_TRIANGLE: return 30;
    }
    return -1;
}

int test_priority_enum(void) {
    Priority p = PRIORITY_HIGH;
    return p; // 2
}

int test_array_of_unions(void) {
    Value arr[4];
    arr[0].i = 10;
    arr[1].i = 20;
    arr[2].i = 30;
    arr[3].i = 40;
    int sum = 0;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        sum = sum + arr[i].i;
    }
    return sum; // 100
}

int test_union_typedef_char(void) {
    Value v;
    v.i = 0;
    v.c = 65;
    return v.c; // 65
}

int test_struct_with_enum(void) {
    Task t;
    t.id = 1;
    t.priority = PRIORITY_HIGH;
    t.done = 0;
    return t.priority; // 2
}

int test_task_array(void) {
    Task tasks[3];
    tasks[0].id = 1;
    tasks[0].priority = PRIORITY_LOW;
    tasks[0].done = 1;
    tasks[1].id = 2;
    tasks[1].priority = PRIORITY_MED;
    tasks[1].done = 0;
    tasks[2].id = 3;
    tasks[2].priority = PRIORITY_HIGH;
    tasks[2].done = 1;

    int sum_priority = 0;
    int done_count = 0;
    int i;
    for (i = 0; i < 3; i = i + 1) {
        sum_priority = sum_priority + tasks[i].priority;
        done_count = done_count + tasks[i].done;
    }
    return sum_priority * 10 + done_count;
    // (0+1+2)*10 + 2 = 32
}

int test_task_pointer(void) {
    Task t;
    t.id = 42;
    t.priority = PRIORITY_MED;
    t.done = 1;
    TaskPtr p = &t;
    return p->id + p->priority + p->done;
    // 42 + 1 + 1 = 44
}

int test_shape_data(void) {
    ShapeData s;
    s.type = SHAPE_CIRCLE;
    s.param1 = 10; // radius
    s.param2 = 0;
    return s.param1; // 10
}

int test_shape_area_approx(void) {
    ShapeData shapes[3];
    shapes[0].type = SHAPE_CIRCLE;
    shapes[0].param1 = 5;
    shapes[0].param2 = 0;
    shapes[1].type = SHAPE_RECT;
    shapes[1].param1 = 4;
    shapes[1].param2 = 6;
    shapes[2].type = SHAPE_TRIANGLE;
    shapes[2].param1 = 3;
    shapes[2].param2 = 8;

    int total = 0;
    int i;
    for (i = 0; i < 3; i = i + 1) {
        if (shapes[i].type == SHAPE_RECT) {
            total = total + shapes[i].param1 * shapes[i].param2;
        } else if (shapes[i].type == SHAPE_CIRCLE) {
            // approximate: r*r*3
            total = total + shapes[i].param1 * shapes[i].param1 * 3;
        } else {
            // triangle: base*height/2
            total = total + shapes[i].param1 * shapes[i].param2 / 2;
        }
    }
    // circle: 5*5*3=75, rect: 4*6=24, triangle: 3*8/2=12
    return total; // 111
}

int test_pointer_to_typedef(void) {
    Priority p = PRIORITY_HIGH;
    Priority *pp = &p;
    return *pp; // 2
}

int test_comparator_typedef(void) {
    Comparator cmp = compare_asc;
    return cmp(3, 5); // -1
}

int test_comparator_desc(void) {
    Comparator cmp = compare_desc;
    return cmp(3, 5); // 1
}

int test_sort_three(void) {
    int arr[3];
    arr[0] = 30;
    arr[1] = 10;
    arr[2] = 20;
    Comparator cmp = compare_asc;

    // simple bubble sort of 3 elements
    int i;
    int j;
    for (i = 0; i < 2; i = i + 1) {
        for (j = 0; j < 2 - i; j = j + 1) {
            if (cmp(arr[j], arr[j + 1]) > 0) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    // sorted: 10, 20, 30
    return arr[0] * 100 + arr[1] * 10 + arr[2];
    // 1000 + 200 + 30 = 1230
}

int test_enum_in_union(void) {
    // Store an enum value in a union int field
    Value v;
    v.i = SHAPE_TRIANGLE; // 2
    ShapeType s = v.i;
    return s; // 2
}

int main(void) {
    // EXPECT: typedef enum: 1
    printf("typedef enum: %d\n", test_typedef_enum());

    // EXPECT: typedef enum switch: 30
    printf("typedef enum switch: %d\n", test_typedef_enum_switch());

    // EXPECT: priority enum: 2
    printf("priority enum: %d\n", test_priority_enum());

    // EXPECT: array of unions: 100
    printf("array of unions: %d\n", test_array_of_unions());

    // EXPECT: union typedef char: 65
    printf("union typedef char: %d\n", test_union_typedef_char());

    // EXPECT: struct with enum: 2
    printf("struct with enum: %d\n", test_struct_with_enum());

    // EXPECT: task array: 32
    printf("task array: %d\n", test_task_array());

    // EXPECT: task pointer: 44
    printf("task pointer: %d\n", test_task_pointer());

    // EXPECT: shape data: 10
    printf("shape data: %d\n", test_shape_data());

    // EXPECT: shape area approx: 111
    printf("shape area approx: %d\n", test_shape_area_approx());

    // EXPECT: pointer to typedef: 2
    printf("pointer to typedef: %d\n", test_pointer_to_typedef());

    // EXPECT: comparator asc: -1
    printf("comparator asc: %d\n", test_comparator_typedef());

    // EXPECT: comparator desc: 1
    printf("comparator desc: %d\n", test_comparator_desc());

    // EXPECT: sort three: 1230
    printf("sort three: %d\n", test_sort_three());

    // EXPECT: enum in union: 2
    printf("enum in union: %d\n", test_enum_in_union());

    ShapeData local;
    local.type = SHAPE_RECT;
    local.param1 = 7;
    local.param2 = 8;
    // EXPECT: local rect area: 56
    printf("local rect area: %d\n", local.param1 * local.param2);

    return 0;
}
