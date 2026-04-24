int printf(const char *fmt, ...);
// EXPECT: empty: 1\nremaining: 64\navailable: 2\nread: H\nread: i\nempty: 1\navailable: 8\nread int: 12345\nread int: -42\nafter string: 6 bytes\nread string: Test!\nempty: 1
// Test: buffer struct with read/write operations

struct Buffer {
    char data[64];
    int read_pos;
    int write_pos;
    int size;
};

void buf_init(struct Buffer *b) {
    b->read_pos = 0;
    b->write_pos = 0;
    b->size = 0;
}

int buf_available(struct Buffer *b) {
    return b->size;
}

int buf_remaining(struct Buffer *b) {
    return 64 - b->size;
}

int buf_empty(struct Buffer *b) {
    return b->size == 0;
}

void buf_write_byte(struct Buffer *b, char c) {
    if (b->size < 64) {
        b->data[b->write_pos] = c;
        b->write_pos = (b->write_pos + 1) % 64;
        b->size = b->size + 1;
    }
}

char buf_read_byte(struct Buffer *b) {
    if (b->size > 0) {
        char c = b->data[b->read_pos];
        b->read_pos = (b->read_pos + 1) % 64;
        b->size = b->size - 1;
        return c;
    }
    return 0;
}

void buf_write_int(struct Buffer *b, int val) {
    buf_write_byte(b, (char)(val & 0xFF));
    buf_write_byte(b, (char)((val >> 8) & 0xFF));
    buf_write_byte(b, (char)((val >> 16) & 0xFF));
    buf_write_byte(b, (char)((val >> 24) & 0xFF));
}

int buf_read_int(struct Buffer *b) {
    int b0 = (int)buf_read_byte(b) & 0xFF;
    int b1 = (int)buf_read_byte(b) & 0xFF;
    int b2 = (int)buf_read_byte(b) & 0xFF;
    int b3 = (int)buf_read_byte(b) & 0xFF;
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

void buf_write_string(struct Buffer *b, char *s) {
    int i = 0;
    while (s[i] != 0) {
        buf_write_byte(b, s[i]);
        i = i + 1;
    }
    buf_write_byte(b, 0);
}

int main(void) {
    struct Buffer buf;
    buf_init(&buf);

    printf("empty: %d\n", buf_empty(&buf));
    printf("remaining: %d\n", buf_remaining(&buf));

    buf_write_byte(&buf, 'H');
    buf_write_byte(&buf, 'i');
    printf("available: %d\n", buf_available(&buf));
    printf("read: %c\n", buf_read_byte(&buf));
    printf("read: %c\n", buf_read_byte(&buf));
    printf("empty: %d\n", buf_empty(&buf));

    buf_write_int(&buf, 12345);
    buf_write_int(&buf, -42);
    printf("available: %d\n", buf_available(&buf));
    printf("read int: %d\n", buf_read_int(&buf));
    printf("read int: %d\n", buf_read_int(&buf));

    char msg[6];
    msg[0] = 'T'; msg[1] = 'e'; msg[2] = 's'; msg[3] = 't'; msg[4] = '!'; msg[5] = 0;
    buf_write_string(&buf, msg);
    printf("after string: %d bytes\n", buf_available(&buf));

    int i;
    char out[10];
    for (i = 0; i < 6; i++) {
        out[i] = buf_read_byte(&buf);
    }
    printf("read string: %c%c%c%c%c\n", out[0], out[1], out[2], out[3], out[4]);
    printf("empty: %d\n", buf_empty(&buf));

    return 0;
}
