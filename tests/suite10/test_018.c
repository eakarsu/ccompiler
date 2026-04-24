int printf(const char *fmt, ...);

// HTTP Request Parser

struct HTTPHeader {
    char name[32];
    char value[64];
};

struct HTTPRequest {
    char method[8];
    char path[64];
    int version_major;
    int version_minor;
    struct HTTPHeader headers[8];
    int header_count;
    int content_length;
    int status_code;
};

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = '\0';
}

int str_len(char *s) {
    int len = 0;
    while (s[len] != '\0') len = len + 1;
    return len;
}

int starts_with(char *str, char *prefix) {
    int i = 0;
    while (prefix[i] != '\0') {
        if (str[i] != prefix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

void init_request(struct HTTPRequest *req) {
    req->header_count = 0;
    req->content_length = 0;
    req->status_code = 0;
    req->version_major = 1;
    req->version_minor = 1;
    str_copy(req->method, "");
    str_copy(req->path, "");
}

void set_method(struct HTTPRequest *req, char *method) {
    str_copy(req->method, method);
}

void set_path(struct HTTPRequest *req, char *path) {
    str_copy(req->path, path);
}

void add_header(struct HTTPRequest *req, char *name, char *value) {
    if (req->header_count >= 8) return;
    int idx = req->header_count;
    str_copy(req->headers[idx].name, name);
    str_copy(req->headers[idx].value, value);
    req->header_count = req->header_count + 1;
}

char *find_header(struct HTTPRequest *req, char *name) {
    int i;
    for (i = 0; i < req->header_count; i++) {
        if (str_eq(req->headers[i].name, name)) {
            return req->headers[i].value;
        }
    }
    return "";
}

int is_valid_method(char *method) {
    if (str_eq(method, "GET")) return 1;
    if (str_eq(method, "POST")) return 1;
    if (str_eq(method, "PUT")) return 1;
    if (str_eq(method, "DELETE")) return 1;
    if (str_eq(method, "HEAD")) return 1;
    if (str_eq(method, "OPTIONS")) return 1;
    if (str_eq(method, "PATCH")) return 1;
    return 0;
}

int is_secure_path(char *path) {
    // Check for directory traversal
    int i = 0;
    while (path[i] != '\0' && path[i + 1] != '\0') {
        if (path[i] == '.' && path[i + 1] == '.') return 0;
        i = i + 1;
    }
    // Must start with /
    if (path[0] != '/') return 0;
    return 1;
}

int count_path_segments(char *path) {
    int count = 0;
    int i = 0;
    while (path[i] != '\0') {
        if (path[i] == '/' && path[i + 1] != '\0' && path[i + 1] != '/') {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

char *status_text(int code) {
    if (code == 200) return "OK";
    if (code == 201) return "Created";
    if (code == 301) return "Moved Permanently";
    if (code == 302) return "Found";
    if (code == 400) return "Bad Request";
    if (code == 401) return "Unauthorized";
    if (code == 403) return "Forbidden";
    if (code == 404) return "Not Found";
    if (code == 500) return "Internal Server Error";
    return "Unknown";
}

int has_query_string(char *path) {
    int i = 0;
    while (path[i] != '\0') {
        if (path[i] == '?') return 1;
        i = i + 1;
    }
    return 0;
}

int main() {
    struct HTTPRequest req;

    // Test 1: Build GET request
    init_request(&req);
    set_method(&req, "GET");
    set_path(&req, "/index.html");
    add_header(&req, "Host", "example.com");
    add_header(&req, "User-Agent", "TestBot");
    add_header(&req, "Accept", "text/html");

    printf("method: %s\n", req.method);
    // EXPECT: method: GET
    printf("path: %s\n", req.path);
    // EXPECT: path: /index.html
    printf("headers: %d\n", req.header_count);
    // EXPECT: headers: 3

    // Test 2: Find headers
    printf("Host: %s\n", find_header(&req, "Host"));
    // EXPECT: Host: example.com
    printf("User-Agent: %s\n", find_header(&req, "User-Agent"));
    // EXPECT: User-Agent: TestBot
    printf("Accept: %s\n", find_header(&req, "Accept"));
    // EXPECT: Accept: text/html
    printf("Missing:%s\n", find_header(&req, "Cookie"));
    // EXPECT: Missing:

    // Test 3: Valid methods
    printf("GET valid: %d\n", is_valid_method("GET"));
    // EXPECT: GET valid: 1
    printf("POST valid: %d\n", is_valid_method("POST"));
    // EXPECT: POST valid: 1
    printf("INVALID valid: %d\n", is_valid_method("INVALID"));
    // EXPECT: INVALID valid: 0

    // Test 4: Path security
    printf("/index.html secure: %d\n", is_secure_path("/index.html"));
    // EXPECT: /index.html secure: 1
    printf("/../etc/passwd secure: %d\n", is_secure_path("/../etc/passwd"));
    // EXPECT: /../etc/passwd secure: 0
    printf("relative secure: %d\n", is_secure_path("relative"));
    // EXPECT: relative secure: 0

    // Test 5: Path segments
    printf("segments /: %d\n", count_path_segments("/"));
    // EXPECT: segments /: 0
    printf("segments /a/b/c: %d\n", count_path_segments("/a/b/c"));
    // EXPECT: segments /a/b/c: 3
    printf("segments /api/v2/users: %d\n", count_path_segments("/api/v2/users"));
    // EXPECT: segments /api/v2/users: 3

    // Test 6: Status text
    printf("200: %s\n", status_text(200));
    // EXPECT: 200: OK
    printf("404: %s\n", status_text(404));
    // EXPECT: 404: Not Found
    printf("500: %s\n", status_text(500));
    // EXPECT: 500: Internal Server Error
    printf("301: %s\n", status_text(301));
    // EXPECT: 301: Moved Permanently

    // Test 7: POST request with content-length
    init_request(&req);
    set_method(&req, "POST");
    set_path(&req, "/api/data");
    add_header(&req, "Content-Type", "application/json");
    add_header(&req, "Content-Length", "42");
    req.content_length = 42;
    printf("POST path: %s\n", req.path);
    // EXPECT: POST path: /api/data
    printf("Content-Type: %s\n", find_header(&req, "Content-Type"));
    // EXPECT: Content-Type: application/json
    printf("content_length: %d\n", req.content_length);
    // EXPECT: content_length: 42

    // Test 8: query string detection
    printf("has query /search?q=test: %d\n", has_query_string("/search?q=test"));
    // EXPECT: has query /search?q=test: 1
    printf("has query /index.html: %d\n", has_query_string("/index.html"));
    // EXPECT: has query /index.html: 0

    // Test 9: starts_with
    printf("starts GET: %d\n", starts_with("GET /index", "GET"));
    // EXPECT: starts GET: 1
    printf("starts POST: %d\n", starts_with("GET /index", "POST"));
    // EXPECT: starts POST: 0

    return 0;
}
