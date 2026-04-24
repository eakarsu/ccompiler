int printf(const char *fmt, ...);
// EXPECT: Traffic light FSM:\ns=1 s=2 s=0 s=0 s=0 s=1 s=1 s=2 s=1 s=1 s=2 s=2 s=0 s=0 s=0 s=1 s=2 s=0 s=0 s=0 \nParity detector:\n1101110010111001\nfinal parity=1\nCounter FSM:\ncounter=8
int main(void) {
    int state = 0;
    int input[20];
    input[0] = 1; input[1] = 2; input[2] = 1; input[3] = 3;
    input[4] = 2; input[5] = 1; input[6] = 1; input[7] = 2;
    input[8] = 3; input[9] = 1; input[10] = 2; input[11] = 2;
    input[12] = 1; input[13] = 3; input[14] = 3; input[15] = 1;
    input[16] = 2; input[17] = 1; input[18] = 3; input[19] = 2;
    int i;
    int output;

    printf("Traffic light FSM:\n");
    state = 0;
    for (i = 0; i < 20; i++) {
        switch (state) {
            case 0:
                if (input[i] == 1) {
                    state = 1;
                    output = 10;
                } else {
                    output = 11;
                }
                break;
            case 1:
                if (input[i] == 2) {
                    state = 2;
                    output = 20;
                } else if (input[i] == 3) {
                    state = 0;
                    output = 21;
                } else {
                    output = 22;
                }
                break;
            case 2:
                if (input[i] == 1) {
                    state = 0;
                    output = 30;
                } else if (input[i] == 3) {
                    state = 1;
                    output = 31;
                } else {
                    output = 32;
                }
                break;
        }
        printf("s=%d ", state);
    }
    printf("\n");

    printf("Parity detector:\n");
    int bits[16];
    bits[0] = 1; bits[1] = 0; bits[2] = 1; bits[3] = 1;
    bits[4] = 0; bits[5] = 0; bits[6] = 1; bits[7] = 0;
    bits[8] = 1; bits[9] = 1; bits[10] = 1; bits[11] = 0;
    bits[12] = 0; bits[13] = 1; bits[14] = 0; bits[15] = 1;
    state = 0;
    for (i = 0; i < 16; i++) {
        switch (state) {
            case 0:
                if (bits[i] == 1) state = 1;
                break;
            case 1:
                if (bits[i] == 1) state = 0;
                break;
        }
        printf("%d", state);
    }
    printf("\nfinal parity=%d\n", state);

    printf("Counter FSM:\n");
    state = 0;
    int counter = 0;
    for (i = 0; i < 20; i++) {
        switch (state) {
            case 0:
                if (input[i] == 1) {
                    state = 1;
                    counter++;
                }
                break;
            case 1:
                if (input[i] == 2) {
                    state = 2;
                    counter++;
                } else {
                    state = 0;
                }
                break;
            case 2:
                if (input[i] == 3) {
                    state = 0;
                    counter = counter + 10;
                } else {
                    state = 0;
                }
                break;
        }
    }
    printf("counter=%d\n", counter);

    return 0;
}
