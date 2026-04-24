int printf(const char *fmt, ...);

struct CSVField {
    char data[64];
};

struct CSVRow {
    struct CSVField fields[10];
    int num_fields;
};

struct CSVTable {
    struct CSVRow rows[20];
    int num_rows;
    int num_cols;
};

int str_len(const char *s) {
    int i;
    i = 0;
    while (s[i] != 0) i = i + 1;
    return i;
}

int str_eq(const char *a, const char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void str_copy(char *dst, const char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int parse_row(const char *line, struct CSVRow *row) {
    int i;
    int fi;
    int ci;
    fi = 0;
    ci = 0;
    i = 0;
    while (line[i] != 0) {
        if (line[i] == ',') {
            row->fields[fi].data[ci] = 0;
            fi = fi + 1;
            ci = 0;
        } else {
            row->fields[fi].data[ci] = line[i];
            ci = ci + 1;
        }
        i = i + 1;
    }
    row->fields[fi].data[ci] = 0;
    row->num_fields = fi + 1;
    return row->num_fields;
}

void add_csv_row(struct CSVTable *table, const char *line) {
    int idx;
    idx = table->num_rows;
    parse_row(line, &table->rows[idx]);
    if (table->rows[idx].num_fields > table->num_cols) {
        table->num_cols = table->rows[idx].num_fields;
    }
    table->num_rows = table->num_rows + 1;
}

void init_table(struct CSVTable *table) {
    table->num_rows = 0;
    table->num_cols = 0;
}

void print_table(struct CSVTable *table) {
    int r;
    int c;
    for (r = 0; r < table->num_rows; r = r + 1) {
        for (c = 0; c < table->rows[r].num_fields; c = c + 1) {
            if (c > 0) printf(" | ");
            printf("%s", table->rows[r].fields[c].data);
        }
        printf("\n");
    }
}

int find_col(struct CSVTable *table, const char *header) {
    int c;
    if (table->num_rows == 0) return -1;
    for (c = 0; c < table->rows[0].num_fields; c = c + 1) {
        if (str_eq(table->rows[0].fields[c].data, header)) return c;
    }
    return -1;
}

int str_to_int(const char *s) {
    int result;
    int i;
    int neg;
    result = 0;
    i = 0;
    neg = 0;
    if (s[0] == '-') {
        neg = 1;
        i = 1;
    }
    while (s[i] >= '0' && s[i] <= '9') {
        result = result * 10 + (s[i] - '0');
        i = i + 1;
    }
    if (neg) return -result;
    return result;
}

int sum_column(struct CSVTable *table, int col) {
    int r;
    int sum;
    sum = 0;
    for (r = 1; r < table->num_rows; r = r + 1) {
        if (col < table->rows[r].num_fields) {
            sum = sum + str_to_int(table->rows[r].fields[col].data);
        }
    }
    return sum;
}

int count_matching(struct CSVTable *table, int col, const char *val) {
    int r;
    int cnt;
    cnt = 0;
    for (r = 1; r < table->num_rows; r = r + 1) {
        if (col < table->rows[r].num_fields) {
            if (str_eq(table->rows[r].fields[col].data, val)) {
                cnt = cnt + 1;
            }
        }
    }
    return cnt;
}

int max_in_column(struct CSVTable *table, int col) {
    int r;
    int mx;
    int v;
    mx = -999999;
    for (r = 1; r < table->num_rows; r = r + 1) {
        if (col < table->rows[r].num_fields) {
            v = str_to_int(table->rows[r].fields[col].data);
            if (v > mx) mx = v;
        }
    }
    return mx;
}

int main() {
    struct CSVTable table;
    int col_age;
    int col_city;
    int col_score;

    // EXPECT: === CSV Parser Test ===
    printf("=== CSV Parser Test ===\n");

    init_table(&table);
    add_csv_row(&table, "Name,Age,City,Score");
    add_csv_row(&table, "Alice,30,NYC,85");
    add_csv_row(&table, "Bob,25,LA,92");
    add_csv_row(&table, "Charlie,35,NYC,78");
    add_csv_row(&table, "Diana,28,Chicago,95");
    add_csv_row(&table, "Eve,22,LA,88");

    // EXPECT: Table dimensions: 6 rows x 4 cols
    printf("Table dimensions: %d rows x %d cols\n", table.num_rows, table.num_cols);

    // EXPECT: Name | Age | City | Score
    // EXPECT: Alice | 30 | NYC | 85
    // EXPECT: Bob | 25 | LA | 92
    // EXPECT: Charlie | 35 | NYC | 78
    // EXPECT: Diana | 28 | Chicago | 95
    // EXPECT: Eve | 22 | LA | 88
    print_table(&table);

    col_age = find_col(&table, "Age");
    col_city = find_col(&table, "City");
    col_score = find_col(&table, "Score");

    // EXPECT: Age column index: 1
    printf("Age column index: %d\n", col_age);
    // EXPECT: City column index: 2
    printf("City column index: %d\n", col_city);

    // EXPECT: Sum of ages: 140
    printf("Sum of ages: %d\n", sum_column(&table, col_age));
    // EXPECT: Sum of scores: 438
    printf("Sum of scores: %d\n", sum_column(&table, col_score));
    // EXPECT: Max score: 95
    printf("Max score: %d\n", max_in_column(&table, col_score));
    // EXPECT: People in NYC: 2
    printf("People in NYC: %d\n", count_matching(&table, col_city, "NYC"));
    // EXPECT: People in LA: 2
    printf("People in LA: %d\n", count_matching(&table, col_city, "LA"));
    // EXPECT: People in Chicago: 1
    printf("People in Chicago: %d\n", count_matching(&table, col_city, "Chicago"));

    // EXPECT: Field [2][0]: Charlie
    printf("Field [2][0]: %s\n", table.rows[3].fields[0].data);

    // EXPECT: === Done ===
    printf("=== Done ===\n");
    return 0;
}
