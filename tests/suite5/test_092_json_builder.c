int printf(const char *fmt, ...);

struct Field {
    char key[20];
    char value[20];
    int is_int;
    int int_val;
};

struct JsonObj {
    struct Field fields[16];
    int count;
};

void copy_str(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_len(const char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

void add_string_field(struct JsonObj *obj, const char *key, const char *val) {
    int idx = obj->count;
    copy_str(obj->fields[idx].key, key);
    copy_str(obj->fields[idx].value, val);
    obj->fields[idx].is_int = 0;
    obj->fields[idx].int_val = 0;
    obj->count = obj->count + 1;
}

void add_int_field(struct JsonObj *obj, const char *key, int val) {
    int idx = obj->count;
    copy_str(obj->fields[idx].key, key);
    obj->fields[idx].value[0] = 0;
    obj->fields[idx].is_int = 1;
    obj->fields[idx].int_val = val;
    obj->count = obj->count + 1;
}

void print_indent(int depth) {
    int i = 0;
    while (i < depth) {
        printf("  ");
        i = i + 1;
    }
}

void print_json(struct JsonObj *obj, int depth) {
    printf("{\n");
    int i = 0;
    while (i < obj->count) {
        print_indent(depth + 1);
        if (obj->fields[i].is_int) {
            printf("\"%s\": %d", obj->fields[i].key,
                   obj->fields[i].int_val);
        } else {
            printf("\"%s\": \"%s\"",
                   obj->fields[i].key,
                   obj->fields[i].value);
        }
        if (i < obj->count - 1) {
            printf(",\n");
        } else {
            printf("\n");
        }
        i = i + 1;
    }
    print_indent(depth);
    printf("}");
}

void print_json_array(struct JsonObj *objs, int n, int depth) {
    printf("[\n");
    int i = 0;
    while (i < n) {
        print_indent(depth + 1);
        print_json(&objs[i], depth + 1);
        if (i < n - 1) {
            printf(",\n");
        } else {
            printf("\n");
        }
        i = i + 1;
    }
    print_indent(depth);
    printf("]");
}

int count_fields(struct JsonObj *obj) {
    return obj->count;
}

int sum_int_fields(struct JsonObj *obj) {
    int s = 0;
    int i = 0;
    while (i < obj->count) {
        if (obj->fields[i].is_int) {
            s = s + obj->fields[i].int_val;
        }
        i = i + 1;
    }
    return s;
}

int main(void) {
    struct JsonObj person;
    person.count = 0;
    add_string_field(&person, "name", "Alice");
    add_int_field(&person, "age", 30);
    add_string_field(&person, "city", "NYC");

    // EXPECT: fields_person=3
    printf("fields_person=%d\n", count_fields(&person));

    // EXPECT: sum_ints=30
    printf("sum_ints=%d\n", sum_int_fields(&person));

    // EXPECT: {
    // EXPECT:   "name": "Alice",
    // EXPECT:   "age": 30,
    // EXPECT:   "city": "NYC"
    // EXPECT: }
    print_json(&person, 0);
    printf("\n");

    struct JsonObj item;
    item.count = 0;
    add_string_field(&item, "product", "Widget");
    add_int_field(&item, "price", 25);
    add_int_field(&item, "qty", 10);

    // EXPECT: fields_item=3
    printf("fields_item=%d\n", count_fields(&item));

    // EXPECT: item_sum=35
    printf("item_sum=%d\n", sum_int_fields(&item));

    struct JsonObj arr[2];
    arr[0] = person;
    arr[1] = item;

    // EXPECT: [
    // EXPECT:   {
    // EXPECT:     "name": "Alice",
    // EXPECT:     "age": 30,
    // EXPECT:     "city": "NYC"
    // EXPECT:   },
    // EXPECT:   {
    // EXPECT:     "product": "Widget",
    // EXPECT:     "price": 25,
    // EXPECT:     "qty": 10
    // EXPECT:   }
    // EXPECT: ]
    print_json_array(arr, 2, 0);
    printf("\n");

    struct JsonObj empty;
    empty.count = 0;
    // EXPECT: empty_fields=0
    printf("empty_fields=%d\n", count_fields(&empty));
    // EXPECT: empty_sum=0
    printf("empty_sum=%d\n", sum_int_fields(&empty));

    return 0;
}
