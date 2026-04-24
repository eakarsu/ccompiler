int printf(const char *fmt, ...);

/* Password strength checker and entropy estimation */

/* Check if character (as int) is uppercase letter */
int is_upper(int c) {
    return (c >= 65 && c <= 90) ? 1 : 0;
}

/* Check if lowercase */
int is_lower(int c) {
    return (c >= 97 && c <= 122) ? 1 : 0;
}

/* Check if digit */
int is_digit(int c) {
    return (c >= 48 && c <= 57) ? 1 : 0;
}

/* Check if special character (printable non-alphanumeric) */
int is_special(int c) {
    if (c >= 33 && c <= 47) return 1;   /* ! " # $ % & ' ( ) * + , - . / */
    if (c >= 58 && c <= 64) return 1;   /* : ; < = > ? @ */
    if (c >= 91 && c <= 96) return 1;   /* [ \ ] ^ _ ` */
    if (c >= 123 && c <= 126) return 1; /* { | } ~ */
    return 0;
}

/* Analyze a password (array of ASCII ints) */
struct pw_analysis {
    int length;
    int has_upper;
    int has_lower;
    int has_digit;
    int has_special;
    int num_upper;
    int num_lower;
    int num_digit;
    int num_special;
    int char_classes;     /* number of character classes used */
    int pool_size;        /* size of character pool */
    int entropy_bits;     /* estimated entropy in bits (approximate) */
    int score;            /* overall score 0-100 */
};

void analyze_password(int *pw, int len, struct pw_analysis *a) {
    int i;
    int c;

    a->length = len;
    a->has_upper = 0;
    a->has_lower = 0;
    a->has_digit = 0;
    a->has_special = 0;
    a->num_upper = 0;
    a->num_lower = 0;
    a->num_digit = 0;
    a->num_special = 0;

    i = 0;
    while (i < len) {
        c = pw[i];
        if (is_upper(c)) {
            a->has_upper = 1;
            a->num_upper = a->num_upper + 1;
        }
        if (is_lower(c)) {
            a->has_lower = 1;
            a->num_lower = a->num_lower + 1;
        }
        if (is_digit(c)) {
            a->has_digit = 1;
            a->num_digit = a->num_digit + 1;
        }
        if (is_special(c)) {
            a->has_special = 1;
            a->num_special = a->num_special + 1;
        }
        i = i + 1;
    }

    /* Count character classes */
    a->char_classes = a->has_upper + a->has_lower + a->has_digit + a->has_special;

    /* Compute pool size based on character classes used */
    a->pool_size = 0;
    if (a->has_upper) a->pool_size = a->pool_size + 26;
    if (a->has_lower) a->pool_size = a->pool_size + 26;
    if (a->has_digit) a->pool_size = a->pool_size + 10;
    if (a->has_special) a->pool_size = a->pool_size + 32;

    /* Estimate entropy: log2(pool_size) * length
       We approximate log2 using integer math:
       log2(26)~4, log2(36)~5, log2(52)~5, log2(62)~5, log2(94)~6 */
    if (a->pool_size <= 10) a->entropy_bits = len * 3;
    else if (a->pool_size <= 26) a->entropy_bits = len * 4;
    else if (a->pool_size <= 36) a->entropy_bits = len * 5;
    else if (a->pool_size <= 62) a->entropy_bits = len * 5;
    else a->entropy_bits = len * 6;

    /* Score: weighted combination */
    a->score = 0;
    /* Length contributes up to 30 points */
    if (len >= 12) a->score = a->score + 30;
    else if (len >= 8) a->score = a->score + 20;
    else if (len >= 6) a->score = a->score + 10;

    /* Character classes contribute up to 40 points (10 each) */
    a->score = a->score + a->char_classes * 10;

    /* Entropy contributes up to 30 points */
    if (a->entropy_bits >= 60) a->score = a->score + 30;
    else if (a->entropy_bits >= 40) a->score = a->score + 20;
    else if (a->entropy_bits >= 20) a->score = a->score + 10;
}

