int printf(const char *fmt, ...);

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

int is_digit(char c) {
    return c >= 48 && c <= 57;
}

int is_alpha(char c) {
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

int is_alnum(char c) {
    return is_alpha(c) || is_digit(c);
}

int balanced_parens(char *s) {
    int depth = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 40) depth = depth + 1;
        if (s[i] == 41) depth = depth - 1;
        if (depth < 0) return 0;
        i = i + 1;
    }
    return depth == 0;
}

int balanced_brackets(char *s) {
    int stack[50];
    int top = 0;
    int i = 0;
    while (s[i] != 0) {
        char c = s[i];
        if (c == 40 || c == 91 || c == 123) {
            stack[top] = c;
            top = top + 1;
        } else if (c == 41 || c == 93 || c == 125) {
            if (top == 0) return 0;
            top = top - 1;
            int open = stack[top];
            if (c == 41 && open != 40) return 0;
            if (c == 93 && open != 91) return 0;
            if (c == 125 && open != 123) return 0;
        }
        i = i + 1;
    }
    return top == 0;
}

int valid_integer(char *s) {
    int len = str_len(s);
    if (len == 0) return 0;
    int i = 0;
    if (s[0] == 45 || s[0] == 43) {
        i = 1;
        if (len == 1) return 0;
    }
    while (i < len) {
        if (!is_digit(s[i])) return 0;
        i = i + 1;
    }
    return 1;
}

int valid_identifier(char *s) {
    int len = str_len(s);
    if (len == 0) return 0;
    if (!is_alpha(s[0]) && s[0] != 95) return 0;
    int i = 1;
    while (i < len) {
        if (!is_alnum(s[i]) && s[i] != 95) return 0;
        i = i + 1;
    }
    return 1;
}

int is_palindrome(char *s) {
    int len = str_len(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        if (s[i] != s[j]) return 0;
        i = i + 1;
        j = j - 1;
    }
    return 1;
}

int all_unique(char *s) {
    int seen[128];
    int i = 0;
    while (i < 128) {
        seen[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (s[i] != 0) {
        int idx = (int)s[i];
        if (seen[idx]) return 0;
        seen[idx] = 1;
        i = i + 1;
    }
    return 1;
}

int main(void) {
    char p1[10];
    p1[0]='('; p1[1]='('; p1[2]=')';
    p1[3]=')'; p1[4]=0;
    // EXPECT: balanced (()): 1
    printf("balanced (()): %d\n", balanced_parens(p1));

    char p2[10];
    p2[0]='('; p2[1]=')'; p2[2]=')'; p2[3]=0;
    // EXPECT: balanced ()): 0
    printf("balanced ()): %d\n", balanced_parens(p2));

    char p3[10];
    p3[0]='('; p3[1]='['; p3[2]=']';
    p3[3]=')'; p3[4]=0;
    // EXPECT: brackets ([]):  1
    printf("brackets ([]):  %d\n",
           balanced_brackets(p3));

    char p4[10];
    p4[0]='('; p4[1]='['; p4[2]=')';
    p4[3]=']'; p4[4]=0;
    // EXPECT: brackets ([)]: 0
    printf("brackets ([)]: %d\n",
           balanced_brackets(p4));

    char p5[10];
    p5[0]='{'; p5[1]='('; p5[2]=')';
    p5[3]='}'; p5[4]=0;
    // EXPECT: brackets {()}: 1
    printf("brackets {()}: %d\n",
           balanced_brackets(p5));

    char n1[10];
    n1[0]='1'; n1[1]='2'; n1[2]='3'; n1[3]=0;
    // EXPECT: valid_int 123: 1
    printf("valid_int 123: %d\n", valid_integer(n1));

    char n2[10];
    n2[0]='-'; n2[1]='4'; n2[2]='2'; n2[3]=0;
    // EXPECT: valid_int -42: 1
    printf("valid_int -42: %d\n", valid_integer(n2));

    char n3[10];
    n3[0]='1'; n3[1]='a'; n3[2]=0;
    // EXPECT: valid_int 1a: 0
    printf("valid_int 1a: %d\n", valid_integer(n3));

    char n4[10];
    n4[0]='-'; n4[1]=0;
    // EXPECT: valid_int -: 0
    printf("valid_int -: %d\n", valid_integer(n4));

    char id1[10];
    id1[0]='_'; id1[1]='f'; id1[2]='o';
    id1[3]='o'; id1[4]=0;
    // EXPECT: valid_id _foo: 1
    printf("valid_id _foo: %d\n", valid_identifier(id1));

    char id2[10];
    id2[0]='3'; id2[1]='a'; id2[2]='b'; id2[3]=0;
    // EXPECT: valid_id 3ab: 0
    printf("valid_id 3ab: %d\n", valid_identifier(id2));

    char id3[10];
    id3[0]='a'; id3[1]='1'; id3[2]='_'; id3[3]=0;
    // EXPECT: valid_id a1_: 1
    printf("valid_id a1_: %d\n", valid_identifier(id3));

    char pal1[10];
    pal1[0]='a'; pal1[1]='b'; pal1[2]='a'; pal1[3]=0;
    // EXPECT: palindrome aba: 1
    printf("palindrome aba: %d\n", is_palindrome(pal1));

    char pal2[10];
    pal2[0]='a'; pal2[1]='b'; pal2[2]='c'; pal2[3]=0;
    // EXPECT: palindrome abc: 0
    printf("palindrome abc: %d\n", is_palindrome(pal2));

    char u1[10];
    u1[0]='a'; u1[1]='b'; u1[2]='c'; u1[3]=0;
    // EXPECT: unique abc: 1
    printf("unique abc: %d\n", all_unique(u1));

    char u2[10];
    u2[0]='a'; u2[1]='b'; u2[2]='a'; u2[3]=0;
    // EXPECT: unique aba: 0
    printf("unique aba: %d\n", all_unique(u2));

    return 0;
}
