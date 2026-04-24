int printf(const char *fmt, ...);

// Password Strength Checker

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int is_upper(char c) {
    return c >= 65 && c <= 90;
}

int is_lower(char c) {
    return c >= 97 && c <= 122;
}

int is_digit(char c) {
    return c >= 48 && c <= 57;
}

int is_special(char c) {
    if (c >= 33 && c <= 47) return 1;   // ! " # $ % & ' ( ) * + , - . /
    if (c >= 58 && c <= 64) return 1;   // : ; < = > ? @
    if (c >= 91 && c <= 96) return 1;   // [ \ ] ^ _ `
    if (c >= 123 && c <= 126) return 1; // { | } ~
    return 0;
}

struct StrengthResult {
    int score;
    int has_upper;
    int has_lower;
    int has_digit;
    int has_special;
    int length;
    int has_repeat;
    int has_sequence;
};

void check_strength(char *password, struct StrengthResult *res) {
    int len = str_len(password);
    res->length = len;
    res->has_upper = 0;
    res->has_lower = 0;
    res->has_digit = 0;
    res->has_special = 0;
    res->has_repeat = 0;
    res->has_sequence = 0;
    res->score = 0;

    int i;
    int upper_count = 0;
    int lower_count = 0;
    int digit_count = 0;
    int special_count = 0;

    for (i = 0; i < len; i++) {
        char c = password[i];
        if (is_upper(c)) { res->has_upper = 1; upper_count = upper_count + 1; }
        if (is_lower(c)) { res->has_lower = 1; lower_count = lower_count + 1; }
        if (is_digit(c)) { res->has_digit = 1; digit_count = digit_count + 1; }
        if (is_special(c)) { res->has_special = 1; special_count = special_count + 1; }
    }

    // Check for repeated characters (3+ in a row)
    for (i = 0; i < len - 2; i++) {
        if (password[i] == password[i+1] && password[i+1] == password[i+2]) {
            res->has_repeat = 1;
        }
    }

    // Check for sequential characters (abc, 123, etc.)
    for (i = 0; i < len - 2; i++) {
        int c0 = (int)password[i];
        int c1 = (int)password[i+1];
        int c2 = (int)password[i+2];
        if (c1 == c0 + 1 && c2 == c1 + 1) {
            res->has_sequence = 1;
        }
    }

    // Score calculation
    // Length points
    if (len >= 8) res->score = res->score + 1;
    if (len >= 12) res->score = res->score + 1;
    if (len >= 16) res->score = res->score + 1;

    // Character variety
    int variety = res->has_upper + res->has_lower + res->has_digit + res->has_special;
    res->score = res->score + variety;

    // Penalties
    if (res->has_repeat) res->score = res->score - 1;
    if (res->has_sequence) res->score = res->score - 1;

    // Bonus for mixing
    if (variety >= 3 && len >= 10) res->score = res->score + 1;

    if (res->score < 0) res->score = 0;
}

char *strength_label(int score) {
    if (score <= 2) return "weak";
    if (score <= 4) return "moderate";
    if (score <= 6) return "strong";
    return "very strong";
}

// Check if password contains a common pattern
int has_common_pattern(char *password) {
    // Check for "password", "1234", "qwerty" substrings
    char *patterns[4];
    patterns[0] = "pass";
    patterns[1] = "1234";
    patterns[2] = "qwer";
    patterns[3] = "abcd";

    int plen = str_len(password);
    int p;
    for (p = 0; p < 4; p++) {
        int patlen = str_len(patterns[p]);
        int i;
        for (i = 0; i <= plen - patlen; i++) {
            int match = 1;
            int j;
            for (j = 0; j < patlen; j++) {
                char pc = password[i + j];
                char pp = patterns[p][j];
                // Case-insensitive comparison
                if (is_upper(pc)) pc = (char)(pc + 32);
                if (pc != pp) { match = 0; break; }
            }
            if (match) return 1;
        }
    }
    return 0;
}

int main() {
    struct StrengthResult res;

    // Test 1: weak password - too short, only lowercase
    check_strength("abc", &res);
    printf("abc score: %d\n", res.score);
    // EXPECT: abc score: 0
    printf("abc label: %s\n", strength_label(res.score));
    // EXPECT: abc label: weak

    // Test 2: moderate password
    check_strength("Hello123", &res);
    printf("Hello123 score: %d\n", res.score);
    // EXPECT: Hello123 score: 3
    printf("Hello123 label: %s\n", strength_label(res.score));
    // EXPECT: Hello123 label: moderate

    // Test 3: strong password
    check_strength("MyP@ssw0rd!", &res);
    printf("MyP@ssw0rd! score: %d\n", res.score);
    // EXPECT: MyP@ssw0rd! score: 6
    printf("MyP@ssw0rd! label: %s\n", strength_label(res.score));
    // EXPECT: MyP@ssw0rd! label: strong

    // Test 4: password with repeats
    check_strength("aaabbb11", &res);
    printf("Repeats detected: %d\n", res.has_repeat);
    // EXPECT: Repeats detected: 1
    printf("aaabbb11 score: %d\n", res.score);
    // EXPECT: aaabbb11 score: 2

    // Test 5: password with sequences
    check_strength("xyz12345", &res);
    printf("Sequence detected: %d\n", res.has_sequence);
    // EXPECT: Sequence detected: 1

    // Test 6: all digits only
    check_strength("99887766", &res);
    printf("Digits only score: %d\n", res.score);
    // EXPECT: Digits only score: 2

    // Test 7: character variety detection
    check_strength("aA1!", &res);
    printf("Has upper: %d\n", res.has_upper);
    // EXPECT: Has upper: 1
    printf("Has lower: %d\n", res.has_lower);
    // EXPECT: Has lower: 1
    printf("Has digit: %d\n", res.has_digit);
    // EXPECT: Has digit: 1
    printf("Has special: %d\n", res.has_special);
    // EXPECT: Has special: 1

    // Test 8: common pattern detection
    int cp1 = has_common_pattern("mypassword");
    printf("Common pattern pass: %d\n", cp1);
    // EXPECT: Common pattern pass: 1

    int cp2 = has_common_pattern("1234abcd");
    printf("Common pattern 1234: %d\n", cp2);
    // EXPECT: Common pattern 1234: 1

    int cp3 = has_common_pattern("Zx9vKm2w");
    printf("No common pattern: %d\n", cp3);
    // EXPECT: No common pattern: 0

    // Test 9: long password bonus
    check_strength("abcdefghijklmnop", &res);
    printf("Long length: %d\n", res.length);
    // EXPECT: Long length: 16

    // Test 10: empty password
    check_strength("", &res);
    printf("Empty score: %d\n", res.score);
    // EXPECT: Empty score: 0
    printf("Empty length: %d\n", res.length);
    // EXPECT: Empty length: 0

    printf("Password strength tests passed\n");
    // EXPECT: Password strength tests passed

    return 0;
}
