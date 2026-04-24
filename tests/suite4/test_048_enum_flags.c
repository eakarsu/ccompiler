int printf(const char *fmt, ...);

enum Permission {
    PERM_NONE    = 0,
    PERM_READ    = 1,
    PERM_WRITE   = 2,
    PERM_EXEC    = 4
};

enum FileFlag {
    FLAG_NONE     = 0,
    FLAG_HIDDEN   = 1,
    FLAG_SYSTEM   = 2,
    FLAG_READONLY = 4,
    FLAG_ARCHIVE  = 8
};

enum ColorChannel {
    CHAN_NONE  = 0,
    CHAN_RED   = 1,
    CHAN_GREEN = 2,
    CHAN_BLUE  = 4,
    CHAN_ALPHA = 8
};

int has_permission(int perms, int perm) {
    return (perms & perm) != 0;
}

int set_flag(int flags, int flag) {
    return flags | flag;
}

int clear_flag(int flags, int flag) {
    return flags & ~flag;
}

int toggle_flag(int flags, int flag) {
    return flags ^ flag;
}

int count_flags(int flags) {
    int count = 0;
    if (flags & 1) count = count + 1;
    if (flags & 2) count = count + 1;
    if (flags & 4) count = count + 1;
    if (flags & 8) count = count + 1;
    return count;
}

int test_basic_or(void) {
    int perms = PERM_READ | PERM_WRITE;
    return perms; // 1 | 2 = 3
}

int test_basic_and(void) {
    int perms = PERM_READ | PERM_WRITE | PERM_EXEC; // 7
    return perms & PERM_WRITE; // 2
}

int test_has_read(void) {
    int perms = PERM_READ | PERM_EXEC; // 5
    return has_permission(perms, PERM_READ); // 1
}

int test_no_write(void) {
    int perms = PERM_READ | PERM_EXEC; // 5
    return has_permission(perms, PERM_WRITE); // 0
}

int test_all_perms(void) {
    int perms = PERM_READ | PERM_WRITE | PERM_EXEC; // 7
    return perms;
}

int test_set_flag(void) {
    int flags = FLAG_HIDDEN; // 1
    flags = set_flag(flags, FLAG_ARCHIVE); // 1 | 8 = 9
    return flags;
}

int test_clear_flag(void) {
    int flags = FLAG_HIDDEN | FLAG_SYSTEM | FLAG_READONLY; // 1|2|4 = 7
    flags = clear_flag(flags, FLAG_SYSTEM); // 7 & ~2 = 7 & (-3) = 5
    return flags;
}

int test_toggle_on(void) {
    int flags = FLAG_HIDDEN; // 1
    flags = toggle_flag(flags, FLAG_ARCHIVE); // 1 ^ 8 = 9
    return flags;
}

int test_toggle_off(void) {
    int flags = FLAG_HIDDEN | FLAG_ARCHIVE; // 1 | 8 = 9
    flags = toggle_flag(flags, FLAG_ARCHIVE); // 9 ^ 8 = 1
    return flags;
}

int test_count_flags(void) {
    int flags = FLAG_HIDDEN | FLAG_SYSTEM | FLAG_ARCHIVE; // 1|2|8 = 11
    return count_flags(flags); // bits: 1,2,8 -> 3
}

int test_combine_channels(void) {
    int channels = CHAN_RED | CHAN_GREEN | CHAN_BLUE; // 1|2|4 = 7
    return channels;
}

int test_channel_check(void) {
    int channels = CHAN_RED | CHAN_ALPHA; // 1|8 = 9
    int has_red = (channels & CHAN_RED) != 0;
    int has_green = (channels & CHAN_GREEN) != 0;
    int has_alpha = (channels & CHAN_ALPHA) != 0;
    return has_red * 100 + has_green * 10 + has_alpha;
    // 1*100 + 0*10 + 1 = 101
}

int test_flag_accumulate(void) {
    int flags = FLAG_NONE;
    flags = flags | FLAG_HIDDEN;
    flags = flags | FLAG_SYSTEM;
    flags = flags | FLAG_READONLY;
    flags = flags | FLAG_ARCHIVE;
    return flags; // 1|2|4|8 = 15
}

int test_flag_array(void) {
    int flags[4];
    flags[0] = PERM_READ;
    flags[1] = PERM_WRITE;
    flags[2] = PERM_EXEC;
    flags[3] = PERM_READ | PERM_WRITE | PERM_EXEC;
    int combined = 0;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        combined = combined | flags[i];
    }
    return combined; // 7
}

int test_xor_swap(void) {
    int a = PERM_READ;  // 1
    int b = PERM_EXEC;  // 4
    a = a ^ b;
    b = b ^ a;
    a = a ^ b;
    // after swap: a=4, b=1
    return a * 10 + b; // 41
}

int test_not_flags(void) {
    int perms = PERM_READ | PERM_WRITE; // 3
    int missing = (~perms) & (PERM_READ | PERM_WRITE | PERM_EXEC);
    // ~3 & 7 = ...11111100 & 111 = 100 = 4
    return missing; // 4 (PERM_EXEC)
}

int main(void) {
    // EXPECT: basic or: 3
    printf("basic or: %d\n", test_basic_or());

    // EXPECT: basic and: 2
    printf("basic and: %d\n", test_basic_and());

    // EXPECT: has read: 1
    printf("has read: %d\n", test_has_read());

    // EXPECT: no write: 0
    printf("no write: %d\n", test_no_write());

    // EXPECT: all perms: 7
    printf("all perms: %d\n", test_all_perms());

    // EXPECT: set flag: 9
    printf("set flag: %d\n", test_set_flag());

    // EXPECT: clear flag: 5
    printf("clear flag: %d\n", test_clear_flag());

    // EXPECT: toggle on: 9
    printf("toggle on: %d\n", test_toggle_on());

    // EXPECT: toggle off: 1
    printf("toggle off: %d\n", test_toggle_off());

    // EXPECT: count flags: 3
    printf("count flags: %d\n", test_count_flags());

    // EXPECT: combine channels: 7
    printf("combine channels: %d\n", test_combine_channels());

    // EXPECT: channel check: 101
    printf("channel check: %d\n", test_channel_check());

    // EXPECT: flag accumulate: 15
    printf("flag accumulate: %d\n", test_flag_accumulate());

    // EXPECT: flag array: 7
    printf("flag array: %d\n", test_flag_array());

    // EXPECT: xor swap: 41
    printf("xor swap: %d\n", test_xor_swap());

    // EXPECT: not flags missing: 4
    printf("not flags missing: %d\n", test_not_flags());

    int perms = PERM_NONE;
    perms = perms | PERM_READ;
    perms = perms | PERM_EXEC;
    // EXPECT: inline combine: 5
    printf("inline combine: %d\n", perms);

    // EXPECT: inline has exec: 1
    printf("inline has exec: %d\n", has_permission(perms, PERM_EXEC));

    return 0;
}
