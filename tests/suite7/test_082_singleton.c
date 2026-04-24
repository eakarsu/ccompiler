int printf(const char *fmt, ...);

struct Config {
    int initialized;
    int max_connections;
    int timeout;
    int retries;
    int verbose;
    int port;
};

struct Config g_config;
int g_init_count;

void config_init(void) {
    if (g_config.initialized) {
        return;
    }
    g_config.initialized = 1;
    g_config.max_connections = 100;
    g_config.timeout = 30;
    g_config.retries = 3;
    g_config.verbose = 0;
    g_config.port = 8080;
    g_init_count = g_init_count + 1;
}

struct Config *get_config(void) {
    config_init();
    return &g_config;
}

int config_get_max_connections(void) {
    struct Config *c;
    c = get_config();
    return c->max_connections;
}

int config_get_timeout(void) {
    struct Config *c;
    c = get_config();
    return c->timeout;
}

int config_get_retries(void) {
    struct Config *c;
    c = get_config();
    return c->retries;
}

int config_get_port(void) {
    struct Config *c;
    c = get_config();
    return c->port;
}

void config_set_verbose(int v) {
    struct Config *c;
    c = get_config();
    c->verbose = v;
}

void config_set_timeout(int t) {
    struct Config *c;
    c = get_config();
    c->timeout = t;
}

void config_set_port(int p) {
    struct Config *c;
    c = get_config();
    c->port = p;
}

int config_is_verbose(void) {
    struct Config *c;
    c = get_config();
    return c->verbose;
}

int config_validate(void) {
    struct Config *c;
    c = get_config();
    if (c->max_connections <= 0) return 0;
    if (c->timeout <= 0) return 0;
    if (c->retries < 0) return 0;
    if (c->port < 1 || c->port > 65535) return 0;
    return 1;
}

void config_reset(void) {
    g_config.initialized = 0;
    g_init_count = 0;
}

int main(void) {
    struct Config *c1;
    struct Config *c2;
    int val;

    config_reset();

    c1 = get_config();
    // EXPECT: init_count: 1
    printf("init_count: %d\n", g_init_count);

    c2 = get_config();
    // EXPECT: init_count after second get: 1
    printf("init_count after second get: %d\n", g_init_count);

    // EXPECT: max_connections: 100
    printf("max_connections: %d\n", config_get_max_connections());

    // EXPECT: timeout: 30
    printf("timeout: %d\n", config_get_timeout());

    // EXPECT: retries: 3
    printf("retries: %d\n", config_get_retries());

    // EXPECT: port: 8080
    printf("port: %d\n", config_get_port());

    // EXPECT: verbose: 0
    printf("verbose: %d\n", config_is_verbose());

    config_set_verbose(1);
    // EXPECT: verbose after set: 1
    printf("verbose after set: %d\n", config_is_verbose());

    config_set_timeout(60);
    // EXPECT: timeout after set: 60
    printf("timeout after set: %d\n", config_get_timeout());

    config_set_port(3000);
    // EXPECT: port after set: 3000
    printf("port after set: %d\n", config_get_port());

    // EXPECT: valid: 1
    printf("valid: %d\n", config_validate());

    // EXPECT: init_count final: 1
    printf("init_count final: %d\n", g_init_count);

    config_reset();
    // EXPECT: after reset init: 0
    printf("after reset init: %d\n", g_config.initialized);

    c1 = get_config();
    // EXPECT: re-init count: 1
    printf("re-init count: %d\n", g_init_count);

    // EXPECT: re-init max_connections: 100
    printf("re-init max_connections: %d\n", config_get_max_connections());

    // EXPECT: re-init timeout: 30
    printf("re-init timeout: %d\n", config_get_timeout());

    return 0;
}
