int printf(const char *fmt, ...);
// EXPECT: initial: r0=100 r1=50 r2=65280 r3=0 r4=0 r5=0 r6=0 r7=0 flags=0\nr0 += r1: r0=150 flags=0\nr0 -= r1: r0=100 flags=0\nr3 -= r0: r3=-50 flags=2\nr4 & r5: r4=15\nr6 | r7: r6=255\nr0 bytes: 1 2 3 4\nr0 after clear byte0: 67305984
// Test: union-based register simulation

union Register {
    int full;
    char bytes[4];
};

struct RegisterFile {
    union Register regs[8];
    int flags;
};

void rf_init(struct RegisterFile *rf) {
    int i;
    for (i = 0; i < 8; i++) {
        rf->regs[i].full = 0;
    }
    rf->flags = 0;
}

void rf_set(struct RegisterFile *rf, int reg, int value) {
    rf->regs[reg].full = value;
}

int rf_get(struct RegisterFile *rf, int reg) {
    return rf->regs[reg].full;
}

void rf_set_byte(struct RegisterFile *rf, int reg, int byte_idx, char val) {
    rf->regs[reg].bytes[byte_idx] = val;
}

int rf_get_byte(struct RegisterFile *rf, int reg, int byte_idx) {
    return (int)rf->regs[reg].bytes[byte_idx] & 0xFF;
}

void rf_add(struct RegisterFile *rf, int dst, int src) {
    rf->regs[dst].full = rf->regs[dst].full + rf->regs[src].full;
    rf->flags = 0;
    if (rf->regs[dst].full == 0) rf->flags = 1;
    if (rf->regs[dst].full < 0) rf->flags = rf->flags | 2;
}

void rf_sub(struct RegisterFile *rf, int dst, int src) {
    rf->regs[dst].full = rf->regs[dst].full - rf->regs[src].full;
    rf->flags = 0;
    if (rf->regs[dst].full == 0) rf->flags = 1;
    if (rf->regs[dst].full < 0) rf->flags = rf->flags | 2;
}

void rf_and(struct RegisterFile *rf, int dst, int src) {
    rf->regs[dst].full = rf->regs[dst].full & rf->regs[src].full;
}

void rf_or(struct RegisterFile *rf, int dst, int src) {
    rf->regs[dst].full = rf->regs[dst].full | rf->regs[src].full;
}

void rf_print(struct RegisterFile *rf) {
    int i;
    for (i = 0; i < 8; i++) {
        printf("r%d=%d ", i, rf->regs[i].full);
    }
    printf("flags=%d\n", rf->flags);
}

int main(void) {
    struct RegisterFile rf;
    rf_init(&rf);

    rf_set(&rf, 0, 100);
    rf_set(&rf, 1, 50);
    rf_set(&rf, 2, 0xFF00);
    printf("initial: "); rf_print(&rf);

    rf_add(&rf, 0, 1);
    printf("r0 += r1: r0=%d flags=%d\n", rf_get(&rf, 0), rf.flags);

    rf_sub(&rf, 0, 1);
    printf("r0 -= r1: r0=%d flags=%d\n", rf_get(&rf, 0), rf.flags);

    rf_set(&rf, 3, 50);
    rf_sub(&rf, 3, 0);
    printf("r3 -= r0: r3=%d flags=%d\n", rf_get(&rf, 3), rf.flags);

    rf_set(&rf, 4, 0xFF);
    rf_set(&rf, 5, 0x0F);
    rf_and(&rf, 4, 5);
    printf("r4 & r5: r4=%d\n", rf_get(&rf, 4));

    rf_set(&rf, 6, 0xF0);
    rf_set(&rf, 7, 0x0F);
    rf_or(&rf, 6, 7);
    printf("r6 | r7: r6=%d\n", rf_get(&rf, 6));

    rf_set(&rf, 0, 0x04030201);
    printf("r0 bytes: %d %d %d %d\n",
           rf_get_byte(&rf, 0, 0), rf_get_byte(&rf, 0, 1),
           rf_get_byte(&rf, 0, 2), rf_get_byte(&rf, 0, 3));

    rf_set_byte(&rf, 0, 0, 0);
    printf("r0 after clear byte0: %d\n", rf_get(&rf, 0));

    return 0;
}
