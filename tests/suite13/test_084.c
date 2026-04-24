int printf(const char *fmt, ...);

// Edit Distance (Levenshtein Distance)

int str_len(char *s) {
    int len;
    len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int min2(int a, int b) {
    if (a < b) return a;
    return b;
}

int min3(int a, int b, int c) {
    return min2(min2(a, b), c);
}

// Standard edit distance: insert, delete, replace each cost 1
int edit_distance(char *s1, char *s2) {
    int len1;
    int len2;
    int dp[64][64];
    int i;
    int j;
    int cost;

    len1 = str_len(s1);
    len2 = str_len(s2);

    for (i = 0; i <= len1; i++) {
        dp[i][0] = i;
    }
    for (j = 0; j <= len2; j++) {
        dp[0][j] = j;
    }

    for (i = 1; i <= len1; i++) {
        for (j = 1; j <= len2; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                cost = 0;
            } else {
                cost = 1;
            }
            dp[i][j] = min3(
                dp[i - 1][j] + 1,       // delete
                dp[i][j - 1] + 1,       // insert
                dp[i - 1][j - 1] + cost // replace
            );
        }
    }

    return dp[len1][len2];
}

// Weighted edit distance: custom costs for insert, delete, replace
int weighted_edit(char *s1, char *s2, int ins, int del, int rep) {
    int len1;
    int len2;
    int dp[64][64];
    int i;
    int j;

    len1 = str_len(s1);
    len2 = str_len(s2);

    for (i = 0; i <= len1; i++) {
        dp[i][0] = i * del;
    }
    for (j = 0; j <= len2; j++) {
        dp[0][j] = j * ins;
    }

    for (i = 1; i <= len1; i++) {
        for (j = 1; j <= len2; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = min3(
                    dp[i][j - 1] + ins,
                    dp[i - 1][j] + del,
                    dp[i - 1][j - 1] + rep
                );
            }
        }
    }

    return dp[len1][len2];
}

// Check if one string is one edit away from another
int is_one_edit(char *s1, char *s2) {
    return edit_distance(s1, s2) <= 1;
}

// Longest common substring length using DP
int longest_common_substr(char *s1, char *s2) {
    int len1;
    int len2;
    int dp[64][64];
    int i;
    int j;
    int max_len;

    len1 = str_len(s1);
    len2 = str_len(s2);
    max_len = 0;

    for (i = 0; i <= len1; i++) {
        for (j = 0; j <= len2; j++) {
            dp[i][j] = 0;
        }
    }

    for (i = 1; i <= len1; i++) {
        for (j = 1; j <= len2; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > max_len) {
                    max_len = dp[i][j];
                }
            }
        }
    }

    return max_len;
}

int main() {
    char s1[32];
    char s2[32];

    // "kitten" -> "sitting": 3 edits
    // k->s, e->i, +g
    s1[0]='k'; s1[1]='i'; s1[2]='t'; s1[3]='t'; s1[4]='e'; s1[5]='n'; s1[6]=0;
    s2[0]='s'; s2[1]='i'; s2[2]='t'; s2[3]='t'; s2[4]='i'; s2[5]='n'; s2[6]='g'; s2[7]=0;
    printf("%d\n", edit_distance(s1, s2));
    // EXPECT: 3

    // Same strings: distance 0
    printf("%d\n", edit_distance(s1, s1));
    // EXPECT: 0

    // Empty to "abc": 3 insertions
    s2[0] = 'a'; s2[1] = 'b'; s2[2] = 'c'; s2[3] = 0;
    s1[0] = 0;
    printf("%d\n", edit_distance(s1, s2));
    // EXPECT: 3

    // "abc" to empty: 3 deletions
    s1[0] = 'a'; s1[1] = 'b'; s1[2] = 'c'; s1[3] = 0;
    s2[0] = 0;
    printf("%d\n", edit_distance(s1, s2));
    // EXPECT: 3

    // "horse" -> "ros": 3
    s1[0]='h'; s1[1]='o'; s1[2]='r'; s1[3]='s'; s1[4]='e'; s1[5]=0;
    s2[0]='r'; s2[1]='o'; s2[2]='s'; s2[3]=0;
    printf("%d\n", edit_distance(s1, s2));
    // EXPECT: 3

    // "intention" -> "execution": 5
    s1[0]='i'; s1[1]='n'; s1[2]='t'; s1[3]='e'; s1[4]='n';
    s1[5]='t'; s1[6]='i'; s1[7]='o'; s1[8]='n'; s1[9]=0;
    s2[0]='e'; s2[1]='x'; s2[2]='e'; s2[3]='c'; s2[4]='u';
    s2[5]='t'; s2[6]='i'; s2[7]='o'; s2[8]='n'; s2[9]=0;
    printf("%d\n", edit_distance(s1, s2));
    // EXPECT: 5

    // "abc" -> "abc": 0
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]=0;
    s2[0]='a'; s2[1]='b'; s2[2]='c'; s2[3]=0;
    printf("%d\n", edit_distance(s1, s2));
    // EXPECT: 0

    // Is one edit: "cat" and "bat" => yes (replace)
    s1[0]='c'; s1[1]='a'; s1[2]='t'; s1[3]=0;
    s2[0]='b'; s2[1]='a'; s2[2]='t'; s2[3]=0;
    printf("%d\n", is_one_edit(s1, s2));
    // EXPECT: 1

    // Is one edit: "cat" and "cats" => yes (insert)
    s2[0]='c'; s2[1]='a'; s2[2]='t'; s2[3]='s'; s2[4]=0;
    printf("%d\n", is_one_edit(s1, s2));
    // EXPECT: 1

    // Is one edit: "cat" and "dog" => no (3 edits)
    s2[0]='d'; s2[1]='o'; s2[2]='g'; s2[3]=0;
    printf("%d\n", is_one_edit(s1, s2));
    // EXPECT: 0

    // Weighted edit: insert=2, delete=3, replace=5
    // "ab" -> "b": delete 'a', cost=3
    s1[0]='a'; s1[1]='b'; s1[2]=0;
    s2[0]='b'; s2[1]=0;
    printf("%d\n", weighted_edit(s1, s2, 2, 3, 5));
    // EXPECT: 3

    // Longest common substring: "abcde" and "abfce" => "ab" length 2
    s1[0]='a'; s1[1]='b'; s1[2]='c'; s1[3]='d'; s1[4]='e'; s1[5]=0;
    s2[0]='a'; s2[1]='b'; s2[2]='f'; s2[3]='c'; s2[4]='e'; s2[5]=0;
    printf("%d\n", longest_common_substr(s1, s2));
    // EXPECT: 2

    // Longest common substring: "xyzabc" and "wqabc" => "abc" length 3
    s1[0]='x'; s1[1]='y'; s1[2]='z'; s1[3]='a'; s1[4]='b'; s1[5]='c'; s1[6]=0;
    s2[0]='w'; s2[1]='q'; s2[2]='a'; s2[3]='b'; s2[4]='c'; s2[5]=0;
    printf("%d\n", longest_common_substr(s1, s2));
    // EXPECT: 3

    printf("done\n");
    // EXPECT: done

    return 0;
}
