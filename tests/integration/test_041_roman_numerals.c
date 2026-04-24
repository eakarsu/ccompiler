int printf(const char *fmt, ...);
// EXPECT: C1=73 C2=73,86 C3=73,88 C4=88,73,86 C5=88,76 C6=88,67 C7=67,67,67,88,67,73,88 C8=67,68 C9=67,77,88,76,73,86 C10=77 C11=77,67,77,88,67,73,86 C12=77,77,88,88,73,86 C13=77,77,77,67,77,88,67,73,88 C14=77,77,77,68,67,67,67,76,88,88,88,86,73,73,73 L1=1 L2=2 L3=2 L4=15 L5=9 RT1=1 RT2=1 RT3=1 RT4=1 RT5=1 RT6=1 RT7=1 RT8=1 RT9=1 RT10=1 RTB=50 V1=1 V2=1 V3=1 V4=0 V5=0 V6=0 V7=1 V8=1 S1=0 S2=1 S3=1 S4=0 S5=1 CS20=4 CS50=18 LR50=38 LR100=88 CD1=3 CD2=3 CD3=3 CD4=0 DV1=3 DV2=6 DV3=1221 ADD1=2000 ADD2=58 SUB1=500 SUB2=-1 CMP1=-1 CMP2=0 CMP3=1 TC=181 DC1=1 DC2=3 DC3=7 CW1=3 CW2=4 CW3=2 MXL=5 MNL=1 AVL=2 VC1=3 VC2=3 VC3=1 
/* ====================================================================== */
/* Test 041: Roman Numeral Conversion and Validation                      */
/* Convert integers to roman digit codes, roman to integer, validate      */
/* ====================================================================== */

/* Roman digit ASCII codes: M=77 D=68 C=67 L=76 X=88 V=86 I=73 */

int roman_char_value(int ch) {
    if (ch == 77) return 1000;
    if (ch == 68) return 500;
    if (ch == 67) return 100;
    if (ch == 76) return 50;
    if (ch == 88) return 10;
    if (ch == 86) return 5;
    if (ch == 73) return 1;
    return 0;
}

/* Store roman numeral as array of ASCII codes, return length */
int to_roman_array(int num, int *out) {
    int values[13];
    int c1[13];
    int c2[13];
    int idx;
    int i;

    values[0]=1000; c1[0]=77;  c2[0]=0;
    values[1]=900;  c1[1]=67;  c2[1]=77;
    values[2]=500;  c1[2]=68;  c2[2]=0;
    values[3]=400;  c1[3]=67;  c2[3]=68;
    values[4]=100;  c1[4]=67;  c2[4]=0;
    values[5]=90;   c1[5]=88;  c2[5]=67;
    values[6]=50;   c1[6]=76;  c2[6]=0;
    values[7]=40;   c1[7]=88;  c2[7]=76;
    values[8]=10;   c1[8]=88;  c2[8]=0;
    values[9]=9;    c1[9]=73;  c2[9]=88;
    values[10]=5;   c1[10]=86; c2[10]=0;
    values[11]=4;   c1[11]=73; c2[11]=86;
    values[12]=1;   c1[12]=73; c2[12]=0;

    idx = 0;
    i = 0;
    while (i < 13) {
        while (num >= values[i]) {
            out[idx] = c1[i];
            idx = idx + 1;
            if (c2[i] != 0) {
                out[idx] = c2[i];
                idx = idx + 1;
            }
            num = num - values[i];
        }
        i = i + 1;
    }
    return idx;
}

/* Convert roman array to integer */
int roman_to_int(int *digits, int len) {
    int total;
    int i;
    int cur;
    int nxt;

    total = 0;
    i = 0;
    while (i < len) {
        cur = roman_char_value(digits[i]);
        nxt = 0;
        if (i + 1 < len) {
            nxt = roman_char_value(digits[i + 1]);
        }
        if (cur < nxt) {
            total = total + (nxt - cur);
            i = i + 2;
        } else {
            total = total + cur;
            i = i + 1;
        }
    }
    return total;
}

/* Get roman length of a number */
int roman_len(int num) {
    int buf[50];
    return to_roman_array(num, buf);
}

