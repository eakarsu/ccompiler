int printf(const char *fmt, ...);

// I2C bus protocol simulation

enum i2c_direction {
    I2C_WRITE,
    I2C_READ
};

enum i2c_ack {
    I2C_ACK,
    I2C_NACK
};

enum i2c_state {
    I2C_IDLE,
    I2C_START,
    I2C_ADDR,
    I2C_DATA,
    I2C_STOP
};

struct i2c_device {
    int address;
    int registers[8];
    int reg_count;
    int selected_reg;
    int active;
};

struct i2c_bus {
    int sda;
    int scl;
    int state;
    int current_addr;
    int direction;
    int byte_count;
    int ack_count;
    int nack_count;
    int error_count;
};

void i2c_device_init(struct i2c_device *dev, int addr) {
    int i;
    dev->address = addr;
    dev->reg_count = 8;
    dev->selected_reg = 0;
    dev->active = 1;
    for (i = 0; i < 8; i++) {
        dev->registers[i] = 0;
    }
}

void i2c_device_set_reg(struct i2c_device *dev, int reg, int val) {
    if (reg >= 0 && reg < dev->reg_count) {
        dev->registers[reg] = val;
    }
}

int i2c_device_get_reg(struct i2c_device *dev, int reg) {
    if (reg >= 0 && reg < dev->reg_count) {
        return dev->registers[reg];
    }
    return -1;
}

void i2c_bus_init(struct i2c_bus *bus) {
    bus->sda = 1;
    bus->scl = 1;
    bus->state = I2C_IDLE;
    bus->current_addr = 0;
    bus->direction = I2C_WRITE;
    bus->byte_count = 0;
    bus->ack_count = 0;
    bus->nack_count = 0;
    bus->error_count = 0;
}

int i2c_start(struct i2c_bus *bus) {
    if (bus->state != I2C_IDLE && bus->state != I2C_STOP) {
        bus->error_count++;
        return -1;
    }
    bus->sda = 0;
    bus->state = I2C_START;
    return 0;
}

int i2c_stop(struct i2c_bus *bus) {
    bus->sda = 1;
    bus->scl = 1;
    bus->state = I2C_STOP;
    bus->current_addr = 0;
    return 0;
}

int i2c_find_device(struct i2c_device *devices, int count, int addr) {
    int i;
    for (i = 0; i < count; i++) {
        if (devices[i].address == addr && devices[i].active) {
            return i;
        }
    }
    return -1;
}

int i2c_send_address(struct i2c_bus *bus, struct i2c_device *devices, int dev_count, int addr, int rw) {
    int dev_idx;
    if (bus->state != I2C_START) {
        bus->error_count++;
        return I2C_NACK;
    }
    bus->current_addr = addr;
    bus->direction = rw;
    bus->state = I2C_ADDR;

    dev_idx = i2c_find_device(devices, dev_count, addr);
    if (dev_idx < 0) {
        bus->nack_count++;
        return I2C_NACK;
    }
    bus->ack_count++;
    bus->state = I2C_DATA;
    return I2C_ACK;
}

int i2c_write_byte(struct i2c_bus *bus, struct i2c_device *dev, int data) {
    if (bus->state != I2C_DATA) {
        bus->error_count++;
        return I2C_NACK;
    }
    if (bus->byte_count == 0) {
        dev->selected_reg = data % dev->reg_count;
    } else {
        int reg = (dev->selected_reg + bus->byte_count - 1) % dev->reg_count;
        dev->registers[reg] = data;
    }
    bus->byte_count++;
    bus->ack_count++;
    return I2C_ACK;
}

int i2c_read_byte(struct i2c_bus *bus, struct i2c_device *dev, int send_ack) {
    int val;
    int reg;
    if (bus->state != I2C_DATA) {
        bus->error_count++;
        return -1;
    }
    reg = (dev->selected_reg + bus->byte_count) % dev->reg_count;
    val = dev->registers[reg];
    bus->byte_count++;
    if (send_ack) {
        bus->ack_count++;
    } else {
        bus->nack_count++;
    }
    return val;
}

