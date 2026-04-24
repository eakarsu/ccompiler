int printf(const char *fmt, ...);

/* Moore machine: output depends only on the current state.
   This Moore machine classifies binary strings by counting consecutive 1s.
   States:
     S0: zero consecutive 1s (output: 0 = "idle")
     S1: one consecutive 1 (output: 1 = "started")
     S2: two consecutive 1s (output: 2 = "building")
     S3: three+ consecutive 1s (output: 3 = "burst")
   Transitions:
     S0 + 0 -> S0,  S0 + 1 -> S1
     S1 + 0 -> S0,  S1 + 1 -> S2
     S2 + 0 -> S0,  S2 + 1 -> S3
     S3 + 0 -> S0,  S3 + 1 -> S3
*/

int moore_next[4][2];
int moore_output[4];

void init_moore(void) {
    moore_next[0][0] = 0;  moore_next[0][1] = 1;
    moore_next[1][0] = 0;  moore_next[1][1] = 2;
    moore_next[2][0] = 0;  moore_next[2][1] = 3;
    moore_next[3][0] = 0;  moore_next[3][1] = 3;

    moore_output[0] = 0;  /* idle */
    moore_output[1] = 1;  /* started */
    moore_output[2] = 2;  /* building */
    moore_output[3] = 3;  /* burst */
}

int moore_step(int state, int input) {
    return moore_next[state][input];
}

int moore_get_output(int state) {
    return moore_output[state];
}

void moore_run(int *bits, int len, int *outputs) {
    int state = 0;
    int i;
    for (i = 0; i < len; i++) {
        state = moore_step(state, bits[i]);
        outputs[i] = moore_get_output(state);
    }
}

int max_burst_length(int *bits, int len) {
    int state = 0;
    int max_len = 0;
    int cur_len = 0;
    int i;
    for (i = 0; i < len; i++) {
        state = moore_step(state, bits[i]);
        if (moore_get_output(state) >= 1) {
            cur_len++;
            if (cur_len > max_len) {
                max_len = cur_len;
            }
        } else {
            cur_len = 0;
        }
    }
    return max_len;
}

int count_bursts(int *bits, int len) {
    /* Count transitions from output<3 to output==3 (entering burst mode) */
    int state = 0;
    int prev_out = 0;
    int count = 0;
    int cur_out;
    int i;
    for (i = 0; i < len; i++) {
        state = moore_step(state, bits[i]);
        cur_out = moore_get_output(state);
        if (cur_out == 3 && prev_out != 3) {
            count++;
        }
        prev_out = cur_out;
    }
    return count;
}

void test_basic_outputs(void) {
    int b1[6];
    int out1[6];
    int b2[4];
    int out2[4];

    printf("=== Moore Basic Outputs ===\n");
    // EXPECT: === Moore Basic Outputs ===

    /* "011100" */
    b1[0]=0; b1[1]=1; b1[2]=1; b1[3]=1; b1[4]=0; b1[5]=0;
    moore_run(b1, 6, out1);
    printf("011100 outputs: %d %d %d %d %d %d\n",
           out1[0], out1[1], out1[2], out1[3], out1[4], out1[5]);
    // EXPECT: 011100 outputs: 0 1 2 3 0 0

    /* "1111" */
    b2[0]=1; b2[1]=1; b2[2]=1; b2[3]=1;
    moore_run(b2, 4, out2);
    printf("1111 outputs: %d %d %d %d\n",
           out2[0], out2[1], out2[2], out2[3]);
    // EXPECT: 1111 outputs: 1 2 3 3
}

void test_alternating(void) {
    int b[8];
    int out[8];

    printf("=== Moore Alternating ===\n");
    // EXPECT: === Moore Alternating ===

    /* "10101010" - never gets past started */
    b[0]=1; b[1]=0; b[2]=1; b[3]=0;
    b[4]=1; b[5]=0; b[6]=1; b[7]=0;
    moore_run(b, 8, out);
    printf("10101010: %d %d %d %d %d %d %d %d\n",
           out[0], out[1], out[2], out[3],
           out[4], out[5], out[6], out[7]);
    // EXPECT: 10101010: 1 0 1 0 1 0 1 0
}

