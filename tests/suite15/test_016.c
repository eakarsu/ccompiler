int printf(const char *fmt, ...);

// HTTP request parser - method, path, version

enum HttpMethod {
    METHOD_UNKNOWN,
    METHOD_GET,
    METHOD_POST,
    METHOD_PUT,
    METHOD_DELETE,
    METHOD_HEAD
};

typedef struct {
    int method;
    char path[64];
    int version_major;
    int version_minor;
    int content_length;
    int valid;
} HttpRequest;

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int starts_with(char *str, char *prefix) {
    int i = 0;
    while (prefix[i] != 0) {
        if (str[i] != prefix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len = len + 1;
    return len;
}

void str_copy(char *dst, char *src, int max) {
    int i = 0;
    while (src[i] != 0 && i < max - 1) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

// Copy n characters from src starting at offset into dst
void str_ncopy(char *dst, char *src, int off, int n) {
    int i;
    for (i = 0; i < n && src[off + i] != 0; i++) {
        dst[i] = src[off + i];
    }
    dst[i] = 0;
}

int parse_method(char *line, int *end_pos) {
    int i = 0;
    char word[16];
    while (line[i] != ' ' && line[i] != 0 && i < 15) {
        word[i] = line[i];
        i = i + 1;
    }
    word[i] = 0;
    *end_pos = i;

    if (str_eq(word, "GET")) return METHOD_GET;
    if (str_eq(word, "POST")) return METHOD_POST;
    if (str_eq(word, "PUT")) return METHOD_PUT;
    if (str_eq(word, "DELETE")) return METHOD_DELETE;
    if (str_eq(word, "HEAD")) return METHOD_HEAD;
    return METHOD_UNKNOWN;
}

void parse_path(char *line, int start, HttpRequest *req) {
    // Skip spaces
    while (line[start] == ' ') start = start + 1;
    int i = 0;
    while (line[start] != ' ' && line[start] != 0 && i < 63) {
        req->path[i] = line[start];
        i = i + 1;
        start = start + 1;
    }
    req->path[i] = 0;
}

void parse_version(char *line, int start, HttpRequest *req) {
    while (line[start] == ' ') start = start + 1;
    // Expect "HTTP/x.y"
    if (starts_with(&line[start], "HTTP/")) {
        req->version_major = line[start + 5] - '0';
        if (line[start + 6] == '.') {
            req->version_minor = line[start + 7] - '0';
        }
    }
}

void parse_request(char *line, HttpRequest *req) {
    int end_pos;
    req->method = parse_method(line, &end_pos);
    parse_path(line, end_pos, req);
    // Find the version part after the path
    int i = end_pos;
    // Skip past path
    while (line[i] == ' ') i = i + 1;
    while (line[i] != ' ' && line[i] != 0) i = i + 1;
    parse_version(line, i, req);
    req->content_length = 0;
    req->valid = 1;
}

char *method_name(int m) {
    if (m == METHOD_GET) return "GET";
    if (m == METHOD_POST) return "POST";
    if (m == METHOD_PUT) return "PUT";
    if (m == METHOD_DELETE) return "DELETE";
    if (m == METHOD_HEAD) return "HEAD";
    return "UNKNOWN";
}

// Count path depth (number of '/' segments)
int path_depth(char *path) {
    int count = 0;
    int i = 0;
    while (path[i] != 0) {
        if (path[i] == '/') count = count + 1;
        i = i + 1;
    }
    return count;
}

// Check if path has a query string (contains '?')
int has_query(char *path) {
    int i = 0;
    while (path[i] != 0) {
        if (path[i] == '?') return 1;
        i = i + 1;
    }
    return 0;
}

// Extract query string portion after '?' into dst
void extract_query(char *path, char *dst, int max) {
    int i = 0;
    while (path[i] != 0 && path[i] != '?') i = i + 1;
    if (path[i] == '?') i = i + 1;
    int j = 0;
    while (path[i] != 0 && j < max - 1) {
        dst[j] = path[i];
        j = j + 1;
        i = i + 1;
    }
    dst[j] = 0;
}

int main(void) {
    HttpRequest req;

    // Parse GET request
    parse_request("GET /index.html HTTP/1.1", &req);
    printf("method=%s\n", method_name(req.method));
    // EXPECT: method=GET

    printf("path=%s\n", req.path);
    // EXPECT: path=/index.html

    printf("version=%d.%d\n", req.version_major, req.version_minor);
    // EXPECT: version=1.1

    // Parse POST request
    parse_request("POST /api/users HTTP/1.0", &req);
    printf("method=%s\n", method_name(req.method));
    // EXPECT: method=POST

    printf("path=%s\n", req.path);
    // EXPECT: path=/api/users

    printf("version=%d.%d\n", req.version_major, req.version_minor);
    // EXPECT: version=1.0

    // Parse DELETE request
    parse_request("DELETE /api/item/42 HTTP/1.1", &req);
    printf("method=%s\n", method_name(req.method));
    // EXPECT: method=DELETE

    printf("path=%s\n", req.path);
    // EXPECT: path=/api/item/42

    // Parse PUT
    parse_request("PUT /data HTTP/1.1", &req);
    printf("method=%s\n", method_name(req.method));
    // EXPECT: method=PUT

    // Parse HEAD
    parse_request("HEAD / HTTP/1.1", &req);
    printf("method=%s\n", method_name(req.method));
    // EXPECT: method=HEAD

    printf("path=%s\n", req.path);
    // EXPECT: path=/

    // Path depth tests
    printf("depth=%d\n", path_depth("/"));
    // EXPECT: depth=1

    printf("depth=%d\n", path_depth("/api/users"));
    // EXPECT: depth=2

    printf("depth=%d\n", path_depth("/a/b/c/d"));
    // EXPECT: depth=4

    // Query string tests
    parse_request("GET /search?q=hello HTTP/1.1", &req);
    printf("has_q=%d\n", has_query(req.path));
    // EXPECT: has_q=1

    char qbuf[32];
    extract_query(req.path, qbuf, 32);
    printf("query=%s\n", qbuf);
    // EXPECT: query=q=hello

    parse_request("GET /plain HTTP/1.1", &req);
    printf("has_q=%d\n", has_query(req.path));
    // EXPECT: has_q=0

    // String length tests
    printf("len=%d\n", str_len("GET"));
    // EXPECT: len=3

    printf("len=%d\n", str_len("/api/v2/users"));
    // EXPECT: len=13

    printf("http done\n");
    // EXPECT: http done

    return 0;
}
