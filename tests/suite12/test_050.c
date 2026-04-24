int printf(const char *fmt, ...);

// Neural Network Forward Pass with integer weights
// 2-layer network: input(4) -> hidden(4) -> output(2)
// All weights and biases are integers, using scaled arithmetic

// Weights: input to hidden (4x4)
int w1[4][4];
// Biases: hidden layer
int b1[4];
// Weights: hidden to output (4x2)
int w2[4][2];
// Biases: output layer
int b2[2];

// Activations
int hidden[4];
int output[2];

// ReLU activation (integer)
int relu(int x) {
    if (x > 0) return x;
    return 0;
}

// Scaled sigmoid approximation (output 0-100)
// Using piecewise linear approximation
int sigmoid_100(int x) {
    if (x < -500) return 0;
    if (x < -200) return (x + 500) * 10 / 300;
    if (x < 200) return (x + 200) * 80 / 400 + 10;
    if (x < 500) return (x - 200) * 10 / 300 + 90;
    return 100;
}

void forward_pass(int *input) {
    // Hidden layer
    int i = 0;
    while (i < 4) {
        int sum = b1[i];
        int j = 0;
        while (j < 4) {
            sum = sum + input[j] * w1[j][i];
            j = j + 1;
        }
        hidden[i] = relu(sum);
        i = i + 1;
    }

    // Output layer
    i = 0;
    while (i < 2) {
        int sum = b2[i];
        int j = 0;
        while (j < 4) {
            sum = sum + hidden[j] * w2[j][i];
            j = j + 1;
        }
        output[i] = sum;
        i = i + 1;
    }
}

int argmax(int *arr, int n) {
    int best = 0;
    int i = 1;
    while (i < n) {
        if (arr[i] > arr[best]) best = i;
        i = i + 1;
    }
    return best;
}

// Softmax approximation: return percentage for class 0
int softmax_pct(int a, int b) {
    // Simple approximation: proportion based on difference
    int diff = a - b;
    return sigmoid_100(diff);
}

// Compute dot product
int dot_product(int *a, int *b, int n) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        sum = sum + a[i] * b[i];
        i = i + 1;
    }
    return sum;
}

// Matrix-vector multiply: mat[rows][cols] * vec[cols] -> out[rows]
void mat_vec_mul(int *mat, int rows, int cols, int *vec, int *result) {
    int i = 0;
    while (i < rows) {
        result[i] = 0;
        int j = 0;
        while (j < cols) {
            result[i] = result[i] + mat[i * cols + j] * vec[j];
            j = j + 1;
        }
        i = i + 1;
    }
}

