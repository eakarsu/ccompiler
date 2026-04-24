int printf(const char *fmt, ...);

// Test: Complex struct usage: multiple types, nested structs with arrays, multi-level access

struct Stats {
    int min;
    int max;
    int sum;
    int count;
};

struct Inventory {
    int quantities[6];
    int num_items;
    int total;
};

struct Address {
    int zip;
    int street_num;
};

struct Employee {
    int id;
    int age;
    struct Address addr;
    int scores[3];
};

struct Team {
    struct Employee members[3];
    int size;
    int team_id;
};

struct Wrapper {
    struct Stats stats;
    int data[5];
};

// Compute stats from an array
struct Stats compute_stats(int *arr, int n) {
    struct Stats s;
    s.min = arr[0];
    s.max = arr[0];
    s.sum = 0;
    s.count = n;
    int i;
    for (i = 0; i < n; i = i + 1) {
        s.sum = s.sum + arr[i];
        if (arr[i] < s.min) s.min = arr[i];
        if (arr[i] > s.max) s.max = arr[i];
    }
    return s;
}

int inventory_total(struct Inventory *inv) {
    int total = 0;
    int i;
    for (i = 0; i < inv->num_items; i = i + 1) {
        total = total + inv->quantities[i];
    }
    return total;
}

int employee_score_avg(struct Employee *e) {
    int sum = e->scores[0] + e->scores[1] + e->scores[2];
    return sum / 3;
}

int team_total_scores(struct Team *t) {
    int total = 0;
    int i;
    int j;
    for (i = 0; i < t->size; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            total = total + t->members[i].scores[j];
        }
    }
    return total;
}

int main(void) {
    // Stats struct from array
    int data[5];
    data[0] = 42;
    data[1] = 17;
    data[2] = 93;
    data[3] = 5;
    data[4] = 68;
    struct Stats st = compute_stats(data, 5);
    // EXPECT: 5 93
    printf("%d %d\n", st.min, st.max);
    // sum = 42+17+93+5+68 = 225
    // EXPECT: 225 5
    printf("%d %d\n", st.sum, st.count);

    // Average (integer)
    int avg = st.sum / st.count;
    // EXPECT: 45
    printf("%d\n", avg);

    // Inventory struct
    struct Inventory inv;
    inv.num_items = 6;
    inv.quantities[0] = 10;
    inv.quantities[1] = 25;
    inv.quantities[2] = 7;
    inv.quantities[3] = 33;
    inv.quantities[4] = 12;
    inv.quantities[5] = 8;
    inv.total = inventory_total(&inv);
    // 10+25+7+33+12+8 = 95
    // EXPECT: 95
    printf("%d\n", inv.total);

    // Employee with nested address and scores
    struct Employee emp;
    emp.id = 1001;
    emp.age = 30;
    emp.addr.zip = 90210;
    emp.addr.street_num = 42;
    emp.scores[0] = 85;
    emp.scores[1] = 92;
    emp.scores[2] = 78;
    // EXPECT: 1001 30
    printf("%d %d\n", emp.id, emp.age);
    // EXPECT: 90210 42
    printf("%d %d\n", emp.addr.zip, emp.addr.street_num);
    // EXPECT: 85 92 78
    printf("%d %d %d\n", emp.scores[0], emp.scores[1], emp.scores[2]);

    // Employee score average
    int eavg = employee_score_avg(&emp);
    // (85+92+78)/3 = 255/3 = 85
    // EXPECT: 85
    printf("%d\n", eavg);

    // Team struct with array of employees
    struct Team team;
    team.size = 3;
    team.team_id = 7;

    // Employee 0
    team.members[0].id = 100;
    team.members[0].age = 25;
    team.members[0].addr.zip = 10001;
    team.members[0].addr.street_num = 1;
    team.members[0].scores[0] = 70;
    team.members[0].scores[1] = 80;
    team.members[0].scores[2] = 90;

    // Employee 1
    team.members[1].id = 101;
    team.members[1].age = 28;
    team.members[1].addr.zip = 20002;
    team.members[1].addr.street_num = 2;
    team.members[1].scores[0] = 60;
    team.members[1].scores[1] = 75;
    team.members[1].scores[2] = 85;

    // Employee 2
    team.members[2].id = 102;
    team.members[2].age = 35;
    team.members[2].addr.zip = 30003;
    team.members[2].addr.street_num = 3;
    team.members[2].scores[0] = 95;
    team.members[2].scores[1] = 88;
    team.members[2].scores[2] = 72;

    // Access nested fields: team -> member -> address -> zip
    // EXPECT: 10001 20002 30003
    printf("%d %d %d\n", team.members[0].addr.zip, team.members[1].addr.zip, team.members[2].addr.zip);

    // Total scores across all team members
    int ts = team_total_scores(&team);
    // 70+80+90 + 60+75+85 + 95+88+72 = 240+220+255 = 715
    // EXPECT: 715
    printf("%d\n", ts);

    // Per-member average
    int avg0 = employee_score_avg(&team.members[0]);
    int avg1 = employee_score_avg(&team.members[1]);
    int avg2 = employee_score_avg(&team.members[2]);
    // (70+80+90)/3=80, (60+75+85)/3=73, (95+88+72)/3=85
    // EXPECT: 80 73 85
    printf("%d %d %d\n", avg0, avg1, avg2);

    // Wrapper struct: Stats + data array
    struct Wrapper w;
    w.data[0] = 2;
    w.data[1] = 8;
    w.data[2] = 4;
    w.data[3] = 6;
    w.data[4] = 10;
    w.stats = compute_stats(w.data, 5);
    // EXPECT: 2 10
    printf("%d %d\n", w.stats.min, w.stats.max);
    // 2+8+4+6+10 = 30
    // EXPECT: 30
    printf("%d\n", w.stats.sum);

    // Conditional on struct member
    int youngest_id = team.members[0].id;
    int youngest_age = team.members[0].age;
    int i;
    for (i = 1; i < team.size; i = i + 1) {
        if (team.members[i].age < youngest_age) {
            youngest_age = team.members[i].age;
            youngest_id = team.members[i].id;
        }
    }
    // EXPECT: 100 25
    printf("%d %d\n", youngest_id, youngest_age);

    // Modify deeply nested field and verify
    team.members[1].addr.zip = 99999;
    // EXPECT: 99999
    printf("%d\n", team.members[1].addr.zip);

    // Other members' addresses unchanged
    // EXPECT: 10001 30003
    printf("%d %d\n", team.members[0].addr.zip, team.members[2].addr.zip);

    return 0;
}