void test_max_burst(void) {
    int b1[8];
    int b2[10];
    int b3[5];
    int m;

    printf("=== Max Burst Length ===\n");
    // EXPECT: === Max Burst Length ===

    /* "01110110" -> max burst = 3 */
    b1[0]=0; b1[1]=1; b1[2]=1; b1[3]=1;
    b1[4]=0; b1[5]=1; b1[6]=1; b1[7]=0;
    m = max_burst_length(b1, 8);
    printf("01110110 max: %d\n", m);
    // EXPECT: 01110110 max: 3

    /* "0111101111" -> max burst = 4 */
    b2[0]=0; b2[1]=1; b2[2]=1; b2[3]=1; b2[4]=1;
    b2[5]=0; b2[6]=1; b2[7]=1; b2[8]=1; b2[9]=1;
    m = max_burst_length(b2, 10);
    printf("0111101111 max: %d\n", m);
    // EXPECT: 0111101111 max: 4

    /* "00000" -> max burst = 0 */
    b3[0]=0; b3[1]=0; b3[2]=0; b3[3]=0; b3[4]=0;
    m = max_burst_length(b3, 5);
    printf("00000 max: %d\n", m);
    // EXPECT: 00000 max: 0
}

void test_burst_count(void) {
    int b1[10];
    int b2[8];
    int b3[6];
    int c;

    printf("=== Burst Count ===\n");
    // EXPECT: === Burst Count ===

    /* "0111011100" -> 2 bursts (reaching state 3) */
    b1[0]=0; b1[1]=1; b1[2]=1; b1[3]=1; b1[4]=0;
    b1[5]=1; b1[6]=1; b1[7]=1; b1[8]=0; b1[9]=0;
    c = count_bursts(b1, 10);
    printf("0111011100 bursts: %d\n", c);
    // EXPECT: 0111011100 bursts: 2

    /* "11111111" -> 1 burst (stays in burst) */
    b2[0]=1; b2[1]=1; b2[2]=1; b2[3]=1;
    b2[4]=1; b2[5]=1; b2[6]=1; b2[7]=1;
    c = count_bursts(b2, 8);
    printf("11111111 bursts: %d\n", c);
    // EXPECT: 11111111 bursts: 1

    /* "110110" -> 0 bursts (never reaches 3 consecutive) */
    b3[0]=1; b3[1]=1; b3[2]=0; b3[3]=1; b3[4]=1; b3[5]=0;
    c = count_bursts(b3, 6);
    printf("110110 bursts: %d\n", c);
    // EXPECT: 110110 bursts: 0
}

void test_state_trace(void) {
    int b[7];
    int state;
    int i;

    printf("=== State Trace ===\n");
    // EXPECT: === State Trace ===

    /* Trace "0111010" */
    b[0]=0; b[1]=1; b[2]=1; b[3]=1; b[4]=0; b[5]=1; b[6]=0;
    state = 0;
    printf("start: S%d\n", state);
    // EXPECT: start: S0
    for (i = 0; i < 7; i++) {
        int prev = state;
        state = moore_step(state, b[i]);
        printf("S%d +%d -> S%d (out=%d)\n", prev, b[i], state, moore_get_output(state));
    }
    // EXPECT: S0 +0 -> S0 (out=0)
    // EXPECT: S0 +1 -> S1 (out=1)
    // EXPECT: S1 +1 -> S2 (out=2)
    // EXPECT: S2 +1 -> S3 (out=3)
    // EXPECT: S3 +0 -> S0 (out=0)
    // EXPECT: S0 +1 -> S1 (out=1)
    // EXPECT: S1 +0 -> S0 (out=0)
}

int main(void) {
    init_moore();
    test_basic_outputs();
    test_alternating();
    test_max_burst();
    test_burst_count();
    test_state_trace();

    printf("Moore machine done.\n");
    // EXPECT: Moore machine done.
    return 0;
}
