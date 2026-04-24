int printf(const char *fmt, ...);
// EXPECT: frequencies:\n  a: 1\n  b: 1\n  c: 1\n  d: 1\n  e: 3\n  f: 1\n  g: 1\n  h: 2\n  i: 1\n  j: 1\n  k: 1\n  l: 1\n  m: 1\n  n: 1\n  o: 4\n  p: 1\n  q: 1\n  r: 2\n  s: 1\n  t: 2\n  u: 2\n  v: 1\n  w: 1\n  x: 1\n  y: 1\n  z: 1\npangram: 1\nmost frequent: o (4)\ndigits in 'abc123def456ghi789': 1=1 2=1 3=1 4=1 5=1 6=1 7=1 8=1 9=1 \nunique (freq=1) letters: 20\ntotal letters: 35
// Test: character frequency counting

int my_strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

int main(void) {
    // Count lowercase letter frequencies
    char *text = "the quick brown fox jumps over the lazy dog";
    int freq[26];
    int i;
    for (i = 0; i < 26; i++) freq[i] = 0;

    i = 0;
    while (text[i] != '\0') {
        if (text[i] >= 'a' && text[i] <= 'z') {
            freq[text[i] - 'a'] = freq[text[i] - 'a'] + 1;
        }
        i = i + 1;
    }

    // Print non-zero frequencies
    printf("frequencies:\n");
    for (i = 0; i < 26; i++) {
        if (freq[i] > 0) {
            printf("  %c: %d\n", 'a' + i, freq[i]);
        }
    }

    // Check if pangram (all 26 letters present)
    int pangram = 1;
    for (i = 0; i < 26; i++) {
        if (freq[i] == 0) {
            pangram = 0;
            break;
        }
    }
    printf("pangram: %d\n", pangram);

    // Most frequent
    int max_f = 0;
    char max_c = 'a';
    for (i = 0; i < 26; i++) {
        if (freq[i] > max_f) {
            max_f = freq[i];
            max_c = 'a' + i;
        }
    }
    printf("most frequent: %c (%d)\n", max_c, max_f);

    // Count digits in a string
    char *mixed = "abc123def456ghi789";
    int digit_freq[10];
    for (i = 0; i < 10; i++) digit_freq[i] = 0;
    i = 0;
    while (mixed[i] != '\0') {
        if (mixed[i] >= '0' && mixed[i] <= '9') {
            digit_freq[mixed[i] - '0'] = digit_freq[mixed[i] - '0'] + 1;
        }
        i = i + 1;
    }
    printf("digits in '%s': ", mixed);
    for (i = 0; i < 10; i++) {
        if (digit_freq[i] > 0) {
            printf("%d=%d ", i, digit_freq[i]);
        }
    }
    printf("\n");

    // Count unique characters
    int unique = 0;
    for (i = 0; i < 26; i++) {
        if (freq[i] == 1) unique = unique + 1;
    }
    printf("unique (freq=1) letters: %d\n", unique);

    // Total letters
    int total = 0;
    for (i = 0; i < 26; i++) total = total + freq[i];
    printf("total letters: %d\n", total);

    return 0;
}
