int printf(const char *fmt, ...);
// EXPECT: Search apple: 1\nSearch app: 1\nSearch ap: 0\nSearch banana: 1\nSearch cat: 0\nPrefix app: 3\nPrefix ba: 3\nPrefix z: 0\nTotal words: 6\nNodes used: 21
// Trie (prefix tree) with arrays

struct TrieNode {
    int children[26];
    int is_end;
    int count;
};

struct Trie {
    struct TrieNode nodes[100];
    int next_free;
};

void trie_init(struct Trie *t) {
    int i;
    int j;
    for (i = 0; i < 100; i++) {
        for (j = 0; j < 26; j++) {
            t->nodes[i].children[j] = -1;
        }
        t->nodes[i].is_end = 0;
        t->nodes[i].count = 0;
    }
    t->next_free = 1;
}

int trie_new_node(struct Trie *t) {
    int idx;
    int j;
    if (t->next_free >= 100) return -1;
    idx = t->next_free;
    t->next_free++;
    for (j = 0; j < 26; j++) {
        t->nodes[idx].children[j] = -1;
    }
    t->nodes[idx].is_end = 0;
    t->nodes[idx].count = 0;
    return idx;
}

void trie_insert(struct Trie *t, char *word) {
    int node = 0;
    int i = 0;
    int c;
    int child;
    while (word[i] != 0) {
        c = word[i] - 'a';
        if (t->nodes[node].children[c] == -1) {
            child = trie_new_node(t);
            t->nodes[node].children[c] = child;
        }
        node = t->nodes[node].children[c];
        t->nodes[node].count++;
        i++;
    }
    t->nodes[node].is_end = 1;
}

int trie_search(struct Trie *t, char *word) {
    int node = 0;
    int i = 0;
    int c;
    while (word[i] != 0) {
        c = word[i] - 'a';
        if (t->nodes[node].children[c] == -1) return 0;
        node = t->nodes[node].children[c];
        i++;
    }
    return t->nodes[node].is_end;
}

int trie_starts_with(struct Trie *t, char *prefix) {
    int node = 0;
    int i = 0;
    int c;
    while (prefix[i] != 0) {
        c = prefix[i] - 'a';
        if (t->nodes[node].children[c] == -1) return 0;
        node = t->nodes[node].children[c];
        i++;
    }
    return t->nodes[node].count;
}

int trie_count_words(struct Trie *t, int node) {
    int total = 0;
    int i;
    if (t->nodes[node].is_end) total = 1;
    for (i = 0; i < 26; i++) {
        if (t->nodes[node].children[i] != -1) {
            total = total + trie_count_words(t, t->nodes[node].children[i]);
        }
    }
    return total;
}

int main(void) {
    struct Trie t;
    trie_init(&t);

    trie_insert(&t, "apple");
    trie_insert(&t, "app");
    trie_insert(&t, "application");
    trie_insert(&t, "banana");
    trie_insert(&t, "band");
    trie_insert(&t, "bat");

    printf("Search apple: %d\n", trie_search(&t, "apple"));
    printf("Search app: %d\n", trie_search(&t, "app"));
    printf("Search ap: %d\n", trie_search(&t, "ap"));
    printf("Search banana: %d\n", trie_search(&t, "banana"));
    printf("Search cat: %d\n", trie_search(&t, "cat"));

    printf("Prefix app: %d\n", trie_starts_with(&t, "app"));
    printf("Prefix ba: %d\n", trie_starts_with(&t, "ba"));
    printf("Prefix z: %d\n", trie_starts_with(&t, "z"));

    printf("Total words: %d\n", trie_count_words(&t, 0));
    printf("Nodes used: %d\n", t.next_free);

    return 0;
}
