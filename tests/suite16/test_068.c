int printf(const char *fmt, ...);

// URL Parser State Machine
// Parses scheme, host, port, path from URLs

enum url_state {
    URL_SCHEME,
    URL_COLON,
    URL_SLASH1,
    URL_SLASH2,
    URL_HOST,
    URL_PORT,
    URL_PATH,
    URL_DONE,
    URL_ERROR
};

struct url_parts {
    char scheme[16];
    char host[32];
    char port[8];
    char path[32];
    int scheme_len;
    int host_len;
    int port_len;
    int path_len;
    int state;
    int valid;
};

void url_init(struct url_parts *u) {
    u->scheme_len = 0;
    u->host_len = 0;
    u->port_len = 0;
    u->path_len = 0;
    u->state = URL_SCHEME;
    u->valid = 0;
    int i;
    for (i = 0; i < 16; i++) u->scheme[i] = 0;
    for (i = 0; i < 32; i++) { u->host[i] = 0; u->path[i] = 0; }
    for (i = 0; i < 8; i++) u->port[i] = 0;
}

int is_alpha(int c) {
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

int is_digit(int c) {
    return c >= 48 && c <= 57;
}

int is_alnum(int c) {
    return is_alpha(c) || is_digit(c);
}

void url_feed(struct url_parts *u, char c) {
    int st = u->state;
    if (st == URL_SCHEME) {
        if (is_alpha(c) || c == 43 || c == 45 || c == 46) {
            if (u->scheme_len < 15) {
                u->scheme[u->scheme_len] = c;
                u->scheme_len = u->scheme_len + 1;
            }
        } else if (c == 58) { // ':'
            u->state = URL_COLON;
        } else {
            u->state = URL_ERROR;
        }
    } else if (st == URL_COLON) {
        if (c == 47) { // '/'
            u->state = URL_SLASH1;
        } else {
            u->state = URL_ERROR;
        }
    } else if (st == URL_SLASH1) {
        if (c == 47) {
            u->state = URL_SLASH2;
        } else {
            u->state = URL_ERROR;
        }
    } else if (st == URL_SLASH2) {
        // Start of host
        if (is_alnum(c) || c == 46 || c == 45) {
            u->host[u->host_len] = c;
            u->host_len = u->host_len + 1;
            u->state = URL_HOST;
        } else {
            u->state = URL_ERROR;
        }
    } else if (st == URL_HOST) {
        if (is_alnum(c) || c == 46 || c == 45) {
            if (u->host_len < 31) {
                u->host[u->host_len] = c;
                u->host_len = u->host_len + 1;
            }
        } else if (c == 58) { // ':'
            u->state = URL_PORT;
        } else if (c == 47) { // '/'
            u->path[u->path_len] = c;
            u->path_len = u->path_len + 1;
            u->state = URL_PATH;
        } else if (c == 0) {
            u->state = URL_DONE;
            u->valid = 1;
        } else {
            u->state = URL_ERROR;
        }
    } else if (st == URL_PORT) {
        if (is_digit(c)) {
            if (u->port_len < 7) {
                u->port[u->port_len] = c;
                u->port_len = u->port_len + 1;
            }
        } else if (c == 47) {
            u->path[u->path_len] = c;
            u->path_len = u->path_len + 1;
            u->state = URL_PATH;
        } else if (c == 0) {
            u->state = URL_DONE;
            u->valid = 1;
        } else {
            u->state = URL_ERROR;
        }
    } else if (st == URL_PATH) {
        if (c == 0) {
            u->state = URL_DONE;
            u->valid = 1;
        } else if (c > 32 && c < 127) {
            if (u->path_len < 31) {
                u->path[u->path_len] = c;
                u->path_len = u->path_len + 1;
            }
        } else {
            u->state = URL_ERROR;
        }
    }
}

void parse_url(struct url_parts *u, char *url, int len) {
    url_init(u);
    int i;
    for (i = 0; i < len; i++) {
        url_feed(u, url[i]);
    }
    // Finalize
    if (u->state == URL_HOST || u->state == URL_PORT || u->state == URL_PATH) {
        url_feed(u, 0);
    }
}

int to_int(char *s, int len) {
    int val = 0;
    int i;
    for (i = 0; i < len; i++) {
        val = val * 10 + (s[i] - 48);
    }
    return val;
}

int main() {
    struct url_parts u;

    printf("=== URL Parser ===\n"); // EXPECT: === URL Parser ===

    // Test 1: http://example.com
    char u1[32];
    u1[0]='h'; u1[1]='t'; u1[2]='t'; u1[3]='p';
    u1[4]=':'; u1[5]='/'; u1[6]='/';
    u1[7]='e'; u1[8]='x'; u1[9]='a'; u1[10]='m'; u1[11]='p';
    u1[12]='l'; u1[13]='e'; u1[14]='.'; u1[15]='c'; u1[16]='o'; u1[17]='m';
    u1[18]=0;
    parse_url(&u, u1, 18);
    printf("Valid: %d\n", u.valid);       // EXPECT: Valid: 1
    printf("Scheme: %s\n", u.scheme);     // EXPECT: Scheme: http
    printf("Host: %s\n", u.host);         // EXPECT: Host: example.com
    printf("Port len: %d\n", u.port_len);  // EXPECT: Port len: 0
    printf("Path len: %d\n", u.path_len);  // EXPECT: Path len: 0

    // Test 2: https://api.test.io:8080/data/items
    char u2[48];
    u2[0]='h'; u2[1]='t'; u2[2]='t'; u2[3]='p'; u2[4]='s';
    u2[5]=':'; u2[6]='/'; u2[7]='/';
    u2[8]='a'; u2[9]='p'; u2[10]='i'; u2[11]='.';
    u2[12]='t'; u2[13]='e'; u2[14]='s'; u2[15]='t'; u2[16]='.';
    u2[17]='i'; u2[18]='o';
    u2[19]=':'; u2[20]='8'; u2[21]='0'; u2[22]='8'; u2[23]='0';
    u2[24]='/'; u2[25]='d'; u2[26]='a'; u2[27]='t'; u2[28]='a';
    u2[29]='/'; u2[30]='i'; u2[31]='t'; u2[32]='e'; u2[33]='m'; u2[34]='s';
    u2[35]=0;
    parse_url(&u, u2, 35);
    printf("Valid: %d\n", u.valid);       // EXPECT: Valid: 1
    printf("Scheme: %s\n", u.scheme);     // EXPECT: Scheme: https
    printf("Host: %s\n", u.host);         // EXPECT: Host: api.test.io
    printf("Port: %s\n", u.port);         // EXPECT: Port: 8080
    printf("Path: %s\n", u.path);         // EXPECT: Path: /data/items
    int port_num = to_int(u.port, u.port_len);
    printf("Port num: %d\n", port_num);   // EXPECT: Port num: 8080

    // Test 3: ftp://files.net/pub
    char u3[32];
    u3[0]='f'; u3[1]='t'; u3[2]='p';
    u3[3]=':'; u3[4]='/'; u3[5]='/';
    u3[6]='f'; u3[7]='i'; u3[8]='l'; u3[9]='e'; u3[10]='s';
    u3[11]='.'; u3[12]='n'; u3[13]='e'; u3[14]='t';
    u3[15]='/'; u3[16]='p'; u3[17]='u'; u3[18]='b';
    u3[19]=0;
    parse_url(&u, u3, 19);
    printf("Valid: %d\n", u.valid);       // EXPECT: Valid: 1
    printf("Scheme: %s\n", u.scheme);     // EXPECT: Scheme: ftp
    printf("Host: %s\n", u.host);         // EXPECT: Host: files.net
    printf("Path: %s\n", u.path);         // EXPECT: Path: /pub

    // Test 4: Invalid URL
    char u4[8];
    u4[0]='@'; u4[1]='b'; u4[2]='a'; u4[3]='d'; u4[4]=0;
    parse_url(&u, u4, 4);
    printf("Valid: %d\n", u.valid);       // EXPECT: Valid: 0

    // Test 5: http://localhost:3000/
    char u5[32];
    u5[0]='h'; u5[1]='t'; u5[2]='t'; u5[3]='p';
    u5[4]=':'; u5[5]='/'; u5[6]='/';
    u5[7]='l'; u5[8]='o'; u5[9]='c'; u5[10]='a'; u5[11]='l';
    u5[12]='h'; u5[13]='o'; u5[14]='s'; u5[15]='t';
    u5[16]=':'; u5[17]='3'; u5[18]='0'; u5[19]='0'; u5[20]='0';
    u5[21]='/'; u5[22]=0;
    parse_url(&u, u5, 22);
    printf("Valid: %d\n", u.valid);       // EXPECT: Valid: 1
    printf("Host: %s\n", u.host);         // EXPECT: Host: localhost
    printf("Port: %s\n", u.port);         // EXPECT: Port: 3000
    printf("Path: %s\n", u.path);         // EXPECT: Path: /

    printf("Done\n"); // EXPECT: Done
    return 0;
}
