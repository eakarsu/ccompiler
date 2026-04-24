int printf(const char *fmt, ...);

// UART Protocol Simulator
// Simulates UART frame: start bit, 8 data bits, optional parity, stop bit
// Operates on a bit-stream using a state machine

enum uart_state {
    UART_IDLE,
    UART_START,
    UART_DATA,
    UART_PARITY,
    UART_STOP,
    UART_ERROR
};

struct uart_rx {
    int state;
    int bit_count;
    int data_byte;
    int parity_en;
    int parity_bit;
    int computed_parity;
    int rx_buf[16];
    int rx_count;
    int error_count;
    int frame_count;
};

void uart_init(struct uart_rx *rx, int parity) {
    rx->state = UART_IDLE;
    rx->bit_count = 0;
    rx->data_byte = 0;
    rx->parity_en = parity;
    rx->parity_bit = 0;
    rx->computed_parity = 0;
    rx->rx_count = 0;
    rx->error_count = 0;
    rx->frame_count = 0;
    int i;
    for (i = 0; i < 16; i++) {
        rx->rx_buf[i] = 0;
    }
}

void uart_feed_bit(struct uart_rx *rx, int bit) {
    int st = rx->state;

    if (st == UART_IDLE) {
        if (bit == 0) {
            // Start bit detected
            rx->state = UART_START;
            rx->bit_count = 0;
            rx->data_byte = 0;
            rx->computed_parity = 0;
        }
    } else if (st == UART_START) {
        // First data bit
        rx->data_byte = rx->data_byte | (bit << rx->bit_count);
        rx->computed_parity = rx->computed_parity ^ bit;
        rx->bit_count = rx->bit_count + 1;
        rx->state = UART_DATA;
    } else if (st == UART_DATA) {
        rx->data_byte = rx->data_byte | (bit << rx->bit_count);
        rx->computed_parity = rx->computed_parity ^ bit;
        rx->bit_count = rx->bit_count + 1;
        if (rx->bit_count >= 8) {
            if (rx->parity_en) {
                rx->state = UART_PARITY;
            } else {
                rx->state = UART_STOP;
            }
        }
    } else if (st == UART_PARITY) {
        rx->parity_bit = bit;
        rx->state = UART_STOP;
    } else if (st == UART_STOP) {
        if (bit == 1) {
            // Valid stop bit
            int valid = 1;
            if (rx->parity_en) {
                // Even parity: computed_parity ^ parity_bit should be 0
                if ((rx->computed_parity ^ rx->parity_bit) != 0) {
                    valid = 0;
                    rx->error_count = rx->error_count + 1;
                }
            }
            if (valid && rx->rx_count < 16) {
                rx->rx_buf[rx->rx_count] = rx->data_byte;
                rx->rx_count = rx->rx_count + 1;
            }
            rx->frame_count = rx->frame_count + 1;
        } else {
            // Framing error
            rx->error_count = rx->error_count + 1;
            rx->frame_count = rx->frame_count + 1;
        }
        rx->state = UART_IDLE;
    }
}

// Encode a byte into UART frame bits (no parity)
int encode_frame(int byte_val, int *bits, int parity_en) {
    int idx = 0;
    bits[idx] = 0; // start bit
    idx = idx + 1;

    int i;
    int par = 0;
    for (i = 0; i < 8; i++) {
        int b = (byte_val >> i) & 1;
        bits[idx] = b;
        par = par ^ b;
        idx = idx + 1;
    }

    if (parity_en) {
        bits[idx] = par; // even parity bit
        idx = idx + 1;
    }

    bits[idx] = 1; // stop bit
    idx = idx + 1;
    return idx;
}

