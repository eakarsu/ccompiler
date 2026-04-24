int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void to_upper(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        if (src[i] >= 'a' && src[i] <= 'z') {
            dst[i] = src[i] - 32;
        } else {
            dst[i] = src[i];
        }
        i = i + 1;
    }
    dst[i] = 0;
}

void to_lower(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        if (src[i] >= 'A' && src[i] <= 'Z') {
            dst[i] = src[i] + 32;
        } else {
            dst[i] = src[i];
        }
        i = i + 1;
    }
    dst[i] = 0;
}

void swap_case(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        if (src[i] >= 'a' && src[i] <= 'z') {
            dst[i] = src[i] - 32;
        } else if (src[i] >= 'A' && src[i] <= 'Z') {
            dst[i] = src[i] + 32;
        } else {
            dst[i] = src[i];
        }
        i = i + 1;
    }
    dst[i] = 0;
}

void capitalize_first(char *dst, char *src) {
    my_strcpy(dst, src);
    if (dst[0] >= 'a' && dst[0] <= 'z') {
        dst[0] = dst[0] - 32;
    }
}

void title_case(char *dst, char *src) {
    my_strcpy(dst, src);
    int capitalize_next = 1;
    int i = 0;
    while (dst[i] != 0) {
        if (dst[i] == ' ') {
            capitalize_next = 1;
        } else {
            if (capitalize_next) {
                if (dst[i] >= 'a' && dst[i] <= 'z') {
                    dst[i] = dst[i] - 32;
                }
                capitalize_next = 0;
            } else {
                if (dst[i] >= 'A' && dst[i] <= 'Z') {
                    dst[i] = dst[i] + 32;
                }
            }
        }
        i = i + 1;
    }
}

void camel_to_snake(char *dst, char *src) {
    int j = 0;
    int i = 0;
    while (src[i] != 0) {
        if (src[i] >= 'A' && src[i] <= 'Z') {
            if (i > 0) {
                dst[j] = '_';
                j = j + 1;
            }
            dst[j] = src[i] + 32;
            j = j + 1;
        } else {
            dst[j] = src[i];
            j = j + 1;
        }
        i = i + 1;
    }
    dst[j] = 0;
}

void snake_to_camel(char *dst, char *src) {
    int j = 0;
    int capitalize_next = 0;
    int i = 0;
    while (src[i] != 0) {
        if (src[i] == '_') {
            capitalize_next = 1;
        } else {
            if (capitalize_next) {
                if (src[i] >= 'a' && src[i] <= 'z') {
                    dst[j] = src[i] - 32;
                } else {
                    dst[j] = src[i];
                }
                capitalize_next = 0;
            } else {
                dst[j] = src[i];
            }
            j = j + 1;
        }
        i = i + 1;
    }
    dst[j] = 0;
}

int is_all_upper(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 'a' && s[i] <= 'z') return 0;
        i = i + 1;
    }
    return 1;
}

int is_all_lower(char *s) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 'A' && s[i] <= 'Z') return 0;
        i = i + 1;
    }
    return 1;
}

int main() {
    char buf[128];

    to_upper(buf, "hello");
    // EXPECT: upper hello = HELLO
    printf("upper hello = %s\n", buf);

    to_upper(buf, "Hello World");
    // EXPECT: upper Hello World = HELLO WORLD
    printf("upper Hello World = %s\n", buf);

    to_lower(buf, "HELLO");
    // EXPECT: lower HELLO = hello
    printf("lower HELLO = %s\n", buf);

    to_lower(buf, "Hello World");
    // EXPECT: lower Hello World = hello world
    printf("lower Hello World = %s\n", buf);

    swap_case(buf, "Hello World");
    // EXPECT: swap Hello World = hELLO wORLD
    printf("swap Hello World = %s\n", buf);

    swap_case(buf, "aBcDeF");
    // EXPECT: swap aBcDeF = AbCdEf
    printf("swap aBcDeF = %s\n", buf);

    capitalize_first(buf, "hello");
    // EXPECT: cap hello = Hello
    printf("cap hello = %s\n", buf);

    capitalize_first(buf, "world");
    // EXPECT: cap world = World
    printf("cap world = %s\n", buf);

    title_case(buf, "hello world foo");
    // EXPECT: title = Hello World Foo
    printf("title = %s\n", buf);

    title_case(buf, "the quick brown fox");
    // EXPECT: title2 = The Quick Brown Fox
    printf("title2 = %s\n", buf);

    camel_to_snake(buf, "helloWorld");
    // EXPECT: c2s helloWorld = hello_world
    printf("c2s helloWorld = %s\n", buf);

    camel_to_snake(buf, "myVariableName");
    // EXPECT: c2s myVariableName = my_variable_name
    printf("c2s myVariableName = %s\n", buf);

    snake_to_camel(buf, "hello_world");
    // EXPECT: s2c hello_world = helloWorld
    printf("s2c hello_world = %s\n", buf);

    snake_to_camel(buf, "my_variable_name");
    // EXPECT: s2c my_variable_name = myVariableName
    printf("s2c my_variable_name = %s\n", buf);

    // EXPECT: all_upper HELLO = 1
    printf("all_upper HELLO = %d\n", is_all_upper("HELLO"));
    // EXPECT: all_upper Hello = 0
    printf("all_upper Hello = %d\n", is_all_upper("Hello"));
    // EXPECT: all_lower hello = 1
    printf("all_lower hello = %d\n", is_all_lower("hello"));
    // EXPECT: all_lower Hello = 0
    printf("all_lower Hello = %d\n", is_all_lower("Hello"));

    return 0;
}
