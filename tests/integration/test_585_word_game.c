int printf(const char *fmt, ...);
// EXPECT: Word Analysis:\nWord: hello      len=5 score= 8 vowels=2 consonants=3 palindrome=0 double=1\nWord: world      len=5 score= 9 vowels=1 consonants=4 palindrome=0 double=0\nWord: racecar    len=7 score=11 vowels=3 consonants=4 palindrome=1 double=0\nWord: pizza      len=5 score=25 vowels=2 consonants=3 palindrome=0 double=1\nWord: quiz       len=4 score=22 vowels=2 consonants=2 palindrome=0 double=0\nWord: jazz       len=4 score=29 vowels=1 consonants=3 palindrome=0 double=1\nWord: level      len=5 score= 8 vowels=2 consonants=3 palindrome=1 double=0\nWord: apple      len=5 score= 9 vowels=2 consonants=3 palindrome=0 double=1\nWord: kayak      len=5 score=16 vowels=2 consonants=3 palindrome=1 double=0\nWord: banana     len=6 score= 8 vowels=3 consonants=3 palindrome=0 double=0\nBest word: jazz (29 points)\nTotal score: 145\nPalindromes found: 3

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

int letter_score(char c) {
    c = to_lower(c);
    int scores[26];
    scores[0] = 1; scores[1] = 3; scores[2] = 3; scores[3] = 2;
    scores[4] = 1; scores[5] = 4; scores[6] = 2; scores[7] = 4;
    scores[8] = 1; scores[9] = 8; scores[10] = 5; scores[11] = 1;
    scores[12] = 3; scores[13] = 1; scores[14] = 1; scores[15] = 3;
    scores[16] = 10; scores[17] = 1; scores[18] = 1; scores[19] = 1;
    scores[20] = 1; scores[21] = 4; scores[22] = 4; scores[23] = 8;
    scores[24] = 4; scores[25] = 10;
    if (c >= 'a' && c <= 'z') return scores[c - 'a'];
    return 0;
}

int word_score(char *word) {
    int score = 0;
    int i;
    for (i = 0; word[i] != 0; i++) {
        score = score + letter_score(word[i]);
    }
    return score;
}

int is_palindrome(char *word) {
    int len = str_len(word);
    int i;
    for (i = 0; i < len / 2; i++) {
        if (to_lower(word[i]) != to_lower(word[len - 1 - i])) return 0;
    }
    return 1;
}

int count_vowels(char *word) {
    int count = 0;
    int i;
    for (i = 0; word[i] != 0; i++) {
        char c = to_lower(word[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') count++;
    }
    return count;
}

int count_consonants(char *word) {
    int count = 0;
    int i;
    for (i = 0; word[i] != 0; i++) {
        char c = to_lower(word[i]);
        if (is_alpha(c) && c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            count++;
        }
    }
    return count;
}

int has_double_letter(char *word) {
    int i;
    for (i = 0; word[i] != 0 && word[i + 1] != 0; i++) {
        if (to_lower(word[i]) == to_lower(word[i + 1])) return 1;
    }
    return 0;
}

void analyze_word(char *word) {
    printf("Word: %-10s len=%d score=%2d vowels=%d consonants=%d palindrome=%d double=%d\n",
           word, str_len(word), word_score(word),
           count_vowels(word), count_consonants(word),
           is_palindrome(word), has_double_letter(word));
}

int main(void) {
    char *words[10];
    words[0] = "hello";
    words[1] = "world";
    words[2] = "racecar";
    words[3] = "pizza";
    words[4] = "quiz";
    words[5] = "jazz";
    words[6] = "level";
    words[7] = "apple";
    words[8] = "kayak";
    words[9] = "banana";

    printf("Word Analysis:\n");
    int i;
    for (i = 0; i < 10; i++) {
        analyze_word(words[i]);
    }

    int best = 0;
    int best_score = 0;
    int total_score = 0;
    for (i = 0; i < 10; i++) {
        int s = word_score(words[i]);
        total_score = total_score + s;
        if (s > best_score) {
            best_score = s;
            best = i;
        }
    }
    printf("Best word: %s (%d points)\n", words[best], best_score);
    printf("Total score: %d\n", total_score);

    int palindromes = 0;
    for (i = 0; i < 10; i++) {
        if (is_palindrome(words[i])) palindromes++;
    }
    printf("Palindromes found: %d\n", palindromes);

    return 0;
}
