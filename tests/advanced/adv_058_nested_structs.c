// EXPECT: === Deeply Nested Structs ===
// EXPECT: point: x=10 y=20
// EXPECT: rect: (1,2)-(5,8)
// EXPECT: rect_area: 24
// EXPECT: scene_rect: (0,0)-(100,100) color=(255,0,0)
// EXPECT: scene_label: Hello
// EXPECT: deep3: a=1 b=2 c=3 d=4
// EXPECT: deep3_copy: a=1 b=2 c=3 d=4
// EXPECT: modified_copy: a=10 b=2 c=30 d=4
// EXPECT: original_unchanged: a=1 b=2 c=3 d=4
// EXPECT: person: John age=30 at (40,74)
// EXPECT: company: Acme CEO=Jane at (100,200)
// EXPECT: employee: Bob works at Acme
// EXPECT: employee_addr: (100,200)
// EXPECT: array_of_nested: p0=(0,0)-(1,1) p1=(2,2)-(3,3)
// EXPECT: nested_modify: inner.x=99
// EXPECT: nested_ptr: x=42 y=84
// EXPECT: tree_root: val=1 left=2 right=3
// EXPECT: matrix_cell: row=1 col=2 val=12
// EXPECT: config: width=1920 height=1080 fullscreen=1 volume=80
// EXPECT: config_nested: display=(1920,1080) audio_vol=80

int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Color {
    int r;
    int g;
    int b;
};

struct Rect {
    struct Point top_left;
    struct Point bottom_right;
};

struct ColoredRect {
    struct Rect rect;
    struct Color color;
};

struct Scene {
    struct ColoredRect objects[2];
    char label[20];
    int obj_count;
};

// 3-level deep nesting
struct Level1 {
    int a;
    struct Level2 {
        int b;
        struct Level3 {
            int c;
            int d;
        } inner;
    } mid;
};

struct Address {
    int street_num;
    int zip;
};

struct Person {
    char name[20];
    int age;
    struct Address addr;
};

struct Company {
    char name[20];
    struct Person ceo;
    struct Address hq;
};

struct Employee {
    char name[20];
    struct Company *employer;
};

struct TreeNode {
    int val;
    int has_left;
    int has_right;
    int left_val;
    int right_val;
};

struct Cell {
    int row;
    int col;
    int val;
};

struct DisplayConfig {
    int width;
    int height;
};

struct AudioConfig {
    int volume;
    int mute;
};

struct Config {
    struct DisplayConfig display;
    struct AudioConfig audio;
    int fullscreen;
};

void str_copy(char *dst, const char *src) {
    while (*src) {
        *dst++ = *src++;
    }
    *dst = 0;
}

int rect_area(struct Rect r) {
    int w = r.bottom_right.x - r.top_left.x;
    int h = r.bottom_right.y - r.top_left.y;
    return w * h;
}

struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

struct Rect make_rect(int x1, int y1, int x2, int y2) {
    struct Rect r;
    r.top_left.x = x1;
    r.top_left.y = y1;
    r.bottom_right.x = x2;
    r.bottom_right.y = y2;
    return r;
}

