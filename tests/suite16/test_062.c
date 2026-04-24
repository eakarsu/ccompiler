int printf(const char *fmt, ...);

// CSV Parser State Machine
// Handles quoted fields, commas, newlines

enum csv_state {
    CSV_FIELD_START,
    CSV_UNQUOTED,
    CSV_QUOTED,
    CSV_QUOTE_IN_QUOTED,
    CSV_END_OF_LINE,
    CSV_DONE
};

struct csv_parser {
    int state;
    char fields[8][32];
    int field_lens[8];
    int cur_field;
    int row_count;
    int total_fields;
};

void csv_init(struct csv_parser *p) {
    p->state = CSV_FIELD_START;
    p->cur_field = 0;
    p->row_count = 0;
    p->total_fields = 0;
    int i;
    int j;
    for (i = 0; i < 8; i++) {
        p->field_lens[i] = 0;
        for (j = 0; j < 32; j++) {
            p->fields[i][j] = 0;
        }
    }
}

void csv_add_char(struct csv_parser *p, char c) {
    int f = p->cur_field;
    if (f < 8 && p->field_lens[f] < 31) {
        p->fields[f][p->field_lens[f]] = c;
        p->field_lens[f] = p->field_lens[f] + 1;
    }
}

void csv_next_field(struct csv_parser *p) {
    p->cur_field = p->cur_field + 1;
    p->total_fields = p->total_fields + 1;
}

void csv_end_row(struct csv_parser *p) {
    p->total_fields = p->total_fields + 1;
    p->row_count = p->row_count + 1;
}

void csv_feed(struct csv_parser *p, char c) {
    int st = p->state;
    if (st == CSV_FIELD_START) {
        if (c == 34) {
            p->state = CSV_QUOTED;
        } else if (c == 44) {
            csv_next_field(p);
        } else if (c == 10) {
            csv_end_row(p);
            p->cur_field = 0;
            int i;
            int j;
            for (i = 0; i < 8; i++) {
                p->field_lens[i] = 0;
                for (j = 0; j < 32; j++) {
                    p->fields[i][j] = 0;
                }
            }
        } else if (c == 0) {
            csv_end_row(p);
            p->state = CSV_DONE;
        } else {
            csv_add_char(p, c);
            p->state = CSV_UNQUOTED;
        }
    } else if (st == CSV_UNQUOTED) {
        if (c == 44) {
            csv_next_field(p);
            p->state = CSV_FIELD_START;
        } else if (c == 10) {
            csv_end_row(p);
            p->cur_field = 0;
            int i;
            int j;
            for (i = 0; i < 8; i++) {
                p->field_lens[i] = 0;
                for (j = 0; j < 32; j++) {
                    p->fields[i][j] = 0;
                }
            }
            p->state = CSV_FIELD_START;
        } else if (c == 0) {
            csv_end_row(p);
            p->state = CSV_DONE;
        } else {
            csv_add_char(p, c);
        }
    } else if (st == CSV_QUOTED) {
        if (c == 34) {
            p->state = CSV_QUOTE_IN_QUOTED;
        } else if (c == 0) {
            csv_end_row(p);
            p->state = CSV_DONE;
        } else {
            csv_add_char(p, c);
        }
    } else if (st == CSV_QUOTE_IN_QUOTED) {
        if (c == 34) {
            csv_add_char(p, 34);
            p->state = CSV_QUOTED;
        } else if (c == 44) {
            csv_next_field(p);
            p->state = CSV_FIELD_START;
        } else if (c == 10) {
            csv_end_row(p);
            p->cur_field = 0;
            int i;
            int j;
            for (i = 0; i < 8; i++) {
                p->field_lens[i] = 0;
                for (j = 0; j < 32; j++) {
                    p->fields[i][j] = 0;
                }
            }
            p->state = CSV_FIELD_START;
        } else if (c == 0) {
            csv_end_row(p);
            p->state = CSV_DONE;
        } else {
            p->state = CSV_FIELD_START;
        }
    }
}

void parse_csv(struct csv_parser *p, char *data, int len) {
    csv_init(p);
    int i;
    for (i = 0; i < len; i++) {
        csv_feed(p, data[i]);
    }
    if (p->state != CSV_DONE) {
        csv_feed(p, 0);
    }
}

int main() {
    struct csv_parser p;

    printf("=== CSV Parser ===\n"); // EXPECT: === CSV Parser ===

    // Test 1: Simple CSV row: "hello,world,test"
    char d1[32];
    d1[0]='h'; d1[1]='e'; d1[2]='l'; d1[3]='l'; d1[4]='o';
    d1[5]=44; // comma
    d1[6]='w'; d1[7]='o'; d1[8]='r'; d1[9]='l'; d1[10]='d';
    d1[11]=44;
    d1[12]='t'; d1[13]='e'; d1[14]='s'; d1[15]='t';
    d1[16]=0;
    parse_csv(&p, d1, 16);
    printf("Rows: %d\n", p.row_count);     // EXPECT: Rows: 1
    printf("Fields: %d\n", p.total_fields); // EXPECT: Fields: 3

    // Test 2: Two rows: "a,b\nc,d\n"
    char d2[16];
    d2[0]='a'; d2[1]=44; d2[2]='b'; d2[3]=10;
    d2[4]='c'; d2[5]=44; d2[6]='d'; d2[7]=10;
    d2[8]=0;
    parse_csv(&p, d2, 8);
    printf("Rows: %d\n", p.row_count);     // EXPECT: Rows: 3
    printf("Fields: %d\n", p.total_fields); // EXPECT: Fields: 5

    // Test 3: Quoted field: '"hello",world'
    char d3[32];
    d3[0]=34; d3[1]='h'; d3[2]='e'; d3[3]='l'; d3[4]='l'; d3[5]='o'; d3[6]=34;
    d3[7]=44;
    d3[8]='w'; d3[9]='o'; d3[10]='r'; d3[11]='l'; d3[12]='d';
    d3[13]=0;
    parse_csv(&p, d3, 13);
    printf("Rows: %d\n", p.row_count);     // EXPECT: Rows: 1
    printf("Fields: %d\n", p.total_fields); // EXPECT: Fields: 2
    printf("F0: %s\n", p.fields[0]);       // EXPECT: F0: hello
    printf("F1: %s\n", p.fields[1]);       // EXPECT: F1: world

    // Test 4: Empty fields: ",,"
    char d4[8];
    d4[0]=44; d4[1]=44; d4[2]=0;
    parse_csv(&p, d4, 2);
    printf("Rows: %d\n", p.row_count);     // EXPECT: Rows: 1
    printf("Fields: %d\n", p.total_fields); // EXPECT: Fields: 3

    // Test 5: Single field
    char d5[8];
    d5[0]='X'; d5[1]='Y'; d5[2]=0;
    parse_csv(&p, d5, 2);
    printf("Rows: %d\n", p.row_count);     // EXPECT: Rows: 1
    printf("Fields: %d\n", p.total_fields); // EXPECT: Fields: 1
    printf("F0: %s\n", p.fields[0]);       // EXPECT: F0: XY

    printf("Done\n"); // EXPECT: Done

    return 0;
}