/* Validate roman sequence: returns 1 if valid, 0 if not */
int validate_roman(int *digits, int len) {
    int i;
    int ch;
    int rep;
    int j;
    int cur;
    int nxt;
    int ok;

    i = 0;
    while (i < len) {
        if (roman_char_value(digits[i]) == 0) return 0;
        i = i + 1;
    }

    i = 0;
    while (i < len) {
        ch = digits[i];
        rep = 1;
        j = i + 1;
        while (j < len && digits[j] == ch) {
            rep = rep + 1;
            j = j + 1;
        }
        if (ch == 86 || ch == 76 || ch == 68) {
            if (rep > 1) return 0;
        }
        if (rep > 3) return 0;
        i = i + rep;
    }

    i = 0;
    while (i < len - 1) {
        cur = roman_char_value(digits[i]);
        nxt = roman_char_value(digits[i + 1]);
        if (cur < nxt) {
            ok = 0;
            if (digits[i]==73 && (digits[i+1]==86 || digits[i+1]==88)) ok=1;
            if (digits[i]==88 && (digits[i+1]==76 || digits[i+1]==67)) ok=1;
            if (digits[i]==67 && (digits[i+1]==68 || digits[i+1]==77)) ok=1;
            if (ok == 0) return 0;
        }
        i = i + 1;
    }
    return 1;
}

/* Round-trip test: int -> roman array -> int */
int test_roundtrip(int num) {
    int buf[50];
    int len;
    int result;
    len = to_roman_array(num, buf);
    result = roman_to_int(buf, len);
    if (result == num) return 1;
    return 0;
}

/* Check if a number uses subtractive notation */
int uses_subtractive(int num) {
    int buf[50];
    int len;
    int i;
    len = to_roman_array(num, buf);
    i = 0;
    while (i < len - 1) {
        if (roman_char_value(buf[i]) < roman_char_value(buf[i + 1])) {
            return 1;
        }
        i = i + 1;
    }
    return 0;
}

/* Count subtractive numbers in range [lo..hi] */
int count_subtractive(int lo, int hi) {
    int count;
    int i;
    count = 0;
    i = lo;
    while (i <= hi) {
        count = count + uses_subtractive(i);
        i = i + 1;
    }
    return count;
}

/* Find longest roman representation in range */
int longest_in_range(int lo, int hi) {
    int best_num;
    int best_len;
    int i;
    int l;
    best_num = lo;
    best_len = roman_len(lo);
    i = lo + 1;
    while (i <= hi) {
        l = roman_len(i);
        if (l > best_len) {
            best_len = l;
            best_num = i;
        }
        i = i + 1;
    }
    return best_num;
}

/* Count occurrences of a specific roman char */
int count_digit(int num, int ascii_code) {
    int buf[50];
    int len;
    int count;
    int i;
    len = to_roman_array(num, buf);
    count = 0;
    i = 0;
    while (i < len) {
        if (buf[i] == ascii_code) count = count + 1;
        i = i + 1;
    }
    return count;
}

/* Digit value sum without subtractive interpretation */
int digit_value_sum(int num) {
    int buf[50];
    int len;
    int sum;
    int i;
    len = to_roman_array(num, buf);
    sum = 0;
    i = 0;
    while (i < len) {
        sum = sum + roman_char_value(buf[i]);
        i = i + 1;
    }
    return sum;
}

/* Count numbers with exactly n roman characters */
int count_with_length(int lo, int hi, int n) {
    int count;
    int i;
    count = 0;
    i = lo;
    while (i <= hi) {
        if (roman_len(i) == n) count = count + 1;
        i = i + 1;
    }
    return count;
}

/* Count total roman chars for numbers in range */
int total_chars_in_range(int lo, int hi) {
    int total;
    int i;
    total = 0;
    i = lo;
    while (i <= hi) {
        total = total + roman_len(i);
        i = i + 1;
    }
    return total;
}

/* Count distinct roman chars */
int distinct_roman_chars(int num) {
    int buf[50];
    int len;
    int seen[128];
    int i;
    int count;
    len = to_roman_array(num, buf);
    i = 0;
    while (i < 128) { seen[i] = 0; i = i + 1; }
    i = 0;
    while (i < len) { seen[buf[i]] = 1; i = i + 1; }
    count = 0;
    if (seen[73]) count = count + 1;
    if (seen[86]) count = count + 1;
    if (seen[88]) count = count + 1;
    if (seen[76]) count = count + 1;
    if (seen[67]) count = count + 1;
    if (seen[68]) count = count + 1;
    if (seen[77]) count = count + 1;
    return count;
}

