int printf(const char *fmt, ...);

typedef int IntArray5[5];
typedef int IntArray10[10];

typedef struct {
    int data[8];
    int count;
} IntBuffer;

typedef struct {
    int values[4];
    int label;
} TaggedValues;

void init_buffer(IntBuffer *buf) {
    buf->count = 0;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        buf->data[i] = 0;
    }
}

int buffer_push(IntBuffer *buf, int val) {
    if (buf->count >= 8) return 0;
    buf->data[buf->count] = val;
    buf->count = buf->count + 1;
    return 1;
}

int buffer_pop(IntBuffer *buf) {
    if (buf->count <= 0) return -1;
    buf->count = buf->count - 1;
    return buf->data[buf->count];
}

int buffer_sum(IntBuffer *buf) {
    int total = 0;
    int i;
    for (i = 0; i < buf->count; i = i + 1) {
        total = total + buf->data[i];
    }
    return total;
}

int buffer_max(IntBuffer *buf) {
    if (buf->count == 0) return -1;
    int m = buf->data[0];
    int i;
    for (i = 1; i < buf->count; i = i + 1) {
        if (buf->data[i] > m) m = buf->data[i];
    }
    return m;
}

void fill_array5(IntArray5 arr, int start) {
    int i;
    for (i = 0; i < 5; i = i + 1) {
        arr[i] = start + i;
    }
}

int sum_array5(IntArray5 arr) {
    int total = 0;
    int i;
    for (i = 0; i < 5; i = i + 1) {
        total = total + arr[i];
    }
    return total;
}

void fill_array10(IntArray10 arr, int start) {
    int i;
    for (i = 0; i < 10; i = i + 1) {
        arr[i] = start + i * 2;
    }
}

int sum_array10(IntArray10 arr) {
    int total = 0;
    int i;
    for (i = 0; i < 10; i = i + 1) {
        total = total + arr[i];
    }
    return total;
}

TaggedValues tagged_new(int label, int a, int b, int c, int d) {
    TaggedValues t;
    t.label = label;
    t.values[0] = a;
    t.values[1] = b;
    t.values[2] = c;
    t.values[3] = d;
    return t;
}

int tagged_sum(TaggedValues *t) {
    int total = 0;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        total = total + t->values[i];
    }
    return total;
}

int tagged_min(TaggedValues *t) {
    int m = t->values[0];
    int i;
    for (i = 1; i < 4; i = i + 1) {
        if (t->values[i] < m) m = t->values[i];
    }
    return m;
}

void buffer_reverse(IntBuffer *buf) {
    int i = 0;
    int j = buf->count - 1;
    while (i < j) {
        int tmp = buf->data[i];
        buf->data[i] = buf->data[j];
        buf->data[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

int main(void) {
    IntArray5 a5;
    fill_array5(a5, 1);
    // EXPECT: a5 sum: 15
    printf("a5 sum: %d\n", sum_array5(a5));

    // EXPECT: a5[0]: 1
    printf("a5[0]: %d\n", a5[0]);

    // EXPECT: a5[4]: 5
    printf("a5[4]: %d\n", a5[4]);

    IntArray10 a10;
    fill_array10(a10, 0);
    // EXPECT: a10 sum: 90
    printf("a10 sum: %d\n", sum_array10(a10));

    // EXPECT: a10[0]: 0
    printf("a10[0]: %d\n", a10[0]);

    // EXPECT: a10[9]: 18
    printf("a10[9]: %d\n", a10[9]);

    IntBuffer buf;
    init_buffer(&buf);
    buffer_push(&buf, 10);
    buffer_push(&buf, 20);
    buffer_push(&buf, 30);
    // EXPECT: buf count: 3
    printf("buf count: %d\n", buf.count);

    // EXPECT: buf sum: 60
    printf("buf sum: %d\n", buffer_sum(&buf));

    // EXPECT: buf max: 30
    printf("buf max: %d\n", buffer_max(&buf));

    int popped = buffer_pop(&buf);
    // EXPECT: popped: 30
    printf("popped: %d\n", popped);

    // EXPECT: after pop count: 2
    printf("after pop count: %d\n", buf.count);

    // EXPECT: after pop sum: 30
    printf("after pop sum: %d\n", buffer_sum(&buf));

    TaggedValues tv = tagged_new(42, 5, 10, 15, 20);
    // EXPECT: tv label: 42
    printf("tv label: %d\n", tv.label);

    // EXPECT: tv sum: 50
    printf("tv sum: %d\n", tagged_sum(&tv));

    // EXPECT: tv min: 5
    printf("tv min: %d\n", tagged_min(&tv));

    IntBuffer buf2;
    init_buffer(&buf2);
    buffer_push(&buf2, 1);
    buffer_push(&buf2, 2);
    buffer_push(&buf2, 3);
    buffer_push(&buf2, 4);
    buffer_reverse(&buf2);
    // EXPECT: reversed[0]: 4
    printf("reversed[0]: %d\n", buf2.data[0]);

    // EXPECT: reversed[3]: 1
    printf("reversed[3]: %d\n", buf2.data[3]);

    IntBuffer buf3;
    init_buffer(&buf3);
    int ok = buffer_push(&buf3, 99);
    // EXPECT: push ok: 1
    printf("push ok: %d\n", ok);

    // EXPECT: buf3 pop: 99
    printf("buf3 pop: %d\n", buffer_pop(&buf3));

    // EXPECT: empty pop: -1
    printf("empty pop: %d\n", buffer_pop(&buf3));

    return 0;
}
