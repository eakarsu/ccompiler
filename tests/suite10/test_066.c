int printf(const char *fmt, ...);

// GROUP BY simulation

struct Row {
    int id;
    int group_key;
    int value;
};

struct Table {
    struct Row rows[24];
    int nrows;
};

struct GroupResult {
    int key;
    int count;
    int sum;
    int min_val;
    int max_val;
};

struct GroupByResult {
    struct GroupResult groups[10];
    int ngroups;
};

void table_add(struct Table *t, int id, int gk, int val) {
    t->rows[t->nrows].id = id;
    t->rows[t->nrows].group_key = gk;
    t->rows[t->nrows].value = val;
    t->nrows++;
}

// Find distinct group keys
int find_distinct_keys(struct Table *t, int *keys) {
    int nkeys = 0;
    int i;
    int j;
    for (i = 0; i < t->nrows; i++) {
        int found = 0;
        for (j = 0; j < nkeys; j++) {
            if (keys[j] == t->rows[i].group_key) {
                found = 1;
                break;
            }
        }
        if (!found) {
            keys[nkeys] = t->rows[i].group_key;
            nkeys++;
        }
    }
    return nkeys;
}

void group_by(struct Table *t, struct GroupByResult *gbr) {
    int keys[10];
    int nkeys = find_distinct_keys(t, keys);

    gbr->ngroups = nkeys;

    int g;
    for (g = 0; g < nkeys; g++) {
        gbr->groups[g].key = keys[g];
        gbr->groups[g].count = 0;
        gbr->groups[g].sum = 0;
        gbr->groups[g].min_val = 999999;
        gbr->groups[g].max_val = -999999;

        int i;
        for (i = 0; i < t->nrows; i++) {
            if (t->rows[i].group_key == keys[g]) {
                gbr->groups[g].count++;
                gbr->groups[g].sum = gbr->groups[g].sum + t->rows[i].value;
                if (t->rows[i].value < gbr->groups[g].min_val)
                    gbr->groups[g].min_val = t->rows[i].value;
                if (t->rows[i].value > gbr->groups[g].max_val)
                    gbr->groups[g].max_val = t->rows[i].value;
            }
        }
    }
}

// Sort groups by key for deterministic output
void sort_groups(struct GroupByResult *gbr) {
    int i;
    int j;
    for (i = 1; i < gbr->ngroups; i++) {
        struct GroupResult tmp = gbr->groups[i];
        j = i - 1;
        while (j >= 0 && gbr->groups[j].key > tmp.key) {
            gbr->groups[j + 1] = gbr->groups[j];
            j--;
        }
        gbr->groups[j + 1] = tmp;
    }
}

char *group_name(int key) {
    if (key == 1) return "Alpha";
    if (key == 2) return "Beta";
    if (key == 3) return "Gamma";
    if (key == 4) return "Delta";
    return "Unknown";
}

int main() {
    struct Table t;
    t.nrows = 0;

    // Group Alpha (1)
    table_add(&t, 1, 1, 100);
    table_add(&t, 2, 1, 200);
    table_add(&t, 3, 1, 150);
    table_add(&t, 4, 1, 300);
    table_add(&t, 5, 1, 250);

    // Group Beta (2)
    table_add(&t, 6, 2, 50);
    table_add(&t, 7, 2, 80);
    table_add(&t, 8, 2, 120);

    // Group Gamma (3)
    table_add(&t, 9, 3, 400);
    table_add(&t, 10, 3, 350);
    table_add(&t, 11, 3, 500);
    table_add(&t, 12, 3, 450);

    // Group Delta (4)
    table_add(&t, 13, 4, 10);
    table_add(&t, 14, 4, 20);

    printf("Table has %d rows\n", t.nrows); // EXPECT: Table has 14 rows

    struct GroupByResult gbr;
    group_by(&t, &gbr);
    sort_groups(&gbr);

    printf("Number of groups: %d\n", gbr.ngroups); // EXPECT: Number of groups: 4

    int i;
    for (i = 0; i < gbr.ngroups; i++) {
        int avg = gbr.groups[i].sum / gbr.groups[i].count;
        printf("Group %s: count=%d sum=%d min=%d max=%d avg=%d\n",
               group_name(gbr.groups[i].key),
               gbr.groups[i].count, gbr.groups[i].sum,
               gbr.groups[i].min_val, gbr.groups[i].max_val, avg);
    }
    // EXPECT: Group Alpha: count=5 sum=1000 min=100 max=300 avg=200
    // EXPECT: Group Beta: count=3 sum=250 min=50 max=120 avg=83
    // EXPECT: Group Gamma: count=4 sum=1700 min=350 max=500 avg=425
    // EXPECT: Group Delta: count=2 sum=30 min=10 max=20 avg=15

    // Verify totals
    int total_count = 0;
    int total_sum = 0;
    for (i = 0; i < gbr.ngroups; i++) {
        total_count = total_count + gbr.groups[i].count;
        total_sum = total_sum + gbr.groups[i].sum;
    }
    printf("Total count across groups: %d\n", total_count); // EXPECT: Total count across groups: 14
    printf("Total sum across groups: %d\n", total_sum); // EXPECT: Total sum across groups: 2980

    // Find group with max sum
    int max_sum_group = 0;
    for (i = 1; i < gbr.ngroups; i++) {
        if (gbr.groups[i].sum > gbr.groups[max_sum_group].sum) {
            max_sum_group = i;
        }
    }
    printf("Highest sum group: %s (%d)\n", group_name(gbr.groups[max_sum_group].key),
           gbr.groups[max_sum_group].sum);
    // EXPECT: Highest sum group: Gamma (1700)

    // Find group with max avg
    int max_avg_group = 0;
    int max_avg = gbr.groups[0].sum / gbr.groups[0].count;
    for (i = 1; i < gbr.ngroups; i++) {
        int avg = gbr.groups[i].sum / gbr.groups[i].count;
        if (avg > max_avg) {
            max_avg = avg;
            max_avg_group = i;
        }
    }
    printf("Highest avg group: %s (%d)\n", group_name(gbr.groups[max_avg_group].key), max_avg);
    // EXPECT: Highest avg group: Gamma (425)

    printf("GROUP BY test passed\n"); // EXPECT: GROUP BY test passed
    return 0;
}