/* Print roman digits as ASCII codes */
int print_roman_codes(int num) {
    int buf[50];
    int len;
    int i;
    len = to_roman_array(num, buf);
    i = 0;
    while (i < len) {
        printf("%d", buf[i]);
        if (i < len - 1) printf(",");
        i = i + 1;
    }
    return len;
}

/* Add two roman numbers given as arrays */
int add_roman(int *a, int alen, int *b, int blen) {
    return roman_to_int(a, alen) + roman_to_int(b, blen);
}

/* Subtract roman b from a; returns -1 if b > a */
int sub_roman(int *a, int alen, int *b, int blen) {
    int va;
    int vb;
    va = roman_to_int(a, alen);
    vb = roman_to_int(b, blen);
    if (vb > va) return -1;
    return va - vb;
}

/* Compare two values: -1, 0, 1 */
int compare_roman(int a, int b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Find number of roman chars that are vowels (I=73) */
int count_vowel_chars(int num) {
    return count_digit(num, 73);
}

/* Max length in range */
int max_length_in_range(int lo, int hi) {
    int best;
    int i;
    int l;
    best = 0;
    i = lo;
    while (i <= hi) {
        l = roman_len(i);
        if (l > best) best = l;
        i = i + 1;
    }
    return best;
}

/* Min length in range */
int min_length_in_range(int lo, int hi) {
    int best;
    int i;
    int l;
    best = 100;
    i = lo;
    while (i <= hi) {
        l = roman_len(i);
        if (l < best) best = l;
        i = i + 1;
    }
    return best;
}

/* Average length (integer division) */
int avg_length_in_range(int lo, int hi) {
    int total;
    int count;
    total = total_chars_in_range(lo, hi);
    count = hi - lo + 1;
    return total / count;
}

int main(void) {
    int i;
    int pass;
    int buf1[50];
    int buf2[50];
    int len1;
    int len2;

    /* Print roman codes for several numbers */
    printf("C1="); print_roman_codes(1); printf(" ");
    printf("C2="); print_roman_codes(4); printf(" ");
    printf("C3="); print_roman_codes(9); printf(" ");
    printf("C4="); print_roman_codes(14); printf(" ");
    printf("C5="); print_roman_codes(40); printf(" ");
    printf("C6="); print_roman_codes(90); printf(" ");
    printf("C7="); print_roman_codes(399); printf(" ");
    printf("C8="); print_roman_codes(400); printf(" ");
    printf("C9="); print_roman_codes(944); printf(" ");
    printf("C10="); print_roman_codes(1000); printf(" ");
    printf("C11="); print_roman_codes(1994); printf(" ");
    printf("C12="); print_roman_codes(2024); printf(" ");
    printf("C13="); print_roman_codes(3999); printf(" ");
    printf("C14="); print_roman_codes(3888); printf(" ");

    /* Roman lengths */
    printf("L1=%d ", roman_len(1));
    printf("L2=%d ", roman_len(4));
    printf("L3=%d ", roman_len(9));
    printf("L4=%d ", roman_len(3888));
    printf("L5=%d ", roman_len(3999));

    /* Roundtrip tests */
    printf("RT1=%d ", test_roundtrip(1));
    printf("RT2=%d ", test_roundtrip(4));
    printf("RT3=%d ", test_roundtrip(9));
    printf("RT4=%d ", test_roundtrip(49));
    printf("RT5=%d ", test_roundtrip(99));
    printf("RT6=%d ", test_roundtrip(444));
    printf("RT7=%d ", test_roundtrip(999));
    printf("RT8=%d ", test_roundtrip(1994));
    printf("RT9=%d ", test_roundtrip(2024));
    printf("RT10=%d ", test_roundtrip(3999));

    /* Batch roundtrip 1-50 */
    pass = 0;
    i = 1;
    while (i <= 50) {
        pass = pass + test_roundtrip(i);
        i = i + 1;
    }
    printf("RTB=%d ", pass);

    /* Validation tests */
    {
        int v1[3]; v1[0]=88; v1[1]=73; v1[2]=86;
        printf("V1=%d ", validate_roman(v1, 3));
    }
    {
        int v2[7]; v2[0]=77; v2[1]=67; v2[2]=77; v2[3]=88; v2[4]=67; v2[5]=73; v2[6]=86;
        printf("V2=%d ", validate_roman(v2, 7));
    }
    {
        int v3[3]; v3[0]=73; v3[1]=73; v3[2]=73;
        printf("V3=%d ", validate_roman(v3, 3));
    }
    {
        int v4[4]; v4[0]=73; v4[1]=73; v4[2]=73; v4[3]=73;
        printf("V4=%d ", validate_roman(v4, 4));
    }
    {
        int v5[2]; v5[0]=86; v5[1]=86;
        printf("V5=%d ", validate_roman(v5, 2));
    }
    {
        int v6[2]; v6[0]=73; v6[1]=76;
        printf("V6=%d ", validate_roman(v6, 2));
    }
    {
        int v7[2]; v7[0]=67; v7[1]=68;
        printf("V7=%d ", validate_roman(v7, 2));
    }
    {
        int v8[2]; v8[0]=67; v8[1]=77;
        printf("V8=%d ", validate_roman(v8, 2));
    }

    /* Subtractive notation checks */
    printf("S1=%d ", uses_subtractive(1));
    printf("S2=%d ", uses_subtractive(4));
    printf("S3=%d ", uses_subtractive(9));
    printf("S4=%d ", uses_subtractive(8));
    printf("S5=%d ", uses_subtractive(1994));

    /* Count subtractive in ranges */
    printf("CS20=%d ", count_subtractive(1, 20));
    printf("CS50=%d ", count_subtractive(1, 50));

    /* Longest in range */
    printf("LR50=%d ", longest_in_range(1, 50));
    printf("LR100=%d ", longest_in_range(1, 100));

    /* Count digit occurrences */
    printf("CD1=%d ", count_digit(3888, 77));
    printf("CD2=%d ", count_digit(3888, 73));
    printf("CD3=%d ", count_digit(3, 73));
    printf("CD4=%d ", count_digit(5, 77));

    /* Digit value sums */
    printf("DV1=%d ", digit_value_sum(3));
    printf("DV2=%d ", digit_value_sum(4));
    printf("DV3=%d ", digit_value_sum(999));

    /* Roman arithmetic */
    len1 = to_roman_array(1000, buf1);
    len2 = to_roman_array(1000, buf2);
    printf("ADD1=%d ", add_roman(buf1, len1, buf2, len2));

    len1 = to_roman_array(14, buf1);
    len2 = to_roman_array(44, buf2);
    printf("ADD2=%d ", add_roman(buf1, len1, buf2, len2));

    len1 = to_roman_array(1000, buf1);
    len2 = to_roman_array(500, buf2);
    printf("SUB1=%d ", sub_roman(buf1, len1, buf2, len2));

    len1 = to_roman_array(100, buf1);
    len2 = to_roman_array(500, buf2);
    printf("SUB2=%d ", sub_roman(buf1, len1, buf2, len2));

    /* Compare */
    printf("CMP1=%d ", compare_roman(100, 500));
    printf("CMP2=%d ", compare_roman(42, 42));
    printf("CMP3=%d ", compare_roman(999, 100));

    /* Total chars in range */
    printf("TC=%d ", total_chars_in_range(1, 50));

    /* Distinct chars */
    printf("DC1=%d ", distinct_roman_chars(1));
    printf("DC2=%d ", distinct_roman_chars(14));
    printf("DC3=%d ", distinct_roman_chars(3888));

    /* Count with specific length */
    printf("CW1=%d ", count_with_length(1, 10, 1));
    printf("CW2=%d ", count_with_length(1, 10, 2));
    printf("CW3=%d ", count_with_length(1, 10, 3));

    /* Max/min/avg length in range */
    printf("MXL=%d ", max_length_in_range(1, 20));
    printf("MNL=%d ", min_length_in_range(1, 20));
    printf("AVL=%d ", avg_length_in_range(1, 20));

    /* Vowel chars (I count) for various numbers */
    printf("VC1=%d ", count_vowel_chars(3));
    printf("VC2=%d ", count_vowel_chars(3888));
    printf("VC3=%d ", count_vowel_chars(4));

    printf("\n");
    return 0;
}
