int printf(const char *fmt, ...);

int main(void) {
    // Data array
    int data[10];
    data[0] = 100;
    data[1] = 200;
    data[2] = 300;
    data[3] = 400;
    data[4] = 500;
    data[5] = 600;
    data[6] = 700;
    data[7] = 800;
    data[8] = 900;
    data[9] = 1000;

    // Gather: read from data at specified indices
    int indices[5];
    indices[0] = 2;
    indices[1] = 5;
    indices[2] = 0;
    indices[3] = 9;
    indices[4] = 7;

    int gathered[5];
    int i = 0;
    while (i < 5) {
        gathered[i] = data[indices[i]];
        i = i + 1;
    }

    // EXPECT: gathered[0]: 300
    printf("gathered[0]: %d\n", gathered[0]);
    // EXPECT: gathered[1]: 600
    printf("gathered[1]: %d\n", gathered[1]);
    // EXPECT: gathered[2]: 100
    printf("gathered[2]: %d\n", gathered[2]);
    // EXPECT: gathered[3]: 1000
    printf("gathered[3]: %d\n", gathered[3]);
    // EXPECT: gathered[4]: 800
    printf("gathered[4]: %d\n", gathered[4]);

    // Sum of gathered values
    int gsum = 0;
    i = 0;
    while (i < 5) {
        gsum = gsum + gathered[i];
        i = i + 1;
    }
    // EXPECT: gather sum: 2800
    printf("gather sum: %d\n", gsum);

    // Scatter: write values to specified positions
    int dest[8];
    i = 0;
    while (i < 8) {
        dest[i] = 0;
        i = i + 1;
    }

    int scatter_idx[4];
    scatter_idx[0] = 1;
    scatter_idx[1] = 3;
    scatter_idx[2] = 5;
    scatter_idx[3] = 7;

    int scatter_val[4];
    scatter_val[0] = 11;
    scatter_val[1] = 33;
    scatter_val[2] = 55;
    scatter_val[3] = 77;

    i = 0;
    while (i < 4) {
        dest[scatter_idx[i]] = scatter_val[i];
        i = i + 1;
    }

    // EXPECT: dest[0]: 0
    printf("dest[0]: %d\n", dest[0]);
    // EXPECT: dest[1]: 11
    printf("dest[1]: %d\n", dest[1]);
    // EXPECT: dest[3]: 33
    printf("dest[3]: %d\n", dest[3]);
    // EXPECT: dest[5]: 55
    printf("dest[5]: %d\n", dest[5]);
    // EXPECT: dest[7]: 77
    printf("dest[7]: %d\n", dest[7]);

    // Permutation: rearrange array by index map
    int perm[6];
    perm[0] = 3;
    perm[1] = 0;
    perm[2] = 5;
    perm[3] = 1;
    perm[4] = 4;
    perm[5] = 2;

    int source[6];
    source[0] = 10;
    source[1] = 20;
    source[2] = 30;
    source[3] = 40;
    source[4] = 50;
    source[5] = 60;

    int result[6];
    i = 0;
    while (i < 6) {
        result[i] = source[perm[i]];
        i = i + 1;
    }

    // EXPECT: perm[0]: 40
    printf("perm[0]: %d\n", result[0]);
    // EXPECT: perm[1]: 10
    printf("perm[1]: %d\n", result[1]);
    // EXPECT: perm[5]: 30
    printf("perm[5]: %d\n", result[5]);

    // Double indirection gather
    int table[4];
    table[0] = 7;
    table[1] = 2;
    table[2] = 9;
    table[3] = 0;

    int lookup[10];
    lookup[0] = 111;
    lookup[1] = 222;
    lookup[2] = 333;
    lookup[3] = 444;
    lookup[4] = 555;
    lookup[5] = 666;
    lookup[6] = 777;
    lookup[7] = 888;
    lookup[8] = 999;
    lookup[9] = 1111;

    // Gather: for each table[i], read lookup[table[i]]
    int double_g[4];
    i = 0;
    while (i < 4) {
        double_g[i] = lookup[table[i]];
        i = i + 1;
    }

    // EXPECT: double[0]: 888
    printf("double[0]: %d\n", double_g[0]);
    // EXPECT: double[1]: 333
    printf("double[1]: %d\n", double_g[1]);
    // EXPECT: double[2]: 1111
    printf("double[2]: %d\n", double_g[2]);
    // EXPECT: double[3]: 111
    printf("double[3]: %d\n", double_g[3]);

    // Scatter-add: accumulate into bins
    int bins[5];
    i = 0;
    while (i < 5) {
        bins[i] = 0;
        i = i + 1;
    }

    int bin_idx[8];
    bin_idx[0] = 0;
    bin_idx[1] = 1;
    bin_idx[2] = 2;
    bin_idx[3] = 0;
    bin_idx[4] = 3;
    bin_idx[5] = 1;
    bin_idx[6] = 4;
    bin_idx[7] = 0;

    i = 0;
    while (i < 8) {
        bins[bin_idx[i]] = bins[bin_idx[i]] + 1;
        i = i + 1;
    }

    // EXPECT: bins[0]: 3
    printf("bins[0]: %d\n", bins[0]);
    // EXPECT: bins[1]: 2
    printf("bins[1]: %d\n", bins[1]);
    // EXPECT: bins[4]: 1
    printf("bins[4]: %d\n", bins[4]);

    return 0;
}
