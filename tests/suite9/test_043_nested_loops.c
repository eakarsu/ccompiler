int printf(const char *fmt, ...);

int find_triplets(int limit) {
    int count = 0;
    int i;
    int j;
    int k;
    for (i = 1; i < limit; i = i + 1) {
        for (j = i + 1; j < limit; j = j + 1) {
            for (k = j + 1; k < limit; k = k + 1) {
                if (i * i + j * j == k * k) {
                    count = count + 1;
                }
            }
        }
    }
    return count;
}

void matrix_search(int size) {
    int mat[5][5];
    int i;
    int j;
    int k;
    int val = 1;
    for (i = 0; i < size; i = i + 1) {
        for (j = 0; j < size; j = j + 1) {
            mat[i][j] = val;
            val = val + 1;
        }
    }

    int target = 13;
    int found_i = -1;
    int found_j = -1;
    for (i = 0; i < size; i = i + 1) {
        for (j = 0; j < size; j = j + 1) {
            if (mat[i][j] == target) {
                found_i = i;
                found_j = j;
            }
        }
    }
    printf("found %d at %d,%d\n", target, found_i, found_j);
}

void spiral_count() {
    int n = 4;
    int grid[4][4];
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            grid[i][j] = 0;
        }
    }

    int num = 1;
    int top = 0;
    int bot = n - 1;
    int left = 0;
    int right = n - 1;

    while (top <= bot && left <= right) {
        for (i = left; i <= right; i = i + 1) {
            grid[top][i] = num;
            num = num + 1;
        }
        top = top + 1;
        for (i = top; i <= bot; i = i + 1) {
            grid[i][right] = num;
            num = num + 1;
        }
        right = right - 1;
        if (top <= bot) {
            for (i = right; i >= left; i = i - 1) {
                grid[bot][i] = num;
                num = num + 1;
            }
            bot = bot - 1;
        }
        if (left <= right) {
            for (i = bot; i >= top; i = i - 1) {
                grid[i][left] = num;
                num = num + 1;
            }
            left = left + 1;
        }
    }

    printf("spiral corners: %d %d %d %d\n", grid[0][0], grid[0][3], grid[3][0], grid[3][3]);
    printf("spiral center: %d %d %d %d\n", grid[1][1], grid[1][2], grid[2][1], grid[2][2]);
}

int triple_nested_sum() {
    int sum = 0;
    int i;
    int j;
    int k;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            for (k = 0; k < 5; k = k + 1) {
                if (i + j + k == 6) {
                    sum = sum + i * 100 + j * 10 + k;
                }
            }
        }
    }
    return sum;
}

int main() {
    // EXPECT: triplets under 20: 5
    printf("triplets under 20: %d\n", find_triplets(20));

    // EXPECT: found 13 at 2,2
    matrix_search(5);

    // EXPECT: spiral corners: 1 4 10 7
    // EXPECT: spiral center: 13 14 16 15
    spiral_count();

    // EXPECT: triple sum encoding: 4218
    printf("triple sum encoding: %d\n", triple_nested_sum());

    int count = 0;
    int a;
    int b;
    int c;
    for (a = 1; a <= 4; a = a + 1) {
        for (b = 1; b <= 4; b = b + 1) {
            for (c = 1; c <= 4; c = c + 1) {
                if (a != b && b != c && a != c) {
                    count = count + 1;
                }
            }
        }
    }
    // EXPECT: permutations of 4 choose 3: 24
    printf("permutations of 4 choose 3: %d\n", count);

    return 0;
}
