int printf(const char *fmt, ...);
// EXPECT: Sales Report:\n  North: Q1=120 Q2=150 Q3=130 Q4=180 | Total=580 Avg=145 Best=Q4\n  South: Q1=200 Q2=180 Q3=220 Q4=190 | Total=790 Avg=197 Best=Q3\n  East: Q1=90 Q2=110 Q3=105 Q4=140 | Total=445 Avg=111 Best=Q4\n  West: Q1=170 Q2=165 Q3=190 Q4=200 | Total=725 Avg=181 Best=Q4\nGrand total: 2540\nCompany average: 635\nBest region: South (790)\nQuarter totals: Q1=580 Q2=605 Q3=645 Q4=710 \nEntries above company avg (158): 9

#define NUM_REGIONS 4
#define NUM_QUARTERS 4

struct RegionData {
    char *name;
    int sales[NUM_QUARTERS];
    int total;
    int avg;
    int best_quarter;
};

void region_compute(struct RegionData *r) {
    r->total = 0;
    r->best_quarter = 0;
    int i;
    for (i = 0; i < NUM_QUARTERS; i++) {
        r->total = r->total + r->sales[i];
        if (r->sales[i] > r->sales[r->best_quarter]) {
            r->best_quarter = i;
        }
    }
    r->avg = r->total / NUM_QUARTERS;
}

void region_print(struct RegionData *r) {
    printf("  %s: Q1=%d Q2=%d Q3=%d Q4=%d | Total=%d Avg=%d Best=Q%d\n",
           r->name, r->sales[0], r->sales[1], r->sales[2], r->sales[3],
           r->total, r->avg, r->best_quarter + 1);
}

int main(void) {
    struct RegionData regions[NUM_REGIONS];

    regions[0].name = "North";
    regions[0].sales[0] = 120; regions[0].sales[1] = 150;
    regions[0].sales[2] = 130; regions[0].sales[3] = 180;

    regions[1].name = "South";
    regions[1].sales[0] = 200; regions[1].sales[1] = 180;
    regions[1].sales[2] = 220; regions[1].sales[3] = 190;

    regions[2].name = "East";
    regions[2].sales[0] = 90;  regions[2].sales[1] = 110;
    regions[2].sales[2] = 105; regions[2].sales[3] = 140;

    regions[3].name = "West";
    regions[3].sales[0] = 170; regions[3].sales[1] = 165;
    regions[3].sales[2] = 190; regions[3].sales[3] = 200;

    int i;
    for (i = 0; i < NUM_REGIONS; i++) {
        region_compute(&regions[i]);
    }

    printf("Sales Report:\n");
    for (i = 0; i < NUM_REGIONS; i++) {
        region_print(&regions[i]);
    }

    int grand_total = 0;
    int best_region = 0;
    for (i = 0; i < NUM_REGIONS; i++) {
        grand_total = grand_total + regions[i].total;
        if (regions[i].total > regions[best_region].total) best_region = i;
    }

    printf("Grand total: %d\n", grand_total);
    printf("Company average: %d\n", grand_total / NUM_REGIONS);
    printf("Best region: %s (%d)\n", regions[best_region].name, regions[best_region].total);

    printf("Quarter totals: ");
    int q;
    for (q = 0; q < NUM_QUARTERS; q++) {
        int qt = 0;
        for (i = 0; i < NUM_REGIONS; i++) {
            qt = qt + regions[i].sales[q];
        }
        printf("Q%d=%d ", q + 1, qt);
    }
    printf("\n");

    int above_avg = 0;
    int company_avg = grand_total / (NUM_REGIONS * NUM_QUARTERS);
    for (i = 0; i < NUM_REGIONS; i++) {
        for (q = 0; q < NUM_QUARTERS; q++) {
            if (regions[i].sales[q] > company_avg) above_avg++;
        }
    }
    printf("Entries above company avg (%d): %d\n", company_avg, above_avg);

    return 0;
}
