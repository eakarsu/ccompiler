int printf(const char *fmt, ...);

// GPIO Pin Controller Simulation
// Simulates a 16-pin GPIO port with direction, output, input, pull-up/down

enum GpioDir { GPIO_INPUT, GPIO_OUTPUT };
enum GpioPull { GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN };

struct GpioPin {
    int number;
    int direction;
    int output_val;
    int input_val;
    int pull;
    int enabled;
};

struct GpioPort {
    struct GpioPin pins[16];
    int port_id;
    int num_configured;
};

void gpio_init_port(struct GpioPort *port, int port_id) {
    int i;
    port->port_id = port_id;
    port->num_configured = 0;
    for (i = 0; i < 16; i++) {
        port->pins[i].number = i;
        port->pins[i].direction = GPIO_INPUT;
        port->pins[i].output_val = 0;
        port->pins[i].input_val = 0;
        port->pins[i].pull = GPIO_NOPULL;
        port->pins[i].enabled = 0;
    }
}

void gpio_configure_pin(struct GpioPort *port, int pin, int dir, int pull) {
    if (pin < 0 || pin >= 16) return;
    port->pins[pin].direction = dir;
    port->pins[pin].pull = pull;
    port->pins[pin].enabled = 1;
    port->num_configured = port->num_configured + 1;
}

void gpio_write_pin(struct GpioPort *port, int pin, int val) {
    if (pin < 0 || pin >= 16) return;
    if (port->pins[pin].direction != GPIO_OUTPUT) return;
    if (!port->pins[pin].enabled) return;
    port->pins[pin].output_val = val ? 1 : 0;
}

int gpio_read_pin(struct GpioPort *port, int pin) {
    if (pin < 0 || pin >= 16) return -1;
    if (!port->pins[pin].enabled) return -1;
    if (port->pins[pin].direction == GPIO_OUTPUT) {
        return port->pins[pin].output_val;
    }
    // For input pins, apply pull resistor default
    if (port->pins[pin].pull == GPIO_PULLUP) {
        return port->pins[pin].input_val ? port->pins[pin].input_val : 1;
    }
    if (port->pins[pin].pull == GPIO_PULLDOWN) {
        return 0;
    }
    return port->pins[pin].input_val;
}

void gpio_set_input(struct GpioPort *port, int pin, int val) {
    if (pin < 0 || pin >= 16) return;
    if (port->pins[pin].direction != GPIO_INPUT) return;
    port->pins[pin].input_val = val;
}

int gpio_read_port_outputs(struct GpioPort *port) {
    int result = 0;
    int i;
    for (i = 0; i < 16; i++) {
        if (port->pins[i].direction == GPIO_OUTPUT && port->pins[i].enabled) {
            result = result | (port->pins[i].output_val << i);
        }
    }
    return result;
}

void gpio_toggle_pin(struct GpioPort *port, int pin) {
    if (pin < 0 || pin >= 16) return;
    if (port->pins[pin].direction != GPIO_OUTPUT) return;
    port->pins[pin].output_val = port->pins[pin].output_val ? 0 : 1;
}

int gpio_count_high_outputs(struct GpioPort *port) {
    int count = 0;
    int i;
    for (i = 0; i < 16; i++) {
        if (port->pins[i].direction == GPIO_OUTPUT && port->pins[i].output_val) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    struct GpioPort portA;
    int val;
    int port_out;

    gpio_init_port(&portA, 0);
    printf("GPIO Port %d initialized\n", portA.port_id);
    // EXPECT: GPIO Port 0 initialized

    // Configure pins 0-3 as outputs
    gpio_configure_pin(&portA, 0, GPIO_OUTPUT, GPIO_NOPULL);
    gpio_configure_pin(&portA, 1, GPIO_OUTPUT, GPIO_NOPULL);
    gpio_configure_pin(&portA, 2, GPIO_OUTPUT, GPIO_NOPULL);
    gpio_configure_pin(&portA, 3, GPIO_OUTPUT, GPIO_NOPULL);

    // Configure pins 4-5 as inputs with pull-up/down
    gpio_configure_pin(&portA, 4, GPIO_INPUT, GPIO_PULLUP);
    gpio_configure_pin(&portA, 5, GPIO_INPUT, GPIO_PULLDOWN);

    printf("Configured %d pins\n", portA.num_configured);
    // EXPECT: Configured 6 pins

    // Write to output pins
    gpio_write_pin(&portA, 0, 1);
    gpio_write_pin(&portA, 1, 0);
    gpio_write_pin(&portA, 2, 1);
    gpio_write_pin(&portA, 3, 1);

    // Read back outputs
    val = gpio_read_pin(&portA, 0);
    printf("Pin 0 output: %d\n", val);
    // EXPECT: Pin 0 output: 1

    val = gpio_read_pin(&portA, 1);
    printf("Pin 1 output: %d\n", val);
    // EXPECT: Pin 1 output: 0

    // Read input pins (pull-up should give 1, pull-down gives 0)
    val = gpio_read_pin(&portA, 4);
    printf("Pin 4 input (pullup): %d\n", val);
    // EXPECT: Pin 4 input (pullup): 1

    val = gpio_read_pin(&portA, 5);
    printf("Pin 5 input (pulldown): %d\n", val);
    // EXPECT: Pin 5 input (pulldown): 0

    // Set external input on pin 4
    gpio_set_input(&portA, 4, 0);
    val = gpio_read_pin(&portA, 4);
    printf("Pin 4 after external low: %d\n", val);
    // EXPECT: Pin 4 after external low: 1

    // Read port outputs as bitmask
    port_out = gpio_read_port_outputs(&portA);
    printf("Port output bitmask: %d\n", port_out);
    // EXPECT: Port output bitmask: 13

    // Toggle pin 1
    gpio_toggle_pin(&portA, 1);
    val = gpio_read_pin(&portA, 1);
    printf("Pin 1 after toggle: %d\n", val);
    // EXPECT: Pin 1 after toggle: 1

    // Count high output pins
    printf("High outputs: %d\n", gpio_count_high_outputs(&portA));
    // EXPECT: High outputs: 4

    // Toggle pin 0 twice (back to original)
    gpio_toggle_pin(&portA, 0);
    gpio_toggle_pin(&portA, 0);
    val = gpio_read_pin(&portA, 0);
    printf("Pin 0 after double toggle: %d\n", val);
    // EXPECT: Pin 0 after double toggle: 1

    // Try writing to input pin (should be ignored)
    gpio_write_pin(&portA, 4, 1);
    val = gpio_read_pin(&portA, 4);
    printf("Pin 4 after invalid write: %d\n", val);
    // EXPECT: Pin 4 after invalid write: 1

    // Disabled pin read
    val = gpio_read_pin(&portA, 10);
    printf("Disabled pin read: %d\n", val);
    // EXPECT: Disabled pin read: -1

    printf("GPIO test complete\n");
    // EXPECT: GPIO test complete

    return 0;
}
