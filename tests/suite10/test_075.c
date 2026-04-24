int printf(const char *fmt, ...);

// Test 075: Boundary checking - safe buffer and array operations

struct SafeBuffer {
    int data[20];
    int size;
    int capacity;
    int error_flag;
};

void buf_init(struct SafeBuffer *b) {
    b->size = 0;
    b->capacity = 20;
    b->error_flag = 0;
    for (int i = 0; i < 20; i++) {
        b->data[i] = 0;
    }
}

int buf_push(struct SafeBuffer *b, int val) {
    if (b->size >= b->capacity) {
        b->error_flag = 1;
        return -1;
    }
    b->data[b->size] = val;
    b->size++;
    return 0;
}

int buf_pop(struct SafeBuffer *b, int *val) {
    if (b->size <= 0) {
        b->error_flag = 2;
        return -1;
    }
    b->size--;
    *val = b->data[b->size];
    return 0;
}

int buf_get(struct SafeBuffer *b, int idx, int *val) {
    if (idx < 0 || idx >= b->size) {
        b->error_flag = 3;
        return -1;
    }
    *val = b->data[idx];
    return 0;
}

int buf_set(struct SafeBuffer *b, int idx, int val) {
    if (idx < 0 || idx >= b->size) {
        b->error_flag = 4;
        return -1;
    }
    b->data[idx] = val;
    return 0;
}

int buf_insert(struct SafeBuffer *b, int idx, int val) {
    if (b->size >= b->capacity) {
        b->error_flag = 5;
        return -1;
    }
    if (idx < 0 || idx > b->size) {
        b->error_flag = 6;
        return -1;
    }
    for (int i = b->size; i > idx; i--) {
        b->data[i] = b->data[i - 1];
    }
    b->data[idx] = val;
    b->size++;
    return 0;
}

int buf_remove(struct SafeBuffer *b, int idx) {
    if (idx < 0 || idx >= b->size) {
        b->error_flag = 7;
        return -1;
    }
    for (int i = idx; i < b->size - 1; i++) {
        b->data[i] = b->data[i + 1];
    }
    b->size--;
    return 0;
}

int checked_add(int a, int b, int *result) {
    int sum = a + b;
    if (a > 0 && b > 0 && sum < 0) return -1;
    if (a < 0 && b < 0 && sum > 0) return -1;
    *result = sum;
    return 0;
}

int checked_mul(int a, int b, int *result) {
    if (a == 0 || b == 0) {
        *result = 0;
        return 0;
    }
    int prod = a * b;
    if (prod / a != b) return -1;
    *result = prod;
    return 0;
}

int main() {
    struct SafeBuffer buf;
    buf_init(&buf);

    for (int i = 0; i < 15; i++) {
        int ret = buf_push(&buf, (i + 1) * 10);
        if (ret != 0) {
            printf("Push failed at %d\n", i);
        }
    }
    printf("Size after 15 pushes: %d\n", buf.size);
    // EXPECT: Size after 15 pushes: 15

    int val = 0;
    int ret;

    ret = buf_get(&buf, 0, &val);
    printf("buf[0] = %d ret=%d\n", val, ret);
    // EXPECT: buf[0] = 10 ret=0

    ret = buf_get(&buf, 14, &val);
    printf("buf[14] = %d ret=%d\n", val, ret);
    // EXPECT: buf[14] = 150 ret=0

    ret = buf_get(&buf, 20, &val);
    printf("out of bounds get: ret=%d err=%d\n", ret, buf.error_flag);
    // EXPECT: out of bounds get: ret=-1 err=3

    ret = buf_get(&buf, -1, &val);
    printf("negative index get: ret=%d err=%d\n", ret, buf.error_flag);
    // EXPECT: negative index get: ret=-1 err=3

    buf_set(&buf, 5, 999);
    buf_get(&buf, 5, &val);
    printf("buf[5] after set = %d\n", val);
    // EXPECT: buf[5] after set = 999

    ret = buf_set(&buf, 100, 1);
    printf("out of bounds set: ret=%d err=%d\n", ret, buf.error_flag);
    // EXPECT: out of bounds set: ret=-1 err=4

    buf_insert(&buf, 0, 777);
    buf_get(&buf, 0, &val);
    printf("after insert at 0: buf[0]=%d size=%d\n", val, buf.size);
    // EXPECT: after insert at 0: buf[0]=777 size=16

    buf_remove(&buf, 0);
    buf_get(&buf, 0, &val);
    printf("after remove at 0: buf[0]=%d size=%d\n", val, buf.size);
    // EXPECT: after remove at 0: buf[0]=10 size=15

    for (int i = buf.size; i < 20; i++) {
        buf_push(&buf, (i + 1) * 100);
    }
    printf("Size at capacity: %d\n", buf.size);
    // EXPECT: Size at capacity: 20

    ret = buf_push(&buf, 9999);
    printf("push when full: ret=%d err=%d\n", ret, buf.error_flag);
    // EXPECT: push when full: ret=-1 err=1

    while (buf.size > 0) {
        buf_pop(&buf, &val);
    }
    printf("After drain: size=%d\n", buf.size);
    // EXPECT: After drain: size=0

    ret = buf_pop(&buf, &val);
    printf("pop when empty: ret=%d err=%d\n", ret, buf.error_flag);
    // EXPECT: pop when empty: ret=-1 err=2

    int res = 0;
    ret = checked_add(100, 200, &res);
    printf("100+200=%d ret=%d\n", res, ret);
    // EXPECT: 100+200=300 ret=0

    ret = checked_mul(300, 400, &res);
    printf("300*400=%d ret=%d\n", res, ret);
    // EXPECT: 300*400=120000 ret=0

    printf("Boundary checks done\n");
    // EXPECT: Boundary checks done
    return 0;
}
