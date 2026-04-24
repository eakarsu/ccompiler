int printf(const char *fmt, ...);

// CSV Parser: parse comma-separated fields from a string

struct CSVField {
    char data[64];
    int len;
};

struct CSVRow {
    struct CSVField fields[8];
    int count;
};

int str_len(char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

void copy_str(char *dst, char *src, int len) {
    int i;
    for (i = 0; i < len; i++) {
        dst[i] = src[i];
    }
    dst[len] = 0;
}

int parse_csv_row(char *line, struct CSVRow *row) {
    int i = 0;
    int field_start = 0;
    int field_idx = 0;
    int len = str_len(line);
    int in_quotes = 0;

    while (i <= len) {
        if (i < len && line[i] == '"') {
            in_quotes = 1 - in_quotes;
            i++;
            continue;
        }
        if ((i == len || line[i] == ',') && !in_quotes) {
            int flen = 0;
            int j = field_start;
            // Skip leading quote
            if (j < i && line[j] == '"') j++;
            int end = i;
            // Skip trailing quote
            if (end > j && line[end - 1] == '"') end--;
            flen = end - j;
            if (flen > 63) flen = 63;
            copy_str(row->fields[field_idx].data, line + j, flen);
            row->fields[field_idx].len = flen;
            field_idx++;
            field_start = i + 1;
        }
        i++;
    }
    row->count = field_idx;
    return field_idx;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

int count_fields_total(struct CSVRow *rows, int nrows) {
    int total = 0;
    int i;
    for (i = 0; i < nrows; i++) {
        total = total + rows[i].count;
    }
    return total;
}

int find_field_in_row(struct CSVRow *row, char *target) {
    int i;
    for (i = 0; i < row->count; i++) {
        if (str_eq(row->fields[i].data, target)) return i;
    }
    return -1;
}

int main() {
    struct CSVRow rows[4];
    int nrows = 0;

    // Row 0: header
    char line0[64];
    copy_str(line0, "name,age,city", 13);
    parse_csv_row(line0, &rows[0]);
    nrows++;

    printf("%d\n", rows[0].count);
    // EXPECT: 3
    printf("%s\n", rows[0].fields[0].data);
    // EXPECT: name
    printf("%s\n", rows[0].fields[1].data);
    // EXPECT: age
    printf("%s\n", rows[0].fields[2].data);
    // EXPECT: city

    // Row 1: data
    char line1[64];
    copy_str(line1, "Alice,30,NYC", 12);
    parse_csv_row(line1, &rows[1]);
    nrows++;

    printf("%s\n", rows[1].fields[0].data);
    // EXPECT: Alice
    printf("%s\n", rows[1].fields[1].data);
    // EXPECT: 30
    printf("%s\n", rows[1].fields[2].data);
    // EXPECT: NYC

    // Row 2: data with spaces
    char line2[64];
    copy_str(line2, "Bob,25,LA", 9);
    parse_csv_row(line2, &rows[2]);
    nrows++;

    printf("%s\n", rows[2].fields[0].data);
    // EXPECT: Bob

    // Row 3: more fields
    char line3[64];
    copy_str(line3, "Eve,40,SF,engineer", 18);
    parse_csv_row(line3, &rows[3]);
    nrows++;

    printf("%d\n", rows[3].count);
    // EXPECT: 4
    printf("%s\n", rows[3].fields[3].data);
    // EXPECT: engineer

    // Total field count
    int total = count_fields_total(rows, nrows);
    printf("%d\n", total);
    // EXPECT: 13

    // Find column index
    int idx = find_field_in_row(&rows[0], "city");
    printf("%d\n", idx);
    // EXPECT: 2

    // Use column index to get data from rows
    printf("%s\n", rows[1].fields[idx].data);
    // EXPECT: NYC
    printf("%s\n", rows[2].fields[idx].data);
    // EXPECT: LA

    // Field not found
    int missing = find_field_in_row(&rows[0], "email");
    printf("%d\n", missing);
    // EXPECT: -1

    // Parse empty fields
    struct CSVRow erow;
    char eline[64];
    copy_str(eline, "a,,c", 4);
    parse_csv_row(eline, &erow);
    printf("%d\n", erow.count);
    // EXPECT: 3
    printf("%d\n", erow.fields[1].len);
    // EXPECT: 0
    printf("%s\n", erow.fields[0].data);
    // EXPECT: a
    printf("%s\n", erow.fields[2].data);
    // EXPECT: c

    // Single field
    struct CSVRow srow;
    char sline[64];
    copy_str(sline, "hello", 5);
    parse_csv_row(sline, &srow);
    printf("%d\n", srow.count);
    // EXPECT: 1
    printf("%s\n", srow.fields[0].data);
    // EXPECT: hello

    return 0;
}
