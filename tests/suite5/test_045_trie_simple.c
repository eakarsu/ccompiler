int printf(const char *fmt, ...);

int trie_children[256][10];
int trie_is_end[256];
int trie_count[256];
int trie_next;

void trie_init(void) {
    int i;
    int j;
    trie_next = 1;
    i = 0;
    while (i < 256) {
        trie_is_end[i] = 0;
        trie_count[i] = 0;
        j = 0;
        while (j < 10) {
            trie_children[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
}

void trie_insert(int digits[], int len) {
    int node;
    int i;
    int d;
    node = 0;
    i = 0;
    while (i < len) {
        d = digits[i];
        trie_count[node] = trie_count[node] + 1;
        if (trie_children[node][d] == 0) {
            trie_children[node][d] = trie_next;
            trie_next = trie_next + 1;
        }
        node = trie_children[node][d];
        i = i + 1;
    }
    trie_count[node] = trie_count[node] + 1;
    trie_is_end[node] = 1;
}

int trie_search(int digits[], int len) {
    int node;
    int i;
    int d;
    node = 0;
    i = 0;
    while (i < len) {
        d = digits[i];
        if (trie_children[node][d] == 0) {
            return 0;
        }
        node = trie_children[node][d];
        i = i + 1;
    }
    return trie_is_end[node];
}

int trie_prefix_count(int digits[], int len) {
    int node;
    int i;
    int d;
    node = 0;
    i = 0;
    while (i < len) {
        d = digits[i];
        if (trie_children[node][d] == 0) {
            return 0;
        }
        node = trie_children[node][d];
        i = i + 1;
    }
    return trie_count[node];
}

int trie_starts_with(int digits[], int len) {
    int node;
    int i;
    int d;
    node = 0;
    i = 0;
    while (i < len) {
        d = digits[i];
        if (trie_children[node][d] == 0) {
            return 0;
        }
        node = trie_children[node][d];
        i = i + 1;
    }
    return 1;
}

int main(void) {
    int d1[3];
    int d2[3];
    int d3[2];
    int d4[4];
    int d5[3];
    int prefix[1];
    int prefix2[2];
    int ret;

    trie_init();

    d1[0] = 1; d1[1] = 2; d1[2] = 3;
    d2[0] = 1; d2[1] = 2; d2[2] = 5;
    d3[0] = 1; d3[1] = 2;
    d4[0] = 1; d4[1] = 2; d4[2] = 3; d4[3] = 4;
    d5[0] = 9; d5[1] = 8; d5[2] = 7;

    trie_insert(d1, 3);
    trie_insert(d2, 3);
    trie_insert(d3, 2);
    trie_insert(d4, 4);
    trie_insert(d5, 3);

    // EXPECT: nodes used: 9
    printf("nodes used: %d\n", trie_next);

    ret = trie_search(d1, 3);
    // EXPECT: search 123: 1
    printf("search 123: %d\n", ret);

    ret = trie_search(d2, 3);
    // EXPECT: search 125: 1
    printf("search 125: %d\n", ret);

    ret = trie_search(d3, 2);
    // EXPECT: search 12: 1
    printf("search 12: %d\n", ret);

    prefix[0] = 5;
    ret = trie_search(prefix, 1);
    // EXPECT: search 5: 0
    printf("search 5: %d\n", ret);

    prefix[0] = 1;
    ret = trie_starts_with(prefix, 1);
    // EXPECT: prefix 1: 1
    printf("prefix 1: %d\n", ret);

    prefix[0] = 9;
    ret = trie_starts_with(prefix, 1);
    // EXPECT: prefix 9: 1
    printf("prefix 9: %d\n", ret);

    prefix[0] = 5;
    ret = trie_starts_with(prefix, 1);
    // EXPECT: prefix 5: 0
    printf("prefix 5: %d\n", ret);

    prefix[0] = 1;
    ret = trie_prefix_count(prefix, 1);
    // EXPECT: count prefix 1: 4
    printf("count prefix 1: %d\n", ret);

    prefix2[0] = 1; prefix2[1] = 2;
    ret = trie_prefix_count(prefix2, 2);
    // EXPECT: count prefix 12: 4
    printf("count prefix 12: %d\n", ret);

    prefix[0] = 9;
    ret = trie_prefix_count(prefix, 1);
    // EXPECT: count prefix 9: 1
    printf("count prefix 9: %d\n", ret);

    ret = trie_search(d4, 4);
    // EXPECT: search 1234: 1
    printf("search 1234: %d\n", ret);

    ret = trie_search(d5, 3);
    // EXPECT: search 987: 1
    printf("search 987: %d\n", ret);

    prefix2[0] = 1; prefix2[1] = 3;
    ret = trie_starts_with(prefix2, 2);
    // EXPECT: prefix 13: 0
    printf("prefix 13: %d\n", ret);

    prefix2[0] = 9; prefix2[1] = 8;
    ret = trie_prefix_count(prefix2, 2);
    // EXPECT: count prefix 98: 1
    printf("count prefix 98: %d\n", ret);

    // EXPECT: total strings: 5
    printf("total strings: %d\n", trie_count[0]);

    return 0;
}
