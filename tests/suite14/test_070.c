int printf(const char *fmt, ...);

// Simplified inode-based file system simulation
// Fixed: 32 data blocks, 8 inodes, 8 directory entries

struct Inode {
    int used;
    int size;
    int blocks[4];     // up to 4 direct block pointers
    int block_count;
    int is_dir;
};

struct DirEntry {
    int active;
    int inode;
    char name[16];
};

struct FileSystem {
    struct Inode inodes[8];
    int block_used[32]; // 0=free, 1=used
    struct DirEntry root_dir[8];
    int root_entries;
    int next_block;
};

void fs_init(struct FileSystem *fs) {
    int i;
    int j;
    for (i = 0; i < 8; i++) {
        fs->inodes[i].used = 0;
        fs->inodes[i].size = 0;
        fs->inodes[i].block_count = 0;
        fs->inodes[i].is_dir = 0;
        for (j = 0; j < 4; j++) {
            fs->inodes[i].blocks[j] = -1;
        }
    }
    for (i = 0; i < 32; i++) {
        fs->block_used[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        fs->root_dir[i].active = 0;
        fs->root_dir[i].inode = -1;
        int k;
        for (k = 0; k < 16; k++) {
            fs->root_dir[i].name[k] = 0;
        }
    }
    fs->root_entries = 0;
    fs->next_block = 0;

    // Reserve inode 0 for root directory
    fs->inodes[0].used = 1;
    fs->inodes[0].is_dir = 1;
}

int alloc_inode(struct FileSystem *fs) {
    int i;
    for (i = 1; i < 8; i++) {
        if (!fs->inodes[i].used) {
            fs->inodes[i].used = 1;
            fs->inodes[i].size = 0;
            fs->inodes[i].block_count = 0;
            fs->inodes[i].is_dir = 0;
            int j;
            for (j = 0; j < 4; j++) fs->inodes[i].blocks[j] = -1;
            return i;
        }
    }
    return -1;
}

int alloc_block(struct FileSystem *fs) {
    int i;
    for (i = 0; i < 32; i++) {
        if (!fs->block_used[i]) {
            fs->block_used[i] = 1;
            return i;
        }
    }
    return -1;
}

void copy_name(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0 && i < 15) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

int str_equal(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

// Create a file in root directory, returns inode number or -1
int fs_create(struct FileSystem *fs, const char *name) {
    // Check if name already exists
    int i;
    for (i = 0; i < 8; i++) {
        if (fs->root_dir[i].active && str_equal(fs->root_dir[i].name, name)) {
            return -1; // already exists
        }
    }

    int ino = alloc_inode(fs);
    if (ino == -1) return -1;

    // Find free dir entry
    for (i = 0; i < 8; i++) {
        if (!fs->root_dir[i].active) {
            fs->root_dir[i].active = 1;
            fs->root_dir[i].inode = ino;
            copy_name(fs->root_dir[i].name, name);
            fs->root_entries++;
            return ino;
        }
    }

    // No dir space, free inode
    fs->inodes[ino].used = 0;
    return -1;
}

// Write 'size' bytes to file (simulated - just allocates blocks)
// Block size = 64 bytes
int fs_write(struct FileSystem *fs, int ino, int size) {
    if (ino < 1 || ino >= 8 || !fs->inodes[ino].used) return -1;

    int blocks_needed = (size + 63) / 64;
    if (blocks_needed > 4) return -1; // max 4 direct blocks

    // Free old blocks
    int i;
    for (i = 0; i < fs->inodes[ino].block_count; i++) {
        int blk = fs->inodes[ino].blocks[i];
        if (blk >= 0) fs->block_used[blk] = 0;
        fs->inodes[ino].blocks[i] = -1;
    }

    // Allocate new blocks
    for (i = 0; i < blocks_needed; i++) {
        int blk = alloc_block(fs);
        if (blk == -1) {
            // Rollback
            int j;
            for (j = 0; j < i; j++) {
                fs->block_used[fs->inodes[ino].blocks[j]] = 0;
                fs->inodes[ino].blocks[j] = -1;
            }
            fs->inodes[ino].block_count = 0;
            fs->inodes[ino].size = 0;
            return -1;
        }
        fs->inodes[ino].blocks[i] = blk;
    }
    fs->inodes[ino].block_count = blocks_needed;
    fs->inodes[ino].size = size;
    return size;
}

// Delete a file
int fs_delete(struct FileSystem *fs, const char *name) {
    int i;
    for (i = 0; i < 8; i++) {
        if (fs->root_dir[i].active && str_equal(fs->root_dir[i].name, name)) {
            int ino = fs->root_dir[i].inode;
            // Free blocks
            int j;
            for (j = 0; j < fs->inodes[ino].block_count; j++) {
                int blk = fs->inodes[ino].blocks[j];
                if (blk >= 0) fs->block_used[blk] = 0;
            }
            // Free inode
            fs->inodes[ino].used = 0;
            fs->inodes[ino].size = 0;
            fs->inodes[ino].block_count = 0;
            // Remove dir entry
            fs->root_dir[i].active = 0;
            fs->root_entries--;
            return 1;
        }
    }
    return 0; // not found
}

// Lookup by name, returns inode number or -1
int fs_lookup(struct FileSystem *fs, const char *name) {
    int i;
    for (i = 0; i < 8; i++) {
        if (fs->root_dir[i].active && str_equal(fs->root_dir[i].name, name)) {
            return fs->root_dir[i].inode;
        }
    }
    return -1;
}

int count_used_inodes(struct FileSystem *fs) {
    int c = 0;
    int i;
    for (i = 0; i < 8; i++) {
        if (fs->inodes[i].used) c++;
    }
    return c;
}

int count_used_blocks(struct FileSystem *fs) {
    int c = 0;
    int i;
    for (i = 0; i < 32; i++) {
        if (fs->block_used[i]) c++;
    }
    return c;
}

int main(void) {
    struct FileSystem fs;
    fs_init(&fs);

    printf("Init: inodes=%d blocks=%d\n", count_used_inodes(&fs), count_used_blocks(&fs));
    // EXPECT: Init: inodes=1 blocks=0

    // Create files
    int f1 = fs_create(&fs, "hello.txt");
    printf("Created hello.txt inode=%d\n", f1);
    // EXPECT: Created hello.txt inode=1

    int f2 = fs_create(&fs, "world.c");
    printf("Created world.c inode=%d\n", f2);
    // EXPECT: Created world.c inode=2

    int f3 = fs_create(&fs, "data.bin");
    printf("Created data.bin inode=%d\n", f3);
    // EXPECT: Created data.bin inode=3

    printf("Dir entries: %d, Used inodes: %d\n", fs.root_entries, count_used_inodes(&fs));
    // EXPECT: Dir entries: 3, Used inodes: 4

    // Write data
    int w1 = fs_write(&fs, f1, 100); // 2 blocks (100/64 = 1.56 -> 2)
    printf("Write hello.txt 100 bytes: %d blocks=%d\n", w1, fs.inodes[f1].block_count);
    // EXPECT: Write hello.txt 100 bytes: 100 blocks=2

    int w2 = fs_write(&fs, f2, 64); // exactly 1 block
    printf("Write world.c 64 bytes: %d blocks=%d\n", w2, fs.inodes[f2].block_count);
    // EXPECT: Write world.c 64 bytes: 64 blocks=1

    int w3 = fs_write(&fs, f3, 256); // 4 blocks (max)
    printf("Write data.bin 256 bytes: %d blocks=%d\n", w3, fs.inodes[f3].block_count);
    // EXPECT: Write data.bin 256 bytes: 256 blocks=4

    printf("Used blocks: %d\n", count_used_blocks(&fs));
    // EXPECT: Used blocks: 7

    // Try to write too large (would need 5 blocks)
    int w4 = fs_write(&fs, f1, 257);
    printf("Write too large: %d\n", w4);
    // EXPECT: Write too large: -1

    // Lookup
    int l1 = fs_lookup(&fs, "world.c");
    printf("Lookup world.c: %d\n", l1);
    // EXPECT: Lookup world.c: 2

    int l2 = fs_lookup(&fs, "missing.txt");
    printf("Lookup missing.txt: %d\n", l2);
    // EXPECT: Lookup missing.txt: -1

    // Duplicate create
    int dup = fs_create(&fs, "hello.txt");
    printf("Duplicate create: %d\n", dup);
    // EXPECT: Duplicate create: -1

    // Delete
    int d1 = fs_delete(&fs, "world.c");
    printf("Delete world.c: %d\n", d1);
    // EXPECT: Delete world.c: 1

    printf("After delete: entries=%d inodes=%d blocks=%d\n",
           fs.root_entries, count_used_inodes(&fs), count_used_blocks(&fs));
    // EXPECT: After delete: entries=2 inodes=3 blocks=6

    // Reuse freed inode
    int f4 = fs_create(&fs, "new.log");
    printf("Created new.log inode=%d\n", f4);
    // EXPECT: Created new.log inode=2

    // Rewrite hello.txt (shrink)
    int w5 = fs_write(&fs, f1, 30); // 1 block
    printf("Rewrite hello.txt 30 bytes: blocks=%d\n", fs.inodes[f1].block_count);
    // EXPECT: Rewrite hello.txt 30 bytes: blocks=1

    printf("Blocks after rewrite: %d\n", count_used_blocks(&fs));
    // EXPECT: Blocks after rewrite: 5

    // Fill directory
    fs_create(&fs, "a.txt");
    fs_create(&fs, "b.txt");
    fs_create(&fs, "c.txt");
    fs_create(&fs, "d.txt");
    int overflow = fs_create(&fs, "e.txt");
    printf("Dir overflow: %d\n", overflow);
    // EXPECT: Dir overflow: -1

    printf("Final: entries=%d inodes=%d\n", fs.root_entries, count_used_inodes(&fs));
    // EXPECT: Final: entries=7 inodes=8

    // Delete and re-create
    fs_delete(&fs, "a.txt");
    int reuse = fs_create(&fs, "z.txt");
    printf("Reuse after delete: inode=%d entries=%d\n", reuse, fs.root_entries);
    // EXPECT: Reuse after delete: inode=4 entries=7

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
