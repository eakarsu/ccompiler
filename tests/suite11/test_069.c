int printf(const char *fmt, ...);

// Depreciation calculator: straight-line method
// Tracks asset value over time for accounting purposes

struct Asset {
    int id;
    char *name;
    int purchase_cost_cents;
    int salvage_value_cents;
    int useful_life_years;
};

struct DepreciationSchedule {
    int year;
    int beginning_value;
    int depreciation_expense;
    int accumulated_depreciation;
    int ending_value;
};

int annual_depreciation(struct Asset *a) {
    int depreciable = a->purchase_cost_cents - a->salvage_value_cents;
    return depreciable / a->useful_life_years;
}

void compute_schedule(struct Asset *a, struct DepreciationSchedule *sched, int *count) {
    int annual = annual_depreciation(a);
    int accum = 0;
    int book_value = a->purchase_cost_cents;
    int i;

    for (i = 0; i < a->useful_life_years; i++) {
        sched[i].year = i + 1;
        sched[i].beginning_value = book_value;
        sched[i].depreciation_expense = annual;
        accum = accum + annual;
        sched[i].accumulated_depreciation = accum;
        book_value = a->purchase_cost_cents - accum;
        sched[i].ending_value = book_value;
    }
    *count = a->useful_life_years;
}

int book_value_at_year(struct Asset *a, int year) {
    int annual = annual_depreciation(a);
    int accum = annual * year;
    int bv = a->purchase_cost_cents - accum;
    if (bv < a->salvage_value_cents) bv = a->salvage_value_cents;
    return bv;
}

int is_fully_depreciated(struct Asset *a, int year) {
    return year >= a->useful_life_years;
}

int gain_on_sale(struct Asset *a, int year_sold, int sale_price) {
    int bv = book_value_at_year(a, year_sold);
    return sale_price - bv;
}

void print_asset(struct Asset *a) {
    printf("Asset: %s (ID:%d)\n", a->name, a->id);
    printf("  Cost: %d cents, Salvage: %d cents, Life: %d years\n",
           a->purchase_cost_cents, a->salvage_value_cents, a->useful_life_years);
    printf("  Annual depreciation: %d cents\n", annual_depreciation(a));
}

int main() {
    struct Asset a1;
    struct Asset a2;
    struct Asset a3;
    struct DepreciationSchedule sched[10];
    int count;
    int i;
    int gos;

    printf("=== Depreciation Calculator ===\n");
    // EXPECT: === Depreciation Calculator ===

    // Asset 1: Office computer
    a1.id = 1;
    a1.name = "Computer";
    a1.purchase_cost_cents = 120000;
    a1.salvage_value_cents = 20000;
    a1.useful_life_years = 5;

    print_asset(&a1);
    // EXPECT: Asset: Computer (ID:1)
    // EXPECT:   Cost: 120000 cents, Salvage: 20000 cents, Life: 5 years
    // EXPECT:   Annual depreciation: 20000 cents

    printf("--- Schedule ---\n");
    // EXPECT: --- Schedule ---
    compute_schedule(&a1, sched, &count);
    for (i = 0; i < count; i++) {
        printf("Yr %d: begin=%d dep=%d accum=%d end=%d\n",
               sched[i].year,
               sched[i].beginning_value,
               sched[i].depreciation_expense,
               sched[i].accumulated_depreciation,
               sched[i].ending_value);
    }
    // EXPECT: Yr 1: begin=120000 dep=20000 accum=20000 end=100000
    // EXPECT: Yr 2: begin=100000 dep=20000 accum=40000 end=80000
    // EXPECT: Yr 3: begin=80000 dep=20000 accum=60000 end=60000
    // EXPECT: Yr 4: begin=60000 dep=20000 accum=80000 end=40000
    // EXPECT: Yr 5: begin=40000 dep=20000 accum=100000 end=20000

    // Verify ending value equals salvage
    printf("Final value equals salvage: %s\n",
           sched[count - 1].ending_value == a1.salvage_value_cents ? "YES" : "NO");
    // EXPECT: Final value equals salvage: YES

    // Asset 2: Delivery vehicle
    printf("---\n");
    // EXPECT: ---
    a2.id = 2;
    a2.name = "Vehicle";
    a2.purchase_cost_cents = 3500000;
    a2.salvage_value_cents = 500000;
    a2.useful_life_years = 7;

    print_asset(&a2);
    // EXPECT: Asset: Vehicle (ID:2)
    // EXPECT:   Cost: 3500000 cents, Salvage: 500000 cents, Life: 7 years
    // EXPECT:   Annual depreciation: 428571 cents

    // Book value at various years
    printf("Book value year 0: %d\n", book_value_at_year(&a2, 0));
    // EXPECT: Book value year 0: 3500000
    printf("Book value year 3: %d\n", book_value_at_year(&a2, 3));
    // EXPECT: Book value year 3: 2214287
    printf("Book value year 7: %d\n", book_value_at_year(&a2, 7));
    // EXPECT: Book value year 7: 500003

    // Gain/loss on sale
    printf("--- Sale Scenarios ---\n");
    // EXPECT: --- Sale Scenarios ---
    gos = gain_on_sale(&a2, 3, 2500000);
    printf("Sell at year 3 for 2500000: gain=%d\n", gos);
    // EXPECT: Sell at year 3 for 2500000: gain=285713
    gos = gain_on_sale(&a2, 3, 2000000);
    printf("Sell at year 3 for 2000000: gain=%d\n", gos);
    // EXPECT: Sell at year 3 for 2000000: gain=-214287
    gos = gain_on_sale(&a2, 5, 1000000);
    printf("Sell at year 5 for 1000000: gain=%d\n", gos);
    // EXPECT: Sell at year 5 for 1000000: gain=-357145

    // Asset 3: Building
    printf("---\n");
    // EXPECT: ---
    a3.id = 3;
    a3.name = "Building";
    a3.purchase_cost_cents = 50000000;
    a3.salvage_value_cents = 10000000;
    a3.useful_life_years = 10;

    print_asset(&a3);
    // EXPECT: Asset: Building (ID:3)
    // EXPECT:   Cost: 50000000 cents, Salvage: 10000000 cents, Life: 10 years
    // EXPECT:   Annual depreciation: 4000000 cents

    // Multi-asset total depreciation
    printf("--- Total Depreciation Year 3 ---\n");
    // EXPECT: --- Total Depreciation Year 3 ---
    int total_dep = annual_depreciation(&a1) * 3 +
                    annual_depreciation(&a2) * 3 +
                    annual_depreciation(&a3) * 3;
    printf("Total accumulated (3 years): %d cents\n", total_dep);
    // EXPECT: Total accumulated (3 years): 13345713 cents

    // Check fully depreciated
    printf("Computer depreciated at yr 5: %s\n",
           is_fully_depreciated(&a1, 5) ? "YES" : "NO");
    // EXPECT: Computer depreciated at yr 5: YES
    printf("Computer depreciated at yr 3: %s\n",
           is_fully_depreciated(&a1, 3) ? "YES" : "NO");
    // EXPECT: Computer depreciated at yr 3: NO

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
