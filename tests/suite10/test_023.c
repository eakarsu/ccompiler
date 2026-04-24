int printf(const char *fmt, ...);

// Page Table Simulation: single-level and two-level page tables

struct PageEntry {
    int frame;
    int valid;
    int dirty;
    int referenced;
};

struct PageTable {
    struct PageEntry entries[16];
    int num_pages;
    int page_size;
};

void init_page_table(struct PageTable *pt, int num_pages, int page_size) {
    int i;
    pt->num_pages = num_pages;
    pt->page_size = page_size;
    for (i = 0; i < num_pages; i = i + 1) {
        pt->entries[i].frame = -1;
        pt->entries[i].valid = 0;
        pt->entries[i].dirty = 0;
        pt->entries[i].referenced = 0;
    }
}

void map_page(struct PageTable *pt, int page, int frame) {
    if (page >= 0 && page < pt->num_pages) {
        pt->entries[page].frame = frame;
        pt->entries[page].valid = 1;
        pt->entries[page].referenced = 1;
    }
}

int translate_address(struct PageTable *pt, int virtual_addr) {
    int page;
    int offset;
    page = virtual_addr / pt->page_size;
    offset = virtual_addr - (page * pt->page_size);
    if (page < 0 || page >= pt->num_pages) {
        return -1;
    }
    if (pt->entries[page].valid == 0) {
        return -2; // page fault
    }
    pt->entries[page].referenced = 1;
    return pt->entries[page].frame * pt->page_size + offset;
}

void mark_dirty(struct PageTable *pt, int page) {
    if (page >= 0 && page < pt->num_pages && pt->entries[page].valid) {
        pt->entries[page].dirty = 1;
    }
}

// Two-level page table
struct L2PageTable {
    struct PageEntry entries[4];
    int valid;
};

struct TwoLevelPT {
    struct L2PageTable l2[4]; // 4 L1 entries, each pointing to L2 with 4 entries
    int page_size;
};

void init_two_level(struct TwoLevelPT *pt, int page_size) {
    int i;
    int j;
    pt->page_size = page_size;
    for (i = 0; i < 4; i = i + 1) {
        pt->l2[i].valid = 0;
        for (j = 0; j < 4; j = j + 1) {
            pt->l2[i].entries[j].frame = -1;
            pt->l2[i].entries[j].valid = 0;
            pt->l2[i].entries[j].dirty = 0;
            pt->l2[i].entries[j].referenced = 0;
        }
    }
}

void map_two_level(struct TwoLevelPT *pt, int page, int frame) {
    int l1_idx;
    int l2_idx;
    l1_idx = page / 4;
    l2_idx = page - (l1_idx * 4);
    if (l1_idx >= 0 && l1_idx < 4 && l2_idx >= 0 && l2_idx < 4) {
        pt->l2[l1_idx].valid = 1;
        pt->l2[l1_idx].entries[l2_idx].frame = frame;
        pt->l2[l1_idx].entries[l2_idx].valid = 1;
    }
}

int translate_two_level(struct TwoLevelPT *pt, int virtual_addr) {
    int page;
    int offset;
    int l1_idx;
    int l2_idx;
    page = virtual_addr / pt->page_size;
    offset = virtual_addr - (page * pt->page_size);
    l1_idx = page / 4;
    l2_idx = page - (l1_idx * 4);
    if (l1_idx < 0 || l1_idx >= 4) return -1;
    if (pt->l2[l1_idx].valid == 0) return -2;
    if (l2_idx < 0 || l2_idx >= 4) return -1;
    if (pt->l2[l1_idx].entries[l2_idx].valid == 0) return -2;
    return pt->l2[l1_idx].entries[l2_idx].frame * pt->page_size + offset;
}

int main() {
    struct PageTable pt;
    struct TwoLevelPT tpt;
    int phys;
    int i;
    int dirty_count;
    int valid_count;

    printf("=== Single-Level Page Table ===\n"); // EXPECT: === Single-Level Page Table ===
    init_page_table(&pt, 8, 256);

    map_page(&pt, 0, 5);
    map_page(&pt, 1, 3);
    map_page(&pt, 3, 7);
    map_page(&pt, 5, 2);

    phys = translate_address(&pt, 100);
    printf("VA 100 -> PA %d\n", phys); // EXPECT: VA 100 -> PA 1380

    phys = translate_address(&pt, 300);
    printf("VA 300 -> PA %d\n", phys); // EXPECT: VA 300 -> PA 812

    phys = translate_address(&pt, 800);
    printf("VA 800 -> PA %d\n", phys); // EXPECT: VA 800 -> PA 1824

    phys = translate_address(&pt, 512);
    printf("VA 512 -> fault=%d\n", phys); // EXPECT: VA 512 -> fault=-2

    mark_dirty(&pt, 0);
    mark_dirty(&pt, 3);

    dirty_count = 0;
    valid_count = 0;
    for (i = 0; i < 8; i = i + 1) {
        if (pt.entries[i].valid) valid_count = valid_count + 1;
        if (pt.entries[i].dirty) dirty_count = dirty_count + 1;
    }
    printf("Valid pages: %d\n", valid_count); // EXPECT: Valid pages: 4
    printf("Dirty pages: %d\n", dirty_count); // EXPECT: Dirty pages: 2

    printf("=== Two-Level Page Table ===\n"); // EXPECT: === Two-Level Page Table ===
    init_two_level(&tpt, 128);

    map_two_level(&tpt, 0, 10);
    map_two_level(&tpt, 5, 20);
    map_two_level(&tpt, 9, 15);
    map_two_level(&tpt, 15, 8);

    phys = translate_two_level(&tpt, 50);
    printf("2L: VA 50 -> PA %d\n", phys); // EXPECT: 2L: VA 50 -> PA 1330

    phys = translate_two_level(&tpt, 700);
    printf("2L: VA 700 -> PA %d\n", phys); // EXPECT: 2L: VA 700 -> PA 2620

    phys = translate_two_level(&tpt, 1200);
    printf("2L: VA 1200 -> PA %d\n", phys); // EXPECT: 2L: VA 1200 -> PA 1968

    phys = translate_two_level(&tpt, 1960);
    printf("2L: VA 1960 -> PA %d\n", phys); // EXPECT: 2L: VA 1960 -> PA 1064

    phys = translate_two_level(&tpt, 256);
    printf("2L: VA 256 -> fault=%d\n", phys); // EXPECT: 2L: VA 256 -> fault=-2

    printf("Page table simulation complete\n"); // EXPECT: Page table simulation complete
    return 0;
}
