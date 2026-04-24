int printf(const char *fmt, ...);

// UART Transmit/Receive Buffer Simulation
// Circular buffer based UART with baud rate, parity, framing

enum UartParity { PARITY_NONE, PARITY_EVEN, PARITY_ODD };
enum UartState { UART_DISABLED, UART_ENABLED };

struct CircularBuffer {
    int data[32];
    int head;
    int tail;
    int count;
    int capacity;
};

void cbuf_init(struct CircularBuffer *buf) {
    int i;
    buf->head = 0;
    buf->tail = 0;
    buf->count = 0;
    buf->capacity = 32;
    for (i = 0; i < 32; i++) {
        buf->data[i] = 0;
    }
}

int cbuf_is_full(struct CircularBuffer *buf) {
    return buf->count >= buf->capacity;
}

int cbuf_is_empty(struct CircularBuffer *buf) {
    return buf->count == 0;
}

int cbuf_push(struct CircularBuffer *buf, int val) {
    if (cbuf_is_full(buf)) return -1;
    buf->data[buf->head] = val;
    buf->head = (buf->head + 1) % buf->capacity;
    buf->count = buf->count + 1;
    return 0;
}

int cbuf_pop(struct CircularBuffer *buf) {
    int val;
    if (cbuf_is_empty(buf)) return -1;
    val = buf->data[buf->tail];
    buf->tail = (buf->tail + 1) % buf->capacity;
    buf->count = buf->count - 1;
    return val;
}

int cbuf_peek(struct CircularBuffer *buf) {
    if (cbuf_is_empty(buf)) return -1;
    return buf->data[buf->tail];
}

struct UartPort {
    int port_id;
    int baud_rate;
    int data_bits;
    int stop_bits;
    int parity;
    int state;
    struct CircularBuffer tx_buf;
    struct CircularBuffer rx_buf;
    int tx_count;
    int rx_count;
    int error_count;
    int overrun_count;
};

void uart_init(struct UartPort *uart, int id, int baud) {
    uart->port_id = id;
    uart->baud_rate = baud;
    uart->data_bits = 8;
    uart->stop_bits = 1;
    uart->parity = PARITY_NONE;
    uart->state = UART_DISABLED;
    uart->tx_count = 0;
    uart->rx_count = 0;
    uart->error_count = 0;
    uart->overrun_count = 0;
    cbuf_init(&(uart->tx_buf));
    cbuf_init(&(uart->rx_buf));
}

void uart_enable(struct UartPort *uart) {
    uart->state = UART_ENABLED;
}

void uart_configure(struct UartPort *uart, int data_bits, int stop_bits, int parity) {
    uart->data_bits = data_bits;
    uart->stop_bits = stop_bits;
    uart->parity = parity;
}

int count_set_bits(int val) {
    int count = 0;
    int v = val;
    while (v > 0) {
        count = count + (v & 1);
        v = v >> 1;
    }
    return count;
}

int uart_calc_parity(struct UartPort *uart, int data) {
    int bits = count_set_bits(data & 0xFF);
    if (uart->parity == PARITY_EVEN) {
        return bits % 2;
    } else if (uart->parity == PARITY_ODD) {
        return (bits + 1) % 2;
    }
    return 0;
}

int uart_transmit(struct UartPort *uart, int data) {
    if (uart->state != UART_ENABLED) return -1;
    if (cbuf_is_full(&(uart->tx_buf))) {
        uart->overrun_count = uart->overrun_count + 1;
        return -2;
    }
    cbuf_push(&(uart->tx_buf), data & 0xFF);
    uart->tx_count = uart->tx_count + 1;
    return 0;
}

int uart_receive_byte(struct UartPort *uart, int data) {
    if (uart->state != UART_ENABLED) return -1;
    if (cbuf_is_full(&(uart->rx_buf))) {
        uart->overrun_count = uart->overrun_count + 1;
        return -2;
    }
    cbuf_push(&(uart->rx_buf), data & 0xFF);
    uart->rx_count = uart->rx_count + 1;
    return 0;
}

int uart_read(struct UartPort *uart) {
    if (uart->state != UART_ENABLED) return -1;
    return cbuf_pop(&(uart->rx_buf));
}

int uart_flush_tx(struct UartPort *uart) {
    int count = 0;
    while (!cbuf_is_empty(&(uart->tx_buf))) {
        cbuf_pop(&(uart->tx_buf));
        count = count + 1;
    }
    return count;
}

int uart_tx_pending(struct UartPort *uart) {
    return uart->tx_buf.count;
}

int uart_rx_available(struct UartPort *uart) {
    return uart->rx_buf.count;
}

