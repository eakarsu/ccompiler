int printf(const char *fmt, ...);
// EXPECT: All records:\n  [1] Alice age=25 score=88\n  [2] Bob age=30 score=92\n  [3] Carol age=22 score=78\n  [4] Dave age=28 score=95\n  [5] Eve age=35 score=85\n  [6] Frank age=20 score=70\nSorted by score (desc):\n  [4] Dave age=28 score=95\n  [2] Bob age=30 score=92\n  [1] Alice age=25 score=88\n  [5] Eve age=35 score=85\n  [3] Carol age=22 score=78\n  [6] Frank age=20 score=70\nSorted by age (asc):\n  [6] Frank age=20 score=70\n  [3] Carol age=22 score=78\n  [1] Alice age=25 score=88\n  [4] Dave age=28 score=95\n  [2] Bob age=30 score=92\n  [5] Eve age=35 score=85\nAge >= 25: 4 records\n  [1] Alice age=25 score=88\n  [4] Dave age=28 score=95\n  [2] Bob age=30 score=92\n  [5] Eve age=35 score=85\nAvg score: 84
// Simple database: array of struct records, query, sort, filter

struct Record {
    int id;
    char name[16];
    int age;
    int score;
};

struct Database {
    struct Record records[20];
    int count;
};

void db_init(struct Database *db) {
    db->count = 0;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0 && i < 15) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

void db_insert(struct Database *db, int id, char *name, int age, int score) {
    if (db->count >= 20) return;
    db->records[db->count].id = id;
    str_copy(db->records[db->count].name, name);
    db->records[db->count].age = age;
    db->records[db->count].score = score;
    db->count++;
}

void db_print_record(struct Record *r) {
    printf("  [%d] %s age=%d score=%d\n", r->id, r->name, r->age, r->score);
}

void db_print_all(struct Database *db) {
    int i;
    for (i = 0; i < db->count; i++) {
        db_print_record(&db->records[i]);
    }
}

void db_sort_by_score(struct Database *db) {
    int i;
    int j;
    struct Record tmp;
    for (i = 0; i < db->count - 1; i++) {
        for (j = 0; j < db->count - 1 - i; j++) {
            if (db->records[j].score < db->records[j + 1].score) {
                tmp = db->records[j];
                db->records[j] = db->records[j + 1];
                db->records[j + 1] = tmp;
            }
        }
    }
}

void db_sort_by_age(struct Database *db) {
    int i;
    int j;
    struct Record tmp;
    for (i = 0; i < db->count - 1; i++) {
        for (j = 0; j < db->count - 1 - i; j++) {
            if (db->records[j].age > db->records[j + 1].age) {
                tmp = db->records[j];
                db->records[j] = db->records[j + 1];
                db->records[j + 1] = tmp;
            }
        }
    }
}

int db_filter_age_above(struct Database *db, int min_age, struct Database *result) {
    int i;
    db_init(result);
    for (i = 0; i < db->count; i++) {
        if (db->records[i].age >= min_age) {
            result->records[result->count] = db->records[i];
            result->count++;
        }
    }
    return result->count;
}

int db_avg_score(struct Database *db) {
    int i;
    int total = 0;
    if (db->count == 0) return 0;
    for (i = 0; i < db->count; i++) total += db->records[i].score;
    return total / db->count;
}

int main(void) {
    struct Database db;
    struct Database filtered;
    db_init(&db);

    db_insert(&db, 1, "Alice", 25, 88);
    db_insert(&db, 2, "Bob", 30, 92);
    db_insert(&db, 3, "Carol", 22, 78);
    db_insert(&db, 4, "Dave", 28, 95);
    db_insert(&db, 5, "Eve", 35, 85);
    db_insert(&db, 6, "Frank", 20, 70);

    printf("All records:\n");
    db_print_all(&db);

    db_sort_by_score(&db);
    printf("Sorted by score (desc):\n");
    db_print_all(&db);

    db_sort_by_age(&db);
    printf("Sorted by age (asc):\n");
    db_print_all(&db);

    printf("Age >= 25: %d records\n", db_filter_age_above(&db, 25, &filtered));
    db_print_all(&filtered);

    printf("Avg score: %d\n", db_avg_score(&db));

    return 0;
}
