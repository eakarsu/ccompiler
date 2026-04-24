int printf(const char *fmt, ...);
// EXPECT: Accumulator state machine:\n  s=1 acc=5\n  s=0 acc=15\n  s=0 acc=0\n  s=1 acc=7\n  s=0 acc=14\n  s=0 acc=0\n  s=1 acc=4\n  s=0 acc=-4\n  s=1 acc=2\nemit: 2\n  s=0 acc=0\n  s=1 acc=1\n  s=0 acc=1\n  s=1 acc=2\n  s=0 acc=2\n  s=2 acc=2\nMode machine:\nsum=6\nsum=9\nsum=30\nCounter machine:\ncounts: 4 4 4 4
int main(void) {
    int state = 0;
    int acc = 0;
    int input[15];
    input[0] = 5; input[1] = 3; input[2] = -1;
    input[3] = 7; input[4] = 2; input[5] = -1;
    input[6] = 4; input[7] = -2; input[8] = 6;
    input[9] = -1; input[10] = 1; input[11] = 1;
    input[12] = 1; input[13] = 1; input[14] = -2;

    int i;
    printf("Accumulator state machine:\n");
    for (i = 0; i < 15; i++) {
        int cmd = input[i];
        switch (state) {
            case 0:
                if (cmd >= 0) {
                    acc = acc + cmd;
                    state = 1;
                } else if (cmd == -1) {
                    acc = 0;
                    state = 0;
                } else {
                    state = 2;
                }
                break;
            case 1:
                if (cmd >= 0) {
                    acc = acc * cmd;
                    state = 0;
                } else if (cmd == -1) {
                    printf("emit: %d\n", acc);
                    acc = 0;
                    state = 0;
                } else {
                    acc = -acc;
                    state = 0;
                }
                break;
            case 2:
                if (cmd >= 0) {
                    acc = cmd;
                    state = 0;
                } else {
                    acc = 0;
                    state = 0;
                }
                break;
        }
        printf("  s=%d acc=%d\n", state, acc);
    }

    printf("Mode machine:\n");
    int mode = 0;
    int total = 0;
    int data[12];
    data[0] = 1; data[1] = 2; data[2] = 3; data[3] = 0;
    data[4] = 4; data[5] = 5; data[6] = 0;
    data[7] = 6; data[8] = 7; data[9] = 8; data[10] = 9; data[11] = 0;
    for (i = 0; i < 12; i++) {
        switch (mode) {
            case 0:
                if (data[i] == 0) {
                    printf("sum=%d\n", total);
                    total = 0;
                } else {
                    total = total + data[i];
                    mode = 1;
                }
                break;
            case 1:
                if (data[i] == 0) {
                    printf("sum=%d\n", total);
                    total = 0;
                    mode = 0;
                } else {
                    total = total + data[i];
                    mode = 0;
                }
                break;
        }
    }

    printf("Counter machine:\n");
    int counts[4];
    counts[0] = 0; counts[1] = 0; counts[2] = 0; counts[3] = 0;
    int events[16];
    events[0] = 0; events[1] = 1; events[2] = 2; events[3] = 3;
    events[4] = 0; events[5] = 0; events[6] = 1; events[7] = 2;
    events[8] = 3; events[9] = 3; events[10] = 3; events[11] = 2;
    events[12] = 1; events[13] = 0; events[14] = 1; events[15] = 2;
    for (i = 0; i < 16; i++) {
        switch (events[i]) {
            case 0: counts[0]++; break;
            case 1: counts[1]++; break;
            case 2: counts[2]++; break;
            case 3: counts[3]++; break;
        }
    }
    printf("counts: %d %d %d %d\n", counts[0], counts[1], counts[2], counts[3]);

    return 0;
}
