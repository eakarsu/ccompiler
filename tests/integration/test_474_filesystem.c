int printf(const char *fmt, ...);
// EXPECT: Tree:\n//\n  home/\n    docs/\n      readme.txt (1024)\n      notes.txt (512)\n    pics/\n      photo.jpg (4096)\n      icon.png (256)\n  etc/\n    config.cfg (128)\n\nListing /home:\n  [DIR]  docs\n  [DIR]  pics\n\nListing /home/docs:\n  [FILE] readme.txt (1024 bytes)\n  [FILE] notes.txt (512 bytes)\n\nTotal size: 6016 bytes\nTotal files: 5\nFind 'docs' in home: 1\nFind 'xyz' in home: 0
// File system tree simulation

struct FSNode {
    char name[16];
    int is_dir;
    int parent;
    int children[8];
    int num_children;
    int size;
};

struct FileSystem {
    struct FSNode nodes[32];
    int count;
};

void str_cpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0 && i < 15) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

int str_cmp(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

void fs_init(struct FileSystem *fs) {
    int i;
    int j;
    for (i = 0; i < 32; i++) {
        fs->nodes[i].name[0] = 0;
        fs->nodes[i].is_dir = 0;
        fs->nodes[i].parent = -1;
        fs->nodes[i].num_children = 0;
        fs->nodes[i].size = 0;
        for (j = 0; j < 8; j++) fs->nodes[i].children[j] = -1;
    }
    str_cpy(fs->nodes[0].name, "/");
    fs->nodes[0].is_dir = 1;
    fs->count = 1;
}

int fs_create(struct FileSystem *fs, int parent, char *name, int is_dir, int size) {
    int idx;
    if (fs->count >= 32) return -1;
    if (!fs->nodes[parent].is_dir) return -1;
    idx = fs->count;
    str_cpy(fs->nodes[idx].name, name);
    fs->nodes[idx].is_dir = is_dir;
    fs->nodes[idx].parent = parent;
    fs->nodes[idx].size = size;
    fs->nodes[idx].num_children = 0;
    fs->nodes[parent].children[fs->nodes[parent].num_children] = idx;
    fs->nodes[parent].num_children++;
    fs->count++;
    return idx;
}

int fs_find_child(struct FileSystem *fs, int parent, char *name) {
    int i;
    int child;
    for (i = 0; i < fs->nodes[parent].num_children; i++) {
        child = fs->nodes[parent].children[i];
        if (str_cmp(fs->nodes[child].name, name) == 0) return child;
    }
    return -1;
}

void fs_list(struct FileSystem *fs, int dir) {
    int i;
    int child;
    for (i = 0; i < fs->nodes[dir].num_children; i++) {
        child = fs->nodes[dir].children[i];
        if (fs->nodes[child].is_dir)
            printf("  [DIR]  %s\n", fs->nodes[child].name);
        else
            printf("  [FILE] %s (%d bytes)\n", fs->nodes[child].name, fs->nodes[child].size);
    }
}

int fs_total_size(struct FileSystem *fs, int node) {
    int total;
    int i;
    int child;
    total = fs->nodes[node].size;
    for (i = 0; i < fs->nodes[node].num_children; i++) {
        child = fs->nodes[node].children[i];
        total += fs_total_size(fs, child);
    }
    return total;
}

int fs_count_files(struct FileSystem *fs, int node) {
    int count;
    int i;
    int child;
    count = fs->nodes[node].is_dir ? 0 : 1;
    for (i = 0; i < fs->nodes[node].num_children; i++) {
        child = fs->nodes[node].children[i];
        count += fs_count_files(fs, child);
    }
    return count;
}

void fs_print_tree(struct FileSystem *fs, int node, int depth) {
    int i;
    int child;
    for (i = 0; i < depth; i++) printf("  ");
    if (fs->nodes[node].is_dir)
        printf("%s/\n", fs->nodes[node].name);
    else
        printf("%s (%d)\n", fs->nodes[node].name, fs->nodes[node].size);
    for (i = 0; i < fs->nodes[node].num_children; i++) {
        child = fs->nodes[node].children[i];
        fs_print_tree(fs, child, depth + 1);
    }
}

int main(void) {
    struct FileSystem fs;
    int home;
    int docs;
    int pics;

    fs_init(&fs);

    home = fs_create(&fs, 0, "home", 1, 0);
    docs = fs_create(&fs, home, "docs", 1, 0);
    pics = fs_create(&fs, home, "pics", 1, 0);
    fs_create(&fs, docs, "readme.txt", 0, 1024);
    fs_create(&fs, docs, "notes.txt", 0, 512);
    fs_create(&fs, pics, "photo.jpg", 0, 4096);
    fs_create(&fs, pics, "icon.png", 0, 256);
    fs_create(&fs, 0, "etc", 1, 0);
    fs_create(&fs, 8, "config.cfg", 0, 128);

    printf("Tree:\n");
    fs_print_tree(&fs, 0, 0);

    printf("\nListing /home:\n");
    fs_list(&fs, home);

    printf("\nListing /home/docs:\n");
    fs_list(&fs, docs);

    printf("\nTotal size: %d bytes\n", fs_total_size(&fs, 0));
    printf("Total files: %d\n", fs_count_files(&fs, 0));

    printf("Find 'docs' in home: %d\n", fs_find_child(&fs, home, "docs") >= 0);
    printf("Find 'xyz' in home: %d\n", fs_find_child(&fs, home, "xyz") >= 0);

    return 0;
}
