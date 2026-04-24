int printf(const char *fmt, ...);

struct FSEntry {
    char name[16];
    int is_dir;
    int parent;
    int size;
    int active;
};

struct FileSystem {
    struct FSEntry entries[64];
    int count;
};

void copy_str(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void fs_init(struct FileSystem *fs) {
    fs->count = 0;
    copy_str(fs->entries[0].name, "/");
    fs->entries[0].is_dir = 1;
    fs->entries[0].parent = -1;
    fs->entries[0].size = 0;
    fs->entries[0].active = 1;
    fs->count = 1;
}

int fs_create_dir(struct FileSystem *fs, int parent, const char *name) {
    if (fs->count >= 64) return -1;
    int idx = fs->count;
    copy_str(fs->entries[idx].name, name);
    fs->entries[idx].is_dir = 1;
    fs->entries[idx].parent = parent;
    fs->entries[idx].size = 0;
    fs->entries[idx].active = 1;
    fs->count = fs->count + 1;
    return idx;
}

int fs_create_file(struct FileSystem *fs, int parent, const char *name, int size) {
    if (fs->count >= 64) return -1;
    int idx = fs->count;
    copy_str(fs->entries[idx].name, name);
    fs->entries[idx].is_dir = 0;
    fs->entries[idx].parent = parent;
    fs->entries[idx].size = size;
    fs->entries[idx].active = 1;
    fs->count = fs->count + 1;
    return idx;
}

int fs_find(struct FileSystem *fs, int parent, const char *name) {
    int i = 0;
    while (i < fs->count) {
        if (fs->entries[i].active &&
            fs->entries[i].parent == parent &&
            str_eq(fs->entries[i].name, name)) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int fs_count_children(struct FileSystem *fs, int parent) {
    int c = 0;
    int i = 0;
    while (i < fs->count) {
        if (fs->entries[i].active && fs->entries[i].parent == parent) {
            c = c + 1;
        }
        i = i + 1;
    }
    return c;
}

int fs_dir_size(struct FileSystem *fs, int dir) {
    int total = 0;
    int i = 0;
    while (i < fs->count) {
        if (fs->entries[i].active && fs->entries[i].parent == dir) {
            if (fs->entries[i].is_dir) {
                total = total + fs_dir_size(fs, i);
            } else {
                total = total + fs->entries[i].size;
            }
        }
        i = i + 1;
    }
    return total;
}

int fs_depth(struct FileSystem *fs, int idx) {
    int d = 0;
    while (fs->entries[idx].parent >= 0) {
        idx = fs->entries[idx].parent;
        d = d + 1;
    }
    return d;
}

int fs_delete(struct FileSystem *fs, int idx) {
    if (!fs->entries[idx].active) return 0;
    fs->entries[idx].active = 0;
    if (fs->entries[idx].is_dir) {
        int i = 0;
        while (i < fs->count) {
            if (fs->entries[i].active &&
                fs->entries[i].parent == idx) {
                fs_delete(fs, i);
            }
            i = i + 1;
        }
    }
    return 1;
}

int fs_count_files(struct FileSystem *fs) {
    int c = 0;
    int i = 0;
    while (i < fs->count) {
        if (fs->entries[i].active && !fs->entries[i].is_dir) {
            c = c + 1;
        }
        i = i + 1;
    }
    return c;
}

int fs_count_dirs(struct FileSystem *fs) {
    int c = 0;
    int i = 0;
    while (i < fs->count) {
        if (fs->entries[i].active && fs->entries[i].is_dir) {
            c = c + 1;
        }
        i = i + 1;
    }
    return c;
}

int main(void) {
    struct FileSystem fs;
    fs_init(&fs);

    int home = fs_create_dir(&fs, 0, "home");
    int etc = fs_create_dir(&fs, 0, "etc");
    int user = fs_create_dir(&fs, home, "alice");

    int f1 = fs_create_file(&fs, user, "notes.txt", 100);
    int f2 = fs_create_file(&fs, user, "data.bin", 500);
    int f3 = fs_create_file(&fs, etc, "config.cfg", 50);

    // EXPECT: total_entries=7
    printf("total_entries=%d\n", fs.count);

    // EXPECT: files=3
    printf("files=%d\n", fs_count_files(&fs));

    // EXPECT: dirs=4
    printf("dirs=%d\n", fs_count_dirs(&fs));

    // EXPECT: root_children=2
    printf("root_children=%d\n", fs_count_children(&fs, 0));

    // EXPECT: home_children=1
    printf("home_children=%d\n", fs_count_children(&fs, home));

    // EXPECT: alice_children=2
    printf("alice_children=%d\n", fs_count_children(&fs, user));

    // EXPECT: find_alice=3
    printf("find_alice=%d\n", fs_find(&fs, home, "alice"));

    // EXPECT: find_missing=-1
    printf("find_missing=%d\n", fs_find(&fs, 0, "missing"));

    // EXPECT: depth_root=0
    printf("depth_root=%d\n", fs_depth(&fs, 0));

    // EXPECT: depth_alice=2
    printf("depth_alice=%d\n", fs_depth(&fs, user));

    // EXPECT: depth_notes=3
    printf("depth_notes=%d\n", fs_depth(&fs, f1));

    // EXPECT: root_size=650
    printf("root_size=%d\n", fs_dir_size(&fs, 0));

    // EXPECT: alice_size=600
    printf("alice_size=%d\n", fs_dir_size(&fs, user));

    // EXPECT: etc_size=50
    printf("etc_size=%d\n", fs_dir_size(&fs, etc));

    fs_delete(&fs, user);
    // EXPECT: files_after_del=1
    printf("files_after_del=%d\n", fs_count_files(&fs));

    // EXPECT: dirs_after_del=3
    printf("dirs_after_del=%d\n", fs_count_dirs(&fs));

    // EXPECT: root_size_after=50
    printf("root_size_after=%d\n", fs_dir_size(&fs, 0));

    return 0;
}
