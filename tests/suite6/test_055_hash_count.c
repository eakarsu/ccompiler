int printf(const char *fmt, ...);

int ht_keys[64];
int ht_vals[64];
int ht_used[64];
int ht_size;

int hash_int(int x) {
    if (x < 0) x = -x;
    return x % 64;
}

void ht_init(void) {
    ht_size = 0;
    int i;
    for (i = 0; i < 64; i = i + 1) {
        ht_used[i] = 0;
        ht_keys[i] = 0;
        ht_vals[i] = 0;
    }
}

void ht_inc(int key) {
    int h = hash_int(key);
    int i;
    for (i = 0; i < 64; i = i + 1) {
        int idx = (h + i) % 64;
        if (ht_used[idx] == 0) {
            ht_keys[idx] = key;
            ht_vals[idx] = 1;
            ht_used[idx] = 1;
            ht_size = ht_size + 1;
            return;
        }
        if (ht_keys[idx] == key) {
            ht_vals[idx] = ht_vals[idx] + 1;
            return;
        }
    }
}

int ht_get(int key) {
    int h = hash_int(key);
    int i;
    for (i = 0; i < 64; i = i + 1) {
        int idx = (h + i) % 64;
        if (ht_used[idx] == 0) return 0;
        if (ht_keys[idx] == key) return ht_vals[idx];
    }
    return 0;
}

int ht_most_frequent(void) {
    int best_key = -1;
    int best_val = 0;
    int i;
    for (i = 0; i < 64; i = i + 1) {
        if (ht_used[i] && ht_vals[i] > best_val) {
            best_val = ht_vals[i];
            best_key = ht_keys[i];
        }
    }
    return best_key;
}

void ht_top_k(int k, int *out_keys, int *out_vals) {
    int picked[64];
    int i;
    for (i = 0; i < 64; i = i + 1) picked[i] = 0;

    int ki;
    for (ki = 0; ki < k; ki = ki + 1) {
        int best_idx = -1;
        int best_val = -1;
        for (i = 0; i < 64; i = i + 1) {
            if (ht_used[i] && picked[i] == 0 && ht_vals[i] > best_val) {
                best_val = ht_vals[i];
                best_idx = i;
            }
        }
        if (best_idx == -1) {
            out_keys[ki] = -1;
            out_vals[ki] = 0;
        } else {
            out_keys[ki] = ht_keys[best_idx];
            out_vals[ki] = ht_vals[best_idx];
            picked[best_idx] = 1;
        }
    }
}

int main(void) {
    ht_init();

    int data[20];
    data[0] = 3; data[1] = 1; data[2] = 4; data[3] = 1;
    data[4] = 5; data[5] = 9; data[6] = 2; data[7] = 6;
    data[8] = 5; data[9] = 3; data[10] = 5; data[11] = 1;
    data[12] = 3; data[13] = 5; data[14] = 9; data[15] = 2;
    data[16] = 6; data[17] = 5; data[18] = 3; data[19] = 1;

    int i;
    for (i = 0; i < 20; i = i + 1) {
        ht_inc(data[i]);
    }

    // EXPECT: distinct count: 7
    printf("distinct count: %d\n", ht_size);

    // EXPECT: freq of 1: 4
    printf("freq of 1: %d\n", ht_get(1));

    // EXPECT: freq of 3: 4
    printf("freq of 3: %d\n", ht_get(3));

    // EXPECT: freq of 5: 5
    printf("freq of 5: %d\n", ht_get(5));

    // EXPECT: freq of 9: 2
    printf("freq of 9: %d\n", ht_get(9));

    // EXPECT: freq of 2: 2
    printf("freq of 2: %d\n", ht_get(2));

    // EXPECT: freq of 4: 1
    printf("freq of 4: %d\n", ht_get(4));

    // EXPECT: freq of 6: 2
    printf("freq of 6: %d\n", ht_get(6));

    // EXPECT: freq of 7: 0
    printf("freq of 7: %d\n", ht_get(7));

    // EXPECT: most frequent: 5
    printf("most frequent: %d\n", ht_most_frequent());

    int top_k[3];
    int top_v[3];
    ht_top_k(3, top_k, top_v);

    // EXPECT: top1 val: 5
    printf("top1 val: %d\n", top_v[0]);

    // EXPECT: top1 key: 5
    printf("top1 key: %d\n", top_k[0]);

    // EXPECT: top2 val: 4
    printf("top2 val: %d\n", top_v[1]);

    // EXPECT: top3 val: 4
    printf("top3 val: %d\n", top_v[2]);

    ht_init();
    char text[] = "abracadabra";
    for (i = 0; text[i] != 0; i = i + 1) {
        ht_inc(text[i]);
    }

    // EXPECT: char a freq: 5
    printf("char a freq: %d\n", ht_get(97));

    // EXPECT: char b freq: 2
    printf("char b freq: %d\n", ht_get(98));

    // EXPECT: char r freq: 2
    printf("char r freq: %d\n", ht_get(114));

    // EXPECT: char c freq: 1
    printf("char c freq: %d\n", ht_get(99));

    // EXPECT: char d freq: 1
    printf("char d freq: %d\n", ht_get(100));

    // EXPECT: done
    printf("done\n");

    return 0;
}