int main() {
    struct uart_rx rx;
    int bits[128];
    int frame_len;
    int i;

    printf("=== UART Protocol ===\n"); // EXPECT: === UART Protocol ===

    // Test 1: Send byte 0x41 ('A' = 65) without parity
    uart_init(&rx, 0);
    frame_len = encode_frame(65, bits, 0);
    printf("Frame len: %d\n", frame_len); // EXPECT: Frame len: 10

    // Add idle bits before
    uart_feed_bit(&rx, 1);
    uart_feed_bit(&rx, 1);
    // Feed the frame
    for (i = 0; i < frame_len; i++) {
        uart_feed_bit(&rx, bits[i]);
    }
    printf("RX count: %d\n", rx.rx_count);   // EXPECT: RX count: 1
    printf("Byte 0: %d\n", rx.rx_buf[0]);    // EXPECT: Byte 0: 65
    printf("Errors: %d\n", rx.error_count);   // EXPECT: Errors: 0

    // Test 2: Send multiple bytes: 72, 105 ("Hi")
    uart_init(&rx, 0);
    frame_len = encode_frame(72, bits, 0);
    for (i = 0; i < frame_len; i++) {
        uart_feed_bit(&rx, bits[i]);
    }
    // idle
    uart_feed_bit(&rx, 1);
    frame_len = encode_frame(105, bits, 0);
    for (i = 0; i < frame_len; i++) {
        uart_feed_bit(&rx, bits[i]);
    }
    printf("RX count: %d\n", rx.rx_count);   // EXPECT: RX count: 2
    printf("Byte 0: %d\n", rx.rx_buf[0]);    // EXPECT: Byte 0: 72
    printf("Byte 1: %d\n", rx.rx_buf[1]);    // EXPECT: Byte 1: 105
    printf("Frames: %d\n", rx.frame_count);  // EXPECT: Frames: 2

    // Test 3: With even parity
    uart_init(&rx, 1);
    frame_len = encode_frame(65, bits, 1);
    printf("Parity frame len: %d\n", frame_len); // EXPECT: Parity frame len: 11
    for (i = 0; i < frame_len; i++) {
        uart_feed_bit(&rx, bits[i]);
    }
    printf("RX count: %d\n", rx.rx_count);   // EXPECT: RX count: 1
    printf("Byte 0: %d\n", rx.rx_buf[0]);    // EXPECT: Byte 0: 65
    printf("Errors: %d\n", rx.error_count);   // EXPECT: Errors: 0

    // Test 4: Parity error - flip parity bit
    uart_init(&rx, 1);
    frame_len = encode_frame(65, bits, 1);
    // Flip the parity bit (index 9)
    bits[9] = 1 - bits[9];
    for (i = 0; i < frame_len; i++) {
        uart_feed_bit(&rx, bits[i]);
    }
    printf("RX count: %d\n", rx.rx_count);   // EXPECT: RX count: 0
    printf("Errors: %d\n", rx.error_count);   // EXPECT: Errors: 1

    // Test 5: Framing error - bad stop bit
    uart_init(&rx, 0);
    frame_len = encode_frame(65, bits, 0);
    bits[frame_len - 1] = 0; // corrupt stop bit
    for (i = 0; i < frame_len; i++) {
        uart_feed_bit(&rx, bits[i]);
    }
    printf("RX count: %d\n", rx.rx_count);   // EXPECT: RX count: 0
    printf("Errors: %d\n", rx.error_count);   // EXPECT: Errors: 1

    // Test 6: Send byte 255 (all bits set)
    uart_init(&rx, 0);
    frame_len = encode_frame(255, bits, 0);
    for (i = 0; i < frame_len; i++) {
        uart_feed_bit(&rx, bits[i]);
    }
    printf("Byte 0: %d\n", rx.rx_buf[0]);    // EXPECT: Byte 0: 255

    // Test 7: Send byte 0
    uart_init(&rx, 0);
    frame_len = encode_frame(0, bits, 0);
    for (i = 0; i < frame_len; i++) {
        uart_feed_bit(&rx, bits[i]);
    }
    printf("Byte 0: %d\n", rx.rx_buf[0]);    // EXPECT: Byte 0: 0

    printf("Done\n"); // EXPECT: Done
    return 0;
}
