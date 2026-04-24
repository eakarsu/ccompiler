int printf(const char *fmt, ...);

int accumulate_fall(int start) {
    int total = 0;
    switch (start) {
        case 1:
            total = total + 1;
        case 2:
            total = total + 2;
        case 3:
            total = total + 4;
        case 4:
            total = total + 8;
            break;
        case 5:
            total = total + 16;
        case 6:
            total = total + 32;
            break;
        case 7:
            total = total + 64;
        default:
            total = total + 128;
            break;
    }
    return total;
}

int state_machine(int input) {
    int state = 0;
    int output = 0;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        int val = (input >> (i * 2)) & 3;
        switch (state) {
            case 0:
                switch (val) {
                    case 0: state = 0; break;
                    case 1: state = 1; output = output + 1; break;
                    case 2: state = 2; output = output + 10; break;
                    default: state = 3; output = output + 100; break;
                }
                break;
            case 1:
                switch (val) {
                    case 0:
                    case 1:
                        state = 0;
                        output = output + 2;
                        break;
                    case 2:
                        state = 2;
                        output = output + 20;
                    case 3:
                        output = output + 5;
                        break;
                }
                break;
            case 2:
                output = output + 50;
                state = 0;
                break;
            default:
                output = output + 200;
                state = 0;
                break;
        }
    }
    return output;
}

int main() {
    // EXPECT: fall from 1: 15
    printf("fall from 1: %d\n", accumulate_fall(1));
    // EXPECT: fall from 2: 14
    printf("fall from 2: %d\n", accumulate_fall(2));
    // EXPECT: fall from 3: 12
    printf("fall from 3: %d\n", accumulate_fall(3));
    // EXPECT: fall from 4: 8
    printf("fall from 4: %d\n", accumulate_fall(4));
    // EXPECT: fall from 5: 48
    printf("fall from 5: %d\n", accumulate_fall(5));
    // EXPECT: fall from 6: 32
    printf("fall from 6: %d\n", accumulate_fall(6));
    // EXPECT: fall from 7: 192
    printf("fall from 7: %d\n", accumulate_fall(7));
    // EXPECT: fall from 99: 128
    printf("fall from 99: %d\n", accumulate_fall(99));

    // EXPECT: sm 0: 0
    printf("sm 0: %d\n", state_machine(0));
    // EXPECT: sm 5: 3
    printf("sm 5: %d\n", state_machine(5));
    // EXPECT: sm 6: 60
    printf("sm 6: %d\n", state_machine(6));
    // EXPECT: sm 9: 76
    printf("sm 9: %d\n", state_machine(9));

    int count = 0;
    int val = 3;
    switch (val) {
        case 1: count = count + 1;
        case 2: count = count + 1;
        case 3: count = count + 1;
        case 4: count = count + 1;
        case 5: count = count + 1;
    }
    // EXPECT: chain fall from 3: 3
    printf("chain fall from 3: %d\n", count);

    return 0;
}
