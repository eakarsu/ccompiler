int printf(const char *fmt, ...);
// EXPECT: contacts:\n  [1] (212)5551234 zip=10001 age=30\n  [2] (310)5555678 zip=90210 age=25\n  [3] (212)5559012 zip=10002 age=35\n  [4] (415)5553456 zip=94102 age=28\n  [5] (212)5557890 zip=10003 age=42\nactive: 5\navg age: 32\narea 212: 3 contacts\n  id=1\n  id=3\n  id=5\nafter delete:\nactive: 4\navg age: 31\narea 212 after delete: 2\nfound id1: age=30
// Test: address book with structs

struct Contact {
    int id;
    int area_code;
    int phone;
    int zip;
    int age;
    int active;
};

struct AddressBook {
    struct Contact contacts[16];
    int count;
    int next_id;
};

void ab_init(struct AddressBook *ab) {
    ab->count = 0;
    ab->next_id = 1;
}

int ab_add(struct AddressBook *ab, int area, int phone, int zip, int age) {
    if (ab->count >= 16) return -1;
    int idx = ab->count;
    ab->contacts[idx].id = ab->next_id;
    ab->contacts[idx].area_code = area;
    ab->contacts[idx].phone = phone;
    ab->contacts[idx].zip = zip;
    ab->contacts[idx].age = age;
    ab->contacts[idx].active = 1;
    ab->count = ab->count + 1;
    ab->next_id = ab->next_id + 1;
    return ab->contacts[idx].id;
}

struct Contact *ab_find(struct AddressBook *ab, int id) {
    int i;
    for (i = 0; i < ab->count; i++) {
        if (ab->contacts[i].id == id && ab->contacts[i].active) {
            return &ab->contacts[i];
        }
    }
    return 0;
}

int ab_find_by_area(struct AddressBook *ab, int area, int results[], int max) {
    int found = 0;
    int i;
    for (i = 0; i < ab->count; i++) {
        if (ab->contacts[i].active && ab->contacts[i].area_code == area) {
            if (found < max) {
                results[found] = ab->contacts[i].id;
            }
            found = found + 1;
        }
    }
    return found;
}

int ab_count_active(struct AddressBook *ab) {
    int count = 0;
    int i;
    for (i = 0; i < ab->count; i++) {
        if (ab->contacts[i].active) count = count + 1;
    }
    return count;
}

int ab_average_age(struct AddressBook *ab) {
    int sum = 0;
    int count = 0;
    int i;
    for (i = 0; i < ab->count; i++) {
        if (ab->contacts[i].active) {
            sum = sum + ab->contacts[i].age;
            count = count + 1;
        }
    }
    if (count == 0) return 0;
    return sum / count;
}

void ab_delete(struct AddressBook *ab, int id) {
    struct Contact *c = ab_find(ab, id);
    if (c) c->active = 0;
}

void ab_print(struct AddressBook *ab) {
    int i;
    for (i = 0; i < ab->count; i++) {
        if (ab->contacts[i].active) {
            printf("  [%d] (%d)%d zip=%d age=%d\n",
                   ab->contacts[i].id,
                   ab->contacts[i].area_code,
                   ab->contacts[i].phone,
                   ab->contacts[i].zip,
                   ab->contacts[i].age);
        }
    }
}

int main(void) {
    struct AddressBook ab;
    ab_init(&ab);

    int id1 = ab_add(&ab, 212, 5551234, 10001, 30);
    int id2 = ab_add(&ab, 310, 5555678, 90210, 25);
    int id3 = ab_add(&ab, 212, 5559012, 10002, 35);
    int id4 = ab_add(&ab, 415, 5553456, 94102, 28);
    int id5 = ab_add(&ab, 212, 5557890, 10003, 42);

    printf("contacts:\n");
    ab_print(&ab);
    printf("active: %d\n", ab_count_active(&ab));
    printf("avg age: %d\n", ab_average_age(&ab));

    int results[5];
    int found = ab_find_by_area(&ab, 212, results, 5);
    printf("area 212: %d contacts\n", found);
    int i;
    for (i = 0; i < found; i++) {
        printf("  id=%d\n", results[i]);
    }

    ab_delete(&ab, id3);
    printf("after delete:\n");
    printf("active: %d\n", ab_count_active(&ab));
    printf("avg age: %d\n", ab_average_age(&ab));

    found = ab_find_by_area(&ab, 212, results, 5);
    printf("area 212 after delete: %d\n", found);

    struct Contact *c = ab_find(&ab, id1);
    if (c) printf("found id1: age=%d\n", c->age);

    return 0;
}