int main(void) {
    printf("=== Deeply Nested Structs ===\n");

    // Basic point
    {
        struct Point p;
        p.x = 10;
        p.y = 20;
        printf("point: x=%d y=%d\n", p.x, p.y);
    }

    // Rect with nested points
    {
        struct Rect r;
        r.top_left.x = 1;
        r.top_left.y = 2;
        r.bottom_right.x = 5;
        r.bottom_right.y = 8;
        printf("rect: (%d,%d)-(%d,%d)\n",
               r.top_left.x, r.top_left.y,
               r.bottom_right.x, r.bottom_right.y);
        printf("rect_area: %d\n", rect_area(r));
    }

    // Scene with colored rect (3 levels: Scene->ColoredRect->Rect->Point)
    {
        struct Scene scene;
        scene.objects[0].rect.top_left.x = 0;
        scene.objects[0].rect.top_left.y = 0;
        scene.objects[0].rect.bottom_right.x = 100;
        scene.objects[0].rect.bottom_right.y = 100;
        scene.objects[0].color.r = 255;
        scene.objects[0].color.g = 0;
        scene.objects[0].color.b = 0;
        str_copy(scene.label, "Hello");
        scene.obj_count = 1;

        printf("scene_rect: (%d,%d)-(%d,%d) color=(%d,%d,%d)\n",
               scene.objects[0].rect.top_left.x,
               scene.objects[0].rect.top_left.y,
               scene.objects[0].rect.bottom_right.x,
               scene.objects[0].rect.bottom_right.y,
               scene.objects[0].color.r,
               scene.objects[0].color.g,
               scene.objects[0].color.b);
        printf("scene_label: %s\n", scene.label);
    }

    // 3-level deep
    {
        struct Level1 obj;
        obj.a = 1;
        obj.mid.b = 2;
        obj.mid.inner.c = 3;
        obj.mid.inner.d = 4;
        printf("deep3: a=%d b=%d c=%d d=%d\n",
               obj.a, obj.mid.b, obj.mid.inner.c, obj.mid.inner.d);

        // Copy
        struct Level1 copy;
        copy = obj;
        printf("deep3_copy: a=%d b=%d c=%d d=%d\n",
               copy.a, copy.mid.b, copy.mid.inner.c, copy.mid.inner.d);

        // Modify copy, original unchanged
        copy.a = 10;
        copy.mid.inner.c = 30;
        printf("modified_copy: a=%d b=%d c=%d d=%d\n",
               copy.a, copy.mid.b, copy.mid.inner.c, copy.mid.inner.d);
        printf("original_unchanged: a=%d b=%d c=%d d=%d\n",
               obj.a, obj.mid.b, obj.mid.inner.c, obj.mid.inner.d);
    }

    // Person with address
    {
        struct Person p;
        str_copy(p.name, "John");
        p.age = 30;
        p.addr.street_num = 40;
        p.addr.zip = 74;
        printf("person: %s age=%d at (%d,%d)\n",
               p.name, p.age, p.addr.street_num, p.addr.zip);
    }

    // Company with nested person and address
    {
        struct Company co;
        str_copy(co.name, "Acme");
        str_copy(co.ceo.name, "Jane");
        co.ceo.age = 45;
        co.ceo.addr.street_num = 1;
        co.ceo.addr.zip = 99;
        co.hq.street_num = 100;
        co.hq.zip = 200;
        printf("company: %s CEO=%s at (%d,%d)\n",
               co.name, co.ceo.name, co.hq.street_num, co.hq.zip);

        // Employee with pointer to company
        struct Employee emp;
        str_copy(emp.name, "Bob");
        emp.employer = &co;
        printf("employee: %s works at %s\n", emp.name, emp.employer->name);
        printf("employee_addr: (%d,%d)\n",
               emp.employer->hq.street_num, emp.employer->hq.zip);
    }

    // Array of nested structs
    {
        struct Rect panels[2];
        panels[0] = make_rect(0, 0, 1, 1);
        panels[1] = make_rect(2, 2, 3, 3);
        printf("array_of_nested: p0=(%d,%d)-(%d,%d) p1=(%d,%d)-(%d,%d)\n",
               panels[0].top_left.x, panels[0].top_left.y,
               panels[0].bottom_right.x, panels[0].bottom_right.y,
               panels[1].top_left.x, panels[1].top_left.y,
               panels[1].bottom_right.x, panels[1].bottom_right.y);
    }

    // Modify nested member through outer struct
    {
        struct Rect r;
        r.top_left.x = 0;
        r.top_left.y = 0;
        r.bottom_right.x = 10;
        r.bottom_right.y = 10;
        r.top_left.x = 99;
        printf("nested_modify: inner.x=%d\n", r.top_left.x);
    }

    // Pointer to nested struct
    {
        struct Rect r;
        r.top_left.x = 42;
        r.top_left.y = 84;
        struct Point *pp = &r.top_left;
        printf("nested_ptr: x=%d y=%d\n", pp->x, pp->y);
    }

    // Simulated tree (flat, no dynamic alloc)
    {
        struct TreeNode root;
        root.val = 1;
        root.has_left = 1;
        root.has_right = 1;
        root.left_val = 2;
        root.right_val = 3;
        printf("tree_root: val=%d left=%d right=%d\n",
               root.val, root.left_val, root.right_val);
    }

    // Matrix cell
    {
        struct Cell c;
        c.row = 1;
        c.col = 2;
        c.val = 12;
        printf("matrix_cell: row=%d col=%d val=%d\n", c.row, c.col, c.val);
    }

    // Config with nested display and audio
    {
        struct Config cfg;
        cfg.display.width = 1920;
        cfg.display.height = 1080;
        cfg.audio.volume = 80;
        cfg.audio.mute = 0;
        cfg.fullscreen = 1;
        printf("config: width=%d height=%d fullscreen=%d volume=%d\n",
               cfg.display.width, cfg.display.height,
               cfg.fullscreen, cfg.audio.volume);
        printf("config_nested: display=(%d,%d) audio_vol=%d\n",
               cfg.display.width, cfg.display.height, cfg.audio.volume);
    }

    return 0;
}
