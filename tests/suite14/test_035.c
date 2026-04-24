int printf(const char *fmt, ...);

/* Arithmetic coding (integer-based, simplified) */
/* Uses range [0, RANGE) for encoding */

struct symbol_freq {
    int symbol;
    int freq;
    int cum_low;   /* cumulative lower bound */
    int cum_high;  /* cumulative upper bound */
};

struct arith_model {
    struct symbol_freq syms[8];
    int nsyms;
    int total_freq;
};

void model_init(struct arith_model *m, int *symbols, int *freqs, int n) {
    m->nsyms = n;
    m->total_freq = 0;
    int i;
    for (i = 0; i < n; i++) {
        m->syms[i].symbol = symbols[i];
        m->syms[i].freq = freqs[i];
        m->syms[i].cum_low = m->total_freq;
        m->total_freq = m->total_freq + freqs[i];
        m->syms[i].cum_high = m->total_freq;
    }
}

int model_sym_index(struct arith_model *m, int symbol) {
    int i;
    for (i = 0; i < m->nsyms; i++) {
        if (m->syms[i].symbol == symbol) return i;
    }
    return -1;
}

int model_find_by_cum(struct arith_model *m, int value) {
    int i;
    for (i = 0; i < m->nsyms; i++) {
        if (value >= m->syms[i].cum_low && value < m->syms[i].cum_high) {
            return i;
        }
    }
    return -1;
}

/* Encode: narrow [low, high) for each symbol */
void arith_encode(struct arith_model *m, int *data, int len, int *r_low, int *r_high) {
    int low = 0;
    int high = 10000;
    int i;
    for (i = 0; i < len; i++) {
        int idx = model_sym_index(m, data[i]);
        if (idx < 0) { *r_low = -1; *r_high = -1; return; }
        int range = high - low;
        int new_low = low + (range * m->syms[idx].cum_low) / m->total_freq;
        int new_high = low + (range * m->syms[idx].cum_high) / m->total_freq;
        low = new_low;
        high = new_high;
    }
    *r_low = low;
    *r_high = high;
}

/* Decode: given a value in [low, high), recover symbols */
int arith_decode(struct arith_model *m, int encoded, int msg_len, int *out) {
    int low = 0;
    int high = 10000;
    int i;
    for (i = 0; i < msg_len; i++) {
        int range = high - low;
        if (range <= 0) return i;
        int scaled = ((encoded - low) * m->total_freq) / range;
        int idx = model_find_by_cum(m, scaled);
        if (idx < 0) return i;
        out[i] = m->syms[idx].symbol;
        int new_low = low + (range * m->syms[idx].cum_low) / m->total_freq;
        int new_high = low + (range * m->syms[idx].cum_high) / m->total_freq;
        low = new_low;
        high = new_high;
    }
    return msg_len;
}

/* Approximate log2 */
int approx_log2(int n) {
    if (n <= 0) return 0;
    int log = 0;
    int val = n;
    while (val > 1) {
        val = val >> 1;
        log = log + 1;
    }
    return log;
}

/* Entropy approximation */
int entropy_approx(int *freqs, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) total = total + freqs[i];
    if (total == 0) return 0;
    int sum = 0;
    for (i = 0; i < n; i++) {
        if (freqs[i] > 0) {
            int ratio = total / freqs[i];
            sum = sum + freqs[i] * approx_log2(ratio);
        }
    }
    return sum;
}

