int printf(const char *fmt, ...);

enum Permission {
    PERM_NONE    = 0,
    PERM_READ    = 1,
    PERM_WRITE   = 2,
    PERM_EXEC    = 4,
    PERM_DELETE  = 8,
    PERM_ADMIN   = 16
};

enum FileAttr {
    ATTR_NONE     = 0,
    ATTR_HIDDEN   = 1,
    ATTR_READONLY = 2,
    ATTR_SYSTEM   = 4,
    ATTR_ARCHIVE  = 8
};

int has_flag(int flags, int flag) {
    return (flags & flag) != 0;
}

int set_flag(int flags, int flag) {
    return flags | flag;
}

int clear_flag(int flags, int flag) {
    return flags & (~flag);
}

int toggle_flag(int flags, int flag) {
    return flags ^ flag;
}

void test_basic_flags() {
    int perms = PERM_NONE;
    perms = set_flag(perms, PERM_READ);
    perms = set_flag(perms, PERM_WRITE);
    // EXPECT: perms=3
    printf("perms=%d\n", perms);
    // EXPECT: has_read=1
    printf("has_read=%d\n", has_flag(perms, PERM_READ));
    // EXPECT: has_write=1
    printf("has_write=%d\n", has_flag(perms, PERM_WRITE));
    // EXPECT: has_exec=0
    printf("has_exec=%d\n", has_flag(perms, PERM_EXEC));
}

void test_set_clear() {
    int perms = PERM_READ | PERM_WRITE | PERM_EXEC;
    // EXPECT: initial=7
    printf("initial=%d\n", perms);

    perms = clear_flag(perms, PERM_WRITE);
    // EXPECT: after_clear_write=5
    printf("after_clear_write=%d\n", perms);

    // EXPECT: still_read=1
    printf("still_read=%d\n", has_flag(perms, PERM_READ));
    // EXPECT: still_write=0
    printf("still_write=%d\n", has_flag(perms, PERM_WRITE));
    // EXPECT: still_exec=1
    printf("still_exec=%d\n", has_flag(perms, PERM_EXEC));
}

void test_toggle() {
    int flags = PERM_READ;
    flags = toggle_flag(flags, PERM_WRITE);
    // EXPECT: after_toggle_on=3
    printf("after_toggle_on=%d\n", flags);

    flags = toggle_flag(flags, PERM_READ);
    // EXPECT: after_toggle_off=2
    printf("after_toggle_off=%d\n", flags);

    flags = toggle_flag(flags, PERM_READ);
    // EXPECT: after_toggle_back=3
    printf("after_toggle_back=%d\n", flags);
}

void test_combine() {
    int admin_perms = PERM_READ | PERM_WRITE | PERM_EXEC | PERM_DELETE | PERM_ADMIN;
    // EXPECT: admin_perms=31
    printf("admin_perms=%d\n", admin_perms);

    int user_perms = PERM_READ | PERM_EXEC;
    // EXPECT: user_perms=5
    printf("user_perms=%d\n", user_perms);

    int common = admin_perms & user_perms;
    // EXPECT: common=5
    printf("common=%d\n", common);

    int diff = admin_perms ^ user_perms;
    // EXPECT: diff=26
    printf("diff=%d\n", diff);
}

void test_file_attrs() {
    int attrs = ATTR_HIDDEN | ATTR_ARCHIVE;
    // EXPECT: attrs=9
    printf("attrs=%d\n", attrs);
    // EXPECT: is_hidden=1
    printf("is_hidden=%d\n", has_flag(attrs, ATTR_HIDDEN));
    // EXPECT: is_readonly=0
    printf("is_readonly=%d\n", has_flag(attrs, ATTR_READONLY));
    // EXPECT: is_archive=1
    printf("is_archive=%d\n", has_flag(attrs, ATTR_ARCHIVE));
}

void print_perms(int perms) {
    int r = has_flag(perms, PERM_READ);
    int w = has_flag(perms, PERM_WRITE);
    int x = has_flag(perms, PERM_EXEC);
    // EXPECT: rwx: 1 0 1
    // EXPECT: rwx: 1 1 0
    // EXPECT: rwx: 0 0 0
    printf("rwx: %d %d %d\n", r, w, x);
}

void test_permission_array() {
    int users[3];
    users[0] = PERM_READ | PERM_EXEC;
    users[1] = PERM_READ | PERM_WRITE;
    users[2] = PERM_NONE;

    int i;
    for (i = 0; i < 3; i++) {
        print_perms(users[i]);
    }
}

void test_count_flags() {
    int flags = PERM_READ | PERM_WRITE | PERM_EXEC | PERM_ADMIN;
    int count = 0;
    int bit;
    for (bit = 1; bit <= 16; bit = bit * 2) {
        if (has_flag(flags, bit)) {
            count = count + 1;
        }
    }
    // EXPECT: flag_count=4
    printf("flag_count=%d\n", count);
}

int main() {
    test_basic_flags();
    test_set_clear();
    test_toggle();
    test_combine();
    test_file_attrs();
    test_permission_array();
    test_count_flags();
    // EXPECT: done
    printf("done\n");
    return 0;
}
