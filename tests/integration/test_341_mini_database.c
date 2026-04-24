int printf(const char *fmt, ...);
// EXPECT: inserted ids: 1 2 3 4\nactive: 4\nrecords:\n  [1] f1=10 f2=20\n  [2] f1=30 f2=40\n  [3] f1=50 f2=60\n  [4] f1=70 f2=80\nafter update id2:\n  [1] f1=10 f2=20\n  [2] f1=300 f2=400\n  [3] f1=50 f2=60\n  [4] f1=70 f2=80\nafter delete id3:\nactive: 3\n  [1] f1=10 f2=20\n  [2] f1=300 f2=400\n  [4] f1=70 f2=80\nsum field1: 380\nfound id1: f1=10 f2=20\nfind deleted: 1
// Test: mini database with structs

struct Record {
    int id;
    int field1;
    int field2;
    int active;
};

struct Database {
    struct Record records[16];
    int count;
    int next_id;
};

void db_init(struct Database *db) {
    db->count = 0;
    db->next_id = 1;
}

int db_insert(struct Database *db, int f1, int f2) {
    if (db->count >= 16) return -1;
    int idx = db->count;
    db->records[idx].id = db->next_id;
    db->records[idx].field1 = f1;
    db->records[idx].field2 = f2;
    db->records[idx].active = 1;
    db->count = db->count + 1;
    db->next_id = db->next_id + 1;
    return db->records[idx].id;
}

struct Record *db_find(struct Database *db, int id) {
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].id == id && db->records[i].active) {
            return &db->records[i];
        }
    }
    return 0;
}

int db_update(struct Database *db, int id, int f1, int f2) {
    struct Record *r = db_find(db, id);
    if (!r) return 0;
    r->field1 = f1;
    r->field2 = f2;
    return 1;
}

int db_delete(struct Database *db, int id) {
    struct Record *r = db_find(db, id);
    if (!r) return 0;
    r->active = 0;
    return 1;
}

int db_count_active(struct Database *db) {
    int count = 0;
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active) count = count + 1;
    }
    return count;
}

int db_sum_field1(struct Database *db) {
    int sum = 0;
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active) {
            sum = sum + db->records[i].field1;
        }
    }
    return sum;
}

void db_print(struct Database *db) {
    int i;
    for (i = 0; i < db->count; i++) {
        if (db->records[i].active) {
            printf("  [%d] f1=%d f2=%d\n",
                   db->records[i].id, db->records[i].field1, db->records[i].field2);
        }
    }
}

int main(void) {
    struct Database db;
    db_init(&db);

    int id1 = db_insert(&db, 10, 20);
    int id2 = db_insert(&db, 30, 40);
    int id3 = db_insert(&db, 50, 60);
    int id4 = db_insert(&db, 70, 80);

    printf("inserted ids: %d %d %d %d\n", id1, id2, id3, id4);
    printf("active: %d\n", db_count_active(&db));
    printf("records:\n");
    db_print(&db);

    db_update(&db, id2, 300, 400);
    printf("after update id2:\n");
    db_print(&db);

    db_delete(&db, id3);
    printf("after delete id3:\n");
    printf("active: %d\n", db_count_active(&db));
    db_print(&db);

    printf("sum field1: %d\n", db_sum_field1(&db));

    struct Record *found = db_find(&db, id1);
    if (found) {
        printf("found id1: f1=%d f2=%d\n", found->field1, found->field2);
    }

    struct Record *notfound = db_find(&db, id3);
    printf("find deleted: %d\n", notfound == 0 ? 1 : 0);

    return 0;
}
