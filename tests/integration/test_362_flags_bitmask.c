int printf(const char *fmt, ...);
// EXPECT: user: [RW---] = 3\nadmin: [RWXDA] = 31\nuser can read: 1\nuser can execute: 0\nadmin can delete: 1\nadmin without delete: [RWX-A]\nuser toggle execute: [RWX--]\nuser toggle again: [RW---]\ncommon: [RW---]\ncombined: [RWXDA]\nadmin permission count: 5\nuser0: [R----]\nuser1: [RW---]\nuser2: [RWX--]
// Test: flags and bitmask systems

enum {
    PERM_READ    = 1,
    PERM_WRITE   = 2,
    PERM_EXECUTE = 4,
    PERM_DELETE  = 8,
    PERM_ADMIN   = 16
};

int has_permission(int flags, int perm) {
    return (flags & perm) != 0;
}

int add_permission(int flags, int perm) {
    return flags | perm;
}

int remove_permission(int flags, int perm) {
    return flags & ~perm;
}

int toggle_permission(int flags, int perm) {
    return flags ^ perm;
}

void print_permissions(int flags) {
    printf("[");
    if (has_permission(flags, PERM_READ)) printf("R");
    else printf("-");
    if (has_permission(flags, PERM_WRITE)) printf("W");
    else printf("-");
    if (has_permission(flags, PERM_EXECUTE)) printf("X");
    else printf("-");
    if (has_permission(flags, PERM_DELETE)) printf("D");
    else printf("-");
    if (has_permission(flags, PERM_ADMIN)) printf("A");
    else printf("-");
    printf("]");
}

int main(void) {
    // Build permissions
    int user_flags = 0;
    user_flags = add_permission(user_flags, PERM_READ);
    user_flags = add_permission(user_flags, PERM_WRITE);
    printf("user: ");
    print_permissions(user_flags);
    printf(" = %d\n", user_flags);

    int admin_flags = PERM_READ | PERM_WRITE | PERM_EXECUTE | PERM_DELETE | PERM_ADMIN;
    printf("admin: ");
    print_permissions(admin_flags);
    printf(" = %d\n", admin_flags);

    // Check permissions
    printf("user can read: %d\n", has_permission(user_flags, PERM_READ));
    printf("user can execute: %d\n", has_permission(user_flags, PERM_EXECUTE));
    printf("admin can delete: %d\n", has_permission(admin_flags, PERM_DELETE));

    // Remove permission
    int modified = remove_permission(admin_flags, PERM_DELETE);
    printf("admin without delete: ");
    print_permissions(modified);
    printf("\n");

    // Toggle permission
    int toggled = toggle_permission(user_flags, PERM_EXECUTE);
    printf("user toggle execute: ");
    print_permissions(toggled);
    printf("\n");
    toggled = toggle_permission(toggled, PERM_EXECUTE);
    printf("user toggle again: ");
    print_permissions(toggled);
    printf("\n");

    // Permission intersection (common permissions)
    int common = user_flags & admin_flags;
    printf("common: ");
    print_permissions(common);
    printf("\n");

    // Permission union (combined permissions)
    int combined = user_flags | admin_flags;
    printf("combined: ");
    print_permissions(combined);
    printf("\n");

    // Count permissions
    int count = 0;
    int tmp = admin_flags;
    while (tmp) {
        count = count + (tmp & 1);
        tmp = tmp >> 1;
    }
    printf("admin permission count: %d\n", count);

    // Multiple users
    int users[3];
    users[0] = PERM_READ;
    users[1] = PERM_READ | PERM_WRITE;
    users[2] = PERM_READ | PERM_WRITE | PERM_EXECUTE;
    int i;
    for (i = 0; i < 3; i++) {
        printf("user%d: ", i);
        print_permissions(users[i]);
        printf("\n");
    }

    return 0;
}