int main(void) {
    /* Use a model where total_freq evenly divides our range */
    /* Model: A=1(freq=2), B=2(freq=3), C=3(freq=5), total=10 */
    /* Range = 10000, so each unit = 1000 */
    int symbols[3];
    symbols[0] = 1; symbols[1] = 2; symbols[2] = 3;
    int freqs[3];
    freqs[0] = 2; freqs[1] = 3; freqs[2] = 5;

    struct arith_model model;
    model_init(&model, symbols, freqs, 3);

    printf("total freq: %d\n", model.total_freq);
    // EXPECT: total freq: 10
    printf("sym0: lo=%d hi=%d\n", model.syms[0].cum_low, model.syms[0].cum_high);
    // EXPECT: sym0: lo=0 hi=2
    printf("sym1: lo=%d hi=%d\n", model.syms[1].cum_low, model.syms[1].cum_high);
    // EXPECT: sym1: lo=2 hi=5
    printf("sym2: lo=%d hi=%d\n", model.syms[2].cum_low, model.syms[2].cum_high);
    // EXPECT: sym2: lo=5 hi=10

    /* Find by cumulative value */
    printf("find 0: %d\n", model_find_by_cum(&model, 0));
    // EXPECT: find 0: 0
    printf("find 1: %d\n", model_find_by_cum(&model, 1));
    // EXPECT: find 1: 0
    printf("find 2: %d\n", model_find_by_cum(&model, 2));
    // EXPECT: find 2: 1
    printf("find 5: %d\n", model_find_by_cum(&model, 5));
    // EXPECT: find 5: 2
    printf("find 9: %d\n", model_find_by_cum(&model, 9));
    // EXPECT: find 9: 2

    /* Encode single A: range 10000, sym A cum [0,2) */
    /* low = 0 + 10000*0/10 = 0, high = 0 + 10000*2/10 = 2000 */
    int msg_a[1];
    msg_a[0] = 1;
    int lo_a;
    int hi_a;
    arith_encode(&model, msg_a, 1, &lo_a, &hi_a);
    printf("enc A: low=%d high=%d\n", lo_a, hi_a);
    // EXPECT: enc A: low=0 high=2000

    /* Decode: use midpoint of range */
    int dec_a[2];
    int dl_a = arith_decode(&model, 1000, 1, dec_a);
    printf("dec A: sym=%d len=%d\n", dec_a[0], dl_a);
    // EXPECT: dec A: sym=1 len=1

    /* Encode single B: cum [2,5) */
    /* low = 10000*2/10 = 2000, high = 10000*5/10 = 5000 */
    int msg_b[1];
    msg_b[0] = 2;
    int lo_b;
    int hi_b;
    arith_encode(&model, msg_b, 1, &lo_b, &hi_b);
    printf("enc B: low=%d high=%d\n", lo_b, hi_b);
    // EXPECT: enc B: low=2000 high=5000

    int dec_b[2];
    int dl_b = arith_decode(&model, 3500, 1, dec_b);
    printf("dec B: sym=%d len=%d\n", dec_b[0], dl_b);
    // EXPECT: dec B: sym=2 len=1

    /* Encode single C: cum [5,10) */
    /* low = 10000*5/10 = 5000, high = 10000*10/10 = 10000 */
    int msg_c[1];
    msg_c[0] = 3;
    int lo_c;
    int hi_c;
    arith_encode(&model, msg_c, 1, &lo_c, &hi_c);
    printf("enc C: low=%d high=%d\n", lo_c, hi_c);
    // EXPECT: enc C: low=5000 high=10000

    int dec_c[2];
    int dl_c = arith_decode(&model, 7500, 1, dec_c);
    printf("dec C: sym=%d len=%d\n", dec_c[0], dl_c);
    // EXPECT: dec C: sym=3 len=1

    /* Encode "AB" = [1, 2] */
    /* Step 1 (A): low=0, high=2000 */
    /* Step 2 (B): range=2000, new_low=0+2000*2/10=400, new_high=0+2000*5/10=1000 */
    int msg_ab[2];
    msg_ab[0] = 1; msg_ab[1] = 2;
    int lo_ab;
    int hi_ab;
    arith_encode(&model, msg_ab, 2, &lo_ab, &hi_ab);
    printf("enc AB: low=%d high=%d\n", lo_ab, hi_ab);
    // EXPECT: enc AB: low=400 high=1000

    int dec_ab[4];
    int dl_ab = arith_decode(&model, 700, 2, dec_ab);
    printf("dec AB: %d %d len=%d\n", dec_ab[0], dec_ab[1], dl_ab);
    // EXPECT: dec AB: 1 2 len=2

    /* Encode "CC" = [3, 3] */
    /* Step 1 (C): low=5000, high=10000 */
    /* Step 2 (C): range=5000, new_low=5000+5000*5/10=7500, new_high=5000+5000*10/10=10000 */
    int msg_cc[2];
    msg_cc[0] = 3; msg_cc[1] = 3;
    int lo_cc;
    int hi_cc;
    arith_encode(&model, msg_cc, 2, &lo_cc, &hi_cc);
    printf("enc CC: low=%d high=%d\n", lo_cc, hi_cc);
    // EXPECT: enc CC: low=7500 high=10000

    int dec_cc[4];
    int dl_cc = arith_decode(&model, 8750, 2, dec_cc);
    printf("dec CC: %d %d len=%d\n", dec_cc[0], dec_cc[1], dl_cc);
    // EXPECT: dec CC: 3 3 len=2

    /* Encode "BA" = [2, 1] */
    /* Step 1 (B): low=2000, high=5000, range=3000 */
    /* Step 2 (A): new_low=2000+3000*0/10=2000, new_high=2000+3000*2/10=2600 */
    int msg_ba[2];
    msg_ba[0] = 2; msg_ba[1] = 1;
    int lo_ba;
    int hi_ba;
    arith_encode(&model, msg_ba, 2, &lo_ba, &hi_ba);
    printf("enc BA: low=%d high=%d\n", lo_ba, hi_ba);
    // EXPECT: enc BA: low=2000 high=2600

    int dec_ba[4];
    int dl_ba = arith_decode(&model, 2300, 2, dec_ba);
    printf("dec BA: %d %d len=%d\n", dec_ba[0], dec_ba[1], dl_ba);
    // EXPECT: dec BA: 2 1 len=2

    /* Log2 tests */
    printf("log2(1): %d\n", approx_log2(1));
    // EXPECT: log2(1): 0
    printf("log2(2): %d\n", approx_log2(2));
    // EXPECT: log2(2): 1
    printf("log2(8): %d\n", approx_log2(8));
    // EXPECT: log2(8): 3
    printf("log2(1024): %d\n", approx_log2(1024));
    // EXPECT: log2(1024): 10

    /* Entropy: uniform [1,1,1,1] -> 4*1*log2(4/1) = 4*2 = 8 */
    int uf[4];
    uf[0] = 1; uf[1] = 1; uf[2] = 1; uf[3] = 1;
    printf("uniform entropy: %d\n", entropy_approx(uf, 4));
    // EXPECT: uniform entropy: 8

    /* Entropy: skewed [7,1] -> 7*log2(8/7)+1*log2(8) = 7*0+1*3 = 3 */
    int sk[2];
    sk[0] = 7; sk[1] = 1;
    printf("skewed entropy: %d\n", entropy_approx(sk, 2));
    // EXPECT: skewed entropy: 3

    printf("done\n");
    // EXPECT: done
    return 0;
}
