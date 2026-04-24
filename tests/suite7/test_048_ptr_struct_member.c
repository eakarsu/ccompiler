int printf(const char *fmt, ...);

struct Record {
    int id;
    int score;
    int grade;
};

struct Packed4 {
    char a;
    char b;
    char c;
    char d;
};

struct TwoInts {
    int x;
    int y;
};

struct Nested {
    struct TwoInts pos;
    int z;
};

int main(void) {
    struct Record rec;
    rec.id = 1001;
    rec.score = 95;
    rec.grade = 4;

    // Access members via pointer arithmetic on char*
    char *base = (char *)&rec;

    // id is at offset 0
    int *id_ptr = (int *)(base + 0);
    // EXPECT: id = 1001
    printf("id = %d\n", *id_ptr);

    // score is at offset 4
    int *score_ptr = (int *)(base + 4);
    // EXPECT: score = 95
    printf("score = %d\n", *score_ptr);

    // grade is at offset 8
    int *grade_ptr = (int *)(base + 8);
    // EXPECT: grade = 4
    printf("grade = %d\n", *grade_ptr);

    // Modify via offset pointer
    *(int *)(base + 4) = 88;
    // EXPECT: new_score = 88
    printf("new_score = %d\n", rec.score);

    // Packed4 - all chars, no padding
    struct Packed4 pk;
    pk.a = 10;
    pk.b = 20;
    pk.c = 30;
    pk.d = 40;

    char *pkp = (char *)&pk;
    // EXPECT: pk_a = 10
    printf("pk_a = %d\n", (int)pkp[0]);

    // EXPECT: pk_b = 20
    printf("pk_b = %d\n", (int)pkp[1]);

    // EXPECT: pk_c = 30
    printf("pk_c = %d\n", (int)pkp[2]);

    // EXPECT: pk_d = 40
    printf("pk_d = %d\n", (int)pkp[3]);

    // Modify via byte offset
    pkp[2] = 99;
    // EXPECT: pk_c_mod = 99
    printf("pk_c_mod = %d\n", (int)pk.c);

    // Array of structs accessed via pointer
    struct TwoInts items[3];
    items[0].x = 1; items[0].y = 2;
    items[1].x = 3; items[1].y = 4;
    items[2].x = 5; items[2].y = 6;

    struct TwoInts *tip = items;
    // EXPECT: items[0].x = 1
    printf("items[0].x = %d\n", tip[0].x);

    // EXPECT: items[1].y = 4
    printf("items[1].y = %d\n", tip[1].y);

    // EXPECT: items[2].x = 5
    printf("items[2].x = %d\n", tip[2].x);

    // Access via raw byte offsets (each TwoInts is 8 bytes)
    char *raw = (char *)items;
    // items[1].x at offset 8
    // EXPECT: raw_item1_x = 3
    printf("raw_item1_x = %d\n", *(int *)(raw + 8));

    // items[2].y at offset 20
    // EXPECT: raw_item2_y = 6
    printf("raw_item2_y = %d\n", *(int *)(raw + 20));

    // Nested struct member access
    struct Nested ns;
    ns.pos.x = 100;
    ns.pos.y = 200;
    ns.z = 300;

    char *nbp = (char *)&ns;
    // pos.x at offset 0
    // EXPECT: ns_x = 100
    printf("ns_x = %d\n", *(int *)(nbp + 0));

    // pos.y at offset 4
    // EXPECT: ns_y = 200
    printf("ns_y = %d\n", *(int *)(nbp + 4));

    // z at offset 8
    // EXPECT: ns_z = 300
    printf("ns_z = %d\n", *(int *)(nbp + 8));

    // Pointer to inner struct
    struct TwoInts *inner = &ns.pos;
    // EXPECT: inner_x = 100
    printf("inner_x = %d\n", inner->x);

    // EXPECT: inner_y = 200
    printf("inner_y = %d\n", inner->y);

    // Walk an array of Records using pointer increment
    struct Record recs[3];
    recs[0].id = 1; recs[0].score = 10; recs[0].grade = 1;
    recs[1].id = 2; recs[1].score = 20; recs[1].grade = 2;
    recs[2].id = 3; recs[2].score = 30; recs[2].grade = 3;

    struct Record *rp = recs;
    int total_score = 0;
    int i;
    for (i = 0; i < 3; i++) {
        total_score = total_score + rp->score;
        rp = rp + 1;
    }
    // EXPECT: total_score = 60
    printf("total_score = %d\n", total_score);

    // Distance between struct members using pointer subtraction
    char *id_addr = (char *)&rec.id;
    char *score_addr = (char *)&rec.score;
    char *grade_addr = (char *)&rec.grade;

    // EXPECT: id_to_score = 4
    printf("id_to_score = %d\n", (int)(score_addr - id_addr));

    // EXPECT: score_to_grade = 4
    printf("score_to_grade = %d\n", (int)(grade_addr - score_addr));

    return 0;
}
