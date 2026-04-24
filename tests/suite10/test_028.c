int printf(const char *fmt, ...);

// File Allocation Table (FAT) Simulation

struct FATEntry {
    int next_cluster;  // -1 = end of chain, -2 = free
    int file_id;       // which file owns this cluster
};

struct FileInfo {
    int name_id;
    int start_cluster;
    int size_bytes;
    int num_clusters;
};

struct FATSystem {
    struct FATEntry fat[32];
    struct FileInfo files[10];
    int num_files;
    int num_clusters;
    int cluster_size;
};

void init_fat(struct FATSystem *fs, int nclusters, int csize) {
    int i;
    fs->num_clusters = nclusters;
    fs->cluster_size = csize;
    fs->num_files = 0;
    for (i = 0; i < nclusters; i = i + 1) {
        fs->fat[i].next_cluster = -2; // free
        fs->fat[i].file_id = -1;
    }
}

int find_free_cluster(struct FATSystem *fs) {
    int i;
    for (i = 0; i < fs->num_clusters; i = i + 1) {
        if (fs->fat[i].next_cluster == -2) {
            return i;
        }
    }
    return -1;
}

int create_file(struct FATSystem *fs, int name_id, int size_bytes) {
    int clusters_needed;
    int first;
    int prev;
    int cur;
    int i;
    int fid;

    clusters_needed = size_bytes / fs->cluster_size;
    if (size_bytes - (clusters_needed * fs->cluster_size) > 0) {
        clusters_needed = clusters_needed + 1;
    }

    first = -1;
    prev = -1;
    for (i = 0; i < clusters_needed; i = i + 1) {
        cur = find_free_cluster(fs);
        if (cur == -1) return -1; // no space
        fs->fat[cur].next_cluster = -1; // end for now
        fs->fat[cur].file_id = fs->num_files;
        if (prev >= 0) {
            fs->fat[prev].next_cluster = cur;
        }
        if (first == -1) first = cur;
        prev = cur;
    }

    fid = fs->num_files;
    fs->files[fid].name_id = name_id;
    fs->files[fid].start_cluster = first;
    fs->files[fid].size_bytes = size_bytes;
    fs->files[fid].num_clusters = clusters_needed;
    fs->num_files = fs->num_files + 1;

    return fid;
}

void delete_file(struct FATSystem *fs, int fid) {
    int cur;
    int next;
    if (fid < 0 || fid >= fs->num_files) return;
    cur = fs->files[fid].start_cluster;
    while (cur != -1) {
        next = fs->fat[cur].next_cluster;
        fs->fat[cur].next_cluster = -2;
        fs->fat[cur].file_id = -1;
        cur = next;
    }
    fs->files[fid].start_cluster = -1;
    fs->files[fid].size_bytes = 0;
    fs->files[fid].num_clusters = 0;
}

void print_chain(struct FATSystem *fs, int fid) {
    int cur;
    if (fid < 0 || fid >= fs->num_files) return;
    printf("File %d chain: ", fs->files[fid].name_id);
    cur = fs->files[fid].start_cluster;
    while (cur != -1) {
        printf("%d", cur);
        cur = fs->fat[cur].next_cluster;
        if (cur != -1) printf("->");
    }
    printf("\n");
}

int count_free(struct FATSystem *fs) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < fs->num_clusters; i = i + 1) {
        if (fs->fat[i].next_cluster == -2) {
            count = count + 1;
        }
    }
    return count;
}

int check_fragmentation(struct FATSystem *fs, int fid) {
    int cur;
    int next;
    int frags;
    if (fid < 0 || fid >= fs->num_files) return 0;
    cur = fs->files[fid].start_cluster;
    frags = 0;
    while (cur != -1) {
        next = fs->fat[cur].next_cluster;
        if (next != -1 && next != cur + 1) {
            frags = frags + 1;
        }
        cur = next;
    }
    return frags;
}

int main() {
    struct FATSystem fs;
    int fid;
    int free_count;
    int frags;
    int i;

    printf("=== FAT File System ===\n"); // EXPECT: === FAT File System ===
    init_fat(&fs, 16, 512);

    printf("Free clusters: %d\n", count_free(&fs)); // EXPECT: Free clusters: 16

    fid = create_file(&fs, 1, 1500); // needs 3 clusters
    printf("Created file 1, id=%d, clusters=%d\n", fid, fs.files[fid].num_clusters); // EXPECT: Created file 1, id=0, clusters=3
    print_chain(&fs, fid); // EXPECT: File 1 chain: 0->1->2

    fid = create_file(&fs, 2, 2048); // needs 4 clusters
    printf("Created file 2, id=%d, clusters=%d\n", fid, fs.files[fid].num_clusters); // EXPECT: Created file 2, id=1, clusters=4
    print_chain(&fs, fid); // EXPECT: File 2 chain: 3->4->5->6

    fid = create_file(&fs, 3, 512); // needs 1 cluster
    printf("Created file 3, id=%d, clusters=%d\n", fid, fs.files[fid].num_clusters); // EXPECT: Created file 3, id=2, clusters=1
    print_chain(&fs, fid); // EXPECT: File 3 chain: 7

    printf("Free clusters: %d\n", count_free(&fs)); // EXPECT: Free clusters: 8

    // Delete file 2 (middle), creates gap
    delete_file(&fs, 1);
    printf("Deleted file 2\n"); // EXPECT: Deleted file 2
    printf("Free clusters: %d\n", count_free(&fs)); // EXPECT: Free clusters: 12

    // Create file 4 that fills the gap with fragmentation
    fid = create_file(&fs, 4, 3000); // needs 6 clusters: fills 3,4,5,6 then 8,9
    printf("Created file 4, id=%d, clusters=%d\n", fid, fs.files[fid].num_clusters); // EXPECT: Created file 4, id=3, clusters=6
    print_chain(&fs, fid); // EXPECT: File 4 chain: 3->4->5->6->8->9

    frags = check_fragmentation(&fs, 3);
    printf("File 4 fragments: %d\n", frags); // EXPECT: File 4 fragments: 1

    printf("Free clusters: %d\n", count_free(&fs)); // EXPECT: Free clusters: 6

    // Print FAT map
    printf("FAT map: ");
    for (i = 0; i < 16; i = i + 1) {
        if (fs.fat[i].next_cluster == -2) {
            printf(".");
        } else {
            printf("%d", fs.fat[i].file_id);
        }
    }
    printf("\n"); // EXPECT: FAT map: 0003333233......

    printf("FAT simulation complete\n"); // EXPECT: FAT simulation complete
    return 0;
}
