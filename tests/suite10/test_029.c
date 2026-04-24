int printf(const char *fmt, ...);

// Inode Simulation: Unix-style inode with direct and indirect blocks

struct Inode {
    int file_size;
    int direct[6];       // 6 direct block pointers
    int indirect;        // single indirect block pointer
    int block_count;
    int file_type;       // 0=regular, 1=directory
    int permissions;     // simplified: rwx as int (e.g. 755)
    int link_count;
};

struct IndirectBlock {
    int pointers[8];     // 8 block pointers
    int count;
};

struct InodeTable {
    struct Inode inodes[16];
    struct IndirectBlock indirect_blocks[8];
    int next_inode;
    int next_block;
    int next_indirect;
    int block_size;
};

void init_inode_table(struct InodeTable *it, int bsize) {
    int i;
    int j;
    it->next_inode = 0;
    it->next_block = 0;
    it->next_indirect = 0;
    it->block_size = bsize;
    for (i = 0; i < 16; i = i + 1) {
        it->inodes[i].file_size = 0;
        it->inodes[i].block_count = 0;
        it->inodes[i].file_type = 0;
        it->inodes[i].permissions = 0;
        it->inodes[i].link_count = 0;
        it->inodes[i].indirect = -1;
        for (j = 0; j < 6; j = j + 1) {
            it->inodes[i].direct[j] = -1;
        }
    }
    for (i = 0; i < 8; i = i + 1) {
        it->indirect_blocks[i].count = 0;
        for (j = 0; j < 8; j = j + 1) {
            it->indirect_blocks[i].pointers[j] = -1;
        }
    }
}

int alloc_block(struct InodeTable *it) {
    int b;
    b = it->next_block;
    it->next_block = it->next_block + 1;
    return b;
}

int create_inode(struct InodeTable *it, int ftype, int perms) {
    int ino;
    ino = it->next_inode;
    it->next_inode = it->next_inode + 1;
    it->inodes[ino].file_type = ftype;
    it->inodes[ino].permissions = perms;
    it->inodes[ino].link_count = 1;
    return ino;
}

int write_file(struct InodeTable *it, int ino, int size) {
    int blocks_needed;
    int i;
    int blk;
    struct Inode *inode;
    int indir_idx;

    inode = &it->inodes[ino];
    blocks_needed = size / it->block_size;
    if (size - (blocks_needed * it->block_size) > 0) {
        blocks_needed = blocks_needed + 1;
    }

    inode->file_size = size;

    for (i = 0; i < blocks_needed; i = i + 1) {
        blk = alloc_block(it);
        if (i < 6) {
            inode->direct[i] = blk;
        } else {
            // Use indirect block
            if (inode->indirect == -1) {
                indir_idx = it->next_indirect;
                it->next_indirect = it->next_indirect + 1;
                inode->indirect = indir_idx;
            }
            indir_idx = inode->indirect;
            it->indirect_blocks[indir_idx].pointers[it->indirect_blocks[indir_idx].count] = blk;
            it->indirect_blocks[indir_idx].count = it->indirect_blocks[indir_idx].count + 1;
        }
        inode->block_count = inode->block_count + 1;
    }
    return blocks_needed;
}

int read_block_at(struct InodeTable *it, int ino, int logical_block) {
    struct Inode *inode;
    int indir_idx;
    inode = &it->inodes[ino];
    if (logical_block < 6) {
        return inode->direct[logical_block];
    }
    if (inode->indirect == -1) return -1;
    indir_idx = inode->indirect;
    if (logical_block - 6 < it->indirect_blocks[indir_idx].count) {
        return it->indirect_blocks[indir_idx].pointers[logical_block - 6];
    }
    return -1;
}

void print_inode(struct InodeTable *it, int ino) {
    struct Inode *inode;
    int i;
    int blk;
    inode = &it->inodes[ino];
    if (inode->file_type == 0) {
        printf("Inode %d: file", ino);
    } else {
        printf("Inode %d: dir", ino);
    }
    printf(" size=%d blocks=%d perms=%d links=%d\n",
           inode->file_size, inode->block_count,
           inode->permissions, inode->link_count);

    printf("  Blocks:");
    for (i = 0; i < inode->block_count; i = i + 1) {
        blk = read_block_at(it, ino, i);
        printf(" %d", blk);
    }
    printf("\n");
}

void add_link(struct InodeTable *it, int ino) {
    it->inodes[ino].link_count = it->inodes[ino].link_count + 1;
}

void remove_link(struct InodeTable *it, int ino) {
    it->inodes[ino].link_count = it->inodes[ino].link_count - 1;
}

int main() {
    struct InodeTable it;
    int ino0;
    int ino1;
    int ino2;
    int blocks;
    int blk;

    printf("=== Inode Simulation ===\n"); // EXPECT: === Inode Simulation ===
    init_inode_table(&it, 512);

    // Create root directory
    ino0 = create_inode(&it, 1, 755);
    printf("Root inode: %d\n", ino0); // EXPECT: Root inode: 0
    blocks = write_file(&it, ino0, 512);
    printf("Root blocks: %d\n", blocks); // EXPECT: Root blocks: 1
    print_inode(&it, ino0);
    // EXPECT: Inode 0: dir size=512 blocks=1 perms=755 links=1
    // EXPECT:   Blocks: 0

    // Create a small file (3 blocks, all direct)
    ino1 = create_inode(&it, 0, 644);
    printf("File1 inode: %d\n", ino1); // EXPECT: File1 inode: 1
    blocks = write_file(&it, ino1, 1300);
    printf("File1 blocks: %d\n", blocks); // EXPECT: File1 blocks: 3
    print_inode(&it, ino1);
    // EXPECT: Inode 1: file size=1300 blocks=3 perms=644 links=1
    // EXPECT:   Blocks: 1 2 3

    // Create a large file (8 blocks, needs indirect)
    ino2 = create_inode(&it, 0, 600);
    printf("File2 inode: %d\n", ino2); // EXPECT: File2 inode: 2
    blocks = write_file(&it, ino2, 4000);
    printf("File2 blocks: %d\n", blocks); // EXPECT: File2 blocks: 8
    print_inode(&it, ino2);
    // EXPECT: Inode 2: file size=4000 blocks=8 perms=600 links=1
    // EXPECT:   Blocks: 4 5 6 7 8 9 10 11

    // Test block reads
    blk = read_block_at(&it, ino2, 0);
    printf("File2 logical 0 -> physical %d\n", blk); // EXPECT: File2 logical 0 -> physical 4
    blk = read_block_at(&it, ino2, 5);
    printf("File2 logical 5 -> physical %d\n", blk); // EXPECT: File2 logical 5 -> physical 9
    blk = read_block_at(&it, ino2, 7);
    printf("File2 logical 7 -> physical %d\n", blk); // EXPECT: File2 logical 7 -> physical 11

    // Test hard links
    add_link(&it, ino1);
    add_link(&it, ino1);
    printf("File1 links: %d\n", it.inodes[ino1].link_count); // EXPECT: File1 links: 3
    remove_link(&it, ino1);
    printf("File1 links after unlink: %d\n", it.inodes[ino1].link_count); // EXPECT: File1 links after unlink: 2

    printf("Total blocks allocated: %d\n", it.next_block); // EXPECT: Total blocks allocated: 12

    printf("Inode simulation complete\n"); // EXPECT: Inode simulation complete
    return 0;
}
