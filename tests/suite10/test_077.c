int printf(const char *fmt, ...);

// Test 077: Error logging simulation - log levels, log buffer, filtering

enum LogLevel {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

struct LogEntry {
    int level;
    int timestamp;
    int source_id;
    char message[40];
};

struct Logger {
    struct LogEntry entries[20];
    int count;
    int capacity;
    int min_level;
    int dropped;
    int counts_by_level[6];
};

void str_copy(char *dst, char *src, int max) {
    int i = 0;
    while (src[i] != 0 && i < max - 1) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

char *level_str(int lv) {
    if (lv == LOG_TRACE) return "TRACE";
    if (lv == LOG_DEBUG) return "DEBUG";
    if (lv == LOG_INFO) return "INFO";
    if (lv == LOG_WARN) return "WARN";
    if (lv == LOG_ERROR) return "ERROR";
    if (lv == LOG_FATAL) return "FATAL";
    return "UNKNOWN";
}

void logger_init(struct Logger *log, int min_level) {
    log->count = 0;
    log->capacity = 20;
    log->min_level = min_level;
    log->dropped = 0;
    for (int i = 0; i < 6; i++) {
        log->counts_by_level[i] = 0;
    }
}

int logger_log(struct Logger *log, int level, int timestamp, int source, char *msg) {
    if (level < log->min_level) {
        return 0;
    }

    if (log->count >= log->capacity) {
        log->dropped++;
        return -1;
    }

    struct LogEntry *e = &log->entries[log->count];
    e->level = level;
    e->timestamp = timestamp;
    e->source_id = source;
    str_copy(e->message, msg, 40);
    log->count++;
    log->counts_by_level[level]++;
    return 1;
}

int logger_count_level(struct Logger *log, int level) {
    return log->counts_by_level[level];
}

int logger_has_errors(struct Logger *log) {
    return log->counts_by_level[LOG_ERROR] > 0 || log->counts_by_level[LOG_FATAL] > 0;
}

void logger_dump(struct Logger *log) {
    for (int i = 0; i < log->count; i++) {
        struct LogEntry *e = &log->entries[i];
        printf("[%s] t=%d src=%d: %s\n",
               level_str(e->level), e->timestamp, e->source_id, e->message);
    }
}

int logger_filter_count(struct Logger *log, int source_id) {
    int c = 0;
    for (int i = 0; i < log->count; i++) {
        if (log->entries[i].source_id == source_id) {
            c++;
        }
    }
    return c;
}

int logger_latest_level(struct Logger *log) {
    if (log->count == 0) return -1;
    return log->entries[log->count - 1].level;
}

int main() {
    struct Logger log;
    logger_init(&log, LOG_INFO);

    logger_log(&log, LOG_TRACE, 1, 1, "trace msg");
    logger_log(&log, LOG_DEBUG, 2, 1, "debug msg");
    printf("After trace+debug (filtered): count=%d\n", log.count);
    // EXPECT: After trace+debug (filtered): count=0

    logger_log(&log, LOG_INFO, 3, 1, "system started");
    logger_log(&log, LOG_INFO, 4, 2, "module loaded");
    logger_log(&log, LOG_WARN, 5, 1, "low memory");
    logger_log(&log, LOG_ERROR, 6, 3, "connection lost");
    logger_log(&log, LOG_INFO, 7, 2, "retry attempt");
    logger_log(&log, LOG_FATAL, 8, 3, "disk failure");
    logger_log(&log, LOG_WARN, 9, 1, "high cpu");
    logger_log(&log, LOG_INFO, 10, 2, "recovered");

    printf("Total entries: %d\n", log.count);
    // EXPECT: Total entries: 8

    printf("Has errors: %d\n", logger_has_errors(&log));
    // EXPECT: Has errors: 1

    printf("INFO count: %d\n", logger_count_level(&log, LOG_INFO));
    // EXPECT: INFO count: 4
    printf("WARN count: %d\n", logger_count_level(&log, LOG_WARN));
    // EXPECT: WARN count: 2
    printf("ERROR count: %d\n", logger_count_level(&log, LOG_ERROR));
    // EXPECT: ERROR count: 1
    printf("FATAL count: %d\n", logger_count_level(&log, LOG_FATAL));
    // EXPECT: FATAL count: 1

    printf("Source 1 entries: %d\n", logger_filter_count(&log, 1));
    // EXPECT: Source 1 entries: 3
    printf("Source 2 entries: %d\n", logger_filter_count(&log, 2));
    // EXPECT: Source 2 entries: 3
    printf("Source 3 entries: %d\n", logger_filter_count(&log, 3));
    // EXPECT: Source 3 entries: 2

    printf("Latest level: %s\n", level_str(logger_latest_level(&log)));
    // EXPECT: Latest level: INFO

    printf("Dropped: %d\n", log.dropped);
    // EXPECT: Dropped: 0

    printf("--- Log dump ---\n");
    // EXPECT: --- Log dump ---
    logger_dump(&log);
    // EXPECT: [INFO] t=3 src=1: system started
    // EXPECT: [INFO] t=4 src=2: module loaded
    // EXPECT: [WARN] t=5 src=1: low memory
    // EXPECT: [ERROR] t=6 src=3: connection lost
    // EXPECT: [INFO] t=7 src=2: retry attempt
    // EXPECT: [FATAL] t=8 src=3: disk failure
    // EXPECT: [WARN] t=9 src=1: high cpu
    // EXPECT: [INFO] t=10 src=2: recovered

    struct Logger log2;
    logger_init(&log2, LOG_ERROR);
    logger_log(&log2, LOG_INFO, 1, 1, "filtered out");
    logger_log(&log2, LOG_WARN, 2, 1, "also filtered");
    logger_log(&log2, LOG_ERROR, 3, 1, "kept");
    printf("Strict filter: count=%d\n", log2.count);
    // EXPECT: Strict filter: count=1

    printf("Logging simulation done\n");
    // EXPECT: Logging simulation done
    return 0;
}
