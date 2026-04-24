int printf(const char *fmt, ...);

// Log file analyzer: parse log lines, count levels, extract timestamps and messages

struct LogEntry {
    char timestamp[20];
    int level;          // 0=DEBUG, 1=INFO, 2=WARN, 3=ERROR
    char message[64];
};

struct LogStats {
    int debug_count;
    int info_count;
    int warn_count;
    int error_count;
    int total;
};

struct LogFile {
    struct LogEntry entries[16];
    int count;
    struct LogStats stats;
};

int str_len(char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

void str_ncpy(char *dst, char *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
    dst[i] = 0;
}

int starts_with(char *s, char *prefix) {
    int i = 0;
    while (prefix[i]) {
        if (s[i] != prefix[i]) return 0;
        i++;
    }
    return 1;
}

char *level_str(int level) {
    if (level == 0) return "DEBUG";
    if (level == 1) return "INFO";
    if (level == 2) return "WARN";
    if (level == 3) return "ERROR";
    return "UNKNOWN";
}

int parse_level(char *s, int pos) {
    if (starts_with(s + pos, "DEBUG")) return 0;
    if (starts_with(s + pos, "INFO")) return 1;
    if (starts_with(s + pos, "WARN")) return 2;
    if (starts_with(s + pos, "ERROR")) return 3;
    return -1;
}

int level_len(int level) {
    if (level == 0) return 5;
    if (level == 1) return 4;
    if (level == 2) return 4;
    if (level == 3) return 5;
    return 0;
}

// Format: [HH:MM:SS] LEVEL message
int parse_log_line(char *line, struct LogFile *log) {
    int len = str_len(line);
    if (len < 12) return 0;

    int pos = 0;
    if (line[pos] != '[') return 0;
    pos++;

    // Read timestamp until ]
    int ts_start = pos;
    while (pos < len && line[pos] != ']') pos++;
    int ts_len = pos - ts_start;
    if (ts_len > 19) ts_len = 19;

    int idx = log->count;
    str_ncpy(log->entries[idx].timestamp, line + ts_start, ts_len);

    pos++; // skip ]
    while (pos < len && line[pos] == ' ') pos++;

    // Read level
    int level = parse_level(line, pos);
    if (level < 0) return 0;
    log->entries[idx].level = level;
    pos += level_len(level);

    while (pos < len && line[pos] == ' ') pos++;

    // Rest is message
    int msg_len = len - pos;
    if (msg_len > 63) msg_len = 63;
    str_ncpy(log->entries[idx].message, line + pos, msg_len);

    log->count++;

    // Update stats
    if (level == 0) log->stats.debug_count++;
    else if (level == 1) log->stats.info_count++;
    else if (level == 2) log->stats.warn_count++;
    else if (level == 3) log->stats.error_count++;
    log->stats.total++;

    return 1;
}

void init_log(struct LogFile *log) {
    log->count = 0;
    log->stats.debug_count = 0;
    log->stats.info_count = 0;
    log->stats.warn_count = 0;
    log->stats.error_count = 0;
    log->stats.total = 0;
}

int count_level(struct LogFile *log, int level) {
    int c = 0;
    int i;
    for (i = 0; i < log->count; i++) {
        if (log->entries[i].level == level) c++;
    }
    return c;
}

int find_first_error(struct LogFile *log) {
    int i;
    for (i = 0; i < log->count; i++) {
        if (log->entries[i].level == 3) return i;
    }
    return -1;
}

int contains_word(char *msg, char *word) {
    int mlen = str_len(msg);
    int wlen = str_len(word);
    int i;
    for (i = 0; i <= mlen - wlen; i++) {
        int j;
        int match = 1;
        for (j = 0; j < wlen; j++) {
            if (msg[i + j] != word[j]) { match = 0; break; }
        }
        if (match) return 1;
    }
    return 0;
}

int count_with_word(struct LogFile *log, char *word) {
    int c = 0;
    int i;
    for (i = 0; i < log->count; i++) {
        if (contains_word(log->entries[i].message, word)) c++;
    }
    return c;
}

int main() {
    struct LogFile log;
    init_log(&log);

    parse_log_line("[10:00:01] INFO Server started on port 8080", &log);
    parse_log_line("[10:00:02] DEBUG Loading config file", &log);
    parse_log_line("[10:00:03] INFO Connected to database", &log);
    parse_log_line("[10:00:05] WARN Slow query detected", &log);
    parse_log_line("[10:00:10] ERROR Connection timeout", &log);
    parse_log_line("[10:00:11] INFO Retrying connection", &log);
    parse_log_line("[10:00:15] ERROR Disk space low", &log);
    parse_log_line("[10:00:20] DEBUG Cache cleared", &log);
    parse_log_line("[10:00:25] WARN Memory usage high", &log);
    parse_log_line("[10:00:30] INFO Request processed", &log);

    printf("%d\n", log.count);
    // EXPECT: 10
    printf("%d\n", log.stats.total);
    // EXPECT: 10
    printf("%d\n", log.stats.info_count);
    // EXPECT: 4
    printf("%d\n", log.stats.debug_count);
    // EXPECT: 2
    printf("%d\n", log.stats.warn_count);
    // EXPECT: 2
    printf("%d\n", log.stats.error_count);
    // EXPECT: 2

    // Verify vs count function
    printf("%d\n", count_level(&log, 1));
    // EXPECT: 4
    printf("%d\n", count_level(&log, 3));
    // EXPECT: 2

    // Check first entry
    printf("%s\n", log.entries[0].timestamp);
    // EXPECT: 10:00:01
    printf("%s\n", level_str(log.entries[0].level));
    // EXPECT: INFO
    printf("%s\n", log.entries[0].message);
    // EXPECT: Server started on port 8080

    // Find first error
    int err_idx = find_first_error(&log);
    printf("%d\n", err_idx);
    // EXPECT: 4
    printf("%s\n", log.entries[err_idx].message);
    // EXPECT: Connection timeout
    printf("%s\n", log.entries[err_idx].timestamp);
    // EXPECT: 10:00:10

    // Word search
    int conn_count = count_with_word(&log, "connection");
    printf("%d\n", conn_count);
    // EXPECT: 1

    int conn_count2 = count_with_word(&log, "Connection");
    printf("%d\n", conn_count2);
    // EXPECT: 1

    // Check a WARN entry
    printf("%s\n", log.entries[3].message);
    // EXPECT: Slow query detected
    printf("%s\n", level_str(log.entries[3].level));
    // EXPECT: WARN

    // Check last entry
    printf("%s\n", log.entries[9].message);
    // EXPECT: Request processed
    printf("%s\n", log.entries[9].timestamp);
    // EXPECT: 10:00:30

    return 0;
}
