int printf(const char *fmt, ...);

// Brute-Force PIN Cracker Simulation

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

// Simple hash for PIN verification (not storing PINs in plaintext)
int hash_pin(char *pin) {
    int h = 5381;
    int i = 0;
    while (pin[i] != 0) {
        h = h * 33 + (int)pin[i];
        i = i + 1;
    }
    return h;
}

// Brute force a 4-digit numeric PIN given its hash
// Returns number of attempts, fills found_pin
int brute_force_4digit(int target_hash, char *found_pin) {
    int attempts = 0;
    char try_pin[5];
    try_pin[4] = 0;

    int d0;
    int d1;
    int d2;
    int d3;
    for (d0 = 0; d0 < 10; d0++) {
        for (d1 = 0; d1 < 10; d1++) {
            for (d2 = 0; d2 < 10; d2++) {
                for (d3 = 0; d3 < 10; d3++) {
                    try_pin[0] = (char)(48 + d0);
                    try_pin[1] = (char)(48 + d1);
                    try_pin[2] = (char)(48 + d2);
                    try_pin[3] = (char)(48 + d3);
                    attempts = attempts + 1;
                    if (hash_pin(try_pin) == target_hash) {
                        found_pin[0] = try_pin[0];
                        found_pin[1] = try_pin[1];
                        found_pin[2] = try_pin[2];
                        found_pin[3] = try_pin[3];
                        found_pin[4] = 0;
                        return attempts;
                    }
                }
            }
        }
    }
    found_pin[0] = 0;
    return attempts;
}

// Rate limiting simulation
struct RateLimiter {
    int max_attempts;
    int current_attempts;
    int locked;
    int lockout_remaining;
};

void rate_init(struct RateLimiter *rl, int max) {
    rl->max_attempts = max;
    rl->current_attempts = 0;
    rl->locked = 0;
    rl->lockout_remaining = 0;
}

int rate_try(struct RateLimiter *rl) {
    if (rl->locked) {
        rl->lockout_remaining = rl->lockout_remaining - 1;
        if (rl->lockout_remaining <= 0) {
            rl->locked = 0;
            rl->current_attempts = 0;
        } else {
            return 0; // still locked
        }
    }
    rl->current_attempts = rl->current_attempts + 1;
    if (rl->current_attempts >= rl->max_attempts) {
        rl->locked = 1;
        rl->lockout_remaining = 3; // lockout for 3 cycles
        return 0; // locked out
    }
    return 1; // allowed
}

// Estimate brute force time for different PIN lengths
int pin_space_size(int length, int charset_size) {
    int result = 1;
    int i;
    for (i = 0; i < length; i++) {
        result = result * charset_size;
    }
    return result;
}

// Dictionary attack: check against common PINs
int dictionary_attack(int target_hash) {
    char *common_pins[10];
    common_pins[0] = "1234";
    common_pins[1] = "0000";
    common_pins[2] = "1111";
    common_pins[3] = "1212";
    common_pins[4] = "7777";
    common_pins[5] = "1004";
    common_pins[6] = "2000";
    common_pins[7] = "4321";
    common_pins[8] = "2001";
    common_pins[9] = "9999";

    int i;
    for (i = 0; i < 10; i++) {
        if (hash_pin(common_pins[i]) == target_hash) {
            return i + 1; // found at position i+1
        }
    }
    return 0; // not found
}

int main() {
    char found[8];

    // Test 1: crack PIN "1234"
    int target = hash_pin("1234");
    int attempts = brute_force_4digit(target, found);
    printf("Cracked 1234: %s\n", found);
    // EXPECT: Cracked 1234: 1234
    printf("Attempts for 1234: %d\n", attempts);
    // EXPECT: Attempts for 1234: 1235

    // Test 2: crack PIN "0000"
    target = hash_pin("0000");
    attempts = brute_force_4digit(target, found);
    printf("Cracked 0000: %s\n", found);
    // EXPECT: Cracked 0000: 0000
    printf("Attempts for 0000: %d\n", attempts);
    // EXPECT: Attempts for 0000: 1

    // Test 3: crack PIN "9999"
    target = hash_pin("9999");
    attempts = brute_force_4digit(target, found);
    printf("Cracked 9999: %s\n", found);
    // EXPECT: Cracked 9999: 9999
    printf("Attempts for 9999: %d\n", attempts);
    // EXPECT: Attempts for 9999: 10000

    // Test 4: crack PIN "4567"
    target = hash_pin("4567");
    attempts = brute_force_4digit(target, found);
    printf("Cracked 4567: %s\n", found);
    // EXPECT: Cracked 4567: 4567

    // Test 5: rate limiter
    struct RateLimiter rl;
    rate_init(&rl, 3);
    int r1 = rate_try(&rl);
    int r2 = rate_try(&rl);
    int r3 = rate_try(&rl); // should lock
    printf("Try 1: %d\n", r1);
    // EXPECT: Try 1: 1
    printf("Try 2: %d\n", r2);
    // EXPECT: Try 2: 1
    printf("Try 3 (lock): %d\n", r3);
    // EXPECT: Try 3 (lock): 0

    // Test 6: rate limiter locked
    int r4 = rate_try(&rl);
    printf("While locked: %d\n", r4);
    // EXPECT: While locked: 0

    // Test 7: rate limiter unlocks after cooldown
    rate_try(&rl); // lockout_remaining goes from 2 to 1
    int r6 = rate_try(&rl); // lockout_remaining goes from 1 to 0, unlocks
    printf("After cooldown: %d\n", r6);
    // EXPECT: After cooldown: 1

    // Test 8: PIN space sizes
    int space4 = pin_space_size(4, 10);
    printf("4-digit space: %d\n", space4);
    // EXPECT: 4-digit space: 10000

    int space6 = pin_space_size(6, 10);
    printf("6-digit space: %d\n", space6);
    // EXPECT: 6-digit space: 1000000

    int space4alpha = pin_space_size(4, 36);
    printf("4-char alphanumeric space: %d\n", space4alpha);
    // EXPECT: 4-char alphanumeric space: 1679616

    // Test 9: dictionary attack
    int dict_target = hash_pin("7777");
    int dict_pos = dictionary_attack(dict_target);
    printf("Dictionary found 7777 at: %d\n", dict_pos);
    // EXPECT: Dictionary found 7777 at: 5

    int dict_target2 = hash_pin("5555");
    int dict_pos2 = dictionary_attack(dict_target2);
    printf("Dictionary found 5555: %d\n", dict_pos2);
    // EXPECT: Dictionary found 5555: 0

    // Test 10: hash uniqueness for sequential PINs
    int unique = 1;
    int hashes[10];
    int i;
    for (i = 0; i < 10; i++) {
        char pin[5];
        pin[0] = (char)(48 + i);
        pin[1] = (char)(48 + i);
        pin[2] = (char)(48 + i);
        pin[3] = (char)(48 + i);
        pin[4] = 0;
        hashes[i] = hash_pin(pin);
    }
    int j;
    for (i = 0; i < 10; i++) {
        for (j = i + 1; j < 10; j++) {
            if (hashes[i] == hashes[j]) unique = 0;
        }
    }
    printf("All hashes unique: %d\n", unique);
    // EXPECT: All hashes unique: 1

    printf("PIN cracker tests passed\n");
    // EXPECT: PIN cracker tests passed

    return 0;
}
