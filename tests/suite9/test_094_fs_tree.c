int printf(const char *fmt, ...);

enum NodeType { NODE_FILE, NODE_DIR };

struct FSNode {
    char name[32];
    int type;
    int size;
    int parent;
    int children[10];
    int num_children;
};

struct FileSystem {
    struct FSNode nodes[30];
    int count;
};

void str_copy(char *dst, const char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void init_fs(struct FileSystem *fs) {
    fs->count = 0;
    str_copy(fs->nodes[0].name, "/");
    fs->nodes[0].type = NODE_DIR;
    fs->nodes[0].size = 0;
    fs->nodes[0].parent = -1;
    fs->nodes[0].num_children = 0;
    fs->count = 1;
}

int create_dir(struct FileSystem *fs, int parent, const char *name) {
    int idx;
    idx = fs->count;
    str_copy(fs->nodes[idx].name, name);
    fs->nodes[idx].type = NODE_DIR;
    fs->nodes[idx].size = 0;
    fs->nodes[idx].parent = parent;
    fs->nodes[idx].num_children = 0;
    fs->nodes[parent].children[fs->nodes[parent].num_children] = idx;
    fs->nodes[parent].num_children = fs->nodes[parent].num_children + 1;
    fs->count = fs->count + 1;
    return idx;
}

int create_file(struct FileSystem *fs, int parent, const char *name, int size) {
    int idx;
    idx = fs->count;
    str_copy(fs->nodes[idx].name, name);
    fs->nodes[idx].type = NODE_FILE;
    fs->nodes[idx].size = size;
    fs->nodes[idx].parent = parent;
    fs->nodes[idx].num_children = 0;
    fs->nodes[parent].children[fs->nodes[parent].num_children] = idx;
    fs->nodes[parent].num_children = fs->nodes[parent].num_children + 1;
    fs->count = fs->count + 1;
    return idx;
}

void print_tree(struct FileSystem *fs, int node, int depth) {
    int i;
    int child;
    for (i = 0; i < depth; i = i + 1) {
        printf("  ");
    }
    if (fs->nodes[node].type == NODE_DIR) {
        printf("[D] %s\n", fs->nodes[node].name);
    } else {
        printf("[F] %s (%d)\n", fs->nodes[node].name, fs->nodes[node].size);
    }
    for (i = 0; i < fs->nodes[node].num_children; i = i + 1) {
        child = fs->nodes[node].children[i];
        print_tree(fs, child, depth + 1);
    }
}

int dir_total_size(struct FileSystem *fs, int node) {
    int total;
    int i;
    int child;
    total = 0;
    if (fs->nodes[node].type == NODE_FILE) {
        return fs->nodes[node].size;
    }
    for (i = 0; i < fs->nodes[node].num_children; i = i + 1) {
        child = fs->nodes[node].children[i];
        total = total + dir_total_size(fs, child);
    }
    return total;
}

int count_files(struct FileSystem *fs, int node) {
    int c;
    int i;
    int child;
    if (fs->nodes[node].type == NODE_FILE) return 1;
    c = 0;
    for (i = 0; i < fs->nodes[node].num_children; i = i + 1) {
        child = fs->nodes[node].children[i];
        c = c + count_files(fs, child);
    }
    return c;
}

int count_dirs(struct FileSystem *fs, int node) {
    int c;
    int i;
    int child;
    if (fs->nodes[node].type == NODE_FILE) return 0;
    c = 1;
    for (i = 0; i < fs->nodes[node].num_children; i = i + 1) {
        child = fs->nodes[node].children[i];
        c = c + count_dirs(fs, child);
    }
    return c;
}

int max_depth(struct FileSystem *fs, int node) {
    int best;
    int d;
    int i;
    int child;
    if (fs->nodes[node].num_children == 0) return 0;
    best = 0;
    for (i = 0; i < fs->nodes[node].num_children; i = i + 1) {
        child = fs->nodes[node].children[i];
        d = max_depth(fs, child);
        if (d > best) best = d;
    }
    return best + 1;
}

int main() {
    struct FileSystem fs;
    int home;
    int docs;
    int pics;
    int src;

    init_fs(&fs);

    // EXPECT: === File System Tree ===
    printf("=== File System Tree ===\n");

    home = create_dir(&fs, 0, "home");
    docs = create_dir(&fs, home, "docs");
    pics = create_dir(&fs, home, "pics");
    src = create_dir(&fs, 0, "src");

    create_file(&fs, docs, "readme.txt", 100);
    create_file(&fs, docs, "notes.txt", 250);
    create_file(&fs, pics, "photo.jpg", 5000);
    create_file(&fs, pics, "icon.png", 800);
    create_file(&fs, src, "main.c", 1500);
    create_file(&fs, src, "util.c", 900);
    create_file(&fs, 0, "config.ini", 50);

    // EXPECT: [D] /
    // EXPECT:   [D] home
    // EXPECT:     [D] docs
    // EXPECT:       [F] readme.txt (100)
    // EXPECT:       [F] notes.txt (250)
    // EXPECT:     [D] pics
    // EXPECT:       [F] photo.jpg (5000)
    // EXPECT:       [F] icon.png (800)
    // EXPECT:   [D] src
    // EXPECT:     [F] main.c (1500)
    // EXPECT:     [F] util.c (900)
    // EXPECT:   [F] config.ini (50)
    print_tree(&fs, 0, 0);

    // EXPECT: Total size: 8600
    printf("Total size: %d\n", dir_total_size(&fs, 0));
    // EXPECT: Home size: 6150
    printf("Home size: %d\n", dir_total_size(&fs, home));
    // EXPECT: File count: 7
    printf("File count: %d\n", count_files(&fs, 0));
    // EXPECT: Dir count: 5
    printf("Dir count: %d\n", count_dirs(&fs, 0));
    // EXPECT: Max depth: 3
    printf("Max depth: %d\n", max_depth(&fs, 0));
    // EXPECT: Node count: 12
    printf("Node count: %d\n", fs.count);

    // EXPECT: === Done ===
    printf("=== Done ===\n");
    return 0;
}