// Calculate frame time in microseconds
int uart_frame_time_us(struct UartPort *uart) {
    int bits_per_frame;
    bits_per_frame = 1 + uart->data_bits + uart->stop_bits;
    if (uart->parity != PARITY_NONE) {
        bits_per_frame = bits_per_frame + 1;
    }
    // time = bits * 1000000 / baud
    return (bits_per_frame * 1000000) / uart->baud_rate;
}

// Loopback: move tx to rx (simulates external loopback)
int uart_loopback(struct UartPort *uart) {
    int count = 0;
    int val;
    while (!cbuf_is_empty(&(uart->tx_buf))) {
        val = cbuf_pop(&(uart->tx_buf));
        if (!cbuf_is_full(&(uart->rx_buf))) {
            cbuf_push(&(uart->rx_buf), val);
            count = count + 1;
        }
    }
    return count;
}

int main() {
    struct UartPort uart0;
    int val;
    int i;
    int count;

    uart_init(&uart0, 0, 9600);
    printf("UART %d init at %d baud\n", uart0.port_id, uart0.baud_rate);
    // EXPECT: UART 0 init at 9600 baud

    uart_enable(&uart0);
    printf("UART state: %d\n", uart0.state);
    // EXPECT: UART state: 1

    // Configure 8N1
    uart_configure(&uart0, 8, 1, PARITY_NONE);
    printf("Config: %dN%d\n", uart0.data_bits, uart0.stop_bits);
    // EXPECT: Config: 8N1

    // Transmit some bytes
    uart_transmit(&uart0, 65);  // 'A'
    uart_transmit(&uart0, 66);  // 'B'
    uart_transmit(&uart0, 67);  // 'C'
    printf("TX pending: %d\n", uart_tx_pending(&uart0));
    // EXPECT: TX pending: 3
    printf("TX count: %d\n", uart0.tx_count);
    // EXPECT: TX count: 3

    // Loopback test
    count = uart_loopback(&uart0);
    printf("Loopback transferred: %d\n", count);
    // EXPECT: Loopback transferred: 3

    printf("RX available: %d\n", uart_rx_available(&uart0));
    // EXPECT: RX available: 3

    // Read back
    val = uart_read(&uart0);
    printf("Read: %d\n", val);
    // EXPECT: Read: 65
    val = uart_read(&uart0);
    printf("Read: %d\n", val);
    // EXPECT: Read: 66
    val = uart_read(&uart0);
    printf("Read: %d\n", val);
    // EXPECT: Read: 67

    // Empty read
    val = uart_read(&uart0);
    printf("Empty read: %d\n", val);
    // EXPECT: Empty read: -1

    // Test parity calculation
    uart_configure(&uart0, 8, 1, PARITY_EVEN);
    val = uart_calc_parity(&uart0, 0x55);  // 01010101 -> 4 bits set -> even parity = 0
    printf("Even parity 0x55: %d\n", val);
    // EXPECT: Even parity 0x55: 0

    val = uart_calc_parity(&uart0, 0x07);  // 00000111 -> 3 bits set -> even parity = 1
    printf("Even parity 0x07: %d\n", val);
    // EXPECT: Even parity 0x07: 1

    uart_configure(&uart0, 8, 1, PARITY_ODD);
    val = uart_calc_parity(&uart0, 0x55);  // 4 bits -> odd parity = 1
    printf("Odd parity 0x55: %d\n", val);
    // EXPECT: Odd parity 0x55: 1

    // Frame time
    uart_configure(&uart0, 8, 1, PARITY_NONE);
    val = uart_frame_time_us(&uart0);
    printf("Frame time 9600/8N1: %d us\n", val);
    // EXPECT: Frame time 9600/8N1: 1041 us

    uart_configure(&uart0, 8, 2, PARITY_EVEN);
    val = uart_frame_time_us(&uart0);
    printf("Frame time 9600/8E2: %d us\n", val);
    // EXPECT: Frame time 9600/8E2: 1250 us

    // Fill TX buffer to test overrun
    uart_configure(&uart0, 8, 1, PARITY_NONE);
    for (i = 0; i < 32; i++) {
        uart_transmit(&uart0, i);
    }
    printf("TX full, pending: %d\n", uart_tx_pending(&uart0));
    // EXPECT: TX full, pending: 32

    val = uart_transmit(&uart0, 99);
    printf("Overrun result: %d\n", val);
    // EXPECT: Overrun result: -2
    printf("Overrun count: %d\n", uart0.overrun_count);
    // EXPECT: Overrun count: 1

    // Flush TX
    count = uart_flush_tx(&uart0);
    printf("Flushed: %d\n", count);
    // EXPECT: Flushed: 32
    printf("TX pending after flush: %d\n", uart_tx_pending(&uart0));
    // EXPECT: TX pending after flush: 0

    printf("UART test complete\n");
    // EXPECT: UART test complete

    return 0;
}
