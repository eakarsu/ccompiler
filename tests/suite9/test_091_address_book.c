int printf(const char *fmt, ...);

struct Contact {
    char name[32];
    int age;
    char phone[16];
    int active;
};

struct AddressBook {
    struct Contact entries[20];
    int count;
};

void str_copy(char *dst, const char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
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

void add_contact(struct AddressBook *book, const char *name, int age, const char *phone) {
    int idx;
    idx = book->count;
    if (idx >= 20) {
        printf("Book full\n");
        return;
    }
    str_copy(book->entries[idx].name, name);
    book->entries[idx].age = age;
    str_copy(book->entries[idx].phone, phone);
    book->entries[idx].active = 1;
    book->count = book->count + 1;
}

int find_contact(struct AddressBook *book, const char *name) {
    int i;
    for (i = 0; i < book->count; i = i + 1) {
        if (book->entries[i].active && str_eq(book->entries[i].name, name)) {
            return i;
        }
    }
    return -1;
}

void delete_contact(struct AddressBook *book, const char *name) {
    int idx;
    idx = find_contact(book, name);
    if (idx >= 0) {
        book->entries[idx].active = 0;
    }
}

void print_book(struct AddressBook *book) {
    int i;
    int printed;
    printed = 0;
    for (i = 0; i < book->count; i = i + 1) {
        if (book->entries[i].active) {
            printf("  %s age=%d phone=%s\n", book->entries[i].name, book->entries[i].age, book->entries[i].phone);
            printed = printed + 1;
        }
    }
    printf("Total active: %d\n", printed);
}

int count_active(struct AddressBook *book) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < book->count; i = i + 1) {
        if (book->entries[i].active) c = c + 1;
    }
    return c;
}

void update_phone(struct AddressBook *book, const char *name, const char *newphone) {
    int idx;
    idx = find_contact(book, name);
    if (idx >= 0) {
        str_copy(book->entries[idx].phone, newphone);
    }
}

int main() {
    struct AddressBook book;
    int idx;
    book.count = 0;

    // EXPECT: === Address Book Test ===
    printf("=== Address Book Test ===\n");

    add_contact(&book, "Alice", 30, "555-0001");
    add_contact(&book, "Bob", 25, "555-0002");
    add_contact(&book, "Charlie", 35, "555-0003");
    add_contact(&book, "Diana", 28, "555-0004");
    add_contact(&book, "Eve", 22, "555-0005");

    // EXPECT: After adding 5 contacts:
    printf("After adding 5 contacts:\n");
    // EXPECT:   Alice age=30 phone=555-0001
    // EXPECT:   Bob age=25 phone=555-0002
    // EXPECT:   Charlie age=35 phone=555-0003
    // EXPECT:   Diana age=28 phone=555-0004
    // EXPECT:   Eve age=22 phone=555-0005
    // EXPECT: Total active: 5
    print_book(&book);

    idx = find_contact(&book, "Charlie");
    // EXPECT: Found Charlie at index 2
    printf("Found Charlie at index %d\n", idx);

    delete_contact(&book, "Bob");
    // EXPECT: After deleting Bob:
    printf("After deleting Bob:\n");
    // EXPECT:   Alice age=30 phone=555-0001
    // EXPECT:   Charlie age=35 phone=555-0003
    // EXPECT:   Diana age=28 phone=555-0004
    // EXPECT:   Eve age=22 phone=555-0005
    // EXPECT: Total active: 4
    print_book(&book);

    // EXPECT: Active count: 4
    printf("Active count: %d\n", count_active(&book));

    update_phone(&book, "Alice", "555-9999");
    // EXPECT: After updating Alice phone:
    printf("After updating Alice phone:\n");
    // EXPECT:   Alice age=30 phone=555-9999
    // EXPECT:   Charlie age=35 phone=555-0003
    // EXPECT:   Diana age=28 phone=555-0004
    // EXPECT:   Eve age=22 phone=555-0005
    // EXPECT: Total active: 4
    print_book(&book);

    idx = find_contact(&book, "Bob");
    // EXPECT: Bob search after delete: -1
    printf("Bob search after delete: %d\n", idx);

    // EXPECT: === Done ===
    printf("=== Done ===\n");
    return 0;
}
