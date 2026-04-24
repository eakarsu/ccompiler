int printf(const char *fmt, ...);

/* Simulated memory with page table */
/* Page size = 256 bytes, 16 pages = 4096 bytes total */

struct Page {
    int data[64];
    int valid;
    int dirty;
    int access_count;
};

struct PageTable {
    int virtual_to_physical[16];
    int present[16];
};

struct MemorySystem {
    struct Page physical_pages[8];
    struct PageTable page_table;
    int page_faults;
    int total_accesses;
    int next_victim;
};

void mem_init(struct MemorySystem *mem) {
    int i;
    mem->page_faults = 0;
    mem->total_accesses = 0;
    mem->next_victim = 0;
    i = 0;
    while (i < 8) {
        mem->physical_pages[i].valid = 0;
        mem->physical_pages[i].dirty = 0;
        mem->physical_pages[i].access_count = 0;
        i = i + 1;
    }
    i = 0;
    while (i < 16) {
        mem->page_table.virtual_to_physical[i] = -1;
        mem->page_table.present[i] = 0;
        i = i + 1;
    }
}

int find_free_frame(struct MemorySystem *mem) {
    int i;
    i = 0;
    while (i < 8) {
        if (mem->physical_pages[i].valid == 0) return i;
        i = i + 1;
    }
    return -1;
}

/* FIFO page replacement */
int evict_page(struct MemorySystem *mem) {
    int victim;
    int i;
    victim = mem->next_victim;
    mem->next_victim = (mem->next_victim + 1) % 8;
    /* Invalidate old mapping */
    i = 0;
    while (i < 16) {
        if (mem->page_table.present[i] && mem->page_table.virtual_to_physical[i] == victim) {
            mem->page_table.present[i] = 0;
            mem->page_table.virtual_to_physical[i] = -1;
            break;
        }
        i = i + 1;
    }
    mem->physical_pages[victim].valid = 0;
    mem->physical_pages[victim].dirty = 0;
    mem->physical_pages[victim].access_count = 0;
    return victim;
}

int translate_address(struct MemorySystem *mem, int virtual_addr) {
    int page_num;
    int offset;
    int frame;
    page_num = virtual_addr / 256;
    offset = virtual_addr % 256;
    mem->total_accesses = mem->total_accesses + 1;
    if (page_num < 0 || page_num >= 16) return -1;
    if (mem->page_table.present[page_num] == 0) {
        /* Page fault */
        mem->page_faults = mem->page_faults + 1;
        frame = find_free_frame(mem);
        if (frame < 0) {
            frame = evict_page(mem);
        }
        mem->physical_pages[frame].valid = 1;
        mem->physical_pages[frame].dirty = 0;
        mem->physical_pages[frame].access_count = 0;
        mem->page_table.virtual_to_physical[page_num] = frame;
        mem->page_table.present[page_num] = 1;
    }
    frame = mem->page_table.virtual_to_physical[page_num];
    mem->physical_pages[frame].access_count = mem->physical_pages[frame].access_count + 1;
    return frame * 256 + offset;
}

int mem_write(struct MemorySystem *mem, int virtual_addr, int value) {
    int phys;
    int frame;
    int offset;
    phys = translate_address(mem, virtual_addr);
    if (phys < 0) return -1;
    frame = phys / 256;
    offset = (virtual_addr % 256) / 4;
    if (offset >= 64) return -1;
    mem->physical_pages[frame].data[offset] = value;
    mem->physical_pages[frame].dirty = 1;
    return 0;
}

int mem_read(struct MemorySystem *mem, int virtual_addr) {
    int phys;
    int frame;
    int offset;
    phys = translate_address(mem, virtual_addr);
    if (phys < 0) return -1;
    frame = phys / 256;
    offset = (virtual_addr % 256) / 4;
    if (offset >= 64) return -1;
    return mem->physical_pages[frame].data[offset];
}

int count_present_pages(struct MemorySystem *mem) {
    int i;
    int c;
    c = 0;
    i = 0;
    while (i < 16) {
        if (mem->page_table.present[i]) c = c + 1;
        i = i + 1;
    }
    return c;
}

int count_dirty_frames(struct MemorySystem *mem) {
    int i;
    int c;
    c = 0;
    i = 0;
    while (i < 8) {
        if (mem->physical_pages[i].valid && mem->physical_pages[i].dirty) c = c + 1;
        i = i + 1;
    }
    return c;
}

int total_access_count(struct MemorySystem *mem) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < 8) {
        total = total + mem->physical_pages[i].access_count;
        i = i + 1;
    }
    return total;
}

int main(void) {
    struct MemorySystem mem;
    int phys;
    int val;

    mem_init(&mem);

    // EXPECT: 0
    printf("%d\n", mem.page_faults);
    // EXPECT: 0
    printf("%d\n", mem.total_accesses);

    /* Access page 0 (addr 0): page fault, maps to frame 0 */
    phys = translate_address(&mem, 0);
    // EXPECT: 0
    printf("%d\n", phys);
    // EXPECT: 1
    printf("%d\n", mem.page_faults);

    /* Access page 0 again (addr 4): no fault */
    phys = translate_address(&mem, 4);
    // EXPECT: 4
    printf("%d\n", phys);
    // EXPECT: 1
    printf("%d\n", mem.page_faults);

    /* Access page 1 (addr 256): page fault, maps to frame 1 */
    phys = translate_address(&mem, 256);
    // EXPECT: 256
    printf("%d\n", phys);
    // EXPECT: 2
    printf("%d\n", mem.page_faults);

    // EXPECT: 3
    printf("%d\n", mem.total_accesses);

    /* Write and read back */
    mem_write(&mem, 0, 42);
    val = mem_read(&mem, 0);
    // EXPECT: 42
    printf("%d\n", val);

    mem_write(&mem, 256, 99);
    val = mem_read(&mem, 256);
    // EXPECT: 99
    printf("%d\n", val);

    /* Write to different offsets on same page */
    mem_write(&mem, 4, 100);
    mem_write(&mem, 8, 200);
    val = mem_read(&mem, 4);
    // EXPECT: 100
    printf("%d\n", val);
    val = mem_read(&mem, 8);
    // EXPECT: 200
    printf("%d\n", val);

    /* Present pages: 0 and 1 */
    // EXPECT: 2
    printf("%d\n", count_present_pages(&mem));

    /* Dirty frames (we wrote to pages 0 and 1) */
    // EXPECT: 2
    printf("%d\n", count_dirty_frames(&mem));

    /* Fill up all 8 physical frames: pages 2-7 */
    translate_address(&mem, 512);
    translate_address(&mem, 768);
    translate_address(&mem, 1024);
    translate_address(&mem, 1280);
    translate_address(&mem, 1536);
    translate_address(&mem, 1792);

    // EXPECT: 8
    printf("%d\n", count_present_pages(&mem));
    // EXPECT: 8
    printf("%d\n", mem.page_faults);

    /* Next access causes eviction (page 8, addr 2048) */
    translate_address(&mem, 2048);
    // EXPECT: 9
    printf("%d\n", mem.page_faults);

    /* Page 0 was evicted (FIFO), so 8 pages present (one replaced) */
    // EXPECT: 8
    printf("%d\n", count_present_pages(&mem));

    /* Access page 0 again -> page fault since evicted */
    translate_address(&mem, 0);
    // EXPECT: 10
    printf("%d\n", mem.page_faults);

    return 0;
}
