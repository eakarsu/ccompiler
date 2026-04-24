int printf(const char *fmt, ...);

// UART transmit/receive simulation

enum uart_parity {
    PARITY_NONE,
    PARITY_EVEN,
    PARITY_ODD
};

enum uart_state {
    UART_IDLE,
    UART_TX_BUSY,
    UART_RX_BUSY
};

struct uart_config {
    int baud_rate;
    int data_bits;
    int stop_bits;
    int parity;
};

struct uart_fifo {
    int buffer[16];
    int head;
    int tail;
    int count;
    int capacity;
};

struct uart {
    struct uart_config config;
    struct uart_fifo tx_fifo;
    struct uart_fifo rx_fifo;
    int state;
    int tx_count;
    int rx_count;
    int frame_errors;
    int parity_errors;
};

void fifo_init(struct uart_fifo *f) {
    int i;
    for (i = 0; i < 16; i++) {
        f->buffer[i] = 0;
    }
    f->head = 0;
    f->tail = 0;
    f->count = 0;
    f->capacity = 16;
}

int fifo_push(struct uart_fifo *f, int val) {
    if (f->count >= f->capacity) return -1;
    f->buffer[f->head] = val;
    f->head = (f->head + 1) % f->capacity;
    f->count++;
    return 0;
}

int fifo_pop(struct uart_fifo *f) {
    int val;
    if (f->count <= 0) return -1;
    val = f->buffer[f->tail];
    f->tail = (f->tail + 1) % f->capacity;
    f->count--;
    return val;
}

int fifo_peek(struct uart_fifo *f) {
    if (f->count <= 0) return -1;
    return f->buffer[f->tail];
}

int fifo_is_empty(struct uart_fifo *f) {
    return f->count == 0;
}

int fifo_is_full(struct uart_fifo *f) {
    return f->count >= f->capacity;
}

void uart_init(struct uart *u, int baud, int databits, int stopbits, int parity) {
    u->config.baud_rate = baud;
    u->config.data_bits = databits;
    u->config.stop_bits = stopbits;
    u->config.parity = parity;
    fifo_init(&u->tx_fifo);
    fifo_init(&u->rx_fifo);
    u->state = UART_IDLE;
    u->tx_count = 0;
    u->rx_count = 0;
    u->frame_errors = 0;
    u->parity_errors = 0;
}

int count_bits(int val) {
    int count = 0;
    while (val) {
        count = count + (val & 1);
        val = val >> 1;
    }
    return count;
}

int compute_parity(int data, int parity_type) {
    int ones = count_bits(data);
    if (parity_type == PARITY_EVEN) {
        return ones % 2;
    } else if (parity_type == PARITY_ODD) {
        return (ones + 1) % 2;
    }
    return 0;
}

int uart_send_byte(struct uart *u, int data) {
    int mask = (1 << u->config.data_bits) - 1;
    data = data & mask;
    if (fifo_is_full(&u->tx_fifo)) return -1;
    fifo_push(&u->tx_fifo, data);
    return 0;
}

int uart_transmit(struct uart *u) {
    int transmitted = 0;
    while (!fifo_is_empty(&u->tx_fifo)) {
        int data = fifo_pop(&u->tx_fifo);
        u->tx_count++;
        transmitted++;
        fifo_push(&u->rx_fifo, data);
        u->rx_count++;
    }
    return transmitted;
}

int uart_receive_byte(struct uart *u) {
    if (fifo_is_empty(&u->rx_fifo)) return -1;
    return fifo_pop(&u->rx_fifo);
}

int uart_calc_frame_time_us(struct uart *u) {
    int bits_per_frame;
    int bit_time_us;

    bits_per_frame = 1 + u->config.data_bits + u->config.stop_bits;
    if (u->config.parity != PARITY_NONE) bits_per_frame++;

    bit_time_us = 1000000 / u->config.baud_rate;
    return bits_per_frame * bit_time_us;
}

int uart_loopback_test(struct uart *u, int count) {
    int i;
    int errors = 0;
    for (i = 0; i < count; i++) {
        int send_val = (i * 7 + 3) & 0xFF;
        uart_send_byte(u, send_val);
    }
    uart_transmit(u);
    for (i = 0; i < count; i++) {
        int expected = (i * 7 + 3) & 0xFF;
        int received = uart_receive_byte(u);
        if (received != expected) errors++;
    }
    return errors;
}

int main(void) {
    struct uart u1;
    int result;

    uart_init(&u1, 9600, 8, 1, PARITY_NONE);
    printf("%d\n", u1.config.baud_rate);
    // EXPECT: 9600
    printf("%d\n", u1.state);
    // EXPECT: 0
    printf("%d\n", u1.tx_fifo.count);
    // EXPECT: 0

    result = uart_send_byte(&u1, 65);
    printf("%d\n", result);
    // EXPECT: 0
    printf("%d\n", u1.tx_fifo.count);
    // EXPECT: 1

    uart_send_byte(&u1, 66);
    uart_send_byte(&u1, 67);
    printf("%d\n", u1.tx_fifo.count);
    // EXPECT: 3

    int tx_count = uart_transmit(&u1);
    printf("%d\n", tx_count);
    // EXPECT: 3
    printf("%d\n", u1.tx_count);
    // EXPECT: 3
    printf("%d\n", u1.rx_fifo.count);
    // EXPECT: 3

    int byte1 = uart_receive_byte(&u1);
    printf("%d\n", byte1);
    // EXPECT: 65
    int byte2 = uart_receive_byte(&u1);
    printf("%d\n", byte2);
    // EXPECT: 66
    int byte3 = uart_receive_byte(&u1);
    printf("%d\n", byte3);
    // EXPECT: 67

    int empty_read = uart_receive_byte(&u1);
    printf("%d\n", empty_read);
    // EXPECT: -1

    printf("%d\n", count_bits(0));
    // EXPECT: 0
    printf("%d\n", count_bits(0xFF));
    // EXPECT: 8
    printf("%d\n", count_bits(0xAA));
    // EXPECT: 4

    printf("%d\n", compute_parity(0x55, PARITY_EVEN));
    // EXPECT: 0
    printf("%d\n", compute_parity(0x55, PARITY_ODD));
    // EXPECT: 1
    printf("%d\n", compute_parity(0x07, PARITY_EVEN));
    // EXPECT: 1

    int frame_us = uart_calc_frame_time_us(&u1);
    printf("%d\n", frame_us);
    // EXPECT: 1040

    struct uart u2;
    uart_init(&u2, 115200, 8, 1, PARITY_EVEN);
    int frame_us2 = uart_calc_frame_time_us(&u2);
    printf("%d\n", frame_us2);
    // EXPECT: 88

    int errors = uart_loopback_test(&u1, 10);
    printf("%d\n", errors);
    // EXPECT: 0
    printf("%d\n", u1.tx_count);
    // EXPECT: 13

    struct uart_fifo f;
    fifo_init(&f);
    int i;
    for (i = 0; i < 16; i++) {
        fifo_push(&f, i * 10);
    }
    printf("%d\n", fifo_is_full(&f));
    // EXPECT: 1
    result = fifo_push(&f, 999);
    printf("%d\n", result);
    // EXPECT: -1

    printf("%d\n", fifo_peek(&f));
    // EXPECT: 0
    printf("%d\n", fifo_pop(&f));
    // EXPECT: 0
    printf("%d\n", fifo_pop(&f));
    // EXPECT: 10
    printf("%d\n", f.count);
    // EXPECT: 14

    printf("%s\n", "uart_done");
    // EXPECT: uart_done

    return 0;
}
