int printf(const char *fmt, ...);

struct Base {
    int type;
    int id;
    int value;
};

struct DerivedA {
    struct Base base;
    int extra_a;
    int data_a;
};

struct DerivedB {
    struct Base base;
    int extra_b1;
    int extra_b2;
    int data_b;
};

int base_get_type(struct Base *b) {
    return b->type;
}

int base_get_id(struct Base *b) {
    return b->id;
}

int base_get_value(struct Base *b) {
    return b->value;
}

void base_set_value(struct Base *b, int v) {
    b->value = v;
}

int derived_a_sum(struct DerivedA *a) {
    return a->base.value + a->extra_a + a->data_a;
}

int derived_b_sum(struct DerivedB *b) {
    return b->base.value + b->extra_b1 + b->extra_b2 + b->data_b;
}

int process_base(struct Base *b) {
    if (b->type == 1) {
        struct DerivedA *a = (struct DerivedA *)b;
        return derived_a_sum(a);
    } else if (b->type == 2) {
        struct DerivedB *db = (struct DerivedB *)b;
        return derived_b_sum(db);
    }
    return b->value;
}

int compare_bases(struct Base *a, struct Base *b) {
    if (a->value > b->value) return 1;
    if (a->value < b->value) return -1;
    return 0;
}

int main(void) {
    struct DerivedA da;
    da.base.type = 1;
    da.base.id = 10;
    da.base.value = 100;
    da.extra_a = 20;
    da.data_a = 30;

    struct DerivedB db;
    db.base.type = 2;
    db.base.id = 20;
    db.base.value = 200;
    db.extra_b1 = 40;
    db.extra_b2 = 50;
    db.data_b = 60;

    // EXPECT: type_a=1
    printf("type_a=%d\n", base_get_type(&da.base));
    // EXPECT: type_b=2
    printf("type_b=%d\n", base_get_type(&db.base));

    // EXPECT: id_a=10
    printf("id_a=%d\n", base_get_id(&da.base));
    // EXPECT: id_b=20
    printf("id_b=%d\n", base_get_id(&db.base));

    // EXPECT: val_a=100
    printf("val_a=%d\n", base_get_value(&da.base));
    // EXPECT: val_b=200
    printf("val_b=%d\n", base_get_value(&db.base));

    // EXPECT: sum_a=150
    printf("sum_a=%d\n", derived_a_sum(&da));
    // EXPECT: sum_b=350
    printf("sum_b=%d\n", derived_b_sum(&db));

    // EXPECT: process_a=150
    printf("process_a=%d\n", process_base(&da.base));
    // EXPECT: process_b=350
    printf("process_b=%d\n", process_base(&db.base));

    base_set_value(&da.base, 500);
    // EXPECT: new_val_a=500
    printf("new_val_a=%d\n", base_get_value(&da.base));

    // EXPECT: new_sum_a=550
    printf("new_sum_a=%d\n", derived_a_sum(&da));

    // EXPECT: cmp_ab=1
    printf("cmp_ab=%d\n", compare_bases(&da.base, &db.base));

    base_set_value(&db.base, 999);
    // EXPECT: cmp_ab2=-1
    printf("cmp_ab2=%d\n", compare_bases(&da.base, &db.base));

    // EXPECT: new_process_b=1149
    printf("new_process_b=%d\n", process_base(&db.base));

    struct Base plain;
    plain.type = 0;
    plain.id = 99;
    plain.value = 42;
    // EXPECT: plain_process=42
    printf("plain_process=%d\n", process_base(&plain));

    struct DerivedA arr[3];
    int i = 0;
    while (i < 3) {
        arr[i].base.type = 1;
        arr[i].base.id = i;
        arr[i].base.value = (i + 1) * 10;
        arr[i].extra_a = i * 5;
        arr[i].data_a = i * 3;
        i = i + 1;
    }
    // EXPECT: arr0_sum=10
    printf("arr0_sum=%d\n", process_base(&arr[0].base));
    // EXPECT: arr1_sum=28
    printf("arr1_sum=%d\n", process_base(&arr[1].base));
    // EXPECT: arr2_sum=46
    printf("arr2_sum=%d\n", process_base(&arr[2].base));

    int total = 0;
    i = 0;
    while (i < 3) {
        total = total + process_base(&arr[i].base);
        i = i + 1;
    }
    // EXPECT: total=84
    printf("total=%d\n", total);

    return 0;
}
