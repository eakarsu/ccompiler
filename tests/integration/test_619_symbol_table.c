int printf(const char *fmt, ...);
// EXPECT: === Symbol Table Tests ===\nhash(x) = 177693\nhash(y) = 177694\nhash(hello) = 261238937\nhash(world) = 279393645\n--- Global Scope ---\nx = 10\ny = 20\n--- Child Scope (shadows x) ---\nx = 99\ny = 20\n--- Grandchild Scope ---\nx = 99\ny = 20\nz = 50\n--- Collision Test ---\nA = 100\na = 200\n--- Not Found Test ---\nnotfound = -1\n--- Many Entries ---\nv1 = 1\nv2 = 2\nv3 = 3\nv4 = 4\nv5 = 5\nv6 = 6\nv7 = 7\nv8 = 8\n--- Deep Chain ---\ndepth from top = 0\nlevel from top = 1\ndepth from mid = 0\n--- Overwrite Test ---\nkey = 42\nkey = 99\n--- Stats ---\ntotal_tests = 27\npassed = 27

// ============================================================
// Symbol table stress test: hash table with scope chaining
// Simulates a compiler symbol table with nested scopes
// ============================================================

struct Entry {
    char name[32];
    int value;
    int occupied;
};

struct HashTable {
    struct Entry entries[32];
    int parent;
};

struct HashTable tables[8];
int table_count;

// ---------- String utilities ----------

int str_eq(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_len(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

// ---------- Hash function (djb2) ----------

unsigned hash_str(char *s) {
    unsigned hash;
    int i;
    hash = 5381;
    i = 0;
    while (s[i] != 0) {
        hash = hash * 33 + s[i];
        i = i + 1;
    }
    return hash;
}

// ---------- Table operations ----------

void init_tables(void) {
    int i;
    int j;
    table_count = 0;
    for (i = 0; i < 8; i = i + 1) {
        tables[i].parent = -1;
        for (j = 0; j < 32; j = j + 1) {
            tables[i].entries[j].name[0] = 0;
            tables[i].entries[j].value = 0;
            tables[i].entries[j].occupied = 0;
        }
    }
}

int new_table(int parent) {
    int idx;
    idx = table_count;
    tables[idx].parent = parent;
    table_count = table_count + 1;
    return idx;
}

void insert(int table_idx, char *name, int value) {
    unsigned h;
    int idx;
    int start;
    h = hash_str(name);
    idx = h % 32;
    start = idx;

    // Linear probing: find empty slot or existing key
    while (tables[table_idx].entries[idx].occupied) {
        if (str_eq(tables[table_idx].entries[idx].name, name)) {
            // Overwrite existing key
            tables[table_idx].entries[idx].value = value;
            return;
        }
        idx = (idx + 1) % 32;
        if (idx == start) return; // Table full
    }

    str_copy(tables[table_idx].entries[idx].name, name);
    tables[table_idx].entries[idx].value = value;
    tables[table_idx].entries[idx].occupied = 1;
}

// Lookup with scope chaining: search current table, then parent chain
// Returns value if found, -1 if not found
int lookup(int table_idx, char *name) {
    unsigned h;
    int idx;
    int start;
    int current;

    current = table_idx;
    while (current != -1) {
        h = hash_str(name);
        idx = h % 32;
        start = idx;

        // Search in current scope
        while (tables[current].entries[idx].occupied) {
            if (str_eq(tables[current].entries[idx].name, name)) {
                return tables[current].entries[idx].value;
            }
            idx = (idx + 1) % 32;
            if (idx == start) break;
        }

        // Move to parent scope
        current = tables[current].parent;
    }

    return -1;
}

// Count occupied entries in a table
int count_entries(int table_idx) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < 32; i = i + 1) {
        if (tables[table_idx].entries[i].occupied) {
            count = count + 1;
        }
    }
    return count;
}

// ---------- Test infrastructure ----------

int total_tests;
int passed;

void check(int condition, char *test_name) {
    total_tests = total_tests + 1;
    if (condition) {
        passed = passed + 1;
    } else {
        printf("FAIL: %s\n", test_name);
    }
}

// ---------- Main ----------

