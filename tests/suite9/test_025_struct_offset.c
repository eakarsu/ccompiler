int printf(const char *fmt, ...);

// Test: Struct member offset calculation and pointer-based access

struct Simple {
    int a;
    int b;
    int c;
};

struct Packed {
    char x;
    char y;
    char z;
    char w;
};

struct Mixed {
    char tag;
    int value;
    char flag;
    int count;
};

struct Nested {
    int id;
    struct Simple inner;
    int tail;
};

int read_field(char *base, int offset) {
    // Read an int at the given byte offset from base
    int *p = (int *)(base + offset);
    return *p;
}

int main() {
    // Calculate offsets by taking address differences
    struct Simple s;
    s.a = 10;
    s.b = 20;
    s.c = 30;

    char *base = (char *)&s;
    int off_a = (int)((char *)&s.a - base);
    int off_b = (int)((char *)&s.b - base);
    int off_c = (int)((char *)&s.c - base);

    // EXPECT: Simple.a offset: 0
    printf("Simple.a offset: %d\n", off_a);
    // EXPECT: Simple.b offset: 4
    printf("Simple.b offset: %d\n", off_b);
    // EXPECT: Simple.c offset: 8
    printf("Simple.c offset: %d\n", off_c);

    // Access struct fields via byte pointer + offset
    int va = read_field(base, off_a);
    int vb = read_field(base, off_b);
    int vc = read_field(base, off_c);
    // EXPECT: via offset a: 10
    printf("via offset a: %d\n", va);
    // EXPECT: via offset b: 20
    printf("via offset b: %d\n", vb);
    // EXPECT: via offset c: 30
    printf("via offset c: %d\n", vc);

    // Char-only struct has no padding between chars
    struct Packed pk;
    pk.x = 1;
    pk.y = 2;
    pk.z = 3;
    pk.w = 4;
    char *pk_base = (char *)&pk;
    // EXPECT: Packed.x offset: 0
    printf("Packed.x offset: %d\n", (int)((char *)&pk.x - pk_base));
    // EXPECT: Packed.y offset: 1
    printf("Packed.y offset: %d\n", (int)((char *)&pk.y - pk_base));
    // EXPECT: Packed.z offset: 2
    printf("Packed.z offset: %d\n", (int)((char *)&pk.z - pk_base));
    // EXPECT: Packed.w offset: 3
    printf("Packed.w offset: %d\n", (int)((char *)&pk.w - pk_base));

    // Access char fields via raw pointer
    // EXPECT: pk via ptr: 1 2 3 4
    printf("pk via ptr: %d %d %d %d\n",
           (int)pk_base[0], (int)pk_base[1],
           (int)pk_base[2], (int)pk_base[3]);

    // Mixed struct with padding between char and int
    struct Mixed m;
    m.tag = 65;
    m.value = 1000;
    m.flag = 66;
    m.count = 2000;
    char *m_base = (char *)&m;
    int m_off_tag = (int)((char *)&m.tag - m_base);
    int m_off_val = (int)((char *)&m.value - m_base);
    int m_off_flag = (int)((char *)&m.flag - m_base);
    int m_off_count = (int)((char *)&m.count - m_base);

    // EXPECT: Mixed.tag offset: 0
    printf("Mixed.tag offset: %d\n", m_off_tag);
    // value likely at offset 4 due to alignment
    // EXPECT: Mixed.value offset: 4
    printf("Mixed.value offset: %d\n", m_off_val);
    // EXPECT: Mixed.value via ptr: 1000
    printf("Mixed.value via ptr: %d\n", read_field(m_base, m_off_val));
    // EXPECT: Mixed.count via ptr: 2000
    printf("Mixed.count via ptr: %d\n", read_field(m_base, m_off_count));

    // Array of structs: stride between elements
    struct Simple arr[3];
    arr[0].a = 100;
    arr[1].a = 200;
    arr[2].a = 300;
    char *arr_base = (char *)&arr[0];
    int stride = (int)((char *)&arr[1] - (char *)&arr[0]);
    // EXPECT: Simple stride: 12
    printf("Simple stride: %d\n", stride);

    // Access arr[2].a via base + 2*stride
    int val2 = read_field(arr_base, 2 * stride);
    // EXPECT: arr[2].a via stride: 300
    printf("arr[2].a via stride: %d\n", val2);

    // Nested struct offset
    struct Nested n;
    n.id = 1;
    n.inner.a = 11;
    n.inner.b = 22;
    n.inner.c = 33;
    n.tail = 99;
    char *n_base = (char *)&n;
    int n_off_inner = (int)((char *)&n.inner - n_base);
    int n_off_tail = (int)((char *)&n.tail - n_base);
    // EXPECT: Nested.inner offset: 4
    printf("Nested.inner offset: %d\n", n_off_inner);
    // EXPECT: Nested.tail offset: 16
    printf("Nested.tail offset: %d\n", n_off_tail);
    // EXPECT: Nested.inner.b via ptr: 22
    printf("Nested.inner.b via ptr: %d\n", read_field(n_base, n_off_inner + 4));

    // EXPECT: offset done
    printf("offset done\n");

    return 0;
}
