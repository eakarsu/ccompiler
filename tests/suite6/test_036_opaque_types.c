int printf(const char *fmt, ...);

typedef int Handle;

typedef struct {
    int active;
    int value;
    int ref_count;
} Resource;

Resource g_resources[16];
int g_next_handle;

void resource_system_init(void) {
    int i;
    g_next_handle = 0;
    for (i = 0; i < 16; i = i + 1) {
        g_resources[i].active = 0;
        g_resources[i].value = 0;
        g_resources[i].ref_count = 0;
    }
}

Handle resource_create(int initial_value) {
    if (g_next_handle >= 16) return -1;
    Handle h = g_next_handle;
    g_resources[h].active = 1;
    g_resources[h].value = initial_value;
    g_resources[h].ref_count = 1;
    g_next_handle = g_next_handle + 1;
    return h;
}

int resource_is_valid(Handle h) {
    if (h < 0 || h >= 16) return 0;
    return g_resources[h].active;
}

int resource_get(Handle h) {
    if (!resource_is_valid(h)) return -1;
    return g_resources[h].value;
}

int resource_set(Handle h, int val) {
    if (!resource_is_valid(h)) return 0;
    g_resources[h].value = val;
    return 1;
}

int resource_add_ref(Handle h) {
    if (!resource_is_valid(h)) return 0;
    g_resources[h].ref_count = g_resources[h].ref_count + 1;
    return g_resources[h].ref_count;
}

int resource_release(Handle h) {
    if (!resource_is_valid(h)) return 0;
    g_resources[h].ref_count = g_resources[h].ref_count - 1;
    if (g_resources[h].ref_count <= 0) {
        g_resources[h].active = 0;
        g_resources[h].value = 0;
        g_resources[h].ref_count = 0;
        return 0;
    }
    return g_resources[h].ref_count;
}

int resource_ref_count(Handle h) {
    if (!resource_is_valid(h)) return 0;
    return g_resources[h].ref_count;
}

int count_active_resources(void) {
    int count = 0;
    int i;
    for (i = 0; i < 16; i = i + 1) {
        if (g_resources[i].active) count = count + 1;
    }
    return count;
}

int sum_all_values(void) {
    int total = 0;
    int i;
    for (i = 0; i < 16; i = i + 1) {
        if (g_resources[i].active) {
            total = total + g_resources[i].value;
        }
    }
    return total;
}

Handle find_resource_by_value(int val) {
    int i;
    for (i = 0; i < 16; i = i + 1) {
        if (g_resources[i].active && g_resources[i].value == val) {
            return i;
        }
    }
    return -1;
}

int resource_increment(Handle h) {
    if (!resource_is_valid(h)) return -1;
    g_resources[h].value = g_resources[h].value + 1;
    return g_resources[h].value;
}

int main(void) {
    resource_system_init();

    Handle h1 = resource_create(100);
    // EXPECT: h1: 0
    printf("h1: %d\n", h1);

    Handle h2 = resource_create(200);
    // EXPECT: h2: 1
    printf("h2: %d\n", h2);

    Handle h3 = resource_create(300);
    // EXPECT: h3: 2
    printf("h3: %d\n", h3);

    // EXPECT: get h1: 100
    printf("get h1: %d\n", resource_get(h1));

    // EXPECT: get h2: 200
    printf("get h2: %d\n", resource_get(h2));

    // EXPECT: get h3: 300
    printf("get h3: %d\n", resource_get(h3));

    // EXPECT: valid h1: 1
    printf("valid h1: %d\n", resource_is_valid(h1));

    // EXPECT: valid -1: 0
    printf("valid -1: %d\n", resource_is_valid(-1));

    // EXPECT: valid 99: 0
    printf("valid 99: %d\n", resource_is_valid(99));

    resource_set(h1, 150);
    // EXPECT: updated h1: 150
    printf("updated h1: %d\n", resource_get(h1));

    // EXPECT: active count: 3
    printf("active count: %d\n", count_active_resources());

    // EXPECT: sum values: 650
    printf("sum values: %d\n", sum_all_values());

    // EXPECT: ref h1: 1
    printf("ref h1: %d\n", resource_ref_count(h1));

    resource_add_ref(h1);
    // EXPECT: ref h1 after add: 2
    printf("ref h1 after add: %d\n", resource_ref_count(h1));

    resource_add_ref(h1);
    // EXPECT: ref h1 after add2: 3
    printf("ref h1 after add2: %d\n", resource_ref_count(h1));

    int remaining = resource_release(h1);
    // EXPECT: release h1: 2
    printf("release h1: %d\n", remaining);

    remaining = resource_release(h1);
    // EXPECT: release h1 again: 1
    printf("release h1 again: %d\n", remaining);

    remaining = resource_release(h1);
    // EXPECT: release h1 final: 0
    printf("release h1 final: %d\n", remaining);

    // EXPECT: h1 valid after destroy: 0
    printf("h1 valid after destroy: %d\n", resource_is_valid(h1));

    // EXPECT: active after destroy: 2
    printf("active after destroy: %d\n", count_active_resources());

    Handle found = find_resource_by_value(200);
    // EXPECT: find 200: 1
    printf("find 200: %d\n", found);

    Handle not_found = find_resource_by_value(999);
    // EXPECT: find 999: -1
    printf("find 999: %d\n", not_found);

    resource_increment(h2);
    resource_increment(h2);
    // EXPECT: h2 after inc: 202
    printf("h2 after inc: %d\n", resource_get(h2));

    // EXPECT: get invalid: -1
    printf("get invalid: %d\n", resource_get(h1));

    // EXPECT: sum after changes: 502
    printf("sum after changes: %d\n", sum_all_values());

    return 0;
}
