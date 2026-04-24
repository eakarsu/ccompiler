int printf(const char *fmt, ...);

// Anagram grouping: group words that are anagrams of each other
// Uses sorted character signature as hash key

int my_strlen(char *s) {
    int n;
    n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

void my_strcpy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int my_strcmp(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return a[i] - b[i];
        i = i + 1;
    }
    return a[i] - b[i];
}

// Sort characters of a string in-place
void sort_chars(char *s) {
    int i;
    int j;
    int n;
    char tmp;
    n = my_strlen(s);
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (s[j] > s[j + 1]) {
                tmp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = tmp;
            }
        }
    }
}

// Get sorted signature of a word
void get_signature(char *word, char *sig) {
    my_strcpy(sig, word);
    sort_chars(sig);
}

// Check if two words are anagrams
int are_anagrams(char *a, char *b) {
    char sa[20];
    char sb[20];
    if (my_strlen(a) != my_strlen(b)) return 0;
    get_signature(a, sa);
    get_signature(b, sb);
    return my_strcmp(sa, sb) == 0;
}

// Group anagrams: assign group IDs
// words: array of word pointers (simulated with 2D char array)
char words[20][16];
int group_id[20];
int num_words;

void assign_groups(void) {
    int i;
    int j;
    int next_group;
    char sig_i[20];
    char sig_j[20];

    next_group = 0;
    for (i = 0; i < num_words; i++) {
        group_id[i] = -1;
    }

    for (i = 0; i < num_words; i++) {
        if (group_id[i] != -1) continue;
        group_id[i] = next_group;
        get_signature(words[i], sig_i);
        for (j = i + 1; j < num_words; j++) {
            if (group_id[j] != -1) continue;
            get_signature(words[j], sig_j);
            if (my_strcmp(sig_i, sig_j) == 0) {
                group_id[j] = next_group;
            }
        }
        next_group = next_group + 1;
    }
}

int count_groups(void) {
    int i;
    int mx;
    mx = -1;
    for (i = 0; i < num_words; i++) {
        if (group_id[i] > mx) mx = group_id[i];
    }
    return mx + 1;
}

int group_size(int gid) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < num_words; i++) {
        if (group_id[i] == gid) c = c + 1;
    }
    return c;
}

int largest_group(void) {
    int i;
    int ng;
    int mx;
    int gs;
    ng = count_groups();
    mx = 0;
    for (i = 0; i < ng; i++) {
        gs = group_size(i);
        if (gs > mx) mx = gs;
    }
    return mx;
}

// Count character frequencies for a word
void char_freq(char *word, int *freq) {
    int i;
    for (i = 0; i < 26; i++) freq[i] = 0;
    i = 0;
    while (word[i] != 0) {
        freq[word[i] - 'a'] = freq[word[i] - 'a'] + 1;
        i = i + 1;
    }
}

int main(void) {
    char sig[20];
    int freq[26];
    int i;

    // Test basic anagram detection
    // EXPECT: eat-tea anagram=1
    printf("eat-tea anagram=%d\n", are_anagrams("eat", "tea"));
    // EXPECT: eat-ate anagram=1
    printf("eat-ate anagram=%d\n", are_anagrams("eat", "ate"));
    // EXPECT: eat-cat anagram=0
    printf("eat-cat anagram=%d\n", are_anagrams("eat", "cat"));
    // EXPECT: listen-silent anagram=1
    printf("listen-silent anagram=%d\n", are_anagrams("listen", "silent"));
    // EXPECT: hello-world anagram=0
    printf("hello-world anagram=%d\n", are_anagrams("hello", "world"));

    // Test signature
    get_signature("eat", sig);
    // EXPECT: sig(eat)=aet
    printf("sig(eat)=%s\n", sig);
    get_signature("tea", sig);
    // EXPECT: sig(tea)=aet
    printf("sig(tea)=%s\n", sig);
    get_signature("bat", sig);
    // EXPECT: sig(bat)=abt
    printf("sig(bat)=%s\n", sig);

    // Group anagrams
    my_strcpy(words[0], "eat");
    my_strcpy(words[1], "tea");
    my_strcpy(words[2], "tan");
    my_strcpy(words[3], "ate");
    my_strcpy(words[4], "nat");
    my_strcpy(words[5], "bat");
    num_words = 6;

    assign_groups();

    // EXPECT: num groups=3
    printf("num groups=%d\n", count_groups());

    // eat, tea, ate should be in same group (group 0)
    // EXPECT: eat group=0
    printf("eat group=%d\n", group_id[0]);
    // EXPECT: tea group=0
    printf("tea group=%d\n", group_id[1]);
    // EXPECT: ate group=0
    printf("ate group=%d\n", group_id[3]);

    // tan, nat should be in same group (group 1)
    // EXPECT: tan group=1
    printf("tan group=%d\n", group_id[2]);
    // EXPECT: nat group=1
    printf("nat group=%d\n", group_id[4]);

    // bat is alone (group 2)
    // EXPECT: bat group=2
    printf("bat group=%d\n", group_id[5]);

    // EXPECT: group 0 size=3
    printf("group 0 size=%d\n", group_size(0));
    // EXPECT: group 1 size=2
    printf("group 1 size=%d\n", group_size(1));
    // EXPECT: group 2 size=1
    printf("group 2 size=%d\n", group_size(2));
    // EXPECT: largest group=3
    printf("largest group=%d\n", largest_group());

    // Character frequency test
    char_freq("banana", freq);
    // EXPECT: b in banana=1
    printf("b in banana=%d\n", freq['b' - 'a']);
    // EXPECT: a in banana=3
    printf("a in banana=%d\n", freq['a' - 'a']);
    // EXPECT: n in banana=2
    printf("n in banana=%d\n", freq['n' - 'a']);

    // Test with more words
    my_strcpy(words[0], "abc");
    my_strcpy(words[1], "bca");
    my_strcpy(words[2], "cab");
    my_strcpy(words[3], "xyz");
    my_strcpy(words[4], "zyx");
    my_strcpy(words[5], "def");
    my_strcpy(words[6], "fed");
    my_strcpy(words[7], "ghi");
    num_words = 8;
    assign_groups();
    // EXPECT: set2 groups=4
    printf("set2 groups=%d\n", count_groups());
    // EXPECT: set2 largest=3
    printf("set2 largest=%d\n", largest_group());

    // EXPECT: done
    printf("done\n");

    return 0;
}
