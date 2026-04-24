int printf(const char *fmt, ...);
// EXPECT: Created objects: 1 2 3 4\nPool (4 active):\n  id=1 pos=(10,20) type=1\n  id=2 pos=(30,40) type=2\n  id=3 pos=(50,60) type=1\n  id=4 pos=(70,80) type=3\nAfter moves:\nPool (4 active):\n  id=1 pos=(15,15) type=1\n  id=2 pos=(30,40) type=2\n  id=3 pos=(40,70) type=1\n  id=4 pos=(70,80) type=3\nAfter destroying id 2:\nPool (3 active):\n  id=1 pos=(15,15) type=1\n  id=3 pos=(40,70) type=1\n  id=4 pos=(70,80) type=3\nReused slot, new id=5\nPool (4 active):\n  id=1 pos=(15,15) type=1\n  id=5 pos=(100,100) type=2\n  id=3 pos=(40,70) type=1\n  id=4 pos=(70,80) type=3\nType 1 count: 2\nType 2 count: 1\nType 3 count: 1

#define MAX_OBJECTS 10

struct Object {
    int id;
    int x;
    int y;
    int active;
    int type;
};

struct ObjectPool {
    struct Object objects[MAX_OBJECTS];
    int next_id;
    int count;
};

void objpool_init(struct ObjectPool *pool) {
    int i;
    for (i = 0; i < MAX_OBJECTS; i++) {
        pool->objects[i].id = 0;
        pool->objects[i].x = 0;
        pool->objects[i].y = 0;
        pool->objects[i].active = 0;
        pool->objects[i].type = 0;
    }
    pool->next_id = 1;
    pool->count = 0;
}

int objpool_create(struct ObjectPool *pool, int x, int y, int type) {
    int i;
    for (i = 0; i < MAX_OBJECTS; i++) {
        if (pool->objects[i].active == 0) {
            pool->objects[i].id = pool->next_id;
            pool->next_id++;
            pool->objects[i].x = x;
            pool->objects[i].y = y;
            pool->objects[i].type = type;
            pool->objects[i].active = 1;
            pool->count++;
            return pool->objects[i].id;
        }
    }
    return -1;
}

int objpool_destroy(struct ObjectPool *pool, int id) {
    int i;
    for (i = 0; i < MAX_OBJECTS; i++) {
        if (pool->objects[i].active && pool->objects[i].id == id) {
            pool->objects[i].active = 0;
            pool->count--;
            return 0;
        }
    }
    return -1;
}

void objpool_move(struct ObjectPool *pool, int id, int dx, int dy) {
    int i;
    for (i = 0; i < MAX_OBJECTS; i++) {
        if (pool->objects[i].active && pool->objects[i].id == id) {
            pool->objects[i].x = pool->objects[i].x + dx;
            pool->objects[i].y = pool->objects[i].y + dy;
            return;
        }
    }
}

void objpool_print(struct ObjectPool *pool) {
    int i;
    printf("Pool (%d active):\n", pool->count);
    for (i = 0; i < MAX_OBJECTS; i++) {
        if (pool->objects[i].active) {
            printf("  id=%d pos=(%d,%d) type=%d\n",
                   pool->objects[i].id, pool->objects[i].x,
                   pool->objects[i].y, pool->objects[i].type);
        }
    }
}

int objpool_count_type(struct ObjectPool *pool, int type) {
    int c = 0;
    int i;
    for (i = 0; i < MAX_OBJECTS; i++) {
        if (pool->objects[i].active && pool->objects[i].type == type) c++;
    }
    return c;
}

int main(void) {
    struct ObjectPool pool;
    objpool_init(&pool);

    int a = objpool_create(&pool, 10, 20, 1);
    int b = objpool_create(&pool, 30, 40, 2);
    int c = objpool_create(&pool, 50, 60, 1);
    int d = objpool_create(&pool, 70, 80, 3);
    printf("Created objects: %d %d %d %d\n", a, b, c, d);
    objpool_print(&pool);

    objpool_move(&pool, a, 5, -5);
    objpool_move(&pool, c, -10, 10);
    printf("After moves:\n");
    objpool_print(&pool);

    objpool_destroy(&pool, b);
    printf("After destroying id %d:\n", b);
    objpool_print(&pool);

    int e = objpool_create(&pool, 100, 100, 2);
    printf("Reused slot, new id=%d\n", e);
    objpool_print(&pool);

    printf("Type 1 count: %d\n", objpool_count_type(&pool, 1));
    printf("Type 2 count: %d\n", objpool_count_type(&pool, 2));
    printf("Type 3 count: %d\n", objpool_count_type(&pool, 3));

    return 0;
}