/* Check for consecutive repeated characters */
int count_repeats(int *pw, int len) {
    int count;
    int i;
    count = 0;
    i = 1;
    while (i < len) {
        if (pw[i] == pw[i - 1]) count = count + 1;
        i = i + 1;
    }
    return count;
}

/* Check for sequential characters (ascending) */
int count_sequential(int *pw, int len) {
    int count;
    int i;
    count = 0;
    i = 1;
    while (i < len) {
        if (pw[i] == pw[i - 1] + 1) count = count + 1;
        i = i + 1;
    }
    return count;
}

/* Count unique characters in password */
int count_unique(int *pw, int len) {
    int seen[128];
    int i;
    int count;
    i = 0;
    while (i < 128) {
        seen[i] = 0;
        i = i + 1;
    }
    count = 0;
    i = 0;
    while (i < len) {
        if (pw[i] >= 0 && pw[i] < 128 && !seen[pw[i]]) {
            seen[pw[i]] = 1;
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int main() {
    struct pw_analysis a;
    int pw1[8];  /* "Abc1@xyz" */
    int pw2[4];  /* "aaaa" */
    int pw3[12]; /* "MyP@ss1234!!" */
    int pw4[6];  /* "123456" */
    int pw5[8];  /* "abcdefgh" */

    /* Test 1: Character class checks */
    printf("upper_A: %d\n", is_upper(65));
    // EXPECT: upper_A: 1
    printf("upper_a: %d\n", is_upper(97));
    // EXPECT: upper_a: 0
    printf("lower_a: %d\n", is_lower(97));
    // EXPECT: lower_a: 1
    printf("digit_5: %d\n", is_digit(53));
    // EXPECT: digit_5: 1
    printf("special_at: %d\n", is_special(64));
    // EXPECT: special_at: 1
    printf("special_A: %d\n", is_special(65));
    // EXPECT: special_A: 0

    /* Test 2: Password "Abc1@xyz" = 65,98,99,49,64,120,121,122 */
    pw1[0]=65; pw1[1]=98; pw1[2]=99; pw1[3]=49;
    pw1[4]=64; pw1[5]=120; pw1[6]=121; pw1[7]=122;
    analyze_password(pw1, 8, &a);
    printf("pw1_len: %d\n", a.length);
    // EXPECT: pw1_len: 8
    printf("pw1_classes: %d\n", a.char_classes);
    // EXPECT: pw1_classes: 4
    printf("pw1_pool: %d\n", a.pool_size);
    // EXPECT: pw1_pool: 94
    printf("pw1_entropy: %d\n", a.entropy_bits);
    // EXPECT: pw1_entropy: 48
    printf("pw1_score: %d\n", a.score);
    /* length>=8: 20, classes 4*10=40, entropy 48>=40: 20, total=80 */
    // EXPECT: pw1_score: 80

    /* Test 3: Weak password "aaaa" = 97,97,97,97 */
    pw2[0]=97; pw2[1]=97; pw2[2]=97; pw2[3]=97;
    analyze_password(pw2, 4, &a);
    printf("pw2_classes: %d\n", a.char_classes);
    // EXPECT: pw2_classes: 1
    printf("pw2_pool: %d\n", a.pool_size);
    // EXPECT: pw2_pool: 26
    printf("pw2_entropy: %d\n", a.entropy_bits);
    // EXPECT: pw2_entropy: 16
    printf("pw2_score: %d\n", a.score);
    /* length<6: 0, classes 1*10=10, entropy 16<20: 0, total=10 */
    // EXPECT: pw2_score: 10

    /* Test 4: Strong password "MyP@ss1234!!" = 77,121,80,64,115,115,49,50,51,52,33,33 */
    pw3[0]=77; pw3[1]=121; pw3[2]=80; pw3[3]=64;
    pw3[4]=115; pw3[5]=115; pw3[6]=49; pw3[7]=50;
    pw3[8]=51; pw3[9]=52; pw3[10]=33; pw3[11]=33;
    analyze_password(pw3, 12, &a);
    printf("pw3_classes: %d\n", a.char_classes);
    // EXPECT: pw3_classes: 4
    printf("pw3_pool: %d\n", a.pool_size);
    // EXPECT: pw3_pool: 94
    printf("pw3_entropy: %d\n", a.entropy_bits);
    // EXPECT: pw3_entropy: 72
    printf("pw3_score: %d\n", a.score);
    /* length>=12: 30, classes 4*10=40, entropy 72>=60: 30, total=100 */
    // EXPECT: pw3_score: 100

    /* Test 5: Digits only "123456" = 49,50,51,52,53,54 */
    pw4[0]=49; pw4[1]=50; pw4[2]=51; pw4[3]=52; pw4[4]=53; pw4[5]=54;
    analyze_password(pw4, 6, &a);
    printf("pw4_classes: %d\n", a.char_classes);
    // EXPECT: pw4_classes: 1
    printf("pw4_pool: %d\n", a.pool_size);
    // EXPECT: pw4_pool: 10
    printf("pw4_entropy: %d\n", a.entropy_bits);
    // EXPECT: pw4_entropy: 18
    printf("pw4_score: %d\n", a.score);
    /* length>=6: 10, classes 1*10=10, entropy 18<20: 0, total=20 */
    // EXPECT: pw4_score: 20

    /* Test 6: Repeats and sequential */
    printf("repeats_aaaa: %d\n", count_repeats(pw2, 4));
    // EXPECT: repeats_aaaa: 3

    printf("seq_123456: %d\n", count_sequential(pw4, 6));
    // EXPECT: seq_123456: 5

    printf("repeats_pw1: %d\n", count_repeats(pw1, 8));
    /* 65,98,99,49,64,120,121,122: no consecutive repeats */
    // EXPECT: repeats_pw1: 0

    printf("seq_pw1: %d\n", count_sequential(pw1, 8));
    /* 98->99 is +1, 120->121 is +1, 121->122 is +1 = 3 */
    // EXPECT: seq_pw1: 3

    /* Test 7: Unique characters */
    printf("uniq_aaaa: %d\n", count_unique(pw2, 4));
    // EXPECT: uniq_aaaa: 1

    printf("uniq_pw1: %d\n", count_unique(pw1, 8));
    /* All 8 chars are different: 65,98,99,49,64,120,121,122 */
    // EXPECT: uniq_pw1: 8

    printf("uniq_123456: %d\n", count_unique(pw4, 6));
    // EXPECT: uniq_123456: 6

    /* Test 8: Password "abcdefgh" = 97,98,99,100,101,102,103,104 */
    pw5[0]=97; pw5[1]=98; pw5[2]=99; pw5[3]=100;
    pw5[4]=101; pw5[5]=102; pw5[6]=103; pw5[7]=104;
    analyze_password(pw5, 8, &a);
    printf("pw5_classes: %d\n", a.char_classes);
    // EXPECT: pw5_classes: 1
    printf("pw5_score: %d\n", a.score);
    /* length>=8: 20, classes 1*10=10, entropy=8*4=32, 32>=20: 10, total=40 */
    // EXPECT: pw5_score: 40

    printf("seq_abcdefgh: %d\n", count_sequential(pw5, 8));
    // EXPECT: seq_abcdefgh: 7

    /* Test 9: Detailed counts for strong password */
    analyze_password(pw3, 12, &a);
    printf("upper_count: %d\n", a.num_upper);
    /* M(77), P(80) = 2 */
    // EXPECT: upper_count: 2
    printf("lower_count: %d\n", a.num_lower);
    /* y(121), s(115), s(115) = 3 */
    // EXPECT: lower_count: 3
    printf("digit_count: %d\n", a.num_digit);
    /* 1(49), 2(50), 3(51), 4(52) = 4 */
    // EXPECT: digit_count: 4
    printf("special_count: %d\n", a.num_special);
    /* @(64), !(33), !(33) = 3 */
    // EXPECT: special_count: 3

    /* Test 10: Unique chars in strong password */
    printf("uniq_strong: %d\n", count_unique(pw3, 12));
    /* 77,121,80,64,115,115,49,50,51,52,33,33 -> unique: 77,121,80,64,115,49,50,51,52,33 = 10 */
    // EXPECT: uniq_strong: 10

    printf("pwcheck_done\n");
    // EXPECT: pwcheck_done
    return 0;
}
