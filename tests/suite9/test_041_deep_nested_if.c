int printf(const char *fmt, ...);

int classify_value(int x, int y, int z) {
    int result;
    if (x > 0) {
        if (y > 0) {
            if (z > 0) {
                if (x > y) {
                    if (x > z) {
                        printf("path: x>y>z, x largest positive\n");
                        result = 1;
                    } else {
                        printf("path: x>y but z>=x\n");
                        result = 2;
                    }
                } else {
                    if (y > z) {
                        printf("path: y>=x and y>z\n");
                        result = 3;
                    } else {
                        result = 4;
                    }
                }
            } else {
                if (x + y > 10) {
                    if (z < -5) {
                        printf("path: x,y pos, z very neg, sum>10\n");
                        result = 5;
                    } else {
                        result = 6;
                    }
                } else {
                    result = 7;
                }
            }
        } else {
            if (z > 0) {
                if (x + z > y) {
                    if (x > z) {
                        printf("path: x pos, y neg, z pos, x>z\n");
                        result = 8;
                    } else {
                        result = 9;
                    }
                } else {
                    result = 10;
                }
            } else {
                if (x > -y) {
                    if (x > -z) {
                        printf("path: x dominates neg y and neg z\n");
                        result = 11;
                    } else {
                        result = 12;
                    }
                } else {
                    result = 13;
                }
            }
        }
    } else {
        if (y > 0) {
            if (z > 0) {
                if (y > z) {
                    printf("path: x<=0, y>z>0\n");
                    result = 14;
                } else {
                    result = 15;
                }
            } else {
                result = 16;
            }
        } else {
            if (z > 0) {
                result = 17;
            } else {
                if (x < y) {
                    if (y < z) {
                        printf("path: all neg, x<y<z\n");
                        result = 18;
                    } else {
                        result = 19;
                    }
                } else {
                    result = 20;
                }
            }
        }
    }
    return result;
}

int main() {
    int r;

    // EXPECT: path: x>y>z, x largest positive
    r = classify_value(10, 5, 3);
    // EXPECT: result: 1
    printf("result: %d\n", r);

    // EXPECT: path: x>y but z>=x
    r = classify_value(5, 3, 8);
    // EXPECT: result: 2
    printf("result: %d\n", r);

    // EXPECT: path: y>=x and y>z
    r = classify_value(3, 7, 2);
    // EXPECT: result: 3
    printf("result: %d\n", r);

    // EXPECT: path: x,y pos, z very neg, sum>10
    r = classify_value(8, 6, -10);
    // EXPECT: result: 5
    printf("result: %d\n", r);

    // EXPECT: path: x pos, y neg, z pos, x>z
    r = classify_value(10, -3, 2);
    // EXPECT: result: 8
    printf("result: %d\n", r);

    // EXPECT: path: x dominates neg y and neg z
    r = classify_value(10, -3, -2);
    // EXPECT: result: 11
    printf("result: %d\n", r);

    // EXPECT: path: x<=0, y>z>0
    r = classify_value(-1, 5, 2);
    // EXPECT: result: 14
    printf("result: %d\n", r);

    // EXPECT: path: all neg, x<y<z
    r = classify_value(-5, -3, -1);
    // EXPECT: result: 18
    printf("result: %d\n", r);

    return 0;
}
