int printf(const char *fmt, ...);

// Virtual Memory Page Replacement: FIFO and LRU algorithms

struct Frame {
    int page;
    int valid;
    int last_used;  // timestamp for LRU
};

struct VMStats {
    int hits;
    int faults;
    int replacements;
};

void init_frames(struct Frame *frames, int nframes) {
    int i;
    for (i = 0; i < nframes; i = i + 1) {
        frames[i].page = -1;
        frames[i].valid = 0;
        frames[i].last_used = 0;
    }
}

int find_page(struct Frame *frames, int nframes, int page) {
    int i;
    for (i = 0; i < nframes; i = i + 1) {
        if (frames[i].valid && frames[i].page == page) {
            return i;
        }
    }
    return -1;
}

int find_free_frame(struct Frame *frames, int nframes) {
    int i;
    for (i = 0; i < nframes; i = i + 1) {
        if (frames[i].valid == 0) {
            return i;
        }
    }
    return -1;
}

void fifo_replace(struct Frame *frames, int nframes, int *ref_string, int ref_len) {
    struct VMStats stats;
    int fifo_ptr;
    int i;
    int idx;
    int free_idx;

    stats.hits = 0;
    stats.faults = 0;
    stats.replacements = 0;
    fifo_ptr = 0;
    init_frames(frames, nframes);

    printf("=== FIFO Page Replacement ===\n"); // EXPECT: === FIFO Page Replacement ===
    for (i = 0; i < ref_len; i = i + 1) {
        idx = find_page(frames, nframes, ref_string[i]);
        if (idx >= 0) {
            stats.hits = stats.hits + 1;
            printf("Page %d: HIT\n", ref_string[i]);
        } else {
            stats.faults = stats.faults + 1;
            free_idx = find_free_frame(frames, nframes);
            if (free_idx >= 0) {
                frames[free_idx].page = ref_string[i];
                frames[free_idx].valid = 1;
                printf("Page %d: FAULT (loaded to frame %d)\n", ref_string[i], free_idx);
            } else {
                printf("Page %d: FAULT (replaced page %d in frame %d)\n",
                       ref_string[i], frames[fifo_ptr].page, fifo_ptr);
                frames[fifo_ptr].page = ref_string[i];
                stats.replacements = stats.replacements + 1;
                fifo_ptr = fifo_ptr + 1;
                if (fifo_ptr >= nframes) fifo_ptr = 0;
            }
        }
    }
    // EXPECT: Page 1: FAULT (loaded to frame 0)
    // EXPECT: Page 2: FAULT (loaded to frame 1)
    // EXPECT: Page 3: FAULT (loaded to frame 2)
    // EXPECT: Page 4: FAULT (replaced page 1 in frame 0)
    // EXPECT: Page 1: FAULT (replaced page 2 in frame 1)
    // EXPECT: Page 2: FAULT (replaced page 3 in frame 2)
    // EXPECT: Page 5: FAULT (replaced page 4 in frame 0)
    // EXPECT: Page 1: HIT
    // EXPECT: Page 2: HIT
    // EXPECT: Page 3: FAULT (replaced page 1 in frame 1)
    printf("FIFO: faults=%d hits=%d replacements=%d\n", stats.faults, stats.hits, stats.replacements); // EXPECT: FIFO: faults=8 hits=2 replacements=5
}

void lru_replace(struct Frame *frames, int nframes, int *ref_string, int ref_len) {
    struct VMStats stats;
    int i;
    int idx;
    int free_idx;
    int lru_idx;
    int lru_time;
    int j;
    int timestamp;

    stats.hits = 0;
    stats.faults = 0;
    stats.replacements = 0;
    timestamp = 0;
    init_frames(frames, nframes);

    printf("=== LRU Page Replacement ===\n"); // EXPECT: === LRU Page Replacement ===
    for (i = 0; i < ref_len; i = i + 1) {
        timestamp = timestamp + 1;
        idx = find_page(frames, nframes, ref_string[i]);
        if (idx >= 0) {
            stats.hits = stats.hits + 1;
            frames[idx].last_used = timestamp;
            printf("Page %d: HIT\n", ref_string[i]);
        } else {
            stats.faults = stats.faults + 1;
            free_idx = find_free_frame(frames, nframes);
            if (free_idx >= 0) {
                frames[free_idx].page = ref_string[i];
                frames[free_idx].valid = 1;
                frames[free_idx].last_used = timestamp;
                printf("Page %d: FAULT (loaded to frame %d)\n", ref_string[i], free_idx);
            } else {
                lru_idx = 0;
                lru_time = frames[0].last_used;
                for (j = 1; j < nframes; j = j + 1) {
                    if (frames[j].last_used < lru_time) {
                        lru_time = frames[j].last_used;
                        lru_idx = j;
                    }
                }
                printf("Page %d: FAULT (replaced page %d in frame %d)\n",
                       ref_string[i], frames[lru_idx].page, lru_idx);
                frames[lru_idx].page = ref_string[i];
                frames[lru_idx].last_used = timestamp;
                stats.replacements = stats.replacements + 1;
            }
        }
    }
    // EXPECT: Page 1: FAULT (loaded to frame 0)
    // EXPECT: Page 2: FAULT (loaded to frame 1)
    // EXPECT: Page 3: FAULT (loaded to frame 2)
    // EXPECT: Page 4: FAULT (replaced page 1 in frame 0)
    // EXPECT: Page 1: FAULT (replaced page 2 in frame 1)
    // EXPECT: Page 2: FAULT (replaced page 3 in frame 2)
    // EXPECT: Page 5: FAULT (replaced page 4 in frame 0)
    // EXPECT: Page 1: HIT
    // EXPECT: Page 2: HIT
    // EXPECT: Page 3: FAULT (replaced page 5 in frame 0)
    printf("LRU: faults=%d hits=%d replacements=%d\n", stats.faults, stats.hits, stats.replacements); // EXPECT: LRU: faults=8 hits=2 replacements=5
}

int main() {
    struct Frame frames[3];
    int ref_string[10];

    // Reference string: 1 2 3 4 1 2 5 1 2 3
    ref_string[0] = 1; ref_string[1] = 2; ref_string[2] = 3;
    ref_string[3] = 4; ref_string[4] = 1; ref_string[5] = 2;
    ref_string[6] = 5; ref_string[7] = 1; ref_string[8] = 2;
    ref_string[9] = 3;

    fifo_replace(frames, 3, ref_string, 10);
    lru_replace(frames, 3, ref_string, 10);

    printf("Page replacement simulation complete\n"); // EXPECT: Page replacement simulation complete
    return 0;
}
