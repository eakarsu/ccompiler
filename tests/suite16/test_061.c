int printf(const char *fmt, ...);

// HTTP Request Line Parser
// Parses method, path, and version from an HTTP request line

enum http_state {
    HTTP_METHOD,
    HTTP_SPACE1,
    HTTP_PATH,
    HTTP_SPACE2,
    HTTP_VERSION,
    HTTP_DONE,
    HTTP_ERROR
};

int is_alpha(int c) {
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

struct http_request {
    char method[16];
    char path[64];
    char version[16];
    int method_len;
    int path_len;
    int version_len;
    int state;
    int valid;
};

void http_init(struct http_request *req) {
    req->method_len = 0;
    req->path_len = 0;
    req->version_len = 0;
    req->state = HTTP_METHOD;
    req->valid = 0;
    int i;
    for (i = 0; i < 16; i++) {
        req->method[i] = 0;
        req->version[i] = 0;
    }
    for (i = 0; i < 64; i++) {
        req->path[i] = 0;
    }
}

void http_feed(struct http_request *req, char c) {
    int st = req->state;
    if (st == HTTP_METHOD) {
        if (is_alpha(c)) {
            if (req->method_len < 15) {
                req->method[req->method_len] = c;
                req->method_len = req->method_len + 1;
            }
        } else if (c == 32) {
            req->state = HTTP_SPACE1;
        } else {
            req->state = HTTP_ERROR;
        }
    } else if (st == HTTP_SPACE1) {
        if (c == 47 || is_alpha(c) || c == 46) {
            req->path[req->path_len] = c;
            req->path_len = req->path_len + 1;
            req->state = HTTP_PATH;
        } else {
            req->state = HTTP_ERROR;
        }
    } else if (st == HTTP_PATH) {
        if (c == 32) {
            req->state = HTTP_SPACE2;
        } else if (c > 32 && c < 127) {
            if (req->path_len < 63) {
                req->path[req->path_len] = c;
                req->path_len = req->path_len + 1;
            }
        } else {
            req->state = HTTP_ERROR;
        }
    } else if (st == HTTP_SPACE2) {
        if (c == 72) {
            req->version[req->version_len] = c;
            req->version_len = req->version_len + 1;
            req->state = HTTP_VERSION;
        } else {
            req->state = HTTP_ERROR;
        }
    } else if (st == HTTP_VERSION) {
        if (c == 13 || c == 10 || c == 0) {
            req->state = HTTP_DONE;
            req->valid = 1;
        } else {
            if (req->version_len < 15) {
                req->version[req->version_len] = c;
                req->version_len = req->version_len + 1;
            }
        }
    }
}

void parse_line(struct http_request *req, char *line, int len) {
    http_init(req);
    int i;
    for (i = 0; i < len; i++) {
        http_feed(req, line[i]);
    }
    if (req->state == HTTP_VERSION) {
        req->state = HTTP_DONE;
        req->valid = 1;
    }
}

int main() {
    struct http_request req;

    printf("=== HTTP Parser ===\n"); // EXPECT: === HTTP Parser ===

    // Test 1: GET / HTTP/1.1
    char r1[32];
    r1[0]='G'; r1[1]='E'; r1[2]='T'; r1[3]=' ';
    r1[4]='/'; r1[5]=' ';
    r1[6]='H'; r1[7]='T'; r1[8]='T'; r1[9]='P';
    r1[10]='/'; r1[11]='1'; r1[12]='.'; r1[13]='1';
    r1[14]=0;
    parse_line(&req, r1, 14);
    printf("Valid: %d\n", req.valid);       // EXPECT: Valid: 1
    printf("Method: %s\n", req.method);     // EXPECT: Method: GET
    printf("Path: %s\n", req.path);         // EXPECT: Path: /
    printf("Version: %s\n", req.version);   // EXPECT: Version: HTTP/1.1

    // Test 2: POST /api/data HTTP/2.0
    char r2[64];
    r2[0]='P'; r2[1]='O'; r2[2]='S'; r2[3]='T';
    r2[4]=' '; r2[5]='/'; r2[6]='a'; r2[7]='p';
    r2[8]='i'; r2[9]='/'; r2[10]='d'; r2[11]='a';
    r2[12]='t'; r2[13]='a'; r2[14]=' ';
    r2[15]='H'; r2[16]='T'; r2[17]='T'; r2[18]='P';
    r2[19]='/'; r2[20]='2'; r2[21]='.'; r2[22]='0';
    r2[23]=0;
    parse_line(&req, r2, 23);
    printf("Valid: %d\n", req.valid);       // EXPECT: Valid: 1
    printf("Method: %s\n", req.method);     // EXPECT: Method: POST
    printf("Path: %s\n", req.path);         // EXPECT: Path: /api/data
    printf("Version: %s\n", req.version);   // EXPECT: Version: HTTP/2.0

    // Test 3: Invalid request
    char r3[16];
    r3[0]='B'; r3[1]='A'; r3[2]='D'; r3[3]=1; r3[4]=0;
    parse_line(&req, r3, 4);
    printf("Valid: %d\n", req.valid);       // EXPECT: Valid: 0
    printf("State: %d\n", req.state);       // EXPECT: State: 6

    // Test 4: DELETE /item HTTP/1.0
    char r4[32];
    r4[0]='D'; r4[1]='E'; r4[2]='L'; r4[3]='E'; r4[4]='T'; r4[5]='E';
    r4[6]=' '; r4[7]='/'; r4[8]='i'; r4[9]='t'; r4[10]='e'; r4[11]='m';
    r4[12]=' '; r4[13]='H'; r4[14]='T'; r4[15]='T'; r4[16]='P';
    r4[17]='/'; r4[18]='1'; r4[19]='.'; r4[20]='0';
    r4[21]=0;
    parse_line(&req, r4, 21);
    printf("Valid: %d\n", req.valid);       // EXPECT: Valid: 1
    printf("Method: %s\n", req.method);     // EXPECT: Method: DELETE
    printf("Path: %s\n", req.path);         // EXPECT: Path: /item
    printf("Version: %s\n", req.version);   // EXPECT: Version: HTTP/1.0

    printf("State codes: %d %d %d\n", HTTP_METHOD, HTTP_DONE, HTTP_ERROR); // EXPECT: State codes: 0 5 6

    return 0;
}
