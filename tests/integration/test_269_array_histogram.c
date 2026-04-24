int printf(const char *fmt, ...);
// EXPECT: Histogram:\n0:  (0)\n1: ** (2)\n2: ** (2)\n3: **** (4)\n4: ** (2)\n5: *** (3)\n6: * (1)\n7: * (1)\n8: ** (2)\n9: *** (3)\nmode = 3 (freq=4)\ntotal = 20\ncumulative: 0 2 4 8 10 13 14 15 17 20 \nchar freq: d=1 e=1 h=1 l=3 o=2 r=1 w=1 
// Test: array histogram (frequency count)

int main(void) {
    // Count frequency of values 0-9
    int data[20];
    data[0]=3; data[1]=1; data[2]=4; data[3]=1; data[4]=5;
    data[5]=9; data[6]=2; data[7]=6; data[8]=5; data[9]=3;
    data[10]=5; data[11]=8; data[12]=9; data[13]=7; data[14]=9;
    data[15]=3; data[16]=2; data[17]=3; data[18]=8; data[19]=4;

    int freq[10];
    int i;
    for (i = 0; i < 10; i++) freq[i] = 0;

    for (i = 0; i < 20; i++) {
        freq[data[i]] = freq[data[i]] + 1;
    }

    printf("Histogram:\n");
    for (i = 0; i < 10; i++) {
        printf("%d: ", i);
        int j;
        for (j = 0; j < freq[i]; j++) {
            printf("*");
        }
        printf(" (%d)\n", freq[i]);
    }

    // Find mode (most frequent)
    int mode = 0;
    int max_freq = freq[0];
    for (i = 1; i < 10; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            mode = i;
        }
    }
    printf("mode = %d (freq=%d)\n", mode, max_freq);

    // Find total
    int total = 0;
    for (i = 0; i < 10; i++) {
        total = total + freq[i];
    }
    printf("total = %d\n", total);

    // Cumulative histogram
    int cum[10];
    cum[0] = freq[0];
    for (i = 1; i < 10; i++) {
        cum[i] = cum[i-1] + freq[i];
    }
    printf("cumulative: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", cum[i]);
    }
    printf("\n");

    // Character frequency
    char *text = "hello world";
    int char_freq[26];
    for (i = 0; i < 26; i++) char_freq[i] = 0;
    i = 0;
    while (text[i] != '\0') {
        if (text[i] >= 'a' && text[i] <= 'z') {
            char_freq[text[i] - 'a'] = char_freq[text[i] - 'a'] + 1;
        }
        i = i + 1;
    }
    printf("char freq: ");
    for (i = 0; i < 26; i++) {
        if (char_freq[i] > 0) {
            printf("%c=%d ", 'a' + i, char_freq[i]);
        }
    }
    printf("\n");

    return 0;
}
