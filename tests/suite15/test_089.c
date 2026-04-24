int printf(const char *fmt, ...);

/* Neural network forward pass with integer weights
 * Network: 3 inputs -> 4 hidden (ReLU) -> 2 outputs
 * All weights and values scaled by 100.
 * Activation: ReLU (max(0, x))
 */

typedef struct {
    int w_ih[12];  /* input-to-hidden weights: 3x4 = 12 */
    int b_h[4];    /* hidden biases */
    int w_ho[8];   /* hidden-to-output weights: 4x2 = 8 */
    int b_o[2];    /* output biases */
} Network;

/* ReLU activation (scaled values) */
int relu(int x) {
    if (x > 0) return x;
    return 0;
}

/* Clamp to [-10000, 10000] to prevent overflow */
int clamp(int x) {
    if (x > 10000) return 10000;
    if (x < -10000) return -10000;
    return x;
}

/* Forward pass: input[3] -> output[2] */
void forward(Network *net, int *input, int *output) {
    int hidden[4];
    int i;
    int j;
    int sum;

    /* Input to hidden layer */
    i = 0;
    while (i < 4) {
        sum = net->b_h[i];
        j = 0;
        while (j < 3) {
            sum = sum + input[j] * net->w_ih[j * 4 + i] / 100;
            j = j + 1;
        }
        hidden[i] = relu(clamp(sum));
        i = i + 1;
    }

    /* Hidden to output layer (no activation) */
    i = 0;
    while (i < 2) {
        sum = net->b_o[i];
        j = 0;
        while (j < 4) {
            sum = sum + hidden[j] * net->w_ho[j * 2 + i] / 100;
            j = j + 1;
        }
        output[i] = clamp(sum);
        i = i + 1;
    }
}

/* Argmax of array */
int argmax(int *arr, int n) {
    int best;
    int best_i;
    int i;
    best = arr[0];
    best_i = 0;
    i = 1;
    while (i < n) {
        if (arr[i] > best) {
            best = arr[i];
            best_i = i;
        }
        i = i + 1;
    }
    return best_i;
}

/* Initialize network with hand-picked weights */
void init_network(Network *net) {
    /* Input->Hidden weights (3x4) row-major: w_ih[input*4 + hidden] */
    net->w_ih[0] = 150;   /* i0->h0 */
    net->w_ih[1] = -50;   /* i0->h1 */
    net->w_ih[2] = 100;   /* i0->h2 */
    net->w_ih[3] = -100;  /* i0->h3 */

    net->w_ih[4] = -50;   /* i1->h0 */
    net->w_ih[5] = 200;   /* i1->h1 */
    net->w_ih[6] = 50;    /* i1->h2 */
    net->w_ih[7] = 100;   /* i1->h3 */

    net->w_ih[8] = 50;    /* i2->h0 */
    net->w_ih[9] = 50;    /* i2->h1 */
    net->w_ih[10] = -150; /* i2->h2 */
    net->w_ih[11] = 200;  /* i2->h3 */

    net->b_h[0] = -10;
    net->b_h[1] = -20;
    net->b_h[2] = 10;
    net->b_h[3] = -30;

    /* Hidden->Output weights (4x2) row-major: w_ho[hidden*2 + output] */
    net->w_ho[0] = 100;   /* h0->o0 */
    net->w_ho[1] = -100;  /* h0->o1 */
    net->w_ho[2] = -150;  /* h1->o0 */
    net->w_ho[3] = 200;   /* h1->o1 */
    net->w_ho[4] = 50;    /* h2->o0 */
    net->w_ho[5] = 50;    /* h2->o1 */
    net->w_ho[6] = 100;   /* h3->o0 */
    net->w_ho[7] = -50;   /* h3->o1 */

    net->b_o[0] = 5;
    net->b_o[1] = -5;
}

int main(void) {
    Network net;
    int input[3];
    int output[2];

    printf("Neural Network Forward Pass\n");
    // EXPECT: Neural Network Forward Pass

    init_network(&net);

    /* Test 1: Input [100, 0, 0] */
    input[0] = 100;
    input[1] = 0;
    input[2] = 0;
    forward(&net, input, output);
    printf("T1 out0: %d\n", output[0]);
    // EXPECT: T1 out0: 200
    printf("T1 out1: %d\n", output[1]);
    // EXPECT: T1 out1: -90
    printf("T1 class: %d\n", argmax(output, 2));
    // EXPECT: T1 class: 0

    /* Test 2: Input [0, 100, 0] */
    input[0] = 0;
    input[1] = 100;
    input[2] = 0;
    forward(&net, input, output);
    printf("T2 out0: %d\n", output[0]);
    // EXPECT: T2 out0: -165
    printf("T2 out1: %d\n", output[1]);
    // EXPECT: T2 out1: 350
    printf("T2 class: %d\n", argmax(output, 2));
    // EXPECT: T2 class: 1

    /* Test 3: Input [0, 0, 100] */
    input[0] = 0;
    input[1] = 0;
    input[2] = 100;
    forward(&net, input, output);
    printf("T3 out0: %d\n", output[0]);
    // EXPECT: T3 out0: 170
    printf("T3 out1: %d\n", output[1]);
    // EXPECT: T3 out1: -70
    printf("T3 class: %d\n", argmax(output, 2));
    // EXPECT: T3 class: 0

    /* Test 4: Mixed input [50, 50, 50] */
    input[0] = 50;
    input[1] = 50;
    input[2] = 50;
    forward(&net, input, output);
    printf("T4 out0: %d\n", output[0]);
    // EXPECT: T4 out0: 25
    printf("T4 out1: %d\n", output[1]);
    // EXPECT: T4 out1: 60

    /* Test 5: All zeros */
    input[0] = 0;
    input[1] = 0;
    input[2] = 0;
    forward(&net, input, output);
    printf("T5 out0: %d\n", output[0]);
    // EXPECT: T5 out0: 10
    printf("T5 out1: %d\n", output[1]);
    // EXPECT: T5 out1: 0

    /* Test 6: Large input [200, 200, 200] */
    input[0] = 200;
    input[1] = 200;
    input[2] = 200;
    forward(&net, input, output);
    printf("T6 out0: %d\n", output[0]);
    // EXPECT: T6 out0: 100
    printf("T6 out1: %d\n", output[1]);
    // EXPECT: T6 out1: 285

    /* Test 7: All negative inputs - ReLU zeroes hidden */
    input[0] = -100;
    input[1] = -100;
    input[2] = -100;
    forward(&net, input, output);
    printf("T7 out0: %d\n", output[0]);
    // EXPECT: T7 out0: 10
    printf("T7 out1: %d\n", output[1]);
    // EXPECT: T7 out1: 0

    /* Test 8: Asymmetric input */
    input[0] = 200;
    input[1] = 0;
    input[2] = 100;
    forward(&net, input, output);
    printf("T8 out0: %d\n", output[0]);
    // EXPECT: T8 out0: 375
    printf("T8 out1: %d\n", output[1]);
    // EXPECT: T8 out1: -315
    printf("T8 class: %d\n", argmax(output, 2));
    // EXPECT: T8 class: 0

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
