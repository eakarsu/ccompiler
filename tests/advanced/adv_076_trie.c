// EXPECT: === Trie ===
// EXPECT: Inserted: apple app application bat ball
// EXPECT: Search apple: found
// EXPECT: Search app: found
// EXPECT: Search application: found
// EXPECT: Search bat: found
// EXPECT: Search ball: found
// EXPECT: Search ap: not found
// EXPECT: Search bats: not found
// EXPECT: Search cat: not found
// EXPECT: Prefix ap: yes
// EXPECT: Prefix app: yes
// EXPECT: Prefix ba: yes
// EXPECT: Prefix cat: no
// EXPECT: Prefix z: no
// EXPECT: Word count: 5
// EXPECT: Delete app: ok
// EXPECT: Search app: not found
// EXPECT: Search apple: found
// EXPECT: Search application: found
// EXPECT: Word count after delete: 4
// EXPECT: Delete bat: ok
// EXPECT: Search bat: not found
// EXPECT: Search ball: found
// EXPECT: Word count after delete: 3
// EXPECT: Insert and search single char words
// EXPECT: Search a: found
// EXPECT: Search b: found
// EXPECT: Search z: found
// EXPECT: All trie tests passed

int printf(const char *fmt, ...);
void *malloc(unsigned long size);
void free(void *ptr);

typedef struct TrieNode {
    struct TrieNode *children[26];
    int is_end;
} TrieNode;

TrieNode *trie_new_node(void) {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    node->is_end = 0;
    int i;
    for (i = 0; i < 26; i++) {
        node->children[i] = (TrieNode *)0;
    }
    return node;
}

void trie_insert(TrieNode *root, const char *word) {
    TrieNode *cur = root;
    int i = 0;
    while (word[i] != '\0') {
        int idx = word[i] - 'a';
        if (!cur->children[idx]) {
            cur->children[idx] = trie_new_node();
        }
        cur = cur->children[idx];
        i++;
    }
    cur->is_end = 1;
}

int trie_search(TrieNode *root, const char *word) {
    TrieNode *cur = root;
    int i = 0;
    while (word[i] != '\0') {
        int idx = word[i] - 'a';
        if (!cur->children[idx]) return 0;
        cur = cur->children[idx];
        i++;
    }
    return cur->is_end;
}

int trie_starts_with(TrieNode *root, const char *prefix) {
    TrieNode *cur = root;
    int i = 0;
    while (prefix[i] != '\0') {
        int idx = prefix[i] - 'a';
        if (!cur->children[idx]) return 0;
        cur = cur->children[idx];
        i++;
    }
    return 1;
}

int trie_count_words_helper(TrieNode *node) {
    if (!node) return 0;
    int count = 0;
    if (node->is_end) count = 1;
    int i;
    for (i = 0; i < 26; i++) {
        count = count + trie_count_words_helper(node->children[i]);
    }
    return count;
}

int trie_count_words(TrieNode *root) {
    return trie_count_words_helper(root);
}

int trie_has_children(TrieNode *node) {
    int i;
    for (i = 0; i < 26; i++) {
        if (node->children[i]) return 1;
    }
    return 0;
}

int trie_delete_helper(TrieNode *node, const char *word, int depth) {
    if (!node) return 0;

    if (word[depth] == '\0') {
        if (!node->is_end) return 0;
        node->is_end = 0;
        return !trie_has_children(node);
    }

    int idx = word[depth] - 'a';
    if (!node->children[idx]) return 0;

    int should_delete = trie_delete_helper(node->children[idx], word, depth + 1);

    if (should_delete) {
        free(node->children[idx]);
        node->children[idx] = (TrieNode *)0;
        return !node->is_end && !trie_has_children(node);
    }
    return 0;
}

void trie_delete(TrieNode *root, const char *word) {
    trie_delete_helper(root, word, 0);
}

void trie_free(TrieNode *node) {
    if (!node) return;
    int i;
    for (i = 0; i < 26; i++) {
        trie_free(node->children[i]);
    }
    free(node);
}

void test_insert_search(void) {
    TrieNode *root = trie_new_node();

    trie_insert(root, "apple");
    trie_insert(root, "app");
    trie_insert(root, "application");
    trie_insert(root, "bat");
    trie_insert(root, "ball");
    printf("Inserted: apple app application bat ball\n");

    printf("Search apple: %s\n", trie_search(root, "apple") ? "found" : "not found");
    printf("Search app: %s\n", trie_search(root, "app") ? "found" : "not found");
    printf("Search application: %s\n", trie_search(root, "application") ? "found" : "not found");
    printf("Search bat: %s\n", trie_search(root, "bat") ? "found" : "not found");
    printf("Search ball: %s\n", trie_search(root, "ball") ? "found" : "not found");
    printf("Search ap: %s\n", trie_search(root, "ap") ? "found" : "not found");
    printf("Search bats: %s\n", trie_search(root, "bats") ? "found" : "not found");
    printf("Search cat: %s\n", trie_search(root, "cat") ? "found" : "not found");

    printf("Prefix ap: %s\n", trie_starts_with(root, "ap") ? "yes" : "no");
    printf("Prefix app: %s\n", trie_starts_with(root, "app") ? "yes" : "no");
    printf("Prefix ba: %s\n", trie_starts_with(root, "ba") ? "yes" : "no");
    printf("Prefix cat: %s\n", trie_starts_with(root, "cat") ? "yes" : "no");
    printf("Prefix z: %s\n", trie_starts_with(root, "z") ? "yes" : "no");

    printf("Word count: %d\n", trie_count_words(root));

    trie_delete(root, "app");
    printf("Delete app: ok\n");
    printf("Search app: %s\n", trie_search(root, "app") ? "found" : "not found");
    printf("Search apple: %s\n", trie_search(root, "apple") ? "found" : "not found");
    printf("Search application: %s\n", trie_search(root, "application") ? "found" : "not found");
    printf("Word count after delete: %d\n", trie_count_words(root));

    trie_delete(root, "bat");
    printf("Delete bat: ok\n");
    printf("Search bat: %s\n", trie_search(root, "bat") ? "found" : "not found");
    printf("Search ball: %s\n", trie_search(root, "ball") ? "found" : "not found");
    printf("Word count after delete: %d\n", trie_count_words(root));

    trie_free(root);
}

void test_single_char(void) {
    TrieNode *root = trie_new_node();

    trie_insert(root, "a");
    trie_insert(root, "b");
    trie_insert(root, "z");

    printf("Insert and search single char words\n");
    printf("Search a: %s\n", trie_search(root, "a") ? "found" : "not found");
    printf("Search b: %s\n", trie_search(root, "b") ? "found" : "not found");
    printf("Search z: %s\n", trie_search(root, "z") ? "found" : "not found");

    trie_free(root);
}

int main(void) {
    printf("=== Trie ===\n");
    test_insert_search();
    test_single_char();
    printf("All trie tests passed\n");
    return 0;
}