int main() {
    // Initialize weights for an XOR-like problem
    // Input features: x1, x2, and their copies
    // Hidden layer detects patterns
    w1[0][0] = 10; w1[0][1] = -10; w1[0][2] = 5; w1[0][3] = 0;
    w1[1][0] = -10; w1[1][1] = 10; w1[1][2] = 0; w1[1][3] = 5;
    w1[2][0] = 10; w1[2][1] = -10; w1[2][2] = -5; w1[2][3] = 0;
    w1[3][0] = -10; w1[3][1] = 10; w1[3][2] = 0; w1[3][3] = -5;

    b1[0] = -5; b1[1] = -5; b1[2] = 0; b1[3] = 0;

    w2[0][0] = 10; w2[0][1] = -10;
    w2[1][0] = 10; w2[1][1] = -10;
    w2[2][0] = -5; w2[2][1] = 5;
    w2[3][0] = -5; w2[3][1] = 5;

    b2[0] = 0; b2[1] = 0;

    // Test ReLU
    printf("ReLU(5) = %d\n", relu(5));
    // EXPECT: ReLU(5) = 5
    printf("ReLU(-3) = %d\n", relu(-3));
    // EXPECT: ReLU(-3) = 0
    printf("ReLU(0) = %d\n", relu(0));
    // EXPECT: ReLU(0) = 0

    // Test sigmoid approximation
    printf("Sigmoid(0) = %d\n", sigmoid_100(0));
    // EXPECT: Sigmoid(0) = 50
    printf("Sigmoid(1000) = %d\n", sigmoid_100(1000));
    // EXPECT: Sigmoid(1000) = 100
    printf("Sigmoid(-1000) = %d\n", sigmoid_100(-1000));
    // EXPECT: Sigmoid(-1000) = 0

    // Forward pass: input (1, 0, 1, 0)
    int inp1[4];
    inp1[0] = 1; inp1[1] = 0; inp1[2] = 1; inp1[3] = 0;
    forward_pass(inp1);
    printf("Input (1,0,1,0):\n");
    // EXPECT: Input (1,0,1,0):
    printf("  Hidden: %d %d %d %d\n", hidden[0], hidden[1], hidden[2], hidden[3]);
    // EXPECT:   Hidden: 15 0 0 0
    printf("  Output: %d %d\n", output[0], output[1]);
    // EXPECT:   Output: 150 -150
    printf("  Class: %d\n", argmax(output, 2));
    // EXPECT:   Class: 0

    // Forward pass: input (0, 1, 0, 1)
    int inp2[4];
    inp2[0] = 0; inp2[1] = 1; inp2[2] = 0; inp2[3] = 1;
    forward_pass(inp2);
    printf("Input (0,1,0,1):\n");
    // EXPECT: Input (0,1,0,1):
    printf("  Hidden: %d %d %d %d\n", hidden[0], hidden[1], hidden[2], hidden[3]);
    // EXPECT:   Hidden: 0 15 0 0
    printf("  Output: %d %d\n", output[0], output[1]);
    // EXPECT:   Output: 150 -150
    printf("  Class: %d\n", argmax(output, 2));
    // EXPECT:   Class: 0

    // Forward pass: input (1, 1, 1, 1)
    int inp3[4];
    inp3[0] = 1; inp3[1] = 1; inp3[2] = 1; inp3[3] = 1;
    forward_pass(inp3);
    printf("Input (1,1,1,1):\n");
    // EXPECT: Input (1,1,1,1):
    printf("  Hidden: %d %d %d %d\n", hidden[0], hidden[1], hidden[2], hidden[3]);
    // EXPECT:   Hidden: 0 0 0 0
    printf("  Output: %d %d\n", output[0], output[1]);
    // EXPECT:   Output: 0 0
    printf("  Class: %d\n", argmax(output, 2));
    // EXPECT:   Class: 0

    // Forward pass: larger input
    int inp4[4];
    inp4[0] = 10; inp4[1] = 0; inp4[2] = 10; inp4[3] = 0;
    forward_pass(inp4);
    printf("Input (10,0,10,0):\n");
    // EXPECT: Input (10,0,10,0):
    printf("  Hidden: %d %d %d %d\n", hidden[0], hidden[1], hidden[2], hidden[3]);
    // EXPECT:   Hidden: 195 0 0 0
    printf("  Output: %d %d\n", output[0], output[1]);
    // EXPECT:   Output: 1950 -1950
    printf("  Class: %d\n", argmax(output, 2));
    // EXPECT:   Class: 0

    // Test dot product
    int v1[4];
    int v2[4];
    v1[0] = 1; v1[1] = 2; v1[2] = 3; v1[3] = 4;
    v2[0] = 5; v2[1] = 6; v2[2] = 7; v2[3] = 8;
    printf("Dot product: %d\n", dot_product(v1, v2, 4));
    // EXPECT: Dot product: 70

    // Test matrix-vector multiplication
    int mat[6];  // 2x3 matrix
    mat[0] = 1; mat[1] = 2; mat[2] = 3;
    mat[3] = 4; mat[4] = 5; mat[5] = 6;
    int vec[3];
    vec[0] = 1; vec[1] = 1; vec[2] = 1;
    int result[2];
    mat_vec_mul(mat, 2, 3, vec, result);
    printf("Mat-vec: %d %d\n", result[0], result[1]);
    // EXPECT: Mat-vec: 6 15

    // Softmax percentage
    printf("Softmax(100,0): %d\n", softmax_pct(100, 0));
    // EXPECT: Softmax(100,0): 70
    printf("Softmax(0,100): %d\n", softmax_pct(0, 100));
    // EXPECT: Softmax(0,100): 30

    printf("Neural network done\n");
    // EXPECT: Neural network done

    return 0;
}
