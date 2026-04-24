int printf(const char *fmt, ...);
// EXPECT: t1:cnt=4,verify=1,ratio=  66,lit=3,match=1|t2:cnt=5,verify=1,ratio=31,maxmlen=5|t3:cnt=10,verify=1,ratio=100,lit=10|t4:cnt=4,verify=1,avgml=2|t5:cnt=8,verify=1,ratio=  26,maxoff= 20,maxmlen=10|rle:run=4,dec_len=10,verify=1,rle_raw=5,4,3,2,7,3,1,1|rle_nr:rn=6,rle_len=12|delta:10,3,4,5,6,7,8,9,decode=10,13,17,22,28,35,43,52,ok=1|dconst: 5,0,0,0,0,0,ok=1|combo:dl=12,clen=5,ratio=41|single: 1,verify=1|twosame:2,verify=1|dec_out=1,2,3,1,2,3|end

/* LZ-like compression on integer arrays */

/* Dictionary entry: position and length of a previously seen pattern */
struct DictEntry {
    int start;
    int length;
    int next_val; /* the value that follows the match */
};

/* Compressed output: encoded tokens */
/* Token: if match_len > 0, it's (offset, length, next_val) */
/* If match_len == 0, it's a literal (0, 0, literal_val) */
struct CToken {
    int offset;
    int length;
    int next_val;
};

int input_buf[256];
int input_len;

struct CToken compressed[256];
int compressed_len;

int output_buf[256];
int output_len;

/* Simple dictionary: sliding window of size 32 */
int window_size;

void init_compress(void) {
    compressed_len = 0;
    output_len = 0;
    window_size = 32;
}

/* Find longest match in the window */
int find_match(int pos, int *match_offset, int *match_length) {
    int best_offset;
    int best_length;
    int search_start;
    int i;
    int j;
    int len;
    best_offset = 0;
    best_length = 0;
    search_start = pos - window_size;
    if (search_start < 0) search_start = 0;
    i = search_start;
    while (i < pos) {
        len = 0;
        j = 0;
        while (pos + j < input_len && input_buf[i + j] == input_buf[pos + j]) {
            len = len + 1;
            j = j + 1;
            if (i + j >= pos) break; /* don't go past current pos in search */
        }
        if (len > best_length) {
            best_length = len;
            best_offset = pos - i;
        }
        i = i + 1;
    }
    *match_offset = best_offset;
    *match_length = best_length;
    return best_length;
}

/* Compress the input buffer */
int compress(int *data, int len) {
    int pos;
    int match_offset;
    int match_length;
    int i;
    i = 0;
    while (i < len) {
        input_buf[i] = data[i];
        i = i + 1;
    }
    input_len = len;
    compressed_len = 0;
    pos = 0;
    while (pos < input_len) {
        find_match(pos, &match_offset, &match_length);
        if (match_length >= 2) {
            /* Emit match token */
            compressed[compressed_len].offset = match_offset;
            compressed[compressed_len].length = match_length;
            if (pos + match_length < input_len) {
                compressed[compressed_len].next_val = input_buf[pos + match_length];
            } else {
                compressed[compressed_len].next_val = -1;
            }
            compressed_len = compressed_len + 1;
            pos = pos + match_length + 1;
        } else {
            /* Emit literal */
            compressed[compressed_len].offset = 0;
            compressed[compressed_len].length = 0;
            compressed[compressed_len].next_val = input_buf[pos];
            compressed_len = compressed_len + 1;
            pos = pos + 1;
        }
    }
    return compressed_len;
}

/* Decompress */
int decompress(void) {
    int i;
    int j;
    int src;
    output_len = 0;
    i = 0;
    while (i < compressed_len) {
        if (compressed[i].length > 0) {
            /* Copy from earlier in output */
            src = output_len - compressed[i].offset;
            j = 0;
            while (j < compressed[i].length) {
                output_buf[output_len] = output_buf[src + j];
                output_len = output_len + 1;
                j = j + 1;
            }
            /* Append next_val */
            if (compressed[i].next_val >= 0) {
                output_buf[output_len] = compressed[i].next_val;
                output_len = output_len + 1;
            }
        } else {
            /* Literal */
            output_buf[output_len] = compressed[i].next_val;
            output_len = output_len + 1;
        }
        i = i + 1;
    }
    return output_len;
}

