int printf(const char *fmt, ...);
// EXPECT: === 3 disks ===\nMove disk 1 from A to C\nMove disk 2 from A to B\nMove disk 1 from C to B\nMove disk 3 from A to C\nMove disk 1 from B to A\nMove disk 2 from B to C\nMove disk 1 from A to C\nmoves: 7\n=== 4 disks (count only) ===\nmoves: 15\n=== formula ===\nhanoi(1)=1\nhanoi(2)=3\nhanoi(3)=7\nhanoi(4)=15\nhanoi(5)=31\nhanoi(6)=63\nhanoi(7)=127\nhanoi(8)=255\nhanoi(9)=511\nhanoi(10)=1023\n=== simulation 4 disks ===\nbefore:\nPeg 0: 4 3 2 1\nPeg 1:\nPeg 2:\nafter:\nPeg 0:\nPeg 1:\nPeg 2: 4 3 2 1
// Test 214: Tower of Hanoi

int move_count;

void hanoi(int n, char from, char to, char aux) {
    if (n == 0) return;
    hanoi(n - 1, from, aux, to);
    printf("Move disk %d from %c to %c\n", n, from, to);
    move_count = move_count + 1;
    hanoi(n - 1, aux, to, from);
}

void hanoi_count_only(int n, char from, char to, char aux, int *count) {
    if (n == 0) return;
    hanoi_count_only(n - 1, from, aux, to, count);
    *count = *count + 1;
    hanoi_count_only(n - 1, aux, to, from, count);
}

int hanoi_moves(int n) {
    if (n == 0) return 0;
    return 2 * hanoi_moves(n - 1) + 1;
}

int stacks[3][20];
int tops[3];

void init_stacks(int n) {
    tops[0] = n;
    tops[1] = 0;
    tops[2] = 0;
    int i = 0;
    while (i < n) {
        stacks[0][i] = n - i;
        i = i + 1;
    }
}

void move_disk(int from, int to) {
    tops[from] = tops[from] - 1;
    int disk = stacks[from][tops[from]];
    stacks[to][tops[to]] = disk;
    tops[to] = tops[to] + 1;
}

void hanoi_sim(int n, int from, int to, int aux) {
    if (n == 0) return;
    hanoi_sim(n - 1, from, aux, to);
    move_disk(from, to);
    hanoi_sim(n - 1, aux, to, from);
}

void print_stacks(int n) {
    int p = 0;
    while (p < 3) {
        printf("Peg %d:", p);
        int i = 0;
        while (i < tops[p]) {
            printf(" %d", stacks[p][i]);
            i = i + 1;
        }
        printf("\n");
        p = p + 1;
    }
}

int main(void) {
    printf("=== 3 disks ===\n");
    move_count = 0;
    hanoi(3, 'A', 'C', 'B');
    printf("moves: %d\n", move_count);

    printf("=== 4 disks (count only) ===\n");
    int cnt = 0;
    hanoi_count_only(4, 'A', 'C', 'B', &cnt);
    printf("moves: %d\n", cnt);

    printf("=== formula ===\n");
    int i = 1;
    while (i <= 10) {
        printf("hanoi(%d)=%d\n", i, hanoi_moves(i));
        i = i + 1;
    }

    printf("=== simulation 4 disks ===\n");
    init_stacks(4);
    printf("before:\n");
    print_stacks(4);
    hanoi_sim(4, 0, 2, 1);
    printf("after:\n");
    print_stacks(4);

    return 0;
}
