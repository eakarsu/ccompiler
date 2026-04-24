int printf(const char *fmt, ...);
// EXPECT: Insert 5: 1\nInsert 13: 1\nInsert 21: 1\nInsert 10: 1\nInsert 3: 1\nInsert 7: 1\nSize: 6\nContains 5: 1\nContains 13: 1\nContains 99: 0\nTable for 5: 2\nTable for 13: 2\nRemove 13: 1\nContains 13: 0\nSize: 5\nRe-insert 13: 1\nContains 13: 1\nSize: 6\nDup insert 5: 1\nSize: 6\nDone

// Cuckoo hashing simulation with two tables

struct CuckooHash {
    int table1[8];
    int table2[8];
    int occupied1[8];
    int occupied2[8];
    int size;
};

void ch_init(struct CuckooHash *ch) {
    ch->size = 0;
    int i = 0;
    while (i < 8) {
        ch->table1[i] = 0;
        ch->table2[i] = 0;
        ch->occupied1[i] = 0;
        ch->occupied2[i] = 0;
        i = i + 1;
    }
}

int ch_hash1(int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % 8;
}

int ch_hash2(int key) {
    int h = key;
    if (h < 0) h = -h;
    return (h / 8) % 8;
}

int ch_contains(struct CuckooHash *ch, int key) {
    int h1 = ch_hash1(key);
    if (ch->occupied1[h1] && ch->table1[h1] == key) return 1;
    int h2 = ch_hash2(key);
    if (ch->occupied2[h2] && ch->table2[h2] == key) return 1;
    return 0;
}

int ch_get_table(struct CuckooHash *ch, int key) {
    int h1 = ch_hash1(key);
    if (ch->occupied1[h1] && ch->table1[h1] == key) return 1;
    int h2 = ch_hash2(key);
    if (ch->occupied2[h2] && ch->table2[h2] == key) return 2;
    return 0;
}

int ch_insert(struct CuckooHash *ch, int key) {
    if (ch_contains(ch, key)) return 1;

    int current = key;
    int max_kicks = 16;
    int i = 0;
    while (i < max_kicks) {
        int h1 = ch_hash1(current);
        if (!ch->occupied1[h1]) {
            ch->table1[h1] = current;
            ch->occupied1[h1] = 1;
            ch->size = ch->size + 1;
            return 1;
        }
        // Evict from table1
        int evicted = ch->table1[h1];
        ch->table1[h1] = current;
        current = evicted;

        int h2 = ch_hash2(current);
        if (!ch->occupied2[h2]) {
            ch->table2[h2] = current;
            ch->occupied2[h2] = 1;
            ch->size = ch->size + 1;
            return 1;
        }
        // Evict from table2
        evicted = ch->table2[h2];
        ch->table2[h2] = current;
        current = evicted;

        i = i + 1;
    }
    return 0;  // Failed - would need rehash
}

int ch_remove(struct CuckooHash *ch, int key) {
    int h1 = ch_hash1(key);
    if (ch->occupied1[h1] && ch->table1[h1] == key) {
        ch->occupied1[h1] = 0;
        ch->size = ch->size - 1;
        return 1;
    }
    int h2 = ch_hash2(key);
    if (ch->occupied2[h2] && ch->table2[h2] == key) {
        ch->occupied2[h2] = 0;
        ch->size = ch->size - 1;
        return 1;
    }
    return 0;
}

void ch_print_tables(struct CuckooHash *ch) {
    printf("Table1:");
    int i = 0;
    while (i < 8) {
        if (ch->occupied1[i]) printf(" [%d]=%d", i, ch->table1[i]);
        else printf(" [%d]=_", i);
        i = i + 1;
    }
    printf("\n");
    printf("Table2:");
    i = 0;
    while (i < 8) {
        if (ch->occupied2[i]) printf(" [%d]=%d", i, ch->table2[i]);
        else printf(" [%d]=_", i);
        i = i + 1;
    }
    printf("\n");
}

int main(void) {
    struct CuckooHash ch;
    ch_init(&ch);

    printf("Insert 5: %d\n", ch_insert(&ch, 5));
    printf("Insert 13: %d\n", ch_insert(&ch, 13));
    printf("Insert 21: %d\n", ch_insert(&ch, 21));
    printf("Insert 10: %d\n", ch_insert(&ch, 10));
    printf("Insert 3: %d\n", ch_insert(&ch, 3));
    printf("Insert 7: %d\n", ch_insert(&ch, 7));

    printf("Size: %d\n", ch.size);

    printf("Contains 5: %d\n", ch_contains(&ch, 5));
    printf("Contains 13: %d\n", ch_contains(&ch, 13));
    printf("Contains 99: %d\n", ch_contains(&ch, 99));

    printf("Table for 5: %d\n", ch_get_table(&ch, 5));
    printf("Table for 13: %d\n", ch_get_table(&ch, 13));

    // Remove
    printf("Remove 13: %d\n", ch_remove(&ch, 13));
    printf("Contains 13: %d\n", ch_contains(&ch, 13));
    printf("Size: %d\n", ch.size);

    // Re-insert
    printf("Re-insert 13: %d\n", ch_insert(&ch, 13));
    printf("Contains 13: %d\n", ch_contains(&ch, 13));
    printf("Size: %d\n", ch.size);

    // Duplicate insert
    printf("Dup insert 5: %d\n", ch_insert(&ch, 5));
    printf("Size: %d\n", ch.size);

    printf("Done\n");
    return 0;
}
