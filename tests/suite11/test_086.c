int printf(const char *fmt, ...);

// I2C Bus Simulation
// Master/slave communication, address matching, ACK/NACK, data transfer

enum I2cState { I2C_IDLE, I2C_START, I2C_ADDR, I2C_DATA, I2C_STOP };
enum I2cDir { I2C_WRITE, I2C_READ };
enum I2cAck { I2C_ACK, I2C_NACK };

struct I2cDevice {
    int address;        // 7-bit address
    int registers[16];  // internal register space
    int num_registers;
    int reg_pointer;    // current register pointer
    int active;
};

struct I2cBus {
    int sda;            // data line state
    int scl;            // clock line state
    int state;
    int current_addr;
    int direction;
    int byte_count;
    int error_count;
    int transfer_count;
    struct I2cDevice devices[4];
    int num_devices;
    int tx_buffer[32];
    int tx_len;
    int rx_buffer[32];
    int rx_len;
};

void i2c_device_init(struct I2cDevice *dev, int addr) {
    int i;
    dev->address = addr;
    dev->num_registers = 16;
    dev->reg_pointer = 0;
    dev->active = 1;
    for (i = 0; i < 16; i++) {
        dev->registers[i] = 0;
    }
}

void i2c_bus_init(struct I2cBus *bus) {
    int i;
    bus->sda = 1;
    bus->scl = 1;
    bus->state = I2C_IDLE;
    bus->current_addr = 0;
    bus->direction = I2C_WRITE;
    bus->byte_count = 0;
    bus->error_count = 0;
    bus->transfer_count = 0;
    bus->num_devices = 0;
    bus->tx_len = 0;
    bus->rx_len = 0;
    for (i = 0; i < 32; i++) {
        bus->tx_buffer[i] = 0;
        bus->rx_buffer[i] = 0;
    }
}

int i2c_add_device(struct I2cBus *bus, int addr) {
    if (bus->num_devices >= 4) return -1;
    i2c_device_init(&(bus->devices[bus->num_devices]), addr);
    bus->num_devices = bus->num_devices + 1;
    return 0;
}

struct I2cDevice *i2c_find_device(struct I2cBus *bus, int addr) {
    int i;
    for (i = 0; i < bus->num_devices; i++) {
        if (bus->devices[i].address == addr && bus->devices[i].active) {
            return &(bus->devices[i]);
        }
    }
    return (struct I2cDevice *)0;
}

int i2c_start(struct I2cBus *bus) {
    if (bus->state != I2C_IDLE && bus->state != I2C_STOP) {
        // Repeated start
    }
    bus->sda = 0;  // SDA goes low while SCL is high
    bus->state = I2C_START;
    return 0;
}

int i2c_stop(struct I2cBus *bus) {
    bus->sda = 1;  // SDA goes high while SCL is high
    bus->state = I2C_STOP;
    bus->byte_count = 0;
    bus->state = I2C_IDLE;
    return 0;
}

int i2c_send_address(struct I2cBus *bus, int addr, int dir) {
    struct I2cDevice *dev;
    bus->current_addr = addr;
    bus->direction = dir;
    bus->state = I2C_ADDR;

    dev = i2c_find_device(bus, addr);
    if (dev == (struct I2cDevice *)0) {
        bus->error_count = bus->error_count + 1;
        return I2C_NACK;
    }
    bus->state = I2C_DATA;
    bus->transfer_count = bus->transfer_count + 1;
    return I2C_ACK;
}

// Write a byte to the addressed device
int i2c_write_byte(struct I2cBus *bus, int data) {
    struct I2cDevice *dev;
    if (bus->state != I2C_DATA) return I2C_NACK;
    if (bus->direction != I2C_WRITE) return I2C_NACK;

    dev = i2c_find_device(bus, bus->current_addr);
    if (dev == (struct I2cDevice *)0) return I2C_NACK;

    if (bus->byte_count == 0) {
        // First byte after address is register pointer
        dev->reg_pointer = data % dev->num_registers;
    } else {
        // Subsequent bytes are data
        if (dev->reg_pointer < dev->num_registers) {
            dev->registers[dev->reg_pointer] = data & 0xFF;
            dev->reg_pointer = (dev->reg_pointer + 1) % dev->num_registers;
        }
    }
    bus->byte_count = bus->byte_count + 1;
    return I2C_ACK;
}

// Read a byte from the addressed device
int i2c_read_byte(struct I2cBus *bus) {
    struct I2cDevice *dev;
    int val;
    if (bus->state != I2C_DATA) return -1;

    dev = i2c_find_device(bus, bus->current_addr);
    if (dev == (struct I2cDevice *)0) return -1;

    if (dev->reg_pointer < dev->num_registers) {
        val = dev->registers[dev->reg_pointer];
        dev->reg_pointer = (dev->reg_pointer + 1) % dev->num_registers;
    } else {
        val = 0xFF;
    }
    bus->byte_count = bus->byte_count + 1;
    return val;
}

