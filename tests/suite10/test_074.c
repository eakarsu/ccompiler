int printf(const char *fmt, ...);

// Test 074: Validation layers - input validation pipeline

struct ValidationError {
    int code;
    int field_index;
};

struct ValidationResult {
    int is_valid;
    int error_count;
    struct ValidationError errors[10];
};

struct UserInput {
    int age;
    int score;
    int level;
    int team_id;
    char name[32];
};

void init_validation_result(struct ValidationResult *vr) {
    vr->is_valid = 1;
    vr->error_count = 0;
}

void add_error(struct ValidationResult *vr, int code, int field) {
    if (vr->error_count < 10) {
        vr->errors[vr->error_count].code = code;
        vr->errors[vr->error_count].field_index = field;
        vr->error_count++;
    }
    vr->is_valid = 0;
}

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

void validate_age(struct UserInput *u, struct ValidationResult *vr) {
    if (u->age < 0) {
        add_error(vr, 1, 0);
    } else if (u->age > 150) {
        add_error(vr, 2, 0);
    }
}

void validate_score(struct UserInput *u, struct ValidationResult *vr) {
    if (u->score < 0) {
        add_error(vr, 3, 1);
    } else if (u->score > 100) {
        add_error(vr, 4, 1);
    }
}

void validate_level(struct UserInput *u, struct ValidationResult *vr) {
    if (u->level < 1) {
        add_error(vr, 5, 2);
    } else if (u->level > 99) {
        add_error(vr, 6, 2);
    }
}

void validate_team(struct UserInput *u, struct ValidationResult *vr) {
    if (u->team_id < 1 || u->team_id > 20) {
        add_error(vr, 7, 3);
    }
}

void validate_name(struct UserInput *u, struct ValidationResult *vr) {
    int len = str_len(u->name);
    if (len < 2) {
        add_error(vr, 8, 4);
    } else if (len > 20) {
        add_error(vr, 9, 4);
    }
}

void validate_cross_field(struct UserInput *u, struct ValidationResult *vr) {
    if (u->age < 18 && u->level > 50) {
        add_error(vr, 10, 2);
    }
    if (u->score > 90 && u->level < 5) {
        add_error(vr, 11, 1);
    }
}

char *field_name(int idx) {
    if (idx == 0) return "age";
    if (idx == 1) return "score";
    if (idx == 2) return "level";
    if (idx == 3) return "team_id";
    if (idx == 4) return "name";
    return "unknown";
}

struct ValidationResult validate_all(struct UserInput *u) {
    struct ValidationResult vr;
    init_validation_result(&vr);

    validate_age(u, &vr);
    validate_score(u, &vr);
    validate_level(u, &vr);
    validate_team(u, &vr);
    validate_name(u, &vr);
    validate_cross_field(u, &vr);

    return vr;
}

int main() {
    struct UserInput u1;
    u1.age = 25;
    u1.score = 85;
    u1.level = 10;
    u1.team_id = 5;
    str_copy(u1.name, "Alice");

    struct ValidationResult vr = validate_all(&u1);
    printf("User1 valid=%d errors=%d\n", vr.is_valid, vr.error_count);
    // EXPECT: User1 valid=1 errors=0

    struct UserInput u2;
    u2.age = -5;
    u2.score = 200;
    u2.level = 0;
    u2.team_id = 50;
    str_copy(u2.name, "X");

    vr = validate_all(&u2);
    printf("User2 valid=%d errors=%d\n", vr.is_valid, vr.error_count);
    // EXPECT: User2 valid=0 errors=6
    for (int i = 0; i < vr.error_count; i++) {
        printf("  err %d: code=%d field=%s\n", i,
               vr.errors[i].code, field_name(vr.errors[i].field_index));
    }
    // EXPECT:   err 0: code=1 field=age
    // EXPECT:   err 1: code=4 field=score
    // EXPECT:   err 2: code=5 field=level
    // EXPECT:   err 3: code=7 field=team_id
    // EXPECT:   err 4: code=8 field=name
    // EXPECT:   err 5: code=11 field=score

    struct UserInput u3;
    u3.age = 15;
    u3.score = 95;
    u3.level = 60;
    u3.team_id = 3;
    str_copy(u3.name, "Bob");

    vr = validate_all(&u3);
    printf("User3 valid=%d errors=%d\n", vr.is_valid, vr.error_count);
    // EXPECT: User3 valid=0 errors=1
    for (int i = 0; i < vr.error_count; i++) {
        printf("  err %d: code=%d field=%s\n", i,
               vr.errors[i].code, field_name(vr.errors[i].field_index));
    }
    // EXPECT:   err 0: code=10 field=level

    struct UserInput u4;
    u4.age = 150;
    u4.score = 100;
    u4.level = 99;
    u4.team_id = 20;
    str_copy(u4.name, "Max");

    vr = validate_all(&u4);
    printf("User4 valid=%d errors=%d\n", vr.is_valid, vr.error_count);
    // EXPECT: User4 valid=1 errors=0

    printf("Validation tests done\n");
    // EXPECT: Validation tests done
    return 0;
}