int main(void) {
    int global_scope;
    int child_scope;
    int grandchild_scope;
    int collision_scope;
    int many_scope;
    int deep_bot;
    int deep_mid;
    int deep_top;
    int overwrite_scope;
    unsigned hx;
    unsigned hy;
    unsigned hhello;
    unsigned hworld;
    int val;

    total_tests = 0;
    passed = 0;

    init_tables();

    printf("=== Symbol Table Tests ===\n");

    // ---- Test hash function ----
    hx = hash_str("x");
    hy = hash_str("y");
    hhello = hash_str("hello");
    hworld = hash_str("world");

    printf("hash(x) = %u\n", hx);
    printf("hash(y) = %u\n", hy);
    printf("hash(hello) = %u\n", hhello);
    printf("hash(world) = %u\n", hworld);

    check(hx == 177693, "hash x");
    check(hy == 177694, "hash y");
    check(hhello == 261238937, "hash hello");
    check(hworld == 279393645, "hash world");

    // ---- Test global scope ----
    printf("--- Global Scope ---\n");
    global_scope = new_table(-1);

    insert(global_scope, "x", 10);
    insert(global_scope, "y", 20);

    val = lookup(global_scope, "x");
    printf("x = %d\n", val);
    check(val == 10, "global x");

    val = lookup(global_scope, "y");
    printf("y = %d\n", val);
    check(val == 20, "global y");

    // ---- Test child scope with shadowing ----
    printf("--- Child Scope (shadows x) ---\n");
    child_scope = new_table(global_scope);

    insert(child_scope, "x", 99);

    val = lookup(child_scope, "x");
    printf("x = %d\n", val);
    check(val == 99, "child x shadows");

    val = lookup(child_scope, "y");
    printf("y = %d\n", val);
    check(val == 20, "child y from parent");

    // ---- Test grandchild scope ----
    printf("--- Grandchild Scope ---\n");
    grandchild_scope = new_table(child_scope);

    insert(grandchild_scope, "z", 50);

    val = lookup(grandchild_scope, "x");
    printf("x = %d\n", val);
    check(val == 99, "grandchild x from child");

    val = lookup(grandchild_scope, "y");
    printf("y = %d\n", val);
    check(val == 20, "grandchild y from global");

    val = lookup(grandchild_scope, "z");
    printf("z = %d\n", val);
    check(val == 50, "grandchild z local");

    // ---- Test collision handling ----
    // "A" (65) and "a" (97) both hash to bucket 6 mod 32:
    //   hash("A") = 5381*33 + 65 = 177638, 177638 % 32 = 6
    //   hash("a") = 5381*33 + 97 = 177670, 177670 % 32 = 6
    printf("--- Collision Test ---\n");
    collision_scope = new_table(-1);

    insert(collision_scope, "A", 100);
    insert(collision_scope, "a", 200);

    val = lookup(collision_scope, "A");
    printf("A = %d\n", val);
    check(val == 100, "collision A");

    val = lookup(collision_scope, "a");
    printf("a = %d\n", val);
    check(val == 200, "collision a");

    // ---- Test not found ----
    printf("--- Not Found Test ---\n");
    val = lookup(global_scope, "notfound");
    printf("notfound = %d\n", val);
    check(val == -1, "not found");

    // ---- Test many entries in one scope ----
    printf("--- Many Entries ---\n");
    many_scope = new_table(-1);

    insert(many_scope, "v1", 1);
    insert(many_scope, "v2", 2);
    insert(many_scope, "v3", 3);
    insert(many_scope, "v4", 4);
    insert(many_scope, "v5", 5);
    insert(many_scope, "v6", 6);
    insert(many_scope, "v7", 7);
    insert(many_scope, "v8", 8);

    val = lookup(many_scope, "v1");
    printf("v1 = %d\n", val);
    check(val == 1, "many v1");

    val = lookup(many_scope, "v2");
    printf("v2 = %d\n", val);
    check(val == 2, "many v2");

    val = lookup(many_scope, "v3");
    printf("v3 = %d\n", val);
    check(val == 3, "many v3");

    val = lookup(many_scope, "v4");
    printf("v4 = %d\n", val);
    check(val == 4, "many v4");

    val = lookup(many_scope, "v5");
    printf("v5 = %d\n", val);
    check(val == 5, "many v5");

    val = lookup(many_scope, "v6");
    printf("v6 = %d\n", val);
    check(val == 6, "many v6");

    val = lookup(many_scope, "v7");
    printf("v7 = %d\n", val);
    check(val == 7, "many v7");

    val = lookup(many_scope, "v8");
    printf("v8 = %d\n", val);
    check(val == 8, "many v8");

    // ---- Test deep scope chain ----
    printf("--- Deep Chain ---\n");
    deep_bot = new_table(-1);
    insert(deep_bot, "depth", 0);

    deep_mid = new_table(deep_bot);
    insert(deep_mid, "level", 1);

    deep_top = new_table(deep_mid);
    // No inserts in top scope, lookups should chain down

    val = lookup(deep_top, "depth");
    printf("depth from top = %d\n", val);
    check(val == 0, "deep chain depth");

    val = lookup(deep_top, "level");
    printf("level from top = %d\n", val);
    check(val == 1, "deep chain level");

    val = lookup(deep_mid, "depth");
    printf("depth from mid = %d\n", val);
    check(val == 0, "deep chain mid to bot");

    // ---- Test overwrite ----
    printf("--- Overwrite Test ---\n");
    overwrite_scope = new_table(-1);

    insert(overwrite_scope, "key", 42);
    val = lookup(overwrite_scope, "key");
    printf("key = %d\n", val);
    check(val == 42, "overwrite before");

    insert(overwrite_scope, "key", 99);
    val = lookup(overwrite_scope, "key");
    printf("key = %d\n", val);
    check(val == 99, "overwrite after");

    // ---- Summary ----
    printf("--- Stats ---\n");
    printf("total_tests = %d\n", total_tests);
    printf("passed = %d\n", passed);

    return 0;
}
