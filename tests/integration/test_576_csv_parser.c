int printf(const char *fmt, ...);
// EXPECT: Parsed CSV table:\nName | Age | Score | Grade\nAlice | 20 | 95 | A\nBob | 22 | 82 | B\nCharlie | 19 | 71 | C\nDiana | 21 | 93 | A\n\nColumn analysis (scores, column 2):\n  Alice: 95\n  Bob: 82\n  Charlie: 71\n  Diana: 93\nAverage: 85\nMin: 71\nMax: 95\nTotal rows: 5
#define MAX_FIELDS 6
#define MAX_ROWS 5
#define MAX_FIELD_LEN 20

struct CSVRow {
    char fields[MAX_FIELDS][MAX_FIELD_LEN];
    int num_fields;
};

struct CSVTable {
    struct CSVRow rows[MAX_ROWS];
    int num_rows;
};

int parse_row(char *line, struct CSVRow *row) {
    row->num_fields = 0;
    int fi = 0;
    int ci = 0;
    int li = 0;
    while (line[li] != 0) {
        if (line[li] == ',') {
            row->fields[fi][ci] = 0;
            fi++;
            ci = 0;
            if (fi >= MAX_FIELDS) break;
        } else {
            if (ci < MAX_FIELD_LEN - 1) {
                row->fields[fi][ci] = line[li];
                ci++;
            }
        }
        li++;
    }
    row->fields[fi][ci] = 0;
    row->num_fields = fi + 1;
    return row->num_fields;
}

int str_to_int(char *s) {
    int result = 0;
    int neg = 0;
    int i = 0;
    if (s[0] == '-') { neg = 1; i = 1; }
    while (s[i] >= '0' && s[i] <= '9') {
        result = result * 10 + (s[i] - '0');
        i++;
    }
    return neg ? -result : result;
}

void csv_print_table(struct CSVTable *t) {
    int i;
    int j;
    for (i = 0; i < t->num_rows; i++) {
        for (j = 0; j < t->rows[i].num_fields; j++) {
            if (j > 0) printf(" | ");
            printf("%s", t->rows[i].fields[j]);
        }
        printf("\n");
    }
}

int main(void) {
    char *lines[5];
    lines[0] = "Name,Age,Score,Grade";
    lines[1] = "Alice,20,95,A";
    lines[2] = "Bob,22,82,B";
    lines[3] = "Charlie,19,71,C";
    lines[4] = "Diana,21,93,A";

    struct CSVTable table;
    table.num_rows = 0;
    int i;
    for (i = 0; i < 5; i++) {
        parse_row(lines[i], &table.rows[table.num_rows]);
        table.num_rows++;
    }

    printf("Parsed CSV table:\n");
    csv_print_table(&table);

    printf("\nColumn analysis (scores, column 2):\n");
    int total = 0;
    int min_score = 999;
    int max_score = -1;
    for (i = 1; i < table.num_rows; i++) {
        int score = str_to_int(table.rows[i].fields[2]);
        total = total + score;
        if (score < min_score) min_score = score;
        if (score > max_score) max_score = score;
        printf("  %s: %d\n", table.rows[i].fields[0], score);
    }
    int count = table.num_rows - 1;
    printf("Average: %d\n", total / count);
    printf("Min: %d\n", min_score);
    printf("Max: %d\n", max_score);
    printf("Total rows: %d\n", table.num_rows);

    return 0;
}
