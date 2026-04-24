int printf(const char *fmt, ...);
// EXPECT: classify(-200)=-3\nclassify(-75)=-2\nclassify(-25)=-1\nclassify(0)=0\nclassify(5)=0\nclassify(30)=1\nclassify(75)=2\nclassify(150)=3\ngrade(98)=1\ngrade(94)=2\ngrade(91)=3\ngrade(85)=5\ngrade(72)=7\ngrade(55)=9
int classify(int x) {
    int result;
    if (x < 0) {
        if (x < -100) {
            result = -3;
        } else if (x < -50) {
            result = -2;
        } else if (x < -10) {
            result = -1;
        } else {
            result = 0;
        }
    } else if (x == 0) {
        result = 0;
    } else {
        if (x > 100) {
            result = 3;
        } else if (x > 50) {
            result = 2;
        } else if (x > 10) {
            result = 1;
        } else {
            result = 0;
        }
    }
    return result;
}

int grade(int score) {
    int g;
    if (score >= 90) {
        if (score >= 97) {
            g = 1;
        } else if (score >= 93) {
            g = 2;
        } else {
            g = 3;
        }
    } else if (score >= 80) {
        if (score >= 87) {
            g = 4;
        } else if (score >= 83) {
            g = 5;
        } else {
            g = 6;
        }
    } else if (score >= 70) {
        g = 7;
    } else if (score >= 60) {
        g = 8;
    } else {
        g = 9;
    }
    return g;
}

int main(void) {
    int vals[8];
    int i;
    vals[0] = -200;
    vals[1] = -75;
    vals[2] = -25;
    vals[3] = 0;
    vals[4] = 5;
    vals[5] = 30;
    vals[6] = 75;
    vals[7] = 150;
    for (i = 0; i < 8; i++) {
        printf("classify(%d)=%d\n", vals[i], classify(vals[i]));
    }
    int scores[6];
    scores[0] = 98;
    scores[1] = 94;
    scores[2] = 91;
    scores[3] = 85;
    scores[4] = 72;
    scores[5] = 55;
    for (i = 0; i < 6; i++) {
        printf("grade(%d)=%d\n", scores[i], grade(scores[i]));
    }
    return 0;
}
