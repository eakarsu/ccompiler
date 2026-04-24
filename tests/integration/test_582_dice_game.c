int printf(const char *fmt, ...);
// EXPECT: Roll 1: [3,3,3,3,5] sum=17 four-of-a-kind(3)\nRoll 2: [2,2,2,5,5] sum=16 three-of-a-kind(2) full-house\nRoll 3: [1,2,3,4,5] sum=15 straight\nRoll 4: [6,6,6,6,6] sum=30 four-of-a-kind(6)\nRoll 5: [2,3,4,5,6] sum=20 straight\nRoll 6: [1,1,2,3,4] sum=11 straight\nRoll 7: [4,4,4,2,2] sum=16 three-of-a-kind(4) full-house\nScoring complete

struct Die {
    int value;
};

struct DiceSet {
    struct Die dice[5];
    int count;
};

void dice_set(struct DiceSet *ds, int d0, int d1, int d2, int d3, int d4) {
    ds->dice[0].value = d0; ds->dice[1].value = d1;
    ds->dice[2].value = d2; ds->dice[3].value = d3;
    ds->dice[4].value = d4;
    ds->count = 5;
}

int dice_sum(struct DiceSet *ds) {
    int s = 0;
    int i;
    for (i = 0; i < ds->count; i++) s = s + ds->dice[i].value;
    return s;
}

void dice_sort(struct DiceSet *ds) {
    int i;
    int j;
    for (i = 0; i < ds->count - 1; i++) {
        for (j = 0; j < ds->count - 1 - i; j++) {
            if (ds->dice[j].value > ds->dice[j + 1].value) {
                int tmp = ds->dice[j].value;
                ds->dice[j].value = ds->dice[j + 1].value;
                ds->dice[j + 1].value = tmp;
            }
        }
    }
}

int count_val(struct DiceSet *ds, int val) {
    int c = 0;
    int i;
    for (i = 0; i < ds->count; i++) {
        if (ds->dice[i].value == val) c++;
    }
    return c;
}

int has_three_of_kind(struct DiceSet *ds) {
    int v;
    for (v = 1; v <= 6; v++) {
        if (count_val(ds, v) >= 3) return v;
    }
    return 0;
}

int has_four_of_kind(struct DiceSet *ds) {
    int v;
    for (v = 1; v <= 6; v++) {
        if (count_val(ds, v) >= 4) return v;
    }
    return 0;
}

int has_full_house(struct DiceSet *ds) {
    int has3 = 0;
    int has2 = 0;
    int v;
    for (v = 1; v <= 6; v++) {
        int c = count_val(ds, v);
        if (c == 3) has3 = 1;
        if (c == 2) has2 = 1;
    }
    return has3 && has2;
}

int has_straight(struct DiceSet *ds) {
    dice_sort(ds);
    int seq = 1;
    int i;
    for (i = 1; i < ds->count; i++) {
        if (ds->dice[i].value == ds->dice[i - 1].value + 1) seq++;
        else if (ds->dice[i].value != ds->dice[i - 1].value) seq = 1;
    }
    return seq >= 4;
}

void print_dice(struct DiceSet *ds) {
    int i;
    printf("[");
    for (i = 0; i < ds->count; i++) {
        if (i > 0) printf(",");
        printf("%d", ds->dice[i].value);
    }
    printf("]");
}

void evaluate(struct DiceSet *ds) {
    print_dice(ds);
    printf(" sum=%d", dice_sum(ds));
    int fok = has_four_of_kind(ds);
    if (fok) printf(" four-of-a-kind(%d)", fok);
    int tok = has_three_of_kind(ds);
    if (tok && !fok) printf(" three-of-a-kind(%d)", tok);
    if (has_full_house(ds)) printf(" full-house");
    if (has_straight(ds)) printf(" straight");
    printf("\n");
}

int main(void) {
    struct DiceSet ds;

    dice_set(&ds, 3, 3, 3, 3, 5);
    printf("Roll 1: "); evaluate(&ds);

    dice_set(&ds, 2, 2, 2, 5, 5);
    printf("Roll 2: "); evaluate(&ds);

    dice_set(&ds, 1, 2, 3, 4, 5);
    printf("Roll 3: "); evaluate(&ds);

    dice_set(&ds, 6, 6, 6, 6, 6);
    printf("Roll 4: "); evaluate(&ds);

    dice_set(&ds, 2, 3, 4, 5, 6);
    printf("Roll 5: "); evaluate(&ds);

    dice_set(&ds, 1, 1, 2, 3, 4);
    printf("Roll 6: "); evaluate(&ds);

    dice_set(&ds, 4, 4, 4, 2, 2);
    printf("Roll 7: "); evaluate(&ds);

    printf("Scoring complete\n");

    return 0;
}
