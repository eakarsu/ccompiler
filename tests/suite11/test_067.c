int printf(const char *fmt, ...);

// Budget allocator: distribute budget across categories
// Supports fixed allocations, percentage-based, and priority-based

enum AllocType { FIXED, PERCENT, PRIORITY };

struct Category {
    char *name;
    int alloc_type;
    int amount_or_pct;
    int min_cents;
    int allocated;
};

struct Budget {
    int total_cents;
    struct Category categories[8];
    int num_categories;
    int remaining;
};

void init_budget(struct Budget *b, int total) {
    b->total_cents = total;
    b->num_categories = 0;
    b->remaining = total;
}

void add_category(struct Budget *b, char *name, int type, int amount, int min_c) {
    int idx = b->num_categories;
    b->categories[idx].name = name;
    b->categories[idx].alloc_type = type;
    b->categories[idx].amount_or_pct = amount;
    b->categories[idx].min_cents = min_c;
    b->categories[idx].allocated = 0;
    b->num_categories = b->num_categories + 1;
}

void allocate_budget(struct Budget *b) {
    int remaining = b->total_cents;
    int i;
    int alloc;
    int total_priority = 0;
    int priority_pool;

    // Pass 1: Fixed allocations
    for (i = 0; i < b->num_categories; i++) {
        if (b->categories[i].alloc_type == FIXED) {
            alloc = b->categories[i].amount_or_pct;
            if (alloc > remaining) alloc = remaining;
            b->categories[i].allocated = alloc;
            remaining = remaining - alloc;
        }
    }

    // Pass 2: Percentage allocations (of original total)
    for (i = 0; i < b->num_categories; i++) {
        if (b->categories[i].alloc_type == PERCENT) {
            alloc = (b->total_cents * b->categories[i].amount_or_pct) / 10000;
            if (alloc < b->categories[i].min_cents) {
                alloc = b->categories[i].min_cents;
            }
            if (alloc > remaining) alloc = remaining;
            b->categories[i].allocated = alloc;
            remaining = remaining - alloc;
        }
    }

    // Pass 3: Priority-based (split remaining proportionally)
    for (i = 0; i < b->num_categories; i++) {
        if (b->categories[i].alloc_type == PRIORITY) {
            total_priority = total_priority + b->categories[i].amount_or_pct;
        }
    }

    priority_pool = remaining;
    for (i = 0; i < b->num_categories; i++) {
        if (b->categories[i].alloc_type == PRIORITY && total_priority > 0) {
            alloc = (priority_pool * b->categories[i].amount_or_pct) / total_priority;
            if (alloc < b->categories[i].min_cents) {
                alloc = b->categories[i].min_cents;
            }
            if (alloc > remaining) alloc = remaining;
            b->categories[i].allocated = alloc;
            remaining = remaining - alloc;
        }
    }

    b->remaining = remaining;
}

void print_budget(struct Budget *b) {
    int i;
    int pct;
    printf("Budget: %d cents\n", b->total_cents);
    for (i = 0; i < b->num_categories; i++) {
        pct = 0;
        if (b->total_cents > 0) {
            pct = (b->categories[i].allocated * 10000) / b->total_cents;
        }
        printf("  %s: %d cents (%d bps)\n",
               b->categories[i].name,
               b->categories[i].allocated,
               pct);
    }
    printf("  Unallocated: %d cents\n", b->remaining);
}

int check_minimums(struct Budget *b) {
    int i;
    int violations = 0;
    for (i = 0; i < b->num_categories; i++) {
        if (b->categories[i].allocated < b->categories[i].min_cents) {
            violations = violations + 1;
        }
    }
    return violations;
}

int main() {
    struct Budget b1;
    struct Budget b2;

    printf("=== Budget Allocator ===\n");
    // EXPECT: === Budget Allocator ===

    // Budget 1: Monthly household budget $5000
    init_budget(&b1, 500000);
    add_category(&b1, "Rent", FIXED, 150000, 150000);
    add_category(&b1, "Utilities", FIXED, 20000, 15000);
    add_category(&b1, "Groceries", PERCENT, 1500, 30000);  // 15% of total
    add_category(&b1, "Transport", PERCENT, 1000, 10000);   // 10% of total
    add_category(&b1, "Savings", PRIORITY, 3, 25000);       // priority 3
    add_category(&b1, "Entertainment", PRIORITY, 2, 5000);  // priority 2
    add_category(&b1, "Emergency", PRIORITY, 1, 10000);     // priority 1

    allocate_budget(&b1);
    print_budget(&b1);
    // EXPECT: Budget: 500000 cents
    // EXPECT:   Rent: 150000 cents (3000 bps)
    // EXPECT:   Utilities: 20000 cents (400 bps)
    // EXPECT:   Groceries: 75000 cents (1500 bps)
    // EXPECT:   Transport: 50000 cents (1000 bps)
    // EXPECT:   Savings: 102500 cents (2050 bps)
    // EXPECT:   Entertainment: 68333 cents (1366 bps)
    // EXPECT:   Emergency: 34166 cents (683 bps)
    // EXPECT:   Unallocated: 1 cents

    int violations = check_minimums(&b1);
    printf("Minimum violations: %d\n", violations);
    // EXPECT: Minimum violations: 0

    // Budget 2: Small budget where minimums matter
    printf("--- Tight Budget ---\n");
    // EXPECT: --- Tight Budget ---
    init_budget(&b2, 50000);
    add_category(&b2, "Food", FIXED, 20000, 20000);
    add_category(&b2, "Bills", FIXED, 15000, 15000);
    add_category(&b2, "Savings", PERCENT, 2000, 5000);   // 20% = 10000 but min 5000
    add_category(&b2, "Fun", PRIORITY, 1, 2000);

    allocate_budget(&b2);
    print_budget(&b2);
    // EXPECT: Budget: 50000 cents
    // EXPECT:   Food: 20000 cents (4000 bps)
    // EXPECT:   Bills: 15000 cents (3000 bps)
    // EXPECT:   Savings: 10000 cents (2000 bps)
    // EXPECT:   Fun: 5000 cents (1000 bps)
    // EXPECT:   Unallocated: 0 cents

    // Test: Total allocated should equal budget
    int total_alloc = 0;
    int i;
    for (i = 0; i < b2.num_categories; i++) {
        total_alloc = total_alloc + b2.categories[i].allocated;
    }
    total_alloc = total_alloc + b2.remaining;
    printf("Total check: %d == %d ? %s\n",
           total_alloc, b2.total_cents,
           total_alloc == b2.total_cents ? "YES" : "NO");
    // EXPECT: Total check: 50000 == 50000 ? YES

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
