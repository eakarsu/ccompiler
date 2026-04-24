int printf(const char *fmt, ...);

// Access Control List (ACL) System

enum Permission {
    PERM_NONE = 0,
    PERM_READ = 1,
    PERM_WRITE = 2,
    PERM_EXEC = 4,
    PERM_DELETE = 8,
    PERM_ADMIN = 16
};

struct User {
    int id;
    char name[32];
    int role; // 0=guest, 1=user, 2=editor, 3=admin
};

struct Resource {
    int id;
    char name[32];
    int owner_id;
};

struct ACLEntry {
    int user_id;
    int resource_id;
    int permissions; // bitmask of Permission values
};

struct ACLSystem {
    struct User users[8];
    int user_count;
    struct Resource resources[8];
    int resource_count;
    struct ACLEntry entries[32];
    int entry_count;
};

void acl_init(struct ACLSystem *sys) {
    sys->user_count = 0;
    sys->resource_count = 0;
    sys->entry_count = 0;
}

void str_copy_to(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0 && i < 31) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int add_user(struct ACLSystem *sys, int id, char *name, int role) {
    if (sys->user_count >= 8) return -1;
    int idx = sys->user_count;
    sys->users[idx].id = id;
    str_copy_to(sys->users[idx].name, name);
    sys->users[idx].role = role;
    sys->user_count = sys->user_count + 1;
    return idx;
}

int add_resource(struct ACLSystem *sys, int id, char *name, int owner_id) {
    if (sys->resource_count >= 8) return -1;
    int idx = sys->resource_count;
    sys->resources[idx].id = id;
    str_copy_to(sys->resources[idx].name, name);
    sys->resources[idx].owner_id = owner_id;
    sys->resource_count = sys->resource_count + 1;
    return idx;
}

void grant_permission(struct ACLSystem *sys, int user_id, int res_id, int perm) {
    // Check if entry already exists
    int i;
    for (i = 0; i < sys->entry_count; i++) {
        if (sys->entries[i].user_id == user_id && sys->entries[i].resource_id == res_id) {
            sys->entries[i].permissions = sys->entries[i].permissions | perm;
            return;
        }
    }
    // Create new entry
    if (sys->entry_count < 32) {
        int idx = sys->entry_count;
        sys->entries[idx].user_id = user_id;
        sys->entries[idx].resource_id = res_id;
        sys->entries[idx].permissions = perm;
        sys->entry_count = sys->entry_count + 1;
    }
}

void revoke_permission(struct ACLSystem *sys, int user_id, int res_id, int perm) {
    int i;
    for (i = 0; i < sys->entry_count; i++) {
        if (sys->entries[i].user_id == user_id && sys->entries[i].resource_id == res_id) {
            sys->entries[i].permissions = sys->entries[i].permissions & (~perm);
            return;
        }
    }
}

int check_permission(struct ACLSystem *sys, int user_id, int res_id, int perm) {
    // Find user
    int user_role = -1;
    int i;
    for (i = 0; i < sys->user_count; i++) {
        if (sys->users[i].id == user_id) {
            user_role = sys->users[i].role;
            break;
        }
    }
    if (user_role == -1) return 0; // user not found

    // Admin role has all permissions
    if (user_role == 3) return 1;

    // Check if user is the resource owner (owners have read/write)
    for (i = 0; i < sys->resource_count; i++) {
        if (sys->resources[i].id == res_id && sys->resources[i].owner_id == user_id) {
            if (perm == PERM_READ || perm == PERM_WRITE) return 1;
        }
    }

    // Check ACL entries
    for (i = 0; i < sys->entry_count; i++) {
        if (sys->entries[i].user_id == user_id && sys->entries[i].resource_id == res_id) {
            return (sys->entries[i].permissions & perm) != 0;
        }
    }
    return 0;
}

// Count how many permissions a user has on a resource
int count_permissions(struct ACLSystem *sys, int user_id, int res_id) {
    int count = 0;
    if (check_permission(sys, user_id, res_id, PERM_READ)) count = count + 1;
    if (check_permission(sys, user_id, res_id, PERM_WRITE)) count = count + 1;
    if (check_permission(sys, user_id, res_id, PERM_EXEC)) count = count + 1;
    if (check_permission(sys, user_id, res_id, PERM_DELETE)) count = count + 1;
    if (check_permission(sys, user_id, res_id, PERM_ADMIN)) count = count + 1;
    return count;
}

