int printf(const char *fmt, ...);

// Frequency counter: count occurrences of integers in an array
// Uses a simple hash map approach

int freq_keys[128];
int freq_vals[128];
int freq_used[128];
int freq_cap;
int freq_count;

void freq_init(void) {
    int i;
    freq_cap = 128;
    freq_count = 0;
    for (i = 0; i < freq_cap; i++) {
        freq_keys[i] = 0;
        freq_vals[i] = 0;
        freq_used[i] = 0;
    }
}

int freq_hash(int key) {
    int h;
    h = key;
    if (h < 0) h = -h;
    return h % freq_cap;
}

void freq_increment(int key) {
    int idx;
    int start;
    idx = freq_hash(key);
    start = idx;
    while (1) {
        if (freq_used[idx] == 0) {
            freq_keys[idx] = key;
            freq_vals[idx] = 1;
            freq_used[idx] = 1;
            freq_count = freq_count + 1;
            return;
        }
        if (freq_keys[idx] == key) {
            freq_vals[idx] = freq_vals[idx] + 1;
            return;
        }
        idx = (idx + 1) % freq_cap;
        if (idx == start) return;
    }
}

int freq_get(int key) {
    int idx;
    int start;
    idx = freq_hash(key);
    start = idx;
    while (1) {
        if (freq_used[idx] == 0) return 0;
        if (freq_keys[idx] == key) return freq_vals[idx];
        idx = (idx + 1) % freq_cap;
        if (idx == start) return 0;
    }
    return 0;
}

int freq_max_key(void) {
    int i;
    int mx;
    int mk;
    mx = 0;
    mk = -1;
    for (i = 0; i < freq_cap; i++) {
        if (freq_used[i] && freq_vals[i] > mx) {
            mx = freq_vals[i];
            mk = freq_keys[i];
        }
    }
    return mk;
}

int freq_max_val(void) {
    int i;
    int mx;
    mx = 0;
    for (i = 0; i < freq_cap; i++) {
        if (freq_used[i] && freq_vals[i] > mx) {
            mx = freq_vals[i];
        }
    }
    return mx;
}

int freq_unique_count(void) {
    return freq_count;
}

int freq_singletons(void) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < freq_cap; i++) {
        if (freq_used[i] && freq_vals[i] == 1) c = c + 1;
    }
    return c;
}

int main(void) {
    int data1[20];
    int i;

    freq_init();

    // Dataset: some repeated values
    data1[0] = 5; data1[1] = 3; data1[2] = 5; data1[3] = 7;
    data1[4] = 3; data1[5] = 5; data1[6] = 9; data1[7] = 3;
    data1[8] = 7; data1[9] = 5; data1[10] = 1; data1[11] = 3;
    data1[12] = 5; data1[13] = 7; data1[14] = 9; data1[15] = 1;

    for (i = 0; i < 16; i++) {
        freq_increment(data1[i]);
    }

    // EXPECT: freq 5 = 5
    printf("freq 5 = %d\n", freq_get(5));
    // EXPECT: freq 3 = 4
    printf("freq 3 = %d\n", freq_get(3));
    // EXPECT: freq 7 = 3
    printf("freq 7 = %d\n", freq_get(7));
    // EXPECT: freq 9 = 2
    printf("freq 9 = %d\n", freq_get(9));
    // EXPECT: freq 1 = 2
    printf("freq 1 = %d\n", freq_get(1));
    // EXPECT: freq 100 = 0
    printf("freq 100 = %d\n", freq_get(100));

    // EXPECT: unique count = 5
    printf("unique count = %d\n", freq_unique_count());
    // EXPECT: most frequent key = 5
    printf("most frequent key = %d\n", freq_max_key());
    // EXPECT: max frequency = 5
    printf("max frequency = %d\n", freq_max_val());
    // EXPECT: singletons = 0
    printf("singletons = %d\n", freq_singletons());

    // Add some singletons
    freq_increment(42);
    freq_increment(88);
    // EXPECT: singletons after = 2
    printf("singletons after = %d\n", freq_singletons());
    // EXPECT: unique after = 7
    printf("unique after = %d\n", freq_unique_count());

    // Second dataset: character frequency (as ints)
    freq_init();
    {
        char word[20];
        int j;
        word[0] = 'h'; word[1] = 'e'; word[2] = 'l'; word[3] = 'l';
        word[4] = 'o'; word[5] = 'w'; word[6] = 'o'; word[7] = 'r';
        word[8] = 'l'; word[9] = 'd'; word[10] = 0;
        j = 0;
        while (word[j] != 0) {
            freq_increment(word[j]);
            j = j + 1;
        }
    }
    // EXPECT: freq l = 3
    printf("freq l = %d\n", freq_get('l'));
    // EXPECT: freq o = 2
    printf("freq o = %d\n", freq_get('o'));
    // EXPECT: freq h = 1
    printf("freq h = %d\n", freq_get('h'));
    // EXPECT: char unique = 7
    printf("char unique = %d\n", freq_unique_count());

    // EXPECT: done
    printf("done\n");

    return 0;
}
