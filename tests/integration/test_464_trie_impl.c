int printf(const char *fmt, ...);
// EXPECT: Nodes used: 11\nSearch 'cat': 1\nSearch 'car': 1\nSearch 'card': 1\nSearch 'dog': 1\nSearch 'do': 1\nSearch 'ca': 0\nSearch 'dogs': 0\nPrefix 'ca': 3 words\nPrefix 'do': 3 words\nPrefix 'c': 3 words\nPrefix 'z': 0 words\nDone

// Trie implementation with arrays (lowercase letters only)

struct TrieNode {
    int children[26];
    int is_end;
    int count;
};

struct Trie {
    struct TrieNode nodes[64];
    int next_id;
};

void trie_init(struct Trie *t) {
    t->next_id = 1;
    int i = 0;
    while (i < 64) {
        int j = 0;
        while (j < 26) {
            t->nodes[i].children[j] = -1;
            j = j + 1;
        }
        t->nodes[i].is_end = 0;
        t->nodes[i].count = 0;
        i = i + 1;
    }
}

void trie_insert(struct Trie *t, char *word) {
    int node = 0;
    int i = 0;
    while (word[i] != 0) {
        int c = word[i] - 'a';
        if (t->nodes[node].children[c] < 0) {
            t->nodes[node].children[c] = t->next_id;
            t->next_id = t->next_id + 1;
        }
        node = t->nodes[node].children[c];
        t->nodes[node].count = t->nodes[node].count + 1;
        i = i + 1;
    }
    t->nodes[node].is_end = 1;
}

int trie_search(struct Trie *t, char *word) {
    int node = 0;
    int i = 0;
    while (word[i] != 0) {
        int c = word[i] - 'a';
        if (t->nodes[node].children[c] < 0) return 0;
        node = t->nodes[node].children[c];
        i = i + 1;
    }
    return t->nodes[node].is_end;
}

int trie_starts_with(struct Trie *t, char *prefix) {
    int node = 0;
    int i = 0;
    while (prefix[i] != 0) {
        int c = prefix[i] - 'a';
        if (t->nodes[node].children[c] < 0) return 0;
        node = t->nodes[node].children[c];
        i = i + 1;
    }
    return t->nodes[node].count;
}

int trie_count_words_with_prefix(struct Trie *t, char *prefix) {
    int node = 0;
    int i = 0;
    while (prefix[i] != 0) {
        int c = prefix[i] - 'a';
        if (t->nodes[node].children[c] < 0) return 0;
        node = t->nodes[node].children[c];
        i = i + 1;
    }
    return t->nodes[node].count;
}

int main(void) {
    struct Trie trie;
    trie_init(&trie);

    // Insert words
    char w1[4]; w1[0]='c'; w1[1]='a'; w1[2]='t'; w1[3]=0;
    char w2[4]; w2[0]='c'; w2[1]='a'; w2[2]='r'; w2[3]=0;
    char w3[5]; w3[0]='c'; w3[1]='a'; w3[2]='r'; w3[3]='d'; w3[4]=0;
    char w4[4]; w4[0]='d'; w4[1]='o'; w4[2]='g'; w4[3]=0;
    char w5[3]; w5[0]='d'; w5[1]='o'; w5[2]=0;
    char w6[5]; w6[0]='d'; w6[1]='o'; w6[2]='n'; w6[3]='e'; w6[4]=0;

    trie_insert(&trie, w1);
    trie_insert(&trie, w2);
    trie_insert(&trie, w3);
    trie_insert(&trie, w4);
    trie_insert(&trie, w5);
    trie_insert(&trie, w6);

    printf("Nodes used: %d\n", trie.next_id);

    // Search exact
    printf("Search 'cat': %d\n", trie_search(&trie, w1));
    printf("Search 'car': %d\n", trie_search(&trie, w2));
    printf("Search 'card': %d\n", trie_search(&trie, w3));
    printf("Search 'dog': %d\n", trie_search(&trie, w4));
    printf("Search 'do': %d\n", trie_search(&trie, w5));

    // Search non-existent
    char w7[4]; w7[0]='c'; w7[1]='a'; w7[2]=0;
    printf("Search 'ca': %d\n", trie_search(&trie, w7));

    char w8[5]; w8[0]='d'; w8[1]='o'; w8[2]='g'; w8[3]='s'; w8[4]=0;
    printf("Search 'dogs': %d\n", trie_search(&trie, w8));

    // Prefix counts
    char p1[3]; p1[0]='c'; p1[1]='a'; p1[2]=0;
    printf("Prefix 'ca': %d words\n", trie_count_words_with_prefix(&trie, p1));

    char p2[3]; p2[0]='d'; p2[1]='o'; p2[2]=0;
    printf("Prefix 'do': %d words\n", trie_count_words_with_prefix(&trie, p2));

    char p3[2]; p3[0]='c'; p3[1]=0;
    printf("Prefix 'c': %d words\n", trie_count_words_with_prefix(&trie, p3));

    char p4[2]; p4[0]='z'; p4[1]=0;
    printf("Prefix 'z': %d words\n", trie_count_words_with_prefix(&trie, p4));

    printf("Done\n");
    return 0;
}