int i2c_write_register(struct i2c_bus *bus, struct i2c_device *devs, int cnt, int addr, int reg, int val) {
    int dev_idx;
    int ack;

    i2c_start(bus);
    ack = i2c_send_address(bus, devs, cnt, addr, I2C_WRITE);
    if (ack != I2C_ACK) {
        i2c_stop(bus);
        bus->byte_count = 0;
        return -1;
    }
    dev_idx = i2c_find_device(devs, cnt, addr);
    i2c_write_byte(bus, &devs[dev_idx], reg);
    i2c_write_byte(bus, &devs[dev_idx], val);
    i2c_stop(bus);
    bus->byte_count = 0;
    return 0;
}

int i2c_read_register(struct i2c_bus *bus, struct i2c_device *devs, int cnt, int addr, int reg) {
    int dev_idx;
    int ack;
    int val;

    i2c_start(bus);
    ack = i2c_send_address(bus, devs, cnt, addr, I2C_WRITE);
    if (ack != I2C_ACK) {
        i2c_stop(bus);
        bus->byte_count = 0;
        return -1;
    }
    dev_idx = i2c_find_device(devs, cnt, addr);
    i2c_write_byte(bus, &devs[dev_idx], reg);
    bus->byte_count = 0;

    i2c_start(bus);
    bus->state = I2C_START;
    i2c_send_address(bus, devs, cnt, addr, I2C_READ);
    val = i2c_read_byte(bus, &devs[dev_idx], 0);
    i2c_stop(bus);
    bus->byte_count = 0;
    return val;
}

int main(void) {
    struct i2c_bus bus;
    struct i2c_device devices[4];
    int result;

    i2c_bus_init(&bus);
    printf("%d\n", bus.sda);
    // EXPECT: 1
    printf("%d\n", bus.scl);
    // EXPECT: 1
    printf("%d\n", bus.state);
    // EXPECT: 0

    i2c_device_init(&devices[0], 0x48);
    i2c_device_init(&devices[1], 0x50);
    i2c_device_init(&devices[2], 0x68);
    i2c_device_init(&devices[3], 0x77);

    printf("%d\n", devices[0].address);
    // EXPECT: 72
    printf("%d\n", devices[1].address);
    // EXPECT: 80

    i2c_device_set_reg(&devices[0], 0, 42);
    i2c_device_set_reg(&devices[0], 1, 100);
    printf("%d\n", i2c_device_get_reg(&devices[0], 0));
    // EXPECT: 42
    printf("%d\n", i2c_device_get_reg(&devices[0], 1));
    // EXPECT: 100

    result = i2c_write_register(&bus, devices, 4, 0x50, 0, 0xAB);
    printf("%d\n", result);
    // EXPECT: 0
    printf("%d\n", devices[1].registers[0]);
    // EXPECT: 171

    result = i2c_write_register(&bus, devices, 4, 0x50, 3, 0x55);
    printf("%d\n", result);
    // EXPECT: 0
    printf("%d\n", devices[1].registers[3]);
    // EXPECT: 85

    int val = i2c_read_register(&bus, devices, 4, 0x50, 0);
    printf("%d\n", val);
    // EXPECT: 171

    val = i2c_read_register(&bus, devices, 4, 0x50, 3);
    printf("%d\n", val);
    // EXPECT: 85

    result = i2c_write_register(&bus, devices, 4, 0x99, 0, 0xFF);
    printf("%d\n", result);
    // EXPECT: -1

    val = i2c_read_register(&bus, devices, 4, 0x99, 0);
    printf("%d\n", val);
    // EXPECT: -1

    i2c_write_register(&bus, devices, 4, 0x68, 0, 10);
    i2c_write_register(&bus, devices, 4, 0x68, 1, 20);
    i2c_write_register(&bus, devices, 4, 0x68, 2, 30);
    printf("%d\n", i2c_read_register(&bus, devices, 4, 0x68, 0));
    // EXPECT: 10
    printf("%d\n", i2c_read_register(&bus, devices, 4, 0x68, 1));
    // EXPECT: 20
    printf("%d\n", i2c_read_register(&bus, devices, 4, 0x68, 2));
    // EXPECT: 30

    printf("%d\n", bus.ack_count > 0);
    // EXPECT: 1
    printf("%d\n", bus.nack_count > 0);
    // EXPECT: 1

    devices[3].active = 0;
    result = i2c_write_register(&bus, devices, 4, 0x77, 0, 99);
    printf("%d\n", result);
    // EXPECT: -1

    printf("%s\n", "i2c_done");
    // EXPECT: i2c_done

    return 0;
}