int main() {
    struct ACLSystem sys;
    acl_init(&sys);

    // Setup users
    add_user(&sys, 1, "alice", 3);   // admin
    add_user(&sys, 2, "bob", 1);     // regular user
    add_user(&sys, 3, "charlie", 2); // editor
    add_user(&sys, 4, "guest", 0);   // guest

    // Setup resources
    add_resource(&sys, 100, "document", 2);  // owned by bob
    add_resource(&sys, 101, "config", 1);    // owned by alice
    add_resource(&sys, 102, "public", 0);    // no owner

    printf("Users: %d\n", sys.user_count);
    // EXPECT: Users: 4
    printf("Resources: %d\n", sys.resource_count);
    // EXPECT: Resources: 3

    // Test 1: admin has all permissions
    int admin_read = check_permission(&sys, 1, 100, PERM_READ);
    int admin_del = check_permission(&sys, 1, 100, PERM_DELETE);
    printf("Admin read: %d\n", admin_read);
    // EXPECT: Admin read: 1
    printf("Admin delete: %d\n", admin_del);
    // EXPECT: Admin delete: 1

    // Test 2: owner has read/write on own resource
    int owner_read = check_permission(&sys, 2, 100, PERM_READ);
    int owner_write = check_permission(&sys, 2, 100, PERM_WRITE);
    printf("Owner read: %d\n", owner_read);
    // EXPECT: Owner read: 1
    printf("Owner write: %d\n", owner_write);
    // EXPECT: Owner write: 1

    // Test 3: owner doesn't automatically have delete
    int owner_del = check_permission(&sys, 2, 100, PERM_DELETE);
    printf("Owner delete: %d\n", owner_del);
    // EXPECT: Owner delete: 0

    // Test 4: guest has no permissions by default
    int guest_read = check_permission(&sys, 4, 100, PERM_READ);
    printf("Guest read: %d\n", guest_read);
    // EXPECT: Guest read: 0

    // Test 5: grant permission to guest
    grant_permission(&sys, 4, 102, PERM_READ);
    int guest_pub = check_permission(&sys, 4, 102, PERM_READ);
    printf("Guest public read: %d\n", guest_pub);
    // EXPECT: Guest public read: 1

    // Test 6: grant multiple permissions
    grant_permission(&sys, 3, 100, PERM_READ | PERM_WRITE);
    int ed_read = check_permission(&sys, 3, 100, PERM_READ);
    int ed_write = check_permission(&sys, 3, 100, PERM_WRITE);
    int ed_exec = check_permission(&sys, 3, 100, PERM_EXEC);
    printf("Editor read: %d\n", ed_read);
    // EXPECT: Editor read: 1
    printf("Editor write: %d\n", ed_write);
    // EXPECT: Editor write: 1
    printf("Editor exec: %d\n", ed_exec);
    // EXPECT: Editor exec: 0

    // Test 7: revoke permission
    revoke_permission(&sys, 3, 100, PERM_WRITE);
    int ed_write2 = check_permission(&sys, 3, 100, PERM_WRITE);
    int ed_read2 = check_permission(&sys, 3, 100, PERM_READ);
    printf("Editor write revoked: %d\n", ed_write2);
    // EXPECT: Editor write revoked: 0
    printf("Editor read still: %d\n", ed_read2);
    // EXPECT: Editor read still: 1

    // Test 8: count permissions
    int admin_perms = count_permissions(&sys, 1, 100);
    printf("Admin perm count: %d\n", admin_perms);
    // EXPECT: Admin perm count: 5

    int guest_perms = count_permissions(&sys, 4, 100);
    printf("Guest perm count: %d\n", guest_perms);
    // EXPECT: Guest perm count: 0

    // Test 9: non-existent user
    int nouser = check_permission(&sys, 99, 100, PERM_READ);
    printf("Non-existent user: %d\n", nouser);
    // EXPECT: Non-existent user: 0

    // Test 10: ACL entry count
    printf("ACL entries: %d\n", sys.entry_count);
    // EXPECT: ACL entries: 2

    // Test 11: grant exec to bob on config
    grant_permission(&sys, 2, 101, PERM_EXEC);
    int bob_exec = check_permission(&sys, 2, 101, PERM_EXEC);
    printf("Bob exec config: %d\n", bob_exec);
    // EXPECT: Bob exec config: 1

    // Bob is also owner of document - has read there
    int bob_doc_read = check_permission(&sys, 2, 100, PERM_READ);
    printf("Bob own doc read: %d\n", bob_doc_read);
    // EXPECT: Bob own doc read: 1

    printf("ACL tests passed\n");
    // EXPECT: ACL tests passed

    return 0;
}
