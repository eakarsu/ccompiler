int printf(const char *fmt, ...);

struct Address {
    int street_num;
    int zip;
    int city_code;
};

struct PhoneEntry {
    int area_code;
    int number;
};

struct Contact {
    int id;
    char *name;
    struct Address addr;
    struct PhoneEntry phones[3];
    int phone_count;
};

struct ContactBook {
    struct Contact entries[5];
    int count;
};

void init_address(struct Address *a, int street, int zip, int city) {
    a->street_num = street;
    a->zip = zip;
    a->city_code = city;
}

void add_phone(struct Contact *c, int area, int num) {
    int idx = c->phone_count;
    c->phones[idx].area_code = area;
    c->phones[idx].number = num;
    c->phone_count = c->phone_count + 1;
}

void init_contact(struct Contact *c, int id, char *name,
                  int street, int zip, int city) {
    c->id = id;
    c->name = name;
    init_address(&c->addr, street, zip, city);
    c->phone_count = 0;
}

void add_contact(struct ContactBook *book, struct Contact *c) {
    int idx = book->count;
    book->entries[idx].id = c->id;
    book->entries[idx].name = c->name;
    book->entries[idx].addr.street_num = c->addr.street_num;
    book->entries[idx].addr.zip = c->addr.zip;
    book->entries[idx].addr.city_code = c->addr.city_code;
    book->entries[idx].phone_count = 0;
    int i;
    for (i = 0; i < c->phone_count; i = i + 1) {
        book->entries[idx].phones[i].area_code = c->phones[i].area_code;
        book->entries[idx].phones[i].number = c->phones[i].number;
        book->entries[idx].phone_count = book->entries[idx].phone_count + 1;
    }
    book->count = book->count + 1;
}

void print_contact(struct Contact *c) {
    printf("Contact %d: %s\n", c->id, c->name);
    printf("  Address: %d, zip=%d, city=%d\n",
           c->addr.street_num, c->addr.zip, c->addr.city_code);
    int i;
    for (i = 0; i < c->phone_count; i = i + 1) {
        printf("  Phone: (%d) %d\n", c->phones[i].area_code, c->phones[i].number);
    }
}

struct Contact *find_by_id(struct ContactBook *book, int id) {
    int i;
    for (i = 0; i < book->count; i = i + 1) {
        if (book->entries[i].id == id) {
            return &book->entries[i];
        }
    }
    return 0;
}

int count_by_city(struct ContactBook *book, int city_code) {
    int count = 0;
    int i;
    for (i = 0; i < book->count; i = i + 1) {
        if (book->entries[i].addr.city_code == city_code) {
            count = count + 1;
        }
    }
    return count;
}

int count_by_area(struct ContactBook *book, int area_code) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < book->count; i = i + 1) {
        for (j = 0; j < book->entries[i].phone_count; j = j + 1) {
            if (book->entries[i].phones[j].area_code == area_code) {
                count = count + 1;
            }
        }
    }
    return count;
}

int total_phones(struct ContactBook *book) {
    int total = 0;
    int i;
    for (i = 0; i < book->count; i = i + 1) {
        total = total + book->entries[i].phone_count;
    }
    return total;
}

int main() {
    struct ContactBook book;
    book.count = 0;

    struct Contact c1;
    init_contact(&c1, 1, "Alice", 123, 10001, 1);
    add_phone(&c1, 212, 5551234);
    add_phone(&c1, 646, 5555678);

    struct Contact c2;
    init_contact(&c2, 2, "Bob", 456, 90210, 2);
    add_phone(&c2, 310, 5559999);

    struct Contact c3;
    init_contact(&c3, 3, "Carol", 789, 10002, 1);
    add_phone(&c3, 212, 5554321);
    add_phone(&c3, 917, 5551111);
    add_phone(&c3, 212, 5552222);

    add_contact(&book, &c1);
    add_contact(&book, &c2);
    add_contact(&book, &c3);

    // EXPECT: Book has 3 contacts
    printf("Book has %d contacts\n", book.count);

    // EXPECT: Contact 1: Alice
    // EXPECT:   Address: 123, zip=10001, city=1
    // EXPECT:   Phone: (212) 5551234
    // EXPECT:   Phone: (646) 5555678
    print_contact(&book.entries[0]);

    // EXPECT: Contact 2: Bob
    // EXPECT:   Address: 456, zip=90210, city=2
    // EXPECT:   Phone: (310) 5559999
    print_contact(&book.entries[1]);

    // EXPECT: Contact 3: Carol
    // EXPECT:   Address: 789, zip=10002, city=1
    // EXPECT:   Phone: (212) 5554321
    // EXPECT:   Phone: (917) 5551111
    // EXPECT:   Phone: (212) 5552222
    print_contact(&book.entries[2]);

    struct Contact *found = find_by_id(&book, 2);
    // EXPECT: Found id 2: Bob
    printf("Found id 2: %s\n", found->name);

    struct Contact *not_found = find_by_id(&book, 99);
    int is_null = (not_found == 0);
    // EXPECT: Id 99 not found: 1
    printf("Id 99 not found: %d\n", is_null);

    // EXPECT: Contacts in city 1: 2
    printf("Contacts in city 1: %d\n", count_by_city(&book, 1));
    // EXPECT: Contacts in city 2: 1
    printf("Contacts in city 2: %d\n", count_by_city(&book, 2));

    // EXPECT: Phones with area 212: 3
    printf("Phones with area 212: %d\n", count_by_area(&book, 212));
    // EXPECT: Phones with area 310: 1
    printf("Phones with area 310: %d\n", count_by_area(&book, 310));

    // EXPECT: Total phones: 6
    printf("Total phones: %d\n", total_phones(&book));

    // EXPECT: Carol phone 0: (212) 5554321
    printf("Carol phone 0: (%d) %d\n",
           book.entries[2].phones[0].area_code,
           book.entries[2].phones[0].number);

    // EXPECT: Alice zip: 10001
    printf("Alice zip: %d\n", book.entries[0].addr.zip);

    return 0;
}