// High-level write: write multiple bytes to a device register
int i2c_master_write(struct I2cBus *bus, int addr, int reg, int *data, int len) {
    int ack;
    int i;
    i2c_start(bus);
    ack = i2c_send_address(bus, addr, I2C_WRITE);
    if (ack != I2C_ACK) {
        i2c_stop(bus);
        return -1;
    }
    i2c_write_byte(bus, reg);  // set register pointer
    for (i = 0; i < len; i++) {
        ack = i2c_write_byte(bus, data[i]);
        if (ack != I2C_ACK) {
            i2c_stop(bus);
            return -2;
        }
    }
    i2c_stop(bus);
    return len;
}

// High-level read: read multiple bytes from a device register
int i2c_master_read(struct I2cBus *bus, int addr, int reg, int *data, int len) {
    int ack;
    int i;
    // Write register address
    i2c_start(bus);
    ack = i2c_send_address(bus, addr, I2C_WRITE);
    if (ack != I2C_ACK) {
        i2c_stop(bus);
        return -1;
    }
    i2c_write_byte(bus, reg);
    i2c_stop(bus);

    // Repeated start for read
    i2c_start(bus);
    ack = i2c_send_address(bus, addr, I2C_READ);
    if (ack != I2C_ACK) {
        i2c_stop(bus);
        return -1;
    }
    for (i = 0; i < len; i++) {
        data[i] = i2c_read_byte(bus);
    }
    i2c_stop(bus);
    return len;
}

int main() {
    struct I2cBus bus;
    int write_data[4];
    int read_data[4];
    int result;
    int i;

    i2c_bus_init(&bus);
    printf("I2C bus initialized, SDA=%d SCL=%d\n", bus.sda, bus.scl);
    // EXPECT: I2C bus initialized, SDA=1 SCL=1

    // Add devices: temperature sensor (0x48), EEPROM (0x50), IO expander (0x20)
    i2c_add_device(&bus, 48);
    i2c_add_device(&bus, 50);
    i2c_add_device(&bus, 20);
    printf("Devices on bus: %d\n", bus.num_devices);
    // EXPECT: Devices on bus: 3

    // Write to temperature sensor config register
    write_data[0] = 128;
    result = i2c_master_write(&bus, 48, 1, write_data, 1);
    printf("Write to sensor: %d bytes\n", result);
    // EXPECT: Write to sensor: 1 bytes

    // Read back
    read_data[0] = 0;
    result = i2c_master_read(&bus, 48, 1, read_data, 1);
    printf("Read from sensor: %d bytes, data=%d\n", result, read_data[0]);
    // EXPECT: Read from sensor: 1 bytes, data=128

    // Write multiple bytes to EEPROM
    write_data[0] = 10;
    write_data[1] = 20;
    write_data[2] = 30;
    write_data[3] = 40;
    result = i2c_master_write(&bus, 50, 0, write_data, 4);
    printf("Write to EEPROM: %d bytes\n", result);
    // EXPECT: Write to EEPROM: 4 bytes

    // Read back from EEPROM
    result = i2c_master_read(&bus, 50, 0, read_data, 4);
    printf("Read from EEPROM: %d bytes\n", result);
    // EXPECT: Read from EEPROM: 4 bytes
    for (i = 0; i < 4; i++) {
        printf("EEPROM[%d] = %d\n", i, read_data[i]);
    }
    // EXPECT: EEPROM[0] = 10
    // EXPECT: EEPROM[1] = 20
    // EXPECT: EEPROM[2] = 30
    // EXPECT: EEPROM[3] = 40

    // Write to IO expander
    write_data[0] = 255;
    result = i2c_master_write(&bus, 20, 0, write_data, 1);
    printf("Write to IO expander: %d bytes\n", result);
    // EXPECT: Write to IO expander: 1 bytes

    result = i2c_master_read(&bus, 20, 0, read_data, 1);
    printf("IO expander port: %d\n", read_data[0]);
    // EXPECT: IO expander port: 255

    // Try to communicate with non-existent device
    result = i2c_master_write(&bus, 99, 0, write_data, 1);
    printf("Non-existent device: %d\n", result);
    // EXPECT: Non-existent device: -1

    printf("Transfer count: %d\n", bus.transfer_count);
    // EXPECT: Transfer count: 9

    printf("Error count: %d\n", bus.error_count);
    // EXPECT: Error count: 1

    // Verify register wrapping
    write_data[0] = 77;
    i2c_master_write(&bus, 48, 15, write_data, 1);
    result = i2c_master_read(&bus, 48, 15, read_data, 1);
    printf("Reg 15 value: %d\n", read_data[0]);
    // EXPECT: Reg 15 value: 77

    printf("I2C test complete\n");
    // EXPECT: I2C test complete

    return 0;
}
