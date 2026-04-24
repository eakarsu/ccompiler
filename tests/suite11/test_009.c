int printf(const char *fmt, ...);

// Memory-mapped I/O simulation
// Simulates a device with memory-mapped registers for control, status, data

enum DeviceReg {
    DEV_CTRL   = 0,
    DEV_STATUS = 1,
    DEV_DATA   = 2,
    DEV_ADDR   = 3,
    DEV_COUNT  = 4,
    DEV_IRQ    = 5,
    DEV_CONFIG = 6,
    DEV_ID     = 7
};

enum CtrlBits {
    CTRL_ENABLE = 1,
    CTRL_RESET  = 2,
    CTRL_START  = 4,
    CTRL_IRQ_EN = 8
};

enum StatusBits {
    STAT_READY  = 1,
    STAT_BUSY   = 2,
    STAT_ERROR  = 4,
    STAT_DONE   = 8
};

struct Device {
    int mmio[8];
    int internal_buf[16];
    int buf_pos;
};

void dev_init(struct Device *d) {
    int i;
    for (i = 0; i < 8; i++) d->mmio[i] = 0;
    for (i = 0; i < 16; i++) d->internal_buf[i] = 0;
    d->buf_pos = 0;
    d->mmio[DEV_ID] = 42;
    d->mmio[DEV_STATUS] = STAT_READY;
}

void dev_write(struct Device *d, int reg, int val) {
    if (reg == DEV_CTRL) {
        d->mmio[DEV_CTRL] = val;
        if (val & CTRL_RESET) {
            d->mmio[DEV_STATUS] = STAT_READY;
            d->mmio[DEV_COUNT] = 0;
            d->buf_pos = 0;
            int i;
            for (i = 0; i < 16; i++) d->internal_buf[i] = 0;
        }
        if (val & CTRL_START) {
            d->mmio[DEV_STATUS] = STAT_BUSY;
        }
    } else if (reg == DEV_DATA) {
        if (d->buf_pos < 16) {
            d->internal_buf[d->buf_pos] = val;
            d->buf_pos = d->buf_pos + 1;
            d->mmio[DEV_COUNT] = d->buf_pos;
        } else {
            d->mmio[DEV_STATUS] = d->mmio[DEV_STATUS] | STAT_ERROR;
        }
    } else if (reg == DEV_ADDR) {
        d->mmio[DEV_ADDR] = val;
    } else if (reg == DEV_CONFIG) {
        d->mmio[DEV_CONFIG] = val;
    }
}

int dev_read(struct Device *d, int reg) {
    if (reg == DEV_DATA) {
        int addr = d->mmio[DEV_ADDR];
        if (addr >= 0 && addr < 16) {
            return d->internal_buf[addr];
        }
        return -1;
    }
    return d->mmio[reg];
}

void dev_process(struct Device *d) {
    // Simulate processing: double all values in buffer
    if (d->mmio[DEV_STATUS] & STAT_BUSY) {
        int i;
        for (i = 0; i < d->buf_pos; i++) {
            d->internal_buf[i] = d->internal_buf[i] * 2;
        }
        d->mmio[DEV_STATUS] = STAT_DONE | STAT_READY;
        if (d->mmio[DEV_CTRL] & CTRL_IRQ_EN) {
            d->mmio[DEV_IRQ] = 1;
        }
    }
}

int main() {
    struct Device dev;
    dev_init(&dev);

    // Read device ID
    int id = dev_read(&dev, DEV_ID);
    printf("device id: %d\n", id);
    // EXPECT: device id: 42

    // Check initial status
    int status = dev_read(&dev, DEV_STATUS);
    printf("status ready: %d\n", (status & STAT_READY) ? 1 : 0);
    // EXPECT: status ready: 1

    // Write data
    dev_write(&dev, DEV_DATA, 10);
    dev_write(&dev, DEV_DATA, 20);
    dev_write(&dev, DEV_DATA, 30);
    dev_write(&dev, DEV_DATA, 40);

    printf("count: %d\n", dev_read(&dev, DEV_COUNT));
    // EXPECT: count: 4

    // Read back data before processing
    dev_write(&dev, DEV_ADDR, 0);
    printf("buf[0]: %d\n", dev_read(&dev, DEV_DATA));
    // EXPECT: buf[0]: 10
    dev_write(&dev, DEV_ADDR, 2);
    printf("buf[2]: %d\n", dev_read(&dev, DEV_DATA));
    // EXPECT: buf[2]: 30

    // Enable IRQ and start processing
    dev_write(&dev, DEV_CTRL, CTRL_ENABLE | CTRL_START | CTRL_IRQ_EN);
    status = dev_read(&dev, DEV_STATUS);
    printf("busy: %d\n", (status & STAT_BUSY) ? 1 : 0);
    // EXPECT: busy: 1

    // Process (simulates hardware operation)
    dev_process(&dev);

    status = dev_read(&dev, DEV_STATUS);
    printf("done: %d\n", (status & STAT_DONE) ? 1 : 0);
    // EXPECT: done: 1

    // Check IRQ
    printf("irq: %d\n", dev_read(&dev, DEV_IRQ));
    // EXPECT: irq: 1

    // Read processed data (doubled)
    dev_write(&dev, DEV_ADDR, 0);
    printf("processed[0]: %d\n", dev_read(&dev, DEV_DATA));
    // EXPECT: processed[0]: 20
    dev_write(&dev, DEV_ADDR, 1);
    printf("processed[1]: %d\n", dev_read(&dev, DEV_DATA));
    // EXPECT: processed[1]: 40
    dev_write(&dev, DEV_ADDR, 2);
    printf("processed[2]: %d\n", dev_read(&dev, DEV_DATA));
    // EXPECT: processed[2]: 60
    dev_write(&dev, DEV_ADDR, 3);
    printf("processed[3]: %d\n", dev_read(&dev, DEV_DATA));
    // EXPECT: processed[3]: 80

    // Reset device
    dev_write(&dev, DEV_CTRL, CTRL_RESET);
    printf("after reset count: %d\n", dev_read(&dev, DEV_COUNT));
    // EXPECT: after reset count: 0
    printf("after reset status: %d\n", dev_read(&dev, DEV_STATUS));
    // EXPECT: after reset status: 1

    // Test DMA-like transfer
    int src_data[4];
    src_data[0] = 100;
    src_data[1] = 200;
    src_data[2] = 300;
    src_data[3] = 400;

    int j;
    for (j = 0; j < 4; j++) {
        dev_write(&dev, DEV_DATA, src_data[j]);
    }

    dev_write(&dev, DEV_CTRL, CTRL_START);
    dev_process(&dev);

    int sum = 0;
    for (j = 0; j < 4; j++) {
        dev_write(&dev, DEV_ADDR, j);
        sum = sum + dev_read(&dev, DEV_DATA);
    }
    printf("dma sum: %d\n", sum);
    // EXPECT: dma sum: 2000

    printf("mmio done\n");
    // EXPECT: mmio done

    return 0;
}
