int printf(const char *fmt, ...);

// Bit field extraction and insertion (manual, no bitfield syntax)

// Extract 'width' bits starting at position 'pos' from 'val'
int extract_bits(int val, int pos, int width) {
    int mask = (1 << width) - 1;
    return (val >> pos) & mask;
}

// Insert 'width' bits of 'field' into 'val' at position 'pos'
int insert_bits(int val, int field, int pos, int width) {
    int mask = (1 << width) - 1;
    int cleared = val & (~(mask << pos));
    return cleared | ((field & mask) << pos);
}

// Packed RGB color: bits[0:7]=R, bits[8:15]=G, bits[16:23]=B
int pack_rgb(int r, int g, int b) {
    return (r & 0xFF) | ((g & 0xFF) << 8) | ((b & 0xFF) << 16);
}

int get_red(int rgb) {
    return rgb & 0xFF;
}

int get_green(int rgb) {
    return (rgb >> 8) & 0xFF;
}

int get_blue(int rgb) {
    return (rgb >> 16) & 0xFF;
}

int set_red(int rgb, int r) {
    return insert_bits(rgb, r, 0, 8);
}

int set_green(int rgb, int g) {
    return insert_bits(rgb, g, 8, 8);
}

int set_blue(int rgb, int b) {
    return insert_bits(rgb, b, 16, 8);
}

// Packed instruction format:
// bits[0:5]   = opcode (6 bits)
// bits[6:10]  = reg1 (5 bits)
// bits[11:15] = reg2 (5 bits)
// bits[16:31] = immediate (16 bits)
struct Instruction {
    int packed;
};

void instr_set(struct Instruction *inst, int opcode, int r1, int r2, int imm) {
    inst->packed = 0;
    inst->packed = insert_bits(inst->packed, opcode, 0, 6);
    inst->packed = insert_bits(inst->packed, r1, 6, 5);
    inst->packed = insert_bits(inst->packed, r2, 11, 5);
    inst->packed = insert_bits(inst->packed, imm, 16, 16);
}

int instr_opcode(struct Instruction *inst) {
    return extract_bits(inst->packed, 0, 6);
}

int instr_r1(struct Instruction *inst) {
    return extract_bits(inst->packed, 6, 5);
}

int instr_r2(struct Instruction *inst) {
    return extract_bits(inst->packed, 11, 5);
}

int instr_imm(struct Instruction *inst) {
    return extract_bits(inst->packed, 16, 16);
}

// Packed date: bits[0:4]=day(5), bits[5:8]=month(4), bits[9:15]=year_offset(7)
int pack_date(int day, int month, int year_offset) {
    int packed = 0;
    packed = insert_bits(packed, day, 0, 5);
    packed = insert_bits(packed, month, 5, 4);
    packed = insert_bits(packed, year_offset, 9, 7);
    return packed;
}

int get_day(int d) { return extract_bits(d, 0, 5); }
int get_month(int d) { return extract_bits(d, 5, 4); }
int get_year_offset(int d) { return extract_bits(d, 9, 7); }

// Sign-extend a value from 'width' bits to full int
int sign_extend(int val, int width) {
    int sign_bit = (val >> (width - 1)) & 1;
    if (sign_bit) {
        // Set all bits above width
        int mask = (1 << width) - 1;
        return val | (~mask);
    }
    return val;
}

