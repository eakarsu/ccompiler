int printf(const char *fmt, ...);

struct Record {
    int id;
    int age;
    int active;
    char name[20];
};

struct Table {
    struct Record rows[32];
    int count;
};

void copy_str(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int db_insert(struct Table *t, int id, const char *name, int age) {
    if (t->count >= 32) return -1;
    int idx = t->count;
    t->rows[idx].id = id;
    t->rows[idx].age = age;
    t->rows[idx].active = 1;
    copy_str(t->rows[idx].name, name);
    t->count = t->count + 1;
    return idx;
}

int db_find_by_id(struct Table *t, int id) {
    int i = 0;
    while (i < t->count) {
        if (t->rows[i].id == id && t->rows[i].active) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int db_find_by_name(struct Table *t, const char *name) {
    int i = 0;
    while (i < t->count) {
        if (t->rows[i].active && str_eq(t->rows[i].name, name)) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int db_delete(struct Table *t, int id) {
    int idx = db_find_by_id(t, id);
    if (idx < 0) return 0;
    t->rows[idx].active = 0;
    return 1;
}

int db_update_age(struct Table *t, int id, int new_age) {
    int idx = db_find_by_id(t, id);
    if (idx < 0) return 0;
    t->rows[idx].age = new_age;
    return 1;
}

int db_count_active(struct Table *t) {
    int c = 0;
    int i = 0;
    while (i < t->count) {
        if (t->rows[i].active) c = c + 1;
        i = i + 1;
    }
    return c;
}

int db_sum_ages(struct Table *t) {
    int s = 0;
    int i = 0;
    while (i < t->count) {
        if (t->rows[i].active) s = s + t->rows[i].age;
        i = i + 1;
    }
    return s;
}

int db_max_age(struct Table *t) {
    int mx = -1;
    int i = 0;
    while (i < t->count) {
        if (t->rows[i].active && t->rows[i].age > mx) {
            mx = t->rows[i].age;
        }
        i = i + 1;
    }
    return mx;
}

int db_min_age(struct Table *t) {
    int mn = 99999;
    int i = 0;
    while (i < t->count) {
        if (t->rows[i].active && t->rows[i].age < mn) {
            mn = t->rows[i].age;
        }
        i = i + 1;
    }
    if (mn == 99999) return -1;
    return mn;
}

int main(void) {
    struct Table db;
    db.count = 0;

    db_insert(&db, 1, "Alice", 30);
    db_insert(&db, 2, "Bob", 25);
    db_insert(&db, 3, "Carol", 35);
    db_insert(&db, 4, "Dave", 28);
    db_insert(&db, 5, "Eve", 22);

    // EXPECT: count=5
    printf("count=%d\n", db_count_active(&db));

    int idx = db_find_by_id(&db, 3);
    // EXPECT: find_id3=2
    printf("find_id3=%d\n", idx);

    // EXPECT: age_of_3=35
    printf("age_of_3=%d\n", db.rows[idx].age);

    idx = db_find_by_name(&db, "Bob");
    // EXPECT: find_bob=1
    printf("find_bob=%d\n", idx);

    // EXPECT: bob_age=25
    printf("bob_age=%d\n", db.rows[idx].age);

    int del = db_delete(&db, 2);
    // EXPECT: deleted=1
    printf("deleted=%d\n", del);

    // EXPECT: count_after_del=4
    printf("count_after_del=%d\n", db_count_active(&db));

    int notfound = db_find_by_id(&db, 2);
    // EXPECT: find_deleted=-1
    printf("find_deleted=%d\n", notfound);

    db_update_age(&db, 1, 31);
    idx = db_find_by_id(&db, 1);
    // EXPECT: alice_new_age=31
    printf("alice_new_age=%d\n", db.rows[idx].age);

    // EXPECT: sum_ages=116
    printf("sum_ages=%d\n", db_sum_ages(&db));

    // EXPECT: max_age=35
    printf("max_age=%d\n", db_max_age(&db));

    // EXPECT: min_age=22
    printf("min_age=%d\n", db_min_age(&db));

    db_insert(&db, 6, "Frank", 40);
    // EXPECT: count_after_ins=5
    printf("count_after_ins=%d\n", db_count_active(&db));

    // EXPECT: max_age2=40
    printf("max_age2=%d\n", db_max_age(&db));

    db_delete(&db, 5);
    db_delete(&db, 6);
    // EXPECT: count_final=3
    printf("count_final=%d\n", db_count_active(&db));

    // EXPECT: sum_final=94
    printf("sum_final=%d\n", db_sum_ages(&db));

    int dup = db_delete(&db, 99);
    // EXPECT: del_nonexist=0
    printf("del_nonexist=%d\n", dup);

    return 0;
}
