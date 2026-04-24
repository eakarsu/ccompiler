int printf(const char *fmt, ...);

/* Enum as bit flags for a permission system */
enum Permission {
    PERM_NONE    = 0,
    PERM_READ    = 1,
    PERM_WRITE   = 2,
    PERM_EXEC    = 4,
    PERM_DELETE  = 8,
    PERM_ADMIN   = 16
};

/* Combine flags */
int combine_flags(int a, int b) {
    return a | b;
}

/* Test if a specific flag is set */
int has_flag(int flags, int flag) {
    return (flags & flag) != 0;
}

/* Clear a specific flag */
int clear_flag(int flags, int flag) {
    return flags & ~flag;
}

/* Toggle a flag */
int toggle_flag(int flags, int flag) {
    return flags ^ flag;
}

/* Count number of set flags */
int count_flags(int flags) {
    int count = 0;
    int bit = 1;
    while (bit <= 16) {
        if (flags & bit) count++;
        bit = bit * 2;
    }
    return count;
}

/* Get permission string for a single permission */
char *perm_name(int perm) {
    if (perm == PERM_READ) return "READ";
    if (perm == PERM_WRITE) return "WRITE";
    if (perm == PERM_EXEC) return "EXEC";
    if (perm == PERM_DELETE) return "DELETE";
    if (perm == PERM_ADMIN) return "ADMIN";
    return "NONE";
}

/* Role-based permissions */
int get_role_perms(int role) {
    /* 0=guest, 1=user, 2=editor, 3=admin */
    if (role == 0) return PERM_READ;
    if (role == 1) return PERM_READ | PERM_WRITE;
    if (role == 2) return PERM_READ | PERM_WRITE | PERM_DELETE;
    if (role == 3) return PERM_READ | PERM_WRITE | PERM_EXEC | PERM_DELETE | PERM_ADMIN;
    return PERM_NONE;
}

/* Check if user can perform action */
int check_access(int user_perms, int required) {
    return (user_perms & required) == required;
}

/* Find highest permission bit */
int highest_perm(int flags) {
    int highest = 0;
    int bit = 1;
    while (bit <= 16) {
        if (flags & bit) highest = bit;
        bit = bit * 2;
    }
    return highest;
}

int main(void) {
    /* Test combining flags */
    int perms = combine_flags(PERM_READ, PERM_WRITE);
    // EXPECT: Combined: 3
    printf("Combined: %d\n", perms);

    perms = combine_flags(perms, PERM_EXEC);
    // EXPECT: With exec: 7
    printf("With exec: %d\n", perms);

    /* Test has_flag */
    // EXPECT: Has READ: 1
    printf("Has READ: %d\n", has_flag(perms, PERM_READ));
    // EXPECT: Has WRITE: 1
    printf("Has WRITE: %d\n", has_flag(perms, PERM_WRITE));
    // EXPECT: Has DELETE: 0
    printf("Has DELETE: %d\n", has_flag(perms, PERM_DELETE));

    /* Test clear_flag */
    int cleared = clear_flag(perms, PERM_WRITE);
    // EXPECT: After clear WRITE: 5
    printf("After clear WRITE: %d\n", cleared);
    // EXPECT: Still has READ: 1
    printf("Still has READ: %d\n", has_flag(cleared, PERM_READ));
    // EXPECT: Lost WRITE: 0
    printf("Lost WRITE: %d\n", has_flag(cleared, PERM_WRITE));

    /* Test toggle_flag */
    int toggled = toggle_flag(perms, PERM_DELETE);
    // EXPECT: Toggle on DELETE: 15
    printf("Toggle on DELETE: %d\n", toggled);
    toggled = toggle_flag(toggled, PERM_DELETE);
    // EXPECT: Toggle off DELETE: 7
    printf("Toggle off DELETE: %d\n", toggled);

    /* Test count_flags */
    // EXPECT: Count(7): 3
    printf("Count(7): %d\n", count_flags(7));
    // EXPECT: Count(0): 0
    printf("Count(0): %d\n", count_flags(0));
    // EXPECT: Count(31): 5
    printf("Count(31): %d\n", count_flags(31));

    /* Test role permissions */
    int guest = get_role_perms(0);
    int user = get_role_perms(1);
    int editor = get_role_perms(2);
    int admin = get_role_perms(3);

    // EXPECT: Guest perms: 1
    printf("Guest perms: %d\n", guest);
    // EXPECT: User perms: 3
    printf("User perms: %d\n", user);
    // EXPECT: Editor perms: 11
    printf("Editor perms: %d\n", editor);
    // EXPECT: Admin perms: 31
    printf("Admin perms: %d\n", admin);

    /* Test check_access */
    // EXPECT: Guest read: 1
    printf("Guest read: %d\n", check_access(guest, PERM_READ));
    // EXPECT: Guest write: 0
    printf("Guest write: %d\n", check_access(guest, PERM_WRITE));
    // EXPECT: User read+write: 1
    printf("User read+write: %d\n", check_access(user, PERM_READ | PERM_WRITE));
    // EXPECT: Editor delete: 1
    printf("Editor delete: %d\n", check_access(editor, PERM_DELETE));
    // EXPECT: Editor exec: 0
    printf("Editor exec: %d\n", check_access(editor, PERM_EXEC));
    // EXPECT: Admin all: 1
    printf("Admin all: %d\n", check_access(admin, PERM_READ | PERM_WRITE | PERM_EXEC | PERM_DELETE | PERM_ADMIN));

    /* Test highest permission */
    // EXPECT: Highest(7): 4
    printf("Highest(7): %d\n", highest_perm(7));
    // EXPECT: Highest(31): 16
    printf("Highest(31): %d\n", highest_perm(31));
    // EXPECT: Highest(1): 1
    printf("Highest(1): %d\n", highest_perm(1));

    /* Print all set permissions for admin */
    int bit;
    int pcount = 0;
    for (bit = 1; bit <= 16; bit = bit * 2) {
        if (has_flag(admin, bit)) {
            printf("Admin has: %s\n", perm_name(bit));
            pcount++;
        }
    }
    // EXPECT: Admin has: READ
    // EXPECT: Admin has: WRITE
    // EXPECT: Admin has: EXEC
    // EXPECT: Admin has: DELETE
    // EXPECT: Admin has: ADMIN
    // EXPECT: Admin total: 5
    printf("Admin total: %d\n", pcount);

    return 0;
}
