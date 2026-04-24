int printf(const char *fmt, ...);

// Single-level page table simulation
// Virtual address space: 1024 units, page size: 64
// So 16 pages (1024/64), each mapping to a physical frame

struct PageEntry {
    int valid;
    int frame;
    int dirty;
    int accessed;
};

struct PageTable {
    struct PageEntry entries[16];
    int num_pages;
    int page_size;
    int next_frame;
    int page_faults;
    int hits;
};

void pt_init(struct PageTable *pt) {
    int i;
    pt->num_pages = 16;
    pt->page_size = 64;
    pt->next_frame = 0;
    pt->page_faults = 0;
    pt->hits = 0;
    for (i = 0; i < 16; i++) {
        pt->entries[i].valid = 0;
        pt->entries[i].frame = -1;
        pt->entries[i].dirty = 0;
        pt->entries[i].accessed = 0;
    }
}

int translate(struct PageTable *pt, int vaddr, int write) {
    int page = vaddr / pt->page_size;
    int offset = vaddr - (page * pt->page_size);

    if (page < 0 || page >= pt->num_pages) {
        return -1; // invalid address
    }

    if (!pt->entries[page].valid) {
        // Page fault - allocate a frame
        pt->page_faults++;
        pt->entries[page].valid = 1;
        pt->entries[page].frame = pt->next_frame;
        pt->next_frame++;
        pt->entries[page].dirty = 0;
        pt->entries[page].accessed = 0;
    } else {
        pt->hits++;
    }

    pt->entries[page].accessed = 1;
    if (write) {
        pt->entries[page].dirty = 1;
    }

    int physical = pt->entries[page].frame * pt->page_size + offset;
    return physical;
}

void invalidate_page(struct PageTable *pt, int page) {
    if (page >= 0 && page < pt->num_pages) {
        pt->entries[page].valid = 0;
        pt->entries[page].frame = -1;
        pt->entries[page].dirty = 0;
        pt->entries[page].accessed = 0;
    }
}

int count_valid(struct PageTable *pt) {
    int c = 0;
    int i;
    for (i = 0; i < pt->num_pages; i++) {
        if (pt->entries[i].valid) c++;
    }
    return c;
}

int count_dirty(struct PageTable *pt) {
    int c = 0;
    int i;
    for (i = 0; i < pt->num_pages; i++) {
        if (pt->entries[i].valid && pt->entries[i].dirty) c++;
    }
    return c;
}

int main(void) {
    struct PageTable pt;
    pt_init(&pt);

    // Access address 0 (page 0)
    int p1 = translate(&pt, 0, 0);
    printf("vaddr=0 -> paddr=%d (fault)\n", p1);
    // EXPECT: vaddr=0 -> paddr=0 (fault)

    // Access address 65 (page 1, offset 1)
    int p2 = translate(&pt, 65, 0);
    printf("vaddr=65 -> paddr=%d\n", p2);
    // EXPECT: vaddr=65 -> paddr=65

    // Access address 10 (page 0 again - hit)
    int p3 = translate(&pt, 10, 0);
    printf("vaddr=10 -> paddr=%d (hit)\n", p3);
    // EXPECT: vaddr=10 -> paddr=10 (hit)

    printf("faults=%d hits=%d\n", pt.page_faults, pt.hits);
    // EXPECT: faults=2 hits=1

    // Write to address 130 (page 2, offset 2)
    int p4 = translate(&pt, 130, 1);
    printf("write vaddr=130 -> paddr=%d\n", p4);
    // EXPECT: write vaddr=130 -> paddr=130

    printf("dirty pages: %d\n", count_dirty(&pt));
    // EXPECT: dirty pages: 1

    // Access all 16 pages
    int i;
    for (i = 0; i < 16; i++) {
        translate(&pt, i * 64, 0);
    }
    printf("after full scan: faults=%d valid=%d\n", pt.page_faults, count_valid(&pt));
    // EXPECT: after full scan: faults=16 valid=16

    // Invalidate page 5
    invalidate_page(&pt, 5);
    printf("after invalidate 5: valid=%d\n", count_valid(&pt));
    // EXPECT: after invalidate 5: valid=15

    // Re-access page 5 - should fault again
    int old_faults = pt.page_faults;
    translate(&pt, 320, 0); // 320 / 64 = 5
    printf("re-access page 5: new fault=%d\n", pt.page_faults - old_faults);
    // EXPECT: re-access page 5: new fault=1

    // Write to multiple pages
    translate(&pt, 0, 1);
    translate(&pt, 128, 1);
    translate(&pt, 256, 1);
    translate(&pt, 384, 1);
    printf("dirty after writes: %d\n", count_dirty(&pt));
    // EXPECT: dirty after writes: 4

    // Test boundary addresses
    int b1 = translate(&pt, 63, 0);  // last byte of page 0
    int b2 = translate(&pt, 64, 0);  // first byte of page 1
    printf("addr 63 -> page 0 frame: %d\n", pt.entries[0].frame);
    // EXPECT: addr 63 -> page 0 frame: 0
    printf("addr 64 -> page 1 frame: %d\n", pt.entries[1].frame);
    // EXPECT: addr 64 -> page 1 frame: 1

    // Test TLB-like hit rate
    int total = pt.page_faults + pt.hits;
    printf("total accesses: %d\n", total);
    // EXPECT: total accesses: 27

    // Bulk invalidation
    for (i = 0; i < 8; i++) {
        invalidate_page(&pt, i);
    }
    printf("after bulk invalidate: valid=%d\n", count_valid(&pt));
    // EXPECT: after bulk invalidate: valid=8

    // Verify frame assignment after invalidation
    old_faults = pt.page_faults;
    translate(&pt, 0, 0); // page 0 re-fault
    printf("page 0 new frame: %d\n", pt.entries[0].frame);
    // EXPECT: page 0 new frame: 17

    printf("total faults: %d\n", pt.page_faults);
    // EXPECT: total faults: 18

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
