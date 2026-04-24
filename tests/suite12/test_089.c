int printf(const char *fmt, ...);

// TLB (Translation Lookaside Buffer) Simulation
// Simulates virtual-to-physical address translation with TLB caching

// Page table: maps virtual page number to physical frame number
// Page size: 256 bytes (8-bit offset)
// Virtual address space: 4096 bytes (16 pages)
// Physical address space: 2048 bytes (8 frames)

int page_table_frame[16];   // page -> frame mapping
int page_table_valid[16];   // 1 = page is mapped
int page_table_dirty[16];   // 1 = page has been written

// TLB: 4 entries, fully associative, LRU replacement
int tlb_vpn[4];     // virtual page number
int tlb_ppn[4];     // physical page number (frame)
int tlb_valid[4];
int tlb_age[4];     // for LRU: higher = more recently used
int tlb_counter;    // global counter for LRU

int tlb_hits;
int tlb_misses;
int page_faults;
int total_accesses;

void tlb_init() {
    int i;
    for (i = 0; i < 16; i++) {
        page_table_frame[i] = -1;
        page_table_valid[i] = 0;
        page_table_dirty[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        tlb_vpn[i] = -1;
        tlb_ppn[i] = -1;
        tlb_valid[i] = 0;
        tlb_age[i] = 0;
    }
    tlb_counter = 0;
    tlb_hits = 0;
    tlb_misses = 0;
    page_faults = 0;
    total_accesses = 0;
}

// Set up page table mappings
void map_page(int vpn, int frame) {
    if (vpn >= 0 && vpn < 16 && frame >= 0 && frame < 8) {
        page_table_frame[vpn] = frame;
        page_table_valid[vpn] = 1;
    }
}

int find_lru_entry() {
    int i;
    int min_age;
    int min_idx;
    min_age = tlb_age[0];
    min_idx = 0;
    for (i = 1; i < 4; i++) {
        if (tlb_valid[i] == 0) return i; // use empty slot first
        if (tlb_age[i] < min_age) {
            min_age = tlb_age[i];
            min_idx = i;
        }
    }
    // Also check if slot 0 is empty
    if (tlb_valid[0] == 0) return 0;
    return min_idx;
}

void tlb_insert(int vpn, int ppn) {
    int slot;
    slot = find_lru_entry();
    tlb_vpn[slot] = vpn;
    tlb_ppn[slot] = ppn;
    tlb_valid[slot] = 1;
    tlb_counter = tlb_counter + 1;
    tlb_age[slot] = tlb_counter;
}

// Translate virtual address to physical address
// Returns physical address or -1 on page fault
int translate(int vaddr) {
    int vpn;
    int offset;
    int i;
    int ppn;

    vpn = vaddr / 256;
    offset = vaddr % 256;
    total_accesses = total_accesses + 1;

    // Check TLB first
    for (i = 0; i < 4; i++) {
        if (tlb_valid[i] == 1 && tlb_vpn[i] == vpn) {
            // TLB hit
            tlb_hits = tlb_hits + 1;
            tlb_counter = tlb_counter + 1;
            tlb_age[i] = tlb_counter;
            ppn = tlb_ppn[i];
            return ppn * 256 + offset;
        }
    }

    // TLB miss - check page table
    tlb_misses = tlb_misses + 1;

    if (vpn < 0 || vpn >= 16 || page_table_valid[vpn] == 0) {
        page_faults = page_faults + 1;
        return -1;
    }

    ppn = page_table_frame[vpn];
    // Insert into TLB
    tlb_insert(vpn, ppn);
    return ppn * 256 + offset;
}

void tlb_flush() {
    int i;
    for (i = 0; i < 4; i++) {
        tlb_valid[i] = 0;
        tlb_vpn[i] = -1;
        tlb_ppn[i] = -1;
        tlb_age[i] = 0;
    }
}

int tlb_hit_rate_pct() {
    int total;
    total = tlb_hits + tlb_misses;
    if (total == 0) return 0;
    return (tlb_hits * 100) / total;
}

void print_tlb() {
    int i;
    for (i = 0; i < 4; i++) {
        if (tlb_valid[i] == 1) {
            printf("TLB[%d]: vpn=%d ppn=%d\n", i, tlb_vpn[i], tlb_ppn[i]);
        }
    }
}

int main() {
    int paddr;

    tlb_init();
    printf("TLB Simulation\n");
    // EXPECT: TLB Simulation

    // Set up page table: map pages 0-5 to frames
    map_page(0, 3);
    map_page(1, 0);
    map_page(2, 5);
    map_page(3, 1);
    map_page(4, 7);
    map_page(5, 2);
    printf("Page table configured: 6 pages mapped\n");
    // EXPECT: Page table configured: 6 pages mapped

    // Access virtual address 0 (page 0, offset 0)
    paddr = translate(0);
    printf("vaddr 0 -> paddr %d\n", paddr);
    // EXPECT: vaddr 0 -> paddr 768

    // Access virtual address 256 (page 1, offset 0)
    paddr = translate(256);
    printf("vaddr 256 -> paddr %d\n", paddr);
    // EXPECT: vaddr 256 -> paddr 0

    // Access within page 0 again (should be TLB hit)
    paddr = translate(100);
    printf("vaddr 100 -> paddr %d (TLB hit)\n", paddr);
    // EXPECT: vaddr 100 -> paddr 868 (TLB hit)

    printf("Hits: %d, Misses: %d\n", tlb_hits, tlb_misses);
    // EXPECT: Hits: 1, Misses: 2

    // Fill TLB with 4 entries (pages 0,1,2,3)
    translate(512);  // page 2
    translate(768);  // page 3
    printf("TLB after 4 pages:\n");
    // EXPECT: TLB after 4 pages:
    print_tlb();
    // EXPECT: TLB[0]: vpn=3 ppn=1
    // EXPECT: TLB[1]: vpn=0 ppn=3
    // EXPECT: TLB[2]: vpn=1 ppn=0
    // EXPECT: TLB[3]: vpn=2 ppn=5

    // Access page 4 - should evict LRU (page 1, least recently used after page 0 hit)
    paddr = translate(1024);
    printf("vaddr 1024 -> paddr %d\n", paddr);
    // EXPECT: vaddr 1024 -> paddr 1792

    printf("TLB after eviction:\n");
    // EXPECT: TLB after eviction:
    print_tlb();
    // EXPECT: TLB[0]: vpn=3 ppn=1
    // EXPECT: TLB[1]: vpn=0 ppn=3
    // EXPECT: TLB[2]: vpn=4 ppn=7
    // EXPECT: TLB[3]: vpn=2 ppn=5

    // Page fault: access unmapped page 10
    paddr = translate(2560);
    printf("vaddr 2560 -> paddr %d (page fault)\n", paddr);
    // EXPECT: vaddr 2560 -> paddr -1 (page fault)
    printf("Page faults: %d\n", page_faults);
    // EXPECT: Page faults: 1

    // Repeated access pattern for hit rate
    int j;
    for (j = 0; j < 10; j++) {
        translate(0);    // page 0
        translate(512);  // page 2
    }
    printf("After locality test: hits=%d misses=%d rate=%d\n",
           tlb_hits, tlb_misses, tlb_hit_rate_pct());
    // EXPECT: After locality test: hits=21 misses=6 rate=77

    // Flush TLB
    tlb_flush();
    printf("TLB flushed\n");
    // EXPECT: TLB flushed

    // Access after flush - all misses
    translate(0);
    translate(256);
    printf("After flush: hits=%d misses=%d\n", tlb_hits, tlb_misses);
    // EXPECT: After flush: hits=21 misses=8

    printf("Total accesses: %d\n", total_accesses);
    // EXPECT: Total accesses: 29

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
