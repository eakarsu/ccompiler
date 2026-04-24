int printf(const char *fmt, ...);

// Trie (prefix tree) using arrays - insert and search words

struct TrieNode {
    int children[26];
    int is_end;
    int count;
};

struct Trie {
    struct TrieNode nodes[100];
    int size;
};

void trie_init(struct Trie *t) {
    int i;
    int j;
    t->size = 1;
    for (i = 0; i < 100; i++) {
        for (j = 0; j < 26; j++) {
            t->nodes[i].children[j] = -1;
        }
        t->nodes[i].is_end = 0;
        t->nodes[i].count = 0;
    }
}

void trie_insert(struct Trie *t, char *word) {
    int cur = 0;
    int i = 0;
    int c;
    while (word[i] != 0) {
        c = word[i] - 'a';
        if (t->nodes[cur].children[c] == -1) {
            t->nodes[cur].children[c] = t->size;
            t->size = t->size + 1;
        }
        cur = t->nodes[cur].children[c];
        t->nodes[cur].count = t->nodes[cur].count + 1;
        i = i + 1;
    }
    t->nodes[cur].is_end = 1;
}

int trie_search(struct Trie *t, char *word) {
    int cur = 0;
    int i = 0;
    int c;
    while (word[i] != 0) {
        c = word[i] - 'a';
        if (t->nodes[cur].children[c] == -1) {
            return 0;
        }
        cur = t->nodes[cur].children[c];
        i = i + 1;
    }
    return t->nodes[cur].is_end;
}

int trie_starts_with(struct Trie *t, char *prefix) {
    int cur = 0;
    int i = 0;
    int c;
    while (prefix[i] != 0) {
        c = prefix[i] - 'a';
        if (t->nodes[cur].children[c] == -1) {
            return 0;
        }
        cur = t->nodes[cur].children[c];
        i = i + 1;
    }
    return t->nodes[cur].count;
}

int trie_count_words(struct Trie *t) {
    int count = 0;
    int i;
    for (i = 0; i < t->size; i++) {
        if (t->nodes[i].is_end) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    struct Trie trie;
    trie_init(&trie);

    trie_insert(&trie, "apple");
    trie_insert(&trie, "app");
    trie_insert(&trie, "ape");
    trie_insert(&trie, "bat");
    trie_insert(&trie, "bath");
    trie_insert(&trie, "batch");

    printf("search apple: %d\n", trie_search(&trie, "apple"));    // EXPECT: search apple: 1
    printf("search app: %d\n", trie_search(&trie, "app"));        // EXPECT: search app: 1
    printf("search ap: %d\n", trie_search(&trie, "ap"));          // EXPECT: search ap: 0
    printf("search bat: %d\n", trie_search(&trie, "bat"));        // EXPECT: search bat: 1
    printf("search bats: %d\n", trie_search(&trie, "bats"));      // EXPECT: search bats: 0
    printf("search xyz: %d\n", trie_search(&trie, "xyz"));        // EXPECT: search xyz: 0

    printf("prefix ap: %d\n", trie_starts_with(&trie, "ap"));     // EXPECT: prefix ap: 3
    printf("prefix app: %d\n", trie_starts_with(&trie, "app"));   // EXPECT: prefix app: 2
    printf("prefix bat: %d\n", trie_starts_with(&trie, "bat"));   // EXPECT: prefix bat: 3
    printf("prefix b: %d\n", trie_starts_with(&trie, "b"));       // EXPECT: prefix b: 3
    printf("prefix z: %d\n", trie_starts_with(&trie, "z"));       // EXPECT: prefix z: 0

    printf("total words: %d\n", trie_count_words(&trie));          // EXPECT: total words: 6
    printf("nodes used: %d\n", trie.size);                         // EXPECT: nodes used: 13

    trie_insert(&trie, "apply");
    trie_insert(&trie, "application");
    printf("after insert total: %d\n", trie_count_words(&trie));   // EXPECT: after insert total: 8
    printf("prefix appl: %d\n", trie_starts_with(&trie, "appl")); // EXPECT: prefix appl: 3

    return 0;
}
