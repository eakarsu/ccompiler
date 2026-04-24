int printf(const char *fmt, ...);

// Test 089: Log buffer with wrap-around and history replay

struct LogEntry {
    int timestamp;
    int level;   // 0=debug, 1=info, 2=warn, 3=error
    int code;
};

struct LogBuffer {
    struct LogEntry entries[6];
    int write_pos;
    int count;
    int capacity;
    int total_written;
};

void log_init(struct LogBuffer *lb) {
    int i;
    for (i = 0; i < 6; i++) {
        lb->entries[i].timestamp = 0;
        lb->entries[i].level = 0;
        lb->entries[i].code = 0;
    }
    lb->write_pos = 0;
    lb->count = 0;
    lb->capacity = 6;
    lb->total_written = 0;
}

void log_write(struct LogBuffer *lb, int ts, int level, int code) {
    lb->entries[lb->write_pos].timestamp = ts;
    lb->entries[lb->write_pos].level = level;
    lb->entries[lb->write_pos].code = code;
    lb->write_pos = (lb->write_pos + 1) % lb->capacity;
    if (lb->count < lb->capacity) {
        lb->count = lb->count + 1;
    }
    lb->total_written = lb->total_written + 1;
}

int log_read(struct LogBuffer *lb, int index, struct LogEntry *out) {
    int start;
    int actual;
    if (index < 0 || index >= lb->count) return 0;
    // oldest entry is at (write_pos - count + capacity) % capacity
    start = (lb->write_pos - lb->count + lb->capacity) % lb->capacity;
    actual = (start + index) % lb->capacity;
    out->timestamp = lb->entries[actual].timestamp;
    out->level = lb->entries[actual].level;
    out->code = lb->entries[actual].code;
    return 1;
}

int log_count_level(struct LogBuffer *lb, int level) {
    int i;
    int start;
    int actual;
    int cnt;
    cnt = 0;
    start = (lb->write_pos - lb->count + lb->capacity) % lb->capacity;
    for (i = 0; i < lb->count; i++) {
        actual = (start + i) % lb->capacity;
        if (lb->entries[actual].level == level) {
            cnt = cnt + 1;
        }
    }
    return cnt;
}

int main() {
    struct LogBuffer lb;
    struct LogEntry entry;
    int i;
    int ok;

    log_init(&lb);
    printf("Init count: %d\n", lb.count); // EXPECT: Init count: 0

    // Write 4 entries (not filling buffer)
    log_write(&lb, 100, 0, 1001);
    log_write(&lb, 200, 1, 2001);
    log_write(&lb, 300, 2, 3001);
    log_write(&lb, 400, 3, 4001);
    printf("Count: %d written: %d\n", lb.count, lb.total_written); // EXPECT: Count: 4 written: 4

    // Read back
    log_read(&lb, 0, &entry);
    printf("Entry 0: ts=%d lv=%d code=%d\n", entry.timestamp, entry.level, entry.code); // EXPECT: Entry 0: ts=100 lv=0 code=1001
    log_read(&lb, 3, &entry);
    printf("Entry 3: ts=%d lv=%d code=%d\n", entry.timestamp, entry.level, entry.code); // EXPECT: Entry 3: ts=400 lv=3 code=4001

    // Fill buffer completely
    log_write(&lb, 500, 1, 5001);
    log_write(&lb, 600, 2, 6001);
    printf("Count: %d written: %d\n", lb.count, lb.total_written); // EXPECT: Count: 6 written: 6

    // Write more (overwriting oldest)
    log_write(&lb, 700, 3, 7001);
    log_write(&lb, 800, 0, 8001);
    log_write(&lb, 900, 1, 9001);
    printf("Count: %d written: %d\n", lb.count, lb.total_written); // EXPECT: Count: 6 written: 9

    // Oldest should now be ts=400
    log_read(&lb, 0, &entry);
    printf("Oldest: ts=%d code=%d\n", entry.timestamp, entry.code); // EXPECT: Oldest: ts=400 code=4001

    // Newest should be ts=900
    log_read(&lb, 5, &entry);
    printf("Newest: ts=%d code=%d\n", entry.timestamp, entry.code); // EXPECT: Newest: ts=900 code=9001

    // Count by level
    printf("Debug: %d\n", log_count_level(&lb, 0)); // EXPECT: Debug: 1
    printf("Info: %d\n", log_count_level(&lb, 1)); // EXPECT: Info: 2
    printf("Warn: %d\n", log_count_level(&lb, 2)); // EXPECT: Warn: 1
    printf("Error: %d\n", log_count_level(&lb, 3)); // EXPECT: Error: 2

    // Out of bounds read
    ok = log_read(&lb, 6, &entry);
    printf("Out of bounds: %d\n", ok); // EXPECT: Out of bounds: 0
    ok = log_read(&lb, -1, &entry);
    printf("Negative index: %d\n", ok); // EXPECT: Negative index: 0

    // Replay all entries
    printf("Replay:\n"); // EXPECT: Replay:
    log_read(&lb, 0, &entry);
    printf("  ts=%d lv=%d code=%d\n", entry.timestamp, entry.level, entry.code); // EXPECT:   ts=400 lv=3 code=4001
    log_read(&lb, 1, &entry);
    printf("  ts=%d lv=%d code=%d\n", entry.timestamp, entry.level, entry.code); // EXPECT:   ts=500 lv=1 code=5001
    log_read(&lb, 2, &entry);
    printf("  ts=%d lv=%d code=%d\n", entry.timestamp, entry.level, entry.code); // EXPECT:   ts=600 lv=2 code=6001
    log_read(&lb, 3, &entry);
    printf("  ts=%d lv=%d code=%d\n", entry.timestamp, entry.level, entry.code); // EXPECT:   ts=700 lv=3 code=7001
    log_read(&lb, 4, &entry);
    printf("  ts=%d lv=%d code=%d\n", entry.timestamp, entry.level, entry.code); // EXPECT:   ts=800 lv=0 code=8001
    log_read(&lb, 5, &entry);
    printf("  ts=%d lv=%d code=%d\n", entry.timestamp, entry.level, entry.code); // EXPECT:   ts=900 lv=1 code=9001

    // Overwrite everything
    for (i = 0; i < 12; i++) {
        log_write(&lb, 1000 + i, i % 4, 100 + i);
    }
    printf("Total written: %d\n", lb.total_written); // EXPECT: Total written: 21
    log_read(&lb, 0, &entry);
    printf("After overwrite oldest: ts=%d\n", entry.timestamp); // EXPECT: After overwrite oldest: ts=1006
    log_read(&lb, 5, &entry);
    printf("After overwrite newest: ts=%d\n", entry.timestamp); // EXPECT: After overwrite newest: ts=1011

    printf("PASS\n"); // EXPECT: PASS
    return 0;
}
