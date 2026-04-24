int printf(const char *fmt, ...);
// EXPECT: === Neural Network Simulation ===\nNetwork: 2 -> 3 -> 2 -> 1\nInput(0,0) -> raw=5 sig=50 class=0\nInput(100,0) -> raw=25 sig=53 class=1\nInput(0,100) -> raw=0 sig=50 class=0\nInput(100,100) -> raw=33 sig=54 class=1\nInput(50,50) -> raw=20 sig=52 class=1\nInput(200,0) -> raw=45 sig=55 class=1\nInput(0,200) -> raw=0 sig=50 class=0\nInput(150,150) -> raw=46 sig=55 class=1\nActivation tests:\n  relu(-200)=0 sig(-200)=25\n  relu(-100)=0 sig(-100)=38\n  relu(-50)=0 sig(-50)=44\n  relu(0)=0 sig(0)=50\n  relu(50)=50 sig(50)=56\n  relu(100)=100 sig(100)=62\n  relu(200)=200 sig(200)=75

struct Neuron {
    int weights[4];
    int bias;
    int output;
    int num_inputs;
};

struct Layer {
    struct Neuron neurons[4];
    int size;
};

struct Network {
    struct Layer layers[3];
    int num_layers;
};

int relu(int x) {
    return x > 0 ? x : 0;
}

int sigmoid_approx(int x) {
    if (x < -400) return 0;
    if (x > 400) return 100;
    return 50 + x / 8;
}

void init_neuron(struct Neuron *n, int num_inputs) {
    n->num_inputs = num_inputs;
    n->bias = 0;
    n->output = 0;
    int i;
    for (i = 0; i < 4; i++) n->weights[i] = 0;
}

int forward_neuron(struct Neuron *n, int inputs[]) {
    int sum = n->bias;
    int i;
    for (i = 0; i < n->num_inputs; i++) {
        sum = sum + (n->weights[i] * inputs[i]) / 100;
    }
    n->output = relu(sum);
    return n->output;
}

void forward_layer(struct Layer *layer, int inputs[], int outputs[]) {
    int i;
    for (i = 0; i < layer->size; i++) {
        outputs[i] = forward_neuron(&layer->neurons[i], inputs);
    }
}

void init_network(struct Network *net) {
    net->num_layers = 3;

    net->layers[0].size = 3;
    int i;
    for (i = 0; i < 3; i++) {
        init_neuron(&net->layers[0].neurons[i], 2);
    }
    net->layers[0].neurons[0].weights[0] = 50;
    net->layers[0].neurons[0].weights[1] = 30;
    net->layers[0].neurons[0].bias = 10;
    net->layers[0].neurons[1].weights[0] = -40;
    net->layers[0].neurons[1].weights[1] = 60;
    net->layers[0].neurons[1].bias = -5;
    net->layers[0].neurons[2].weights[0] = 20;
    net->layers[0].neurons[2].weights[1] = 20;
    net->layers[0].neurons[2].bias = 15;

    net->layers[1].size = 2;
    for (i = 0; i < 2; i++) {
        init_neuron(&net->layers[1].neurons[i], 3);
    }
    net->layers[1].neurons[0].weights[0] = 40;
    net->layers[1].neurons[0].weights[1] = -30;
    net->layers[1].neurons[0].weights[2] = 50;
    net->layers[1].neurons[0].bias = 0;
    net->layers[1].neurons[1].weights[0] = -20;
    net->layers[1].neurons[1].weights[1] = 40;
    net->layers[1].neurons[1].weights[2] = 30;
    net->layers[1].neurons[1].bias = 10;

    net->layers[2].size = 1;
    init_neuron(&net->layers[2].neurons[0], 2);
    net->layers[2].neurons[0].weights[0] = 60;
    net->layers[2].neurons[0].weights[1] = -50;
    net->layers[2].neurons[0].bias = 5;
}

int predict(struct Network *net, int input0, int input1) {
    int inputs[4];
    int hidden1[4];
    int hidden2[4];
    int output[4];

    inputs[0] = input0;
    inputs[1] = input1;

    forward_layer(&net->layers[0], inputs, hidden1);
    forward_layer(&net->layers[1], hidden1, hidden2);
    forward_layer(&net->layers[2], hidden2, output);

    return output[0];
}

int main(void) {
    struct Network net;
    init_network(&net);

    printf("=== Neural Network Simulation ===\n");
    printf("Network: 2 -> 3 -> 2 -> 1\n");

    int test_inputs[8][2];
    test_inputs[0][0] = 0;   test_inputs[0][1] = 0;
    test_inputs[1][0] = 100; test_inputs[1][1] = 0;
    test_inputs[2][0] = 0;   test_inputs[2][1] = 100;
    test_inputs[3][0] = 100; test_inputs[3][1] = 100;
    test_inputs[4][0] = 50;  test_inputs[4][1] = 50;
    test_inputs[5][0] = 200; test_inputs[5][1] = 0;
    test_inputs[6][0] = 0;   test_inputs[6][1] = 200;
    test_inputs[7][0] = 150; test_inputs[7][1] = 150;

    int i;
    for (i = 0; i < 8; i++) {
        int out = predict(&net, test_inputs[i][0], test_inputs[i][1]);
        int sig = sigmoid_approx(out);
        printf("Input(%d,%d) -> raw=%d sig=%d class=%d\n",
               test_inputs[i][0], test_inputs[i][1],
               out, sig, sig > 50 ? 1 : 0);
    }

    printf("Activation tests:\n");
    int vals[7];
    vals[0] = -200; vals[1] = -100; vals[2] = -50;
    vals[3] = 0;    vals[4] = 50;   vals[5] = 100; vals[6] = 200;
    for (i = 0; i < 7; i++) {
        printf("  relu(%d)=%d sig(%d)=%d\n",
               vals[i], relu(vals[i]), vals[i], sigmoid_approx(vals[i]));
    }

    return 0;
}
