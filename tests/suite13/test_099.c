int printf(const char *fmt, ...);

// Trie (prefix tree) using 2D array
// Each node has 26 children (a-z) stored as indices
// children[node][c] = index of child node for character c, 0 = none

struct Trie {
    int children[128][26]; // children[node_idx][char_idx]
    int is_end[128];       // 1 if this node marks end of a word
    int count;             // next free node (0 = root)
};

void trie_init(struct Trie *t) {
    int i;
    int j;
    for (i = 0; i < 128; i++) {
        for (j = 0; j < 26; j++) {
            t->children[i][j] = 0;
        }
        t->is_end[i] = 0;
    }
    // Node 0 is unused sentinel, node 1 is root
    t->count = 2;
}

int trie_root(struct Trie *t) {
    return 1;
}

void trie_insert(struct Trie *t, char *word) {
    int cur = 1; // root
    int i = 0;
    int c;
    int new_node;

    while (word[i] != 0) {
        c = word[i] - 'a';
        if (t->children[cur][c] == 0) {
            new_node = t->count;
            t->count = t->count + 1;
            t->children[cur][c] = new_node;
        }
        cur = t->children[cur][c];
        i = i + 1;
    }
    t->is_end[cur] = 1;
}

int trie_search(struct Trie *t, char *word) {
    int cur = 1;
    int i = 0;
    int c;

    while (word[i] != 0) {
        c = word[i] - 'a';
        if (t->children[cur][c] == 0) {
            return 0;
        }
        cur = t->children[cur][c];
        i = i + 1;
    }
    return t->is_end[cur];
}

int trie_starts_with(struct Trie *t, char *prefix) {
    int cur = 1;
    int i = 0;
    int c;

    while (prefix[i] != 0) {
        c = prefix[i] - 'a';
        if (t->children[cur][c] == 0) {
            return 0;
        }
        cur = t->children[cur][c];
        i = i + 1;
    }
    return 1;
}

// Count total words stored in trie (DFS)
int trie_count_words(struct Trie *t, int node) {
    int total = 0;
    int i;
    if (t->is_end[node]) total = 1;
    for (i = 0; i < 26; i++) {
        if (t->children[node][i] != 0) {
            total = total + trie_count_words(t, t->children[node][i]);
        }
    }
    return total;
}

// Count total nodes in trie (DFS)
int trie_count_nodes(struct Trie *t, int node) {
    int total = 1;
    int i;
    for (i = 0; i < 26; i++) {
        if (t->children[node][i] != 0) {
            total = total + trie_count_nodes(t, t->children[node][i]);
        }
    }
    return total;
}

// Count words with given prefix
int trie_count_with_prefix(struct Trie *t, char *prefix) {
    int cur = 1;
    int i = 0;
    int c;

    while (prefix[i] != 0) {
        c = prefix[i] - 'a';
        if (t->children[cur][c] == 0) {
            return 0;
        }
        cur = t->children[cur][c];
        i = i + 1;
    }
    return trie_count_words(t, cur);
}

// Find longest common prefix length among all words
// by traversing while there is exactly one child and not end of word
int trie_longest_common_prefix(struct Trie *t) {
    int cur = 1;
    int len = 0;
    int child_count;
    int next;
    int i;

    while (1) {
        if (t->is_end[cur]) return len;
        child_count = 0;
        next = 0;
        for (i = 0; i < 26; i++) {
            if (t->children[cur][i] != 0) {
                child_count = child_count + 1;
                next = t->children[cur][i];
            }
        }
        if (child_count != 1) return len;
        cur = next;
        len = len + 1;
    }
}

int main() {
    struct Trie t;
    trie_init(&t);

    // Insert words
    trie_insert(&t, "apple");
    trie_insert(&t, "app");
    trie_insert(&t, "apricot");
    trie_insert(&t, "banana");
    trie_insert(&t, "band");
    trie_insert(&t, "bandana");
    trie_insert(&t, "cat");
    trie_insert(&t, "car");
    trie_insert(&t, "card");

    // Search tests
    printf("Search apple: %d\n", trie_search(&t, "apple"));
    // EXPECT: Search apple: 1
    printf("Search app: %d\n", trie_search(&t, "app"));
    // EXPECT: Search app: 1
    printf("Search ap: %d\n", trie_search(&t, "ap"));
    // EXPECT: Search ap: 0
    printf("Search banana: %d\n", trie_search(&t, "banana"));
    // EXPECT: Search banana: 1
    printf("Search ban: %d\n", trie_search(&t, "ban"));
    // EXPECT: Search ban: 0
    printf("Search cat: %d\n", trie_search(&t, "cat"));
    // EXPECT: Search cat: 1
    printf("Search dog: %d\n", trie_search(&t, "dog"));
    // EXPECT: Search dog: 0

    // Prefix tests
    printf("Prefix ap: %d\n", trie_starts_with(&t, "ap"));
    // EXPECT: Prefix ap: 1
    printf("Prefix ba: %d\n", trie_starts_with(&t, "ba"));
    // EXPECT: Prefix ba: 1
    printf("Prefix ca: %d\n", trie_starts_with(&t, "ca"));
    // EXPECT: Prefix ca: 1
    printf("Prefix do: %d\n", trie_starts_with(&t, "do"));
    // EXPECT: Prefix do: 0

    // Word count
    printf("Total words: %d\n", trie_count_words(&t, 1));
    // EXPECT: Total words: 9

    // Count with prefix
    printf("Words with ap: %d\n", trie_count_with_prefix(&t, "ap"));
    // EXPECT: Words with ap: 3
    printf("Words with ban: %d\n", trie_count_with_prefix(&t, "ban"));
    // EXPECT: Words with ban: 3
    printf("Words with car: %d\n", trie_count_with_prefix(&t, "car"));
    // EXPECT: Words with car: 2
    printf("Words with ca: %d\n", trie_count_with_prefix(&t, "ca"));
    // EXPECT: Words with ca: 3

    // Node count
    int nodes = trie_count_nodes(&t, 1);
    printf("Total nodes: %d\n", nodes);
    // EXPECT: Total nodes: 26

    // Longest common prefix (none here, since first branch splits at 'a','b','c')
    printf("LCP length: %d\n", trie_longest_common_prefix(&t));
    // EXPECT: LCP length: 0

    // Test with words sharing common prefix
    struct Trie t2;
    trie_init(&t2);
    trie_insert(&t2, "test");
    trie_insert(&t2, "testing");
    trie_insert(&t2, "tested");
    trie_insert(&t2, "tester");

    printf("LCP2 length: %d\n", trie_longest_common_prefix(&t2));
    // EXPECT: LCP2 length: 4

    printf("Words2 total: %d\n", trie_count_words(&t2, 1));
    // EXPECT: Words2 total: 4

    printf("Search testing: %d\n", trie_search(&t2, "testing"));
    // EXPECT: Search testing: 1
    printf("Search teste: %d\n", trie_search(&t2, "teste"));
    // EXPECT: Search teste: 0
    printf("Words with teste: %d\n", trie_count_with_prefix(&t2, "teste"));
    // EXPECT: Words with teste: 2

    return 0;
}
