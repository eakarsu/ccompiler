int printf(const char *fmt, ...);
// EXPECT: p1=3 e0=1 e1=3 e2=2 e3=2 e4=3 e5=4 dn=9 eq=1 cr=150 p2=5 cr2=50 p3=1 v3=7 c3=20 cr3=1000 f1=5 f2=3 f3=2 mf=1 lf=3 dc=3 de0=10 de1=2 de2=3 de3=5 de4=2 deq=1 m0=1 m1=0 m2=0 m3=2 m4=0 meq=1 pk=228 u0=0 u1=1 u2=2 u3=3 runs=3 lr=3 dc0=0 dc1=1 dc2=2 dc3=0 ddc=1

/* ================================================================
 * test_038_compression.c
 * Run-length encoding/decoding on integer arrays.
 * Simple frequency-based encoding.
 * ================================================================ */

/* ---- Run-Length Encoding ---- */
/* Input: array of ints. Output: pairs of (value, count). */
/* encoded[0]=val1, encoded[1]=count1, encoded[2]=val2, encoded[3]=count2, ... */
/* Returns number of pairs (length of encoded = 2 * return value) */
int rle_encode(int *input, int n, int *encoded) {
    int i;
    int oi;
    int cur;
    int cnt;
    if (n == 0) return 0;
    cur = input[0];
    cnt = 1;
    oi = 0;
    i = 1;
    while (i < n) {
        if (input[i] == cur) {
            cnt = cnt + 1;
        } else {
            encoded[oi] = cur;
            encoded[oi + 1] = cnt;
            oi = oi + 2;
            cur = input[i];
            cnt = 1;
        }
        i = i + 1;
    }
    encoded[oi] = cur;
    encoded[oi + 1] = cnt;
    oi = oi + 2;
    return oi / 2;
}

