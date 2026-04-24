int printf(const char *fmt, ...);
// EXPECT: in_range tests:\n-2:N -1:N 0:Y 1:Y 2:Y 3:Y 4:Y 5:Y 6:Y 7:Y 8:Y 9:Y 10:Y 11:N 12:N \nvowel tests: a:1 b:0 e:1 x:0 i:1 o:1 z:0 \nalpha tests: A:1 5:0 z:1 #:0 M:1 \nspecial: 5:1 15:0 50:1 95:1 100:0 0:0 \ntriangle(3,4,5)=1\ntriangle(5,5,5)=3\ntriangle(3,3,5)=2\ntriangle(1,1,10)=0\ntriangle(-1,2,3)=0
int in_range(int x, int lo, int hi) {
    return x >= lo && x <= hi;
}

int is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_special(int x) {
    return (x > 0 && x < 10) || (x > 90 && x < 100) || x == 50;
}

int check_triangle(int a, int b, int c) {
    if (a <= 0 || b <= 0 || c <= 0) {
        return 0;
    }
    if (a + b > c && a + c > b && b + c > a) {
        if (a == b && b == c) {
            return 3;
        } else if (a == b || b == c || a == c) {
            return 2;
        } else {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    int i;
    printf("in_range tests:\n");
    for (i = -2; i <= 12; i++) {
        if (in_range(i, 0, 10)) {
            printf("%d:Y ", i);
        } else {
            printf("%d:N ", i);
        }
    }
    printf("\n");

    printf("vowel tests: ");
    char letters[7];
    letters[0] = 'a';
    letters[1] = 'b';
    letters[2] = 'e';
    letters[3] = 'x';
    letters[4] = 'i';
    letters[5] = 'o';
    letters[6] = 'z';
    for (i = 0; i < 7; i++) {
        printf("%c:%d ", letters[i], is_vowel(letters[i]));
    }
    printf("\n");

    printf("alpha tests: ");
    char chars[5];
    chars[0] = 'A';
    chars[1] = '5';
    chars[2] = 'z';
    chars[3] = '#';
    chars[4] = 'M';
    for (i = 0; i < 5; i++) {
        printf("%c:%d ", chars[i], is_alpha(chars[i]));
    }
    printf("\n");

    printf("special: ");
    int svals[6];
    svals[0] = 5;
    svals[1] = 15;
    svals[2] = 50;
    svals[3] = 95;
    svals[4] = 100;
    svals[5] = 0;
    for (i = 0; i < 6; i++) {
        printf("%d:%d ", svals[i], is_special(svals[i]));
    }
    printf("\n");

    printf("triangle(3,4,5)=%d\n", check_triangle(3, 4, 5));
    printf("triangle(5,5,5)=%d\n", check_triangle(5, 5, 5));
    printf("triangle(3,3,5)=%d\n", check_triangle(3, 3, 5));
    printf("triangle(1,1,10)=%d\n", check_triangle(1, 1, 10));
    printf("triangle(-1,2,3)=%d\n", check_triangle(-1, 2, 3));
    return 0;
}