/* Verify decompression matches original */
int verify(int *orig, int orig_len) {
    int i;
    if (output_len != orig_len) return 0;
    i = 0;
    while (i < orig_len) {
        if (output_buf[i] != orig[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* Compression ratio: compressed_len * 100 / original_len */
int compression_ratio(int orig_len) {
    if (orig_len == 0) return 0;
    return compressed_len * 100 / orig_len;
}

/* Count literal tokens */
int count_literals(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < compressed_len) {
        if (compressed[i].length == 0) {
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

/* Count match tokens */
int count_matches(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < compressed_len) {
        if (compressed[i].length > 0) {
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

/* Max match length found */
int max_match_len(void) {
    int i;
    int m;
    m = 0;
    i = 0;
    while (i < compressed_len) {
        if (compressed[i].length > m) {
            m = compressed[i].length;
        }
        i = i + 1;
    }
    return m;
}

/* Average match length */
int avg_match_len(void) {
    int i;
    int s;
    int cnt;
    s = 0;
    cnt = 0;
    i = 0;
    while (i < compressed_len) {
        if (compressed[i].length > 0) {
            s = s + compressed[i].length;
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    if (cnt == 0) return 0;
    return s / cnt;
}

/* Max offset used */
int max_offset_used(void) {
    int i;
    int m;
    m = 0;
    i = 0;
    while (i < compressed_len) {
        if (compressed[i].offset > m) {
            m = compressed[i].offset;
        }
        i = i + 1;
    }
    return m;
}

/* RLE (Run-Length Encoding) as alternative compression */
int rle_buf[256]; /* pairs of (value, count) */
int rle_len;

int rle_compress(int *data, int len) {
    int i;
    int run;
    rle_len = 0;
    i = 0;
    while (i < len) {
        run = 1;
        while (i + run < len && data[i + run] == data[i]) {
            run = run + 1;
        }
        rle_buf[rle_len] = data[i];
        rle_buf[rle_len + 1] = run;
        rle_len = rle_len + 2;
        i = i + run;
    }
    return rle_len / 2; /* number of runs */
}

int rle_decompress(int *out) {
    int i;
    int pos;
    int j;
    pos = 0;
    i = 0;
    while (i < rle_len) {
        j = 0;
        while (j < rle_buf[i + 1]) {
            out[pos] = rle_buf[i];
            pos = pos + 1;
            j = j + 1;
        }
        i = i + 2;
    }
    return pos;
}

/* Delta encoding */
int delta_buf[256];
int delta_len;

void delta_encode(int *data, int len) {
    int i;
    if (len <= 0) { delta_len = 0; return; }
    delta_buf[0] = data[0];
    i = 1;
    while (i < len) {
        delta_buf[i] = data[i] - data[i - 1];
        i = i + 1;
    }
    delta_len = len;
}

void delta_decode(int *out, int len) {
    int i;
    if (len <= 0) return;
    out[0] = delta_buf[0];
    i = 1;
    while (i < len) {
        out[i] = out[i - 1] + delta_buf[i];
        i = i + 1;
    }
}

/* Print array helper */
void print_array(int *arr, int len) {
    int i;
    i = 0;
    while (i < len) {
        if (i > 0) printf("%c", ',');
        printf("%d", arr[i]);
        i = i + 1;
    }
}

int main(void) {
    int d1[20];
    int d2[16];
    int d3[24];
    int d4[10];
    int d5[30];
    int rle_out[256];
    int delta_out[256];
    int cnt;
    int i;

    init_compress();

    /* Test 1: Simple repetitive data */
    /* ABCABC = 1,2,3,1,2,3 */
    d1[0] = 1; d1[1] = 2; d1[2] = 3; d1[3] = 1; d1[4] = 2; d1[5] = 3;
    cnt = compress(d1, 6);
    printf("%c%c%c%c%c%c%c", 't', '1', ':', 'c', 'n', 't', '=');
    printf("%d", cnt);
    decompress();
    printf("%c%c%c%c%c%c%c%c", ',', 'v', 'e', 'r', 'i', 'f', 'y', '=');
    printf("%d", verify(d1, 6));
    printf("%c%c%c%c%c%c%c%c%c", ',', 'r', 'a', 't', 'i', 'o', '=', ' ', ' ');
    printf("%d", compression_ratio(6));
    printf("%c%c%c%c%c", ',', 'l', 'i', 't', '=');
    printf("%d", count_literals());
    printf("%c%c%c%c%c%c", ',', 'm', 'a', 't', 'c', 'h');
    printf("%c", '=');
    printf("%d", count_matches());

    /* Test 2: Highly repetitive: all same value */
    i = 0;
    while (i < 16) {
        d2[i] = 7;
        i = i + 1;
    }
    cnt = compress(d2, 16);
    printf("%c%c%c%c%c%c%c", '|', 't', '2', ':', 'c', 'n', 't');
    printf("%c", '=');
    printf("%d", cnt);
    decompress();
    printf("%c%c%c%c%c%c%c%c", ',', 'v', 'e', 'r', 'i', 'f', 'y', '=');
    printf("%d", verify(d2, 16));
    printf("%c%c%c%c%c%c", ',', 'r', 'a', 't', 'i', 'o');
    printf("%c", '=');
    printf("%d", compression_ratio(16));
    printf("%c%c%c%c%c%c%c%c%c", ',', 'm', 'a', 'x', 'm', 'l', 'e', 'n', '=');
    printf("%d", max_match_len());

    /* Test 3: No repetition */
    i = 0;
    while (i < 10) {
        d4[i] = i * 17 + 3; /* unique values */
        i = i + 1;
    }
    cnt = compress(d4, 10);
    printf("%c%c%c%c%c%c%c%c", '|', 't', '3', ':', 'c', 'n', 't', '=');
    printf("%d", cnt);
    decompress();
    printf("%c%c%c%c%c%c%c%c", ',', 'v', 'e', 'r', 'i', 'f', 'y', '=');
    printf("%d", verify(d4, 10));
    printf("%c%c%c%c%c%c", ',', 'r', 'a', 't', 'i', 'o');
    printf("%c", '=');
    printf("%d", compression_ratio(10));
    printf("%c%c%c%c%c", ',', 'l', 'i', 't', '=');
    printf("%d", count_literals());

    /* Test 4: Pattern ABAB ABAB */
    d3[0] = 10; d3[1] = 20; d3[2] = 10; d3[3] = 20;
    d3[4] = 10; d3[5] = 20; d3[6] = 10; d3[7] = 20;
    cnt = compress(d3, 8);
    printf("%c%c%c%c%c%c%c%c", '|', 't', '4', ':', 'c', 'n', 't', '=');
    printf("%d", cnt);
    decompress();
    printf("%c%c%c%c%c%c%c%c", ',', 'v', 'e', 'r', 'i', 'f', 'y', '=');
    printf("%d", verify(d3, 8));
    printf("%c%c%c%c%c%c%c", ',', 'a', 'v', 'g', 'm', 'l', '=');
    printf("%d", avg_match_len());

    /* Test 5: Longer data with patterns */
    i = 0;
    while (i < 30) {
        d5[i] = (i % 5) + 1; /* repeating 1,2,3,4,5 */
        i = i + 1;
    }
    cnt = compress(d5, 30);
    printf("%c%c%c%c%c%c%c%c", '|', 't', '5', ':', 'c', 'n', 't', '=');
    printf("%d", cnt);
    decompress();
    printf("%c%c%c%c%c%c%c%c", ',', 'v', 'e', 'r', 'i', 'f', 'y', '=');
    printf("%d", verify(d5, 30));
    printf("%c%c%c%c%c%c%c%c%c", ',', 'r', 'a', 't', 'i', 'o', '=', ' ', ' ');
    printf("%d", compression_ratio(30));
    printf("%c%c%c%c%c%c%c%c%c", ',', 'm', 'a', 'x', 'o', 'f', 'f', '=', ' ');
    printf("%d", max_offset_used());
    printf("%c%c%c%c%c%c%c%c%c", ',', 'm', 'a', 'x', 'm', 'l', 'e', 'n', '=');
    printf("%d", max_match_len());

    /* Test 6: RLE on run data */
    {
        int rle_data[20];
        rle_data[0] = 5; rle_data[1] = 5; rle_data[2] = 5; rle_data[3] = 5;
        rle_data[4] = 3; rle_data[5] = 3;
        rle_data[6] = 7; rle_data[7] = 7; rle_data[8] = 7;
        rle_data[9] = 1;
        cnt = rle_compress(rle_data, 10);
        printf("%c%c%c%c%c%c%c%c%c", '|', 'r', 'l', 'e', ':', 'r', 'u', 'n', '=');
        printf("%d", cnt);
        cnt = rle_decompress(rle_out);
        printf("%c%c%c%c%c%c%c%c%c", ',', 'd', 'e', 'c', '_', 'l', 'e', 'n', '=');
        printf("%d", cnt);
        /* Verify */
        {
            int ok;
            ok = 1;
            i = 0;
            while (i < 10) {
                if (rle_out[i] != rle_data[i]) ok = 0;
                i = i + 1;
            }
            printf("%c%c%c%c%c%c%c%c", ',', 'v', 'e', 'r', 'i', 'f', 'y', '=');
            printf("%d", ok);
        }
        printf("%c%c%c%c%c%c%c%c", ',', 'r', 'l', 'e', '_', 'r', 'a', 'w');
        printf("%c", '=');
        print_array(rle_buf, rle_len);
    }

    /* Test 7: RLE on non-run data */
    {
        int rle_data2[6];
        rle_data2[0] = 1; rle_data2[1] = 2; rle_data2[2] = 3;
        rle_data2[3] = 4; rle_data2[4] = 5; rle_data2[5] = 6;
        cnt = rle_compress(rle_data2, 6);
        printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'r', 'l', 'e', '_', 'n', 'r', ':', 'r', 'n', '=');
        printf("%d", cnt);
        /* Ratio: 12 ints for 6 values -> bad ratio */
        printf("%c%c%c%c%c%c%c%c%c", ',', 'r', 'l', 'e', '_', 'l', 'e', 'n', '=');
        printf("%d", rle_len);
    }

    /* Test 8: Delta encoding on sorted data */
    {
        int dd[8];
        dd[0] = 10; dd[1] = 13; dd[2] = 17; dd[3] = 22;
        dd[4] = 28; dd[5] = 35; dd[6] = 43; dd[7] = 52;
        delta_encode(dd, 8);
        printf("%c%c%c%c%c%c%c", '|', 'd', 'e', 'l', 't', 'a', ':');
        print_array(delta_buf, 8);
        delta_decode(delta_out, 8);
        printf("%c%c%c%c%c%c%c%c", ',', 'd', 'e', 'c', 'o', 'd', 'e', '=');
        print_array(delta_out, 8);
        /* Verify */
        {
            int ok;
            ok = 1;
            i = 0;
            while (i < 8) {
                if (delta_out[i] != dd[i]) ok = 0;
                i = i + 1;
            }
            printf("%c%c%c%c", ',', 'o', 'k', '=');
            printf("%d", ok);
        }
    }

    /* Test 9: Delta encoding on constant data */
    {
        int dc[6];
        dc[0] = 5; dc[1] = 5; dc[2] = 5; dc[3] = 5; dc[4] = 5; dc[5] = 5;
        delta_encode(dc, 6);
        printf("%c%c%c%c%c%c%c%c%c", '|', 'd', 'c', 'o', 'n', 's', 't', ':', ' ');
        print_array(delta_buf, 6);
        delta_decode(delta_out, 6);
        {
            int ok;
            ok = 1;
            i = 0;
            while (i < 6) {
                if (delta_out[i] != dc[i]) ok = 0;
                i = i + 1;
            }
            printf("%c%c%c%c", ',', 'o', 'k', '=');
            printf("%d", ok);
        }
    }

    /* Test 10: Combine delta + LZ */
    {
        int dd2[12];
        dd2[0] = 100; dd2[1] = 102; dd2[2] = 104; dd2[3] = 106;
        dd2[4] = 108; dd2[5] = 110; dd2[6] = 112; dd2[7] = 114;
        dd2[8] = 116; dd2[9] = 118; dd2[10] = 120; dd2[11] = 122;
        delta_encode(dd2, 12);
        /* Delta should be: 100,2,2,2,2,2,2,2,2,2,2,2 -> highly compressible */
        cnt = compress(delta_buf, 12);
        printf("%c%c%c%c%c%c%c%c%c%c", '|', 'c', 'o', 'm', 'b', 'o', ':', 'd', 'l', '=');
        printf("%d", delta_len);
        printf("%c%c%c%c%c%c", ',', 'c', 'l', 'e', 'n', '=');
        printf("%d", cnt);
        printf("%c%c%c%c%c%c", ',', 'r', 'a', 't', 'i', 'o');
        printf("%c", '=');
        printf("%d", compression_ratio(12));
    }

    /* Test 11: Single element */
    {
        int s1[1];
        s1[0] = 42;
        cnt = compress(s1, 1);
        printf("%c%c%c%c%c%c%c%c%c", '|', 's', 'i', 'n', 'g', 'l', 'e', ':', ' ');
        printf("%d", cnt);
        decompress();
        printf("%c%c%c%c%c%c%c%c", ',', 'v', 'e', 'r', 'i', 'f', 'y', '=');
        printf("%d", verify(s1, 1));
    }

    /* Test 12: Two elements same */
    {
        int s2[2];
        s2[0] = 99; s2[1] = 99;
        cnt = compress(s2, 2);
        printf("%c%c%c%c%c%c%c%c", '|', 't', 'w', 'o', 's', 'a', 'm', 'e');
        printf("%c", ':');
        printf("%d", cnt);
        decompress();
        printf("%c%c%c%c%c%c%c%c", ',', 'v', 'e', 'r', 'i', 'f', 'y', '=');
        printf("%d", verify(s2, 2));
    }

    /* Test 13: Decompressed output for test 1 */
    d1[0] = 1; d1[1] = 2; d1[2] = 3; d1[3] = 1; d1[4] = 2; d1[5] = 3;
    compress(d1, 6);
    decompress();
    printf("%c%c%c%c%c%c%c%c", '|', 'd', 'e', 'c', '_', 'o', 'u', 't');
    printf("%c", '=');
    print_array(output_buf, output_len);

    printf("%c%c%c%c", '|', 'e', 'n', 'd');
    printf("%c", '\n');

    return 0;
}
