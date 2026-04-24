int printf(const char *fmt, ...);
// EXPECT: strcmp(hello,hello) = 0\nstrcmp(abc,abd) = -1\nstrcmp(abd,abc) = 1\nstrcmp(ab,abc) = -1\nstrcmp(abc,ab) = 1\nstrncmp(abc,abd,2) = 0\nstrncmp(abc,abd,3) = -1\nsorted: ant bat cat dog 
// Test: string comparison (manual strcmp)

int my_strcmp(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i = i + 1;
    }
    if (a[i] == '\0' && b[i] == '\0') return 0;
    if (a[i] == '\0') return -1;
    return 1;
}

int my_strncmp(char *a, char *b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] == '\0' && b[i] == '\0') return 0;
        if (a[i] == '\0') return -1;
        if (b[i] == '\0') return 1;
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    return 0;
}

int main(void) {
    // Equal strings
    char s1[6]; s1[0]='h'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]='\0';
    char s2[6]; s2[0]='h'; s2[1]='e'; s2[2]='l'; s2[3]='l'; s2[4]='o'; s2[5]='\0';
    printf("strcmp(hello,hello) = %d\n", my_strcmp(s1, s2));

    // Different strings
    char s3[4]; s3[0]='a'; s3[1]='b'; s3[2]='c'; s3[3]='\0';
    char s4[4]; s4[0]='a'; s4[1]='b'; s4[2]='d'; s4[3]='\0';
    printf("strcmp(abc,abd) = %d\n", my_strcmp(s3, s4));
    printf("strcmp(abd,abc) = %d\n", my_strcmp(s4, s3));

    // Prefix
    char s5[3]; s5[0]='a'; s5[1]='b'; s5[2]='\0';
    printf("strcmp(ab,abc) = %d\n", my_strcmp(s5, s3));
    printf("strcmp(abc,ab) = %d\n", my_strcmp(s3, s5));

    // strncmp
    printf("strncmp(abc,abd,2) = %d\n", my_strncmp(s3, s4, 2));
    printf("strncmp(abc,abd,3) = %d\n", my_strncmp(s3, s4, 3));

    // Sort array of strings using strcmp
    char w0[4]; w0[0]='d'; w0[1]='o'; w0[2]='g'; w0[3]='\0';
    char w1[4]; w1[0]='c'; w1[1]='a'; w1[2]='t'; w1[3]='\0';
    char w2[4]; w2[0]='a'; w2[1]='n'; w2[2]='t'; w2[3]='\0';
    char w3[4]; w3[0]='b'; w3[1]='a'; w3[2]='t'; w3[3]='\0';

    char *words[4];
    words[0] = w0; words[1] = w1; words[2] = w2; words[3] = w3;

    // Bubble sort
    int i; int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3 - i; j++) {
            if (my_strcmp(words[j], words[j+1]) > 0) {
                char *tmp = words[j];
                words[j] = words[j+1];
                words[j+1] = tmp;
            }
        }
    }

    printf("sorted: ");
    for (i = 0; i < 4; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");

    return 0;
}