int main() {
    // extract_bits tests
    // 0xFF = 11111111, extract 4 bits at pos 0 = 1111 = 15
    printf("ex1=%d\n", extract_bits(0xFF, 0, 4));
    // EXPECT: ex1=15
    // 0xFF, extract 4 bits at pos 4 = 1111 = 15
    printf("ex2=%d\n", extract_bits(0xFF, 4, 4));
    // EXPECT: ex2=15
    // 0xAB = 10101011, extract 4 bits at pos 0 = 1011 = 11
    printf("ex3=%d\n", extract_bits(0xAB, 0, 4));
    // EXPECT: ex3=11
    // 0xAB, extract 4 bits at pos 4 = 1010 = 10
    printf("ex4=%d\n", extract_bits(0xAB, 4, 4));
    // EXPECT: ex4=10

    // insert_bits tests
    // Insert 0xF at pos 0, width 4 into 0 = 0xF = 15
    printf("in1=%d\n", insert_bits(0, 15, 0, 4));
    // EXPECT: in1=15
    // Insert 0xF at pos 4, width 4 into 0 = 0xF0 = 240
    printf("in2=%d\n", insert_bits(0, 15, 4, 4));
    // EXPECT: in2=240
    // Insert 5 at pos 4, width 4 into 0xFF = clear bits 4-7 (0x0F) | (5<<4=0x50) = 0x5F = 95
    printf("in3=%d\n", insert_bits(0xFF, 5, 4, 4));
    // EXPECT: in3=95

    // RGB packing
    int color = pack_rgb(255, 128, 64);
    printf("r=%d\n", get_red(color));
    // EXPECT: r=255
    printf("g=%d\n", get_green(color));
    // EXPECT: g=128
    printf("b=%d\n", get_blue(color));
    // EXPECT: b=64

    color = set_red(color, 100);
    printf("r2=%d\n", get_red(color));
    // EXPECT: r2=100
    printf("g2=%d\n", get_green(color));
    // EXPECT: g2=128
    printf("b2=%d\n", get_blue(color));
    // EXPECT: b2=64

    color = set_green(color, 200);
    color = set_blue(color, 150);
    printf("r3=%d\n", get_red(color));
    // EXPECT: r3=100
    printf("g3=%d\n", get_green(color));
    // EXPECT: g3=200
    printf("b3=%d\n", get_blue(color));
    // EXPECT: b3=150

    // Instruction packing
    struct Instruction inst;
    instr_set(&inst, 10, 3, 7, 1000);
    printf("op=%d\n", instr_opcode(&inst));
    // EXPECT: op=10
    printf("r1=%d\n", instr_r1(&inst));
    // EXPECT: r1=3
    printf("r2=%d\n", instr_r2(&inst));
    // EXPECT: r2=7
    printf("imm=%d\n", instr_imm(&inst));
    // EXPECT: imm=1000

    // Another instruction
    instr_set(&inst, 63, 31, 31, 65535);
    printf("op2=%d\n", instr_opcode(&inst));
    // EXPECT: op2=63
    printf("r1b=%d\n", instr_r1(&inst));
    // EXPECT: r1b=31
    printf("r2b=%d\n", instr_r2(&inst));
    // EXPECT: r2b=31
    printf("imm2=%d\n", instr_imm(&inst));
    // EXPECT: imm2=65535

    // Date packing
    int date = pack_date(25, 12, 24);
    printf("day=%d\n", get_day(date));
    // EXPECT: day=25
    printf("month=%d\n", get_month(date));
    // EXPECT: month=12
    printf("year=%d\n", get_year_offset(date));
    // EXPECT: year=24

    // Another date
    date = pack_date(1, 1, 0);
    printf("day2=%d\n", get_day(date));
    // EXPECT: day2=1
    printf("month2=%d\n", get_month(date));
    // EXPECT: month2=1
    printf("year2=%d\n", get_year_offset(date));
    // EXPECT: year2=0

    // Sign extension
    // 5 in 4 bits = 0101, sign bit=0, no extension needed = 5
    printf("se1=%d\n", sign_extend(5, 4));
    // EXPECT: se1=5
    // 15 in 4 bits = 1111, sign bit=1, extend = -1
    printf("se2=%d\n", sign_extend(15, 4));
    // EXPECT: se2=-1
    // 8 in 4 bits = 1000, sign bit=1, extend = -8
    printf("se3=%d\n", sign_extend(8, 4));
    // EXPECT: se3=-8
    // 7 in 4 bits = 0111, sign bit=0, no extension = 7
    printf("se4=%d\n", sign_extend(7, 4));
    // EXPECT: se4=7
    // 3 in 3 bits = 011, sign bit=0, no extension = 3
    printf("se5=%d\n", sign_extend(3, 3));
    // EXPECT: se5=3
    // 7 in 3 bits = 111, sign bit=1, extend = -1
    printf("se6=%d\n", sign_extend(7, 3));
    // EXPECT: se6=-1

    printf("done\n");
    // EXPECT: done
    return 0;
}
