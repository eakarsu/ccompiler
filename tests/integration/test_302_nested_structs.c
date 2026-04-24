int printf(const char *fmt, ...);
// EXPECT: Person id=101 age=30\n  street: 42, zip: 90210, city: 5\nPerson id=102 age=25\n  street: 100, zip: 90210, city: 5\nsame city: 1\nCompany employees: 50\nCEO id: 101\n  street: 1, zip: 10001, city: 7\nCEO age updated: 31\nOriginal p1 age: 30
// Test: nested structs

struct Address {
    int street_num;
    int zip;
    int city_code;
};

struct Person {
    int age;
    int id;
    struct Address addr;
};

struct Company {
    int employee_count;
    struct Person ceo;
    struct Address hq;
};

void print_address(struct Address a) {
    printf("  street: %d, zip: %d, city: %d\n", a.street_num, a.zip, a.city_code);
}

void print_person(struct Person p) {
    printf("Person id=%d age=%d\n", p.id, p.age);
    print_address(p.addr);
}

int same_city(struct Person a, struct Person b) {
    return a.addr.city_code == b.addr.city_code;
}

int main(void) {
    struct Person p1;
    p1.age = 30;
    p1.id = 101;
    p1.addr.street_num = 42;
    p1.addr.zip = 90210;
    p1.addr.city_code = 5;

    struct Person p2;
    p2.age = 25;
    p2.id = 102;
    p2.addr.street_num = 100;
    p2.addr.zip = 90210;
    p2.addr.city_code = 5;

    print_person(p1);
    print_person(p2);

    printf("same city: %d\n", same_city(p1, p2));

    struct Company co;
    co.employee_count = 50;
    co.ceo = p1;
    co.hq.street_num = 1;
    co.hq.zip = 10001;
    co.hq.city_code = 7;

    printf("Company employees: %d\n", co.employee_count);
    printf("CEO id: %d\n", co.ceo.id);
    print_address(co.hq);

    co.ceo.age = 31;
    printf("CEO age updated: %d\n", co.ceo.age);
    printf("Original p1 age: %d\n", p1.age);

    return 0;
}
