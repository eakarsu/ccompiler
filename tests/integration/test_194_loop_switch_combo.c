int printf(const char *fmt, ...);
// EXPECT: Switch-driven iteration:\n(1,0) (1,1) (0,0) (1,0) (2,0) (1,-1) (1,0) (1,1) (2,1) (1,0) (0,-1) (0,0) \nLoop patterns by mode:\nmode 0: 0 1 2 3 4 \nmode 1: 4 3 2 1 0 \nmode 2: 0 1 4 9 16 \nmode 3: 100 50 33 25 20 \nWhile-switch processor:\npos=4 neg=4 zero=2\nMulti-pass processing:\npass 0: 4 2 8 6 \npass 1: 7 1 3 5 \npass 2: 7 8 6 5 
int main(void) {
    int i;
    int j;

    printf("Switch-driven iteration:\n");
    int commands[12];
    commands[0] = 1; commands[1] = 2; commands[2] = 3;
    commands[3] = 1; commands[4] = 1; commands[5] = 3;
    commands[6] = 2; commands[7] = 2; commands[8] = 1;
    commands[9] = 3; commands[10] = 3; commands[11] = 2;
    int x = 0;
    int y = 0;
    for (i = 0; i < 12; i++) {
        switch (commands[i]) {
            case 1: x++; break;
            case 2: y++; break;
            case 3: x--; y--; break;
        }
        printf("(%d,%d) ", x, y);
    }
    printf("\n");

    printf("Loop patterns by mode:\n");
    int modes[4];
    modes[0] = 0; modes[1] = 1; modes[2] = 2; modes[3] = 3;
    for (i = 0; i < 4; i++) {
        printf("mode %d: ", modes[i]);
        switch (modes[i]) {
            case 0:
                for (j = 0; j < 5; j++) printf("%d ", j);
                break;
            case 1:
                for (j = 4; j >= 0; j--) printf("%d ", j);
                break;
            case 2:
                for (j = 0; j < 5; j++) printf("%d ", j * j);
                break;
            case 3:
                for (j = 1; j <= 5; j++) printf("%d ", 100 / j);
                break;
        }
        printf("\n");
    }

    printf("While-switch processor:\n");
    int data[10];
    data[0] = 5; data[1] = -3; data[2] = 0; data[3] = 7; data[4] = -1;
    data[5] = 2; data[6] = -5; data[7] = 0; data[8] = 3; data[9] = -2;
    int pos_count = 0;
    int neg_count = 0;
    int zero_count = 0;
    i = 0;
    while (i < 10) {
        int category;
        if (data[i] > 0) category = 1;
        else if (data[i] < 0) category = 2;
        else category = 0;

        switch (category) {
            case 0: zero_count++; break;
            case 1: pos_count++; break;
            case 2: neg_count++; break;
        }
        i++;
    }
    printf("pos=%d neg=%d zero=%d\n", pos_count, neg_count, zero_count);

    printf("Multi-pass processing:\n");
    int arr[8];
    arr[0] = 4; arr[1] = 2; arr[2] = 7; arr[3] = 1;
    arr[4] = 8; arr[5] = 3; arr[6] = 6; arr[7] = 5;
    int pass;
    for (pass = 0; pass < 3; pass++) {
        printf("pass %d: ", pass);
        for (i = 0; i < 8; i++) {
            switch (pass) {
                case 0:
                    if (arr[i] % 2 == 0) printf("%d ", arr[i]);
                    break;
                case 1:
                    if (arr[i] % 2 != 0) printf("%d ", arr[i]);
                    break;
                case 2:
                    if (arr[i] > 4) printf("%d ", arr[i]);
                    break;
            }
        }
        printf("\n");
    }

    return 0;
}
