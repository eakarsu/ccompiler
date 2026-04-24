int printf(const char *fmt, ...);

// GPIO pin configuration and state machine

enum gpio_mode {
    GPIO_INPUT,
    GPIO_OUTPUT,
    GPIO_ALT_FUNC,
    GPIO_ANALOG
};

enum gpio_pull {
    GPIO_NO_PULL,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN
};

enum gpio_state {
    GPIO_LOW,
    GPIO_HIGH
};

enum fsm_state {
    FSM_IDLE,
    FSM_SETUP,
    FSM_ACTIVE,
    FSM_TEARDOWN,
    FSM_ERROR
};

struct gpio_pin {
    int pin_number;
    int mode;
    int pull;
    int state;
    int locked;
};

struct gpio_port {
    struct gpio_pin pins[8];
    int port_id;
    int enabled;
};

void gpio_init_pin(struct gpio_pin *p, int num, int mode, int pull) {
    p->pin_number = num;
    p->mode = mode;
    p->pull = pull;
    p->state = GPIO_LOW;
    p->locked = 0;
}

int gpio_set_mode(struct gpio_pin *p, int mode) {
    if (p->locked) return -1;
    p->mode = mode;
    return 0;
}

int gpio_write(struct gpio_pin *p, int val) {
    if (p->mode != GPIO_OUTPUT) return -1;
    if (p->locked) return -1;
    p->state = val ? GPIO_HIGH : GPIO_LOW;
    return 0;
}

int gpio_read(struct gpio_pin *p) {
    if (p->mode == GPIO_ANALOG) return -1;
    if (p->pull == GPIO_PULL_UP && p->state == GPIO_LOW) {
        return GPIO_HIGH;
    }
    return p->state;
}

void gpio_lock(struct gpio_pin *p) {
    p->locked = 1;
}

void gpio_init_port(struct gpio_port *port, int id) {
    int i;
    port->port_id = id;
    port->enabled = 1;
    for (i = 0; i < 8; i++) {
        gpio_init_pin(&port->pins[i], i, GPIO_INPUT, GPIO_NO_PULL);
    }
}

int gpio_port_write(struct gpio_port *port, int mask, int values) {
    int i;
    int written = 0;
    if (!port->enabled) return -1;
    for (i = 0; i < 8; i++) {
        if (mask & (1 << i)) {
            int val = (values >> i) & 1;
            port->pins[i].mode = GPIO_OUTPUT;
            if (gpio_write(&port->pins[i], val) == 0) {
                written++;
            }
        }
    }
    return written;
}

int gpio_port_read(struct gpio_port *port) {
    int i;
    int result = 0;
    for (i = 0; i < 8; i++) {
        int val = gpio_read(&port->pins[i]);
        if (val > 0) {
            result = result | (1 << i);
        }
    }
    return result;
}

int run_fsm(struct gpio_port *port) {
    int state = FSM_IDLE;
    int cycles = 0;
    int done = 0;

    while (!done) {
        cycles++;
        if (state == FSM_IDLE) {
            if (port->enabled) {
                state = FSM_SETUP;
            }
        } else if (state == FSM_SETUP) {
            gpio_port_write(port, 0x0F, 0x05);
            state = FSM_ACTIVE;
        } else if (state == FSM_ACTIVE) {
            int val = gpio_port_read(port);
            if (val == 0x05) {
                gpio_port_write(port, 0xF0, 0xA0);
                state = FSM_TEARDOWN;
            } else {
                state = FSM_ERROR;
            }
        } else if (state == FSM_TEARDOWN) {
            int i;
            for (i = 0; i < 8; i++) {
                port->pins[i].mode = GPIO_INPUT;
                port->pins[i].state = GPIO_LOW;
            }
            done = 1;
        } else {
            done = 1;
        }
    }
    return cycles;
}

int main(void) {
    struct gpio_pin pin;
    struct gpio_port port;
    int result;

    gpio_init_pin(&pin, 3, GPIO_OUTPUT, GPIO_NO_PULL);
    printf("%d\n", pin.pin_number);
    // EXPECT: 3
    printf("%d\n", pin.mode);
    // EXPECT: 1
    printf("%d\n", pin.state);
    // EXPECT: 0

    gpio_write(&pin, 1);
    printf("%d\n", gpio_read(&pin));
    // EXPECT: 1

    gpio_write(&pin, 0);
    printf("%d\n", gpio_read(&pin));
    // EXPECT: 0

    gpio_lock(&pin);
    result = gpio_write(&pin, 1);
    printf("%d\n", result);
    // EXPECT: -1
    printf("%d\n", pin.state);
    // EXPECT: 0

    struct gpio_pin pullup_pin;
    gpio_init_pin(&pullup_pin, 5, GPIO_INPUT, GPIO_PULL_UP);
    printf("%d\n", gpio_read(&pullup_pin));
    // EXPECT: 1

    gpio_init_port(&port, 0);
    printf("%d\n", port.port_id);
    // EXPECT: 0
    printf("%d\n", port.enabled);
    // EXPECT: 1

    result = gpio_port_write(&port, 0xFF, 0xAA);
    printf("%d\n", result);
    // EXPECT: 8

    int readval = gpio_port_read(&port);
    printf("%d\n", readval);
    // EXPECT: 170

    gpio_init_port(&port, 1);
    int cycles = run_fsm(&port);
    printf("%d\n", cycles);
    // EXPECT: 4

    int final_read = gpio_port_read(&port);
    printf("%d\n", final_read);
    // EXPECT: 0

    struct gpio_pin analog_pin;
    gpio_init_pin(&analog_pin, 7, GPIO_ANALOG, GPIO_NO_PULL);
    printf("%d\n", gpio_read(&analog_pin));
    // EXPECT: -1

    printf("%s\n", "gpio_done");
    // EXPECT: gpio_done

    return 0;
}
