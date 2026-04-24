int printf(const char *fmt, ...);

// Tower of Hanoi - iterative solution
// Pegs: 0=A, 1=B, 2=C
// Uses arrays as stacks for each peg

typedef struct {
    int disks[16];
    int top;
} Peg;

void peg_init(Peg *p) {
    p->top = -1;
    int i;
    for (i = 0; i < 16; i++) p->disks[i] = 0;
}

void peg_push(Peg *p, int disk) {
    p->top++;
    p->disks[p->top] = disk;
}

int peg_pop(Peg *p) {
    int val = p->disks[p->top];
    p->top--;
    return val;
}

int peg_peek(Peg *p) {
    if (p->top < 0) return 999; // empty = infinity
    return p->disks[p->top];
}

int peg_size(Peg *p) {
    return p->top + 1;
}

// Move between two pegs: smaller disk moves to other peg
void move_between(Peg *a, Peg *b, int *move_count) {
    int top_a = peg_peek(a);
    int top_b = peg_peek(b);
    if (top_a < top_b) {
        peg_push(b, peg_pop(a));
    } else {
        peg_push(a, peg_pop(b));
    }
    *move_count = *move_count + 1;
}

// Iterative Tower of Hanoi
int hanoi_iterative(int n, Peg *src, Peg *aux, Peg *dst) {
    int i;
    // Push disks onto source (largest first)
    for (i = n; i >= 1; i--) {
        peg_push(src, i);
    }

    int total_moves = (1 << n) - 1; // 2^n - 1
    int move_count = 0;

    // For odd n: cycle is src->dst->aux
    // For even n: cycle is src->aux->dst
    Peg *peg1;
    Peg *peg2;
    Peg *peg3;
    if (n % 2 == 1) {
        peg1 = src; peg2 = dst; peg3 = aux;
    } else {
        peg1 = src; peg2 = aux; peg3 = dst;
    }

    for (i = 1; i <= total_moves; i++) {
        if (i % 3 == 1)
            move_between(peg1, peg2, &move_count);
        else if (i % 3 == 2)
            move_between(peg1, peg3, &move_count);
        else
            move_between(peg2, peg3, &move_count);
    }

    return move_count;
}

// Recursive count for verification
int hanoi_recursive_count(int n) {
    if (n <= 0) return 0;
    return 2 * hanoi_recursive_count(n - 1) + 1;
}

// Check that destination has all disks in order
int verify_destination(Peg *dst, int n) {
    if (peg_size(dst) != n) return 0;
    int i;
    // disks[0] should be largest (n), disks[top] should be 1
    for (i = 0; i <= dst->top; i++) {
        if (dst->disks[i] != n - i) return 0;
    }
    return 1;
}

int main() {
    // Test n=1
    Peg s1, a1, d1;
    peg_init(&s1); peg_init(&a1); peg_init(&d1);
    int moves1 = hanoi_iterative(1, &s1, &a1, &d1);
    printf("moves_1: %d\n", moves1);
    // EXPECT: moves_1: 1
    printf("verify_1: %d\n", verify_destination(&d1, 1));
    // EXPECT: verify_1: 1

    // Test n=2
    Peg s2, a2, d2;
    peg_init(&s2); peg_init(&a2); peg_init(&d2);
    int moves2 = hanoi_iterative(2, &s2, &a2, &d2);
    printf("moves_2: %d\n", moves2);
    // EXPECT: moves_2: 3
    printf("verify_2: %d\n", verify_destination(&d2, 2));
    // EXPECT: verify_2: 1

    // Test n=3
    Peg s3, a3, d3;
    peg_init(&s3); peg_init(&a3); peg_init(&d3);
    int moves3 = hanoi_iterative(3, &s3, &a3, &d3);
    printf("moves_3: %d\n", moves3);
    // EXPECT: moves_3: 7
    printf("verify_3: %d\n", verify_destination(&d3, 3));
    // EXPECT: verify_3: 1

    // Test n=4
    Peg s4, a4, d4;
    peg_init(&s4); peg_init(&a4); peg_init(&d4);
    int moves4 = hanoi_iterative(4, &s4, &a4, &d4);
    printf("moves_4: %d\n", moves4);
    // EXPECT: moves_4: 15
    printf("verify_4: %d\n", verify_destination(&d4, 4));
    // EXPECT: verify_4: 1

    // Test n=5
    Peg s5, a5, d5;
    peg_init(&s5); peg_init(&a5); peg_init(&d5);
    int moves5 = hanoi_iterative(5, &s5, &a5, &d5);
    printf("moves_5: %d\n", moves5);
    // EXPECT: moves_5: 31
    printf("verify_5: %d\n", verify_destination(&d5, 5));
    // EXPECT: verify_5: 1

    // Verify recursive formula matches
    printf("rec_3: %d\n", hanoi_recursive_count(3));
    // EXPECT: rec_3: 7
    printf("rec_5: %d\n", hanoi_recursive_count(5));
    // EXPECT: rec_5: 31
    printf("rec_10: %d\n", hanoi_recursive_count(10));
    // EXPECT: rec_10: 1023

    // Test n=6
    Peg s6, a6, d6;
    peg_init(&s6); peg_init(&a6); peg_init(&d6);
    int moves6 = hanoi_iterative(6, &s6, &a6, &d6);
    printf("moves_6: %d\n", moves6);
    // EXPECT: moves_6: 63
    printf("verify_6: %d\n", verify_destination(&d6, 6));
    // EXPECT: verify_6: 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
