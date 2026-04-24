int printf(const char *fmt, ...);

// Test 075: Dictionary-based compression - map repeated words to short codes

// Dictionary: up to 16 entries, each up to 8 chars
char dict_words[16][8];
int dict_lens[16];
int dict_count;

void dict_init() {
    dict_count = 0;
}

int str_equal(char *a, char *b, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int dict_find(char *word, int wlen) {
    int i;
    for (i = 0; i < dict_count; i = i + 1) {
        if (dict_lens[i] == wlen && str_equal(dict_words[i], word, wlen)) {
            return i;
        }
    }
    return -1;
}

int dict_add(char *word, int wlen) {
    if (dict_count >= 16) return -1;
    int idx = dict_count;
    int i;
    for (i = 0; i < wlen; i = i + 1) {
        dict_words[idx][i] = word[i];
    }
    dict_lens[idx] = wlen;
    dict_count = dict_count + 1;
    return idx;
}

// Compress: input is array of "word" start positions and lengths
// Output: array of dictionary indices
int dict_compress(char *text, int *starts, int *lens, int nwords, int *output) {
    int i;
    for (i = 0; i < nwords; i = i + 1) {
        int idx = dict_find(text + starts[i], lens[i]);
        if (idx == -1) {
            idx = dict_add(text + starts[i], lens[i]);
        }
        output[i] = idx;
    }
    return nwords;
}

// Decompress: convert indices back to text
int dict_decompress(int *indices, int count, char *output) {
    int pos = 0;
    int i;
    for (i = 0; i < count; i = i + 1) {
        if (i > 0) {
            output[pos] = ' ';
            pos = pos + 1;
        }
        int idx = indices[i];
        int j;
        for (j = 0; j < dict_lens[idx]; j = j + 1) {
            output[pos] = dict_words[idx][j];
            pos = pos + 1;
        }
    }
    output[pos] = 0;
    return pos;
}

int main() {
    dict_init();

    // Input: "the cat sat on the mat the cat"
    // Words: the, cat, sat, on, the, mat, the, cat
    char text[40];
    text[0]='t'; text[1]='h'; text[2]='e'; text[3]=' ';
    text[4]='c'; text[5]='a'; text[6]='t'; text[7]=' ';
    text[8]='s'; text[9]='a'; text[10]='t'; text[11]=' ';
    text[12]='o'; text[13]='n'; text[14]=' ';
    text[15]='t'; text[16]='h'; text[17]='e'; text[18]=' ';
    text[19]='m'; text[20]='a'; text[21]='t'; text[22]=' ';
    text[23]='t'; text[24]='h'; text[25]='e'; text[26]=' ';
    text[27]='c'; text[28]='a'; text[29]='t';

    int starts[8];
    int lens[8];
    starts[0] = 0;  lens[0] = 3;  // the
    starts[1] = 4;  lens[1] = 3;  // cat
    starts[2] = 8;  lens[2] = 3;  // sat
    starts[3] = 12; lens[3] = 2;  // on
    starts[4] = 15; lens[4] = 3;  // the
    starts[5] = 19; lens[5] = 3;  // mat
    starts[6] = 23; lens[6] = 3;  // the
    starts[7] = 27; lens[7] = 3;  // cat
    int nwords = 8;

    int compressed[8];
    int out_len = dict_compress(text, starts, lens, nwords, compressed);

    printf("Dict entries: %d\n", dict_count); // EXPECT: Dict entries: 5
    printf("Words compressed: %d\n", out_len); // EXPECT: Words compressed: 8

    int i;
    for (i = 0; i < out_len; i = i + 1) {
        printf("Code %d: %d\n", i, compressed[i]); // EXPECT: Code 0: 0
        // EXPECT: Code 1: 1
        // EXPECT: Code 2: 2
        // EXPECT: Code 3: 3
        // EXPECT: Code 4: 0
        // EXPECT: Code 5: 4
        // EXPECT: Code 6: 0
        // EXPECT: Code 7: 1
    }

    // Verify "the" appears 3 times -> code 0
    int the_count = 0;
    for (i = 0; i < out_len; i = i + 1) {
        if (compressed[i] == 0) the_count = the_count + 1;
    }
    printf("the count: %d\n", the_count); // EXPECT: the count: 3

    // Decompress
    char output[60];
    int dec_len = dict_decompress(compressed, out_len, output);
    printf("Decompressed len: %d\n", dec_len); // EXPECT: Decompressed len: 30

    // Compression ratio: 8 codes vs 30 chars of text
    int ratio = (8 * 100) / 30;
    printf("Index ratio: %d\n", ratio); // EXPECT: Index ratio: 26

    // Check dict entry lengths
    printf("Dict 0 len: %d\n", dict_lens[0]); // EXPECT: Dict 0 len: 3
    printf("Dict 3 len: %d\n", dict_lens[3]); // EXPECT: Dict 3 len: 2
    printf("Dict 4 len: %d\n", dict_lens[4]); // EXPECT: Dict 4 len: 3

    // Test with all unique words
    dict_init();
    char text2[12];
    text2[0]='a'; text2[1]=' '; text2[2]='b'; text2[3]=' ';
    text2[4]='c'; text2[5]=' '; text2[6]='d';
    int s2[4]; int l2[4];
    s2[0]=0; l2[0]=1; s2[1]=2; l2[1]=1; s2[2]=4; l2[2]=1; s2[3]=6; l2[3]=1;
    int c2[4];
    dict_compress(text2, s2, l2, 4, c2);
    printf("All unique dict: %d\n", dict_count); // EXPECT: All unique dict: 4

    printf("Done\n"); // EXPECT: Done
    return 0;
}