/* Decode RLE back to original array. Returns length of decoded. */
int rle_decode(int *encoded, int pairs, int *output) {
    int i;
    int oi;
    int j;
    int val;
    int cnt;
    oi = 0;
    i = 0;
    while (i < pairs) {
        val = encoded[i * 2];
        cnt = encoded[i * 2 + 1];
        j = 0;
        while (j < cnt) {
            output[oi] = val;
            oi = oi + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    return oi;
}

/* Check if two arrays are equal */
int arrays_equal(int *a, int *b, int n) {
    int i;
    i = 0;
    while (i < n) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* Compression ratio: (original_len * 100) / encoded_len */
int compression_ratio(int orig_len, int pairs) {
    int enc_len;
    enc_len = pairs * 2;
    if (enc_len == 0) return 0;
    return (orig_len * 100) / enc_len;
}

/* ---- Frequency counting ---- */
/* Count frequency of each value (assuming values 0..MAX_VAL) */
void count_freq(int *input, int n, int *freq, int max_val) {
    int i;
    i = 0;
    while (i <= max_val) { freq[i] = 0; i = i + 1; }
    i = 0;
    while (i < n) {
        if (input[i] >= 0 && input[i] <= max_val) {
            freq[input[i]] = freq[input[i]] + 1;
        }
        i = i + 1;
    }
}

/* Find most frequent value */
int most_frequent(int *freq, int max_val) {
    int i; int best; int best_cnt;
    best = 0; best_cnt = freq[0];
    i = 1;
    while (i <= max_val) {
        if (freq[i] > best_cnt) {
            best_cnt = freq[i];
            best = i;
        }
        i = i + 1;
    }
    return best;
}

/* Find least frequent value (that appears at least once) */
int least_frequent(int *freq, int max_val) {
    int i; int best; int best_cnt;
    best = -1; best_cnt = 999999;
    i = 0;
    while (i <= max_val) {
        if (freq[i] > 0 && freq[i] < best_cnt) {
            best_cnt = freq[i];
            best = i;
        }
        i = i + 1;
    }
    return best;
}

/* Count distinct values */
int count_distinct(int *freq, int max_val) {
    int i; int cnt;
    cnt = 0; i = 0;
    while (i <= max_val) {
        if (freq[i] > 0) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

/* ---- Delta Encoding ---- */
/* Store differences between consecutive values */
int delta_encode(int *input, int n, int *output) {
    int i;
    if (n == 0) return 0;
    output[0] = input[0];
    i = 1;
    while (i < n) {
        output[i] = input[i] - input[i - 1];
        i = i + 1;
    }
    return n;
}

int delta_decode(int *input, int n, int *output) {
    int i;
    if (n == 0) return 0;
    output[0] = input[0];
    i = 1;
    while (i < n) {
        output[i] = output[i - 1] + input[i];
        i = i + 1;
    }
    return n;
}

/* ---- Move-to-Front Encoding (simplified for values 0-9) ---- */
int mtf_encode(int *input, int n, int *output) {
    int list[10]; /* alphabet 0-9 */
    int i; int j; int val; int pos;
    /* Initialize list */
    i = 0;
    while (i < 10) { list[i] = i; i = i + 1; }
    i = 0;
    while (i < n) {
        val = input[i];
        /* Find position of val in list */
        pos = 0;
        j = 0;
        while (j < 10) {
            if (list[j] == val) { pos = j; break; }
            j = j + 1;
        }
        output[i] = pos;
        /* Move val to front */
        j = pos;
        while (j > 0) {
            list[j] = list[j - 1];
            j = j - 1;
        }
        list[0] = val;
        i = i + 1;
    }
    return n;
}

int mtf_decode(int *input, int n, int *output) {
    int list[10];
    int i; int j; int pos; int val;
    i = 0;
    while (i < 10) { list[i] = i; i = i + 1; }
    i = 0;
    while (i < n) {
        pos = input[i];
        val = list[pos];
        output[i] = val;
        j = pos;
        while (j > 0) {
            list[j] = list[j - 1];
            j = j - 1;
        }
        list[0] = val;
        i = i + 1;
    }
    return n;
}

/* ---- Bit packing: pack values 0-3 (2 bits each) into ints ---- */
/* Pack 16 values (2 bits each) into one int (32 bits) */
int bit_pack(int *vals, int n) {
    int result;
    int i;
    result = 0;
    i = 0;
    while (i < n && i < 16) {
        result = result | ((vals[i] & 3) << (i * 2));
        i = i + 1;
    }
    return result;
}

/* Unpack */
void bit_unpack(int packed, int *vals, int n) {
    int i;
    i = 0;
    while (i < n && i < 16) {
        vals[i] = (packed >> (i * 2)) & 3;
        i = i + 1;
    }
}

/* ---- Simple dictionary encoder ---- */
/* Dictionary: maps values to shorter codes */
/* dict_keys[i] = original value, dict_vals[i] = code */
int dict_keys[32];
int dict_vals[32];
int dict_size;

void dict_init(void) { dict_size = 0; }

void dict_add(int key, int val) {
    if (dict_size < 32) {
        dict_keys[dict_size] = key;
        dict_vals[dict_size] = val;
        dict_size = dict_size + 1;
    }
}

int dict_lookup(int key) {
    int i;
    i = 0;
    while (i < dict_size) {
        if (dict_keys[i] == key) return dict_vals[i];
        i = i + 1;
    }
    return -1;
}

int dict_reverse(int val) {
    int i;
    i = 0;
    while (i < dict_size) {
        if (dict_vals[i] == val) return dict_keys[i];
        i = i + 1;
    }
    return -1;
}

int dict_encode(int *input, int n, int *output) {
    int i; int code;
    i = 0;
    while (i < n) {
        code = dict_lookup(input[i]);
        if (code >= 0) output[i] = code;
        else output[i] = input[i]; /* no mapping, keep original */
        i = i + 1;
    }
    return n;
}

int dict_decode(int *input, int n, int *output) {
    int i; int val;
    i = 0;
    while (i < n) {
        val = dict_reverse(input[i]);
        if (val >= 0) output[i] = val;
        else output[i] = input[i];
        i = i + 1;
    }
    return n;
}

/* ---- Additional helpers ---- */

/* Count runs in array */
int count_runs(int *input, int n) {
    int i; int cnt;
    if (n == 0) return 0;
    cnt = 1; i = 1;
    while (i < n) {
        if (input[i] != input[i - 1]) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

/* Length of longest run */
int longest_run(int *input, int n) {
    int i; int best; int cur;
    if (n == 0) return 0;
    best = 1; cur = 1; i = 1;
    while (i < n) {
        if (input[i] == input[i - 1]) {
            cur = cur + 1;
            if (cur > best) best = cur;
        } else {
            cur = 1;
        }
        i = i + 1;
    }
    return best;
}

/* Entropy approximation: sum of freq[i] * freq[i] (lower = more uniform) */
int entropy_approx(int *freq, int max_val) {
    int i; int s;
    s = 0; i = 0;
    while (i <= max_val) {
        s = s + freq[i] * freq[i];
        i = i + 1;
    }
    return s;
}

int main(void) {
    int input[64];
    int encoded[128];
    int decoded[64];
    int freq[20];
    int delta_enc[64];
    int delta_dec[64];
    int mtf_enc[64];
    int mtf_dec[64];
    int pack_vals[16];
    int unpack_vals[16];
    int n;
    int pairs;
    int packed;
    int i;

    /* ---- RLE Test 1: simple repetition ---- */
    /* Input: 1,1,1,2,2,3,3,3,3 */
    input[0]=1; input[1]=1; input[2]=1; input[3]=2; input[4]=2;
    input[5]=3; input[6]=3; input[7]=3; input[8]=3;
    pairs = rle_encode(input, 9, encoded);
    printf("p1=%d ", pairs);                       /* 3 pairs */
    printf("e0=%d e1=%d ", encoded[0], encoded[1]); /* 1,3 */
    printf("e2=%d e3=%d ", encoded[2], encoded[3]); /* 2,2 */
    printf("e4=%d e5=%d ", encoded[4], encoded[5]); /* 3,4 */

    n = rle_decode(encoded, pairs, decoded);
    printf("dn=%d ", n);                           /* 9 */
    printf("eq=%d ", arrays_equal(input, decoded, 9)); /* 1 */

    printf("cr=%d ", compression_ratio(9, 3));     /* 150 */

    /* ---- RLE Test 2: no repetition ---- */
    input[0]=1; input[1]=2; input[2]=3; input[3]=4; input[4]=5;
    pairs = rle_encode(input, 5, encoded);
    printf("p2=%d ", pairs);                       /* 5 pairs (no compression) */
    printf("cr2=%d ", compression_ratio(5, 5));    /* 50 */

    /* ---- RLE Test 3: all same ---- */
    i = 0;
    while (i < 20) { input[i] = 7; i = i + 1; }
    pairs = rle_encode(input, 20, encoded);
    printf("p3=%d ", pairs);                       /* 1 pair */
    printf("v3=%d c3=%d ", encoded[0], encoded[1]); /* 7,20 */
    printf("cr3=%d ", compression_ratio(20, 1));   /* 1000 */

    /* ---- Frequency tests ---- */
    input[0]=1; input[1]=2; input[2]=1; input[3]=3; input[4]=1;
    input[5]=2; input[6]=3; input[7]=1; input[8]=2; input[9]=1;
    count_freq(input, 10, freq, 5);
    printf("f1=%d ", freq[1]);                     /* 5 */
    printf("f2=%d ", freq[2]);                     /* 3 */
    printf("f3=%d ", freq[3]);                     /* 2 */
    printf("mf=%d ", most_frequent(freq, 5));      /* 1 */
    printf("lf=%d ", least_frequent(freq, 5));     /* 3 */
    printf("dc=%d ", count_distinct(freq, 5));     /* 3 */

    /* ---- Delta encoding ---- */
    input[0]=10; input[1]=12; input[2]=15; input[3]=20; input[4]=22;
    delta_encode(input, 5, delta_enc);
    printf("de0=%d ", delta_enc[0]);               /* 10 */
    printf("de1=%d ", delta_enc[1]);               /* 2 */
    printf("de2=%d ", delta_enc[2]);               /* 3 */
    printf("de3=%d ", delta_enc[3]);               /* 5 */
    printf("de4=%d ", delta_enc[4]);               /* 2 */

    delta_decode(delta_enc, 5, delta_dec);
    printf("deq=%d ", arrays_equal(input, delta_dec, 5)); /* 1 */

    /* ---- MTF encoding ---- */
    input[0]=1; input[1]=1; input[2]=1; input[3]=2; input[4]=2;
    mtf_encode(input, 5, mtf_enc);
    printf("m0=%d ", mtf_enc[0]);                  /* 1 (position of 1 in [0,1,2,...]) */
    printf("m1=%d ", mtf_enc[1]);                  /* 0 (1 is now at front) */
    printf("m2=%d ", mtf_enc[2]);                  /* 0 */
    printf("m3=%d ", mtf_enc[3]);                  /* 2 (2 is at position 2: [1,0,2,...]) */
    printf("m4=%d ", mtf_enc[4]);                  /* 0 */

    mtf_decode(mtf_enc, 5, mtf_dec);
    printf("meq=%d ", arrays_equal(input, mtf_dec, 5)); /* 1 */

    /* ---- Bit packing ---- */
    pack_vals[0]=0; pack_vals[1]=1; pack_vals[2]=2; pack_vals[3]=3;
    packed = bit_pack(pack_vals, 4);
    printf("pk=%d ", packed);                      /* 0b11100100 = 228 */
    bit_unpack(packed, unpack_vals, 4);
    printf("u0=%d u1=%d u2=%d u3=%d ", unpack_vals[0], unpack_vals[1], unpack_vals[2], unpack_vals[3]);

    /* ---- Run counting ---- */
    input[0]=1; input[1]=1; input[2]=2; input[3]=2; input[4]=2; input[5]=1;
    printf("runs=%d ", count_runs(input, 6));      /* 3 */
    printf("lr=%d ", longest_run(input, 6));       /* 3 */

    /* ---- Dictionary encoding ---- */
    dict_init();
    dict_add(100, 0);
    dict_add(200, 1);
    dict_add(300, 2);
    input[0]=100; input[1]=200; input[2]=300; input[3]=100;
    dict_encode(input, 4, encoded);
    printf("dc0=%d dc1=%d dc2=%d dc3=%d ", encoded[0], encoded[1], encoded[2], encoded[3]);
    dict_decode(encoded, 4, decoded);
    printf("ddc=%d", arrays_equal(input, decoded, 4)); /* 1 */

    printf("\n");
    return 0;
}
