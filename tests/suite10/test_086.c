int printf(const char *fmt, ...);

// Singleton Pattern: simulated via static local variable

struct Config {
    int max_connections;
    int timeout;
    int retry_count;
    int verbose;
    int initialized;
};

struct Config global_config;
int config_initialized;

struct Config *get_config() {
    if (!config_initialized) {
        global_config.max_connections = 100;
        global_config.timeout = 30;
        global_config.retry_count = 3;
        global_config.verbose = 0;
        global_config.initialized = 1;
        config_initialized = 1;
        printf("Config initialized\n");
    }
    return &global_config;
}

void config_set_timeout(int t) {
    struct Config *c;
    c = get_config();
    c->timeout = t;
}

void config_set_verbose(int v) {
    struct Config *c;
    c = get_config();
    c->verbose = v;
}

void config_set_max_conn(int m) {
    struct Config *c;
    c = get_config();
    c->max_connections = m;
}

void config_print(struct Config *c) {
    printf("max_conn: %d\n", c->max_connections);
    printf("timeout: %d\n", c->timeout);
    printf("retry: %d\n", c->retry_count);
    printf("verbose: %d\n", c->verbose);
}

struct Logger {
    int log_count;
    int level;
};

struct Logger global_logger;
int logger_initialized;

struct Logger *get_logger() {
    if (!logger_initialized) {
        global_logger.log_count = 0;
        global_logger.level = 1;
        logger_initialized = 1;
        printf("Logger initialized\n");
    }
    return &global_logger;
}

void log_message(char *msg, int importance) {
    struct Logger *lg;
    lg = get_logger();
    if (importance >= lg->level) {
        lg->log_count = lg->log_count + 1;
        printf("LOG[%d]: %s\n", lg->log_count, msg);
    }
}

void set_log_level(int level) {
    struct Logger *lg;
    lg = get_logger();
    lg->level = level;
}

int main() {
    struct Config *c1;
    c1 = get_config(); // EXPECT: Config initialized
    printf("First access done\n"); // EXPECT: First access done

    struct Config *c2;
    c2 = get_config();
    printf("Second access (no init)\n"); // EXPECT: Second access (no init)

    int same;
    same = (c1 == c2) ? 1 : 0;
    printf("Same instance: %d\n", same); // EXPECT: Same instance: 1

    printf("Default timeout: %d\n", c1->timeout); // EXPECT: Default timeout: 30
    printf("Default max: %d\n", c1->max_connections); // EXPECT: Default max: 100

    config_set_timeout(60);
    printf("New timeout: %d\n", c2->timeout); // EXPECT: New timeout: 60

    config_set_max_conn(200);
    config_set_verbose(1);

    config_print(get_config());
    // EXPECT: max_conn: 200
    // EXPECT: timeout: 60
    // EXPECT: retry: 3
    // EXPECT: verbose: 1

    log_message("system start", 1); // EXPECT: Logger initialized
    // EXPECT: LOG[1]: system start
    log_message("debug info", 0);
    printf("Log count: %d\n", get_logger()->log_count); // EXPECT: Log count: 1

    set_log_level(0);
    log_message("low priority", 0); // EXPECT: LOG[2]: low priority
    log_message("high priority", 5); // EXPECT: LOG[3]: high priority

    printf("Final log count: %d\n", get_logger()->log_count); // EXPECT: Final log count: 3

    printf("Done\n"); // EXPECT: Done
    return 0;
}
