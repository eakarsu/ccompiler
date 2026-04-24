int printf(const char *fmt, ...);
// EXPECT: === hanoi moves ===\nH(1)=1\nH(2)=3\nH(3)=7\nH(4)=15\nH(5)=31\nH(6)=63\nH(7)=127\nH(8)=255\nH(9)=511\nH(10)=1023\n=== 4-peg hanoi (Frame-Stewart) ===\nFS(1)=1\nFS(2)=3\nFS(3)=5\nFS(4)=9\nFS(5)=13\nFS(6)=17\nFS(7)=25\nFS(8)=33\nFS(9)=41\nFS(10)=49\n=== stacking ===\nmax_stack=12\n=== count towers ===\nT(0)=1\nT(1)=1\nT(2)=2\nT(3)=4\nT(4)=8\nT(5)=16\nT(6)=32\nT(7)=64\nT(8)=128\n=== pyramid ===\npyr(1)=1\npyr(2)=5\npyr(3)=14\npyr(4)=30\npyr(5)=55\npyr(6)=91\n=== pyramid layers ===\nlayers(1)=1\nlayers(5)=2\nlayers(14)=3\nlayers(30)=4\nlayers(100)=6
// Test 232: Tower/stacking recursive problems

int hanoi_moves(int n) {
    if (n == 0) return 0;
    return 2 * hanoi_moves(n - 1) + 1;
}

int frame_stewart(int n) {
    // Frame-Stewart for 4-peg Hanoi (approximate)
    if (n <= 1) return n;
    int best = 99999;
    int k = 1;
    while (k < n) {
        int moves = 2 * frame_stewart(k) + hanoi_moves(n - k);
        if (moves < best) best = moves;
        k = k + 1;
    }
    return best;
}

int stacking_blocks(int *heights, int n, int prev_max, int idx) {
    if (idx == n) return 0;
    int skip = stacking_blocks(heights, n, prev_max, idx + 1);
    int take = 0;
    if (heights[idx] <= prev_max) {
        take = heights[idx] + stacking_blocks(heights, n, heights[idx], idx + 1);
    }
    if (take > skip) return take;
    return skip;
}

int count_towers(int n) {
    // Count ways to partition n into ordered decreasing sequences
    if (n == 0) return 1;
    int count = 0;
    int i = 1;
    while (i <= n) {
        count = count + count_towers(n - i);
        i = i + 1;
    }
    return count;
}

int pyramid_sum(int height) {
    if (height <= 0) return 0;
    return height * height + pyramid_sum(height - 1);
}

int pyramid_layers(int blocks) {
    int layer = 1;
    int used = 0;
    while (used + layer * layer <= blocks) {
        used = used + layer * layer;
        layer = layer + 1;
    }
    return layer - 1;
}

int balanced_ternary(int n, int pos) {
    if (n == 0) return 0;
    int power = 1;
    int i = 0;
    while (i < pos) { power = power * 3; i = i + 1; }
    return (n / power) - ((n / power) / 3) * 3;
}

int main(void) {
    printf("=== hanoi moves ===\n");
    int i = 1;
    while (i <= 10) {
        printf("H(%d)=%d\n", i, hanoi_moves(i));
        i = i + 1;
    }

    printf("=== 4-peg hanoi (Frame-Stewart) ===\n");
    i = 1;
    while (i <= 10) {
        printf("FS(%d)=%d\n", i, frame_stewart(i));
        i = i + 1;
    }

    printf("=== stacking ===\n");
    int h[6];
    h[0] = 5; h[1] = 3; h[2] = 4; h[3] = 2; h[4] = 1; h[5] = 6;
    printf("max_stack=%d\n", stacking_blocks(h, 6, 99, 0));

    printf("=== count towers ===\n");
    i = 0;
    while (i <= 8) {
        printf("T(%d)=%d\n", i, count_towers(i));
        i = i + 1;
    }

    printf("=== pyramid ===\n");
    i = 1;
    while (i <= 6) {
        printf("pyr(%d)=%d\n", i, pyramid_sum(i));
        i = i + 1;
    }

    printf("=== pyramid layers ===\n");
    int blocks[5];
    blocks[0] = 1; blocks[1] = 5; blocks[2] = 14; blocks[3] = 30; blocks[4] = 100;
    i = 0;
    while (i < 5) {
        printf("layers(%d)=%d\n", blocks[i], pyramid_layers(blocks[i]));
        i = i + 1;
    }

    return 0;
}
