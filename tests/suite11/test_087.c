int printf(const char *fmt, ...);

// SPI Shift Register Simulation
// Full-duplex SPI with mode selection, chip select, shift register

enum SpiMode { SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3 };
enum SpiRole { SPI_MASTER, SPI_SLAVE };
enum SpiState { SPI_INACTIVE, SPI_ACTIVE };

struct SpiShiftReg {
    int data;       // 8-bit shift register
    int bit_count;  // bits shifted
};

struct SpiDevice {
    int id;
    int role;
    int mode;
    int state;
    struct SpiShiftReg shift_reg;
    int cs_active;      // chip select active (active low, so 1 means selected)
    int tx_data;        // data to transmit
    int rx_data;        // last received data
    int clock_div;
    int transfer_count;
    int register_file[8];
    int reg_count;
};

struct SpiBus {
    struct SpiDevice master;
    struct SpiDevice slaves[4];
    int num_slaves;
    int mosi;       // master out, slave in line
    int miso;       // master in, slave out line
    int sclk;       // clock line
};

void spi_device_init(struct SpiDevice *dev, int id, int role, int mode) {
    int i;
    dev->id = id;
    dev->role = role;
    dev->mode = mode;
    dev->state = SPI_INACTIVE;
    dev->shift_reg.data = 0;
    dev->shift_reg.bit_count = 0;
    dev->cs_active = 0;
    dev->tx_data = 0;
    dev->rx_data = 0;
    dev->clock_div = 1;
    dev->transfer_count = 0;
    dev->reg_count = 8;
    for (i = 0; i < 8; i++) {
        dev->register_file[i] = 0;
    }
}

void spi_bus_init(struct SpiBus *bus) {
    bus->num_slaves = 0;
    bus->mosi = 0;
    bus->miso = 0;
    bus->sclk = 0;
    spi_device_init(&(bus->master), 0, SPI_MASTER, SPI_MODE0);
}

int spi_add_slave(struct SpiBus *bus, int mode) {
    int idx;
    if (bus->num_slaves >= 4) return -1;
    idx = bus->num_slaves;
    spi_device_init(&(bus->slaves[idx]), idx + 1, SPI_SLAVE, mode);
    bus->num_slaves = bus->num_slaves + 1;
    return idx;
}

void spi_select_slave(struct SpiBus *bus, int idx) {
    int i;
    // Deselect all first
    for (i = 0; i < bus->num_slaves; i++) {
        bus->slaves[i].cs_active = 0;
    }
    if (idx >= 0 && idx < bus->num_slaves) {
        bus->slaves[idx].cs_active = 1;
        bus->slaves[idx].state = SPI_ACTIVE;
    }
}

void spi_deselect_all(struct SpiBus *bus) {
    int i;
    for (i = 0; i < bus->num_slaves; i++) {
        bus->slaves[i].cs_active = 0;
        bus->slaves[i].state = SPI_INACTIVE;
    }
}

// Shift one bit: MSB first
int spi_shift_bit(struct SpiShiftReg *reg, int input_bit) {
    int output_bit;
    output_bit = (reg->data >> 7) & 1;  // MSB out
    reg->data = ((reg->data << 1) & 0xFF) | (input_bit & 1);
    reg->bit_count = reg->bit_count + 1;
    return output_bit;
}

// Transfer one byte full-duplex
int spi_transfer_byte(struct SpiBus *bus, int slave_idx, int tx_byte) {
    int i;
    int mosi_bit;
    int miso_bit;
    struct SpiDevice *slave;

    if (slave_idx < 0 || slave_idx >= bus->num_slaves) return -1;
    slave = &(bus->slaves[slave_idx]);
    if (!slave->cs_active) return -1;

    // Load shift registers
    bus->master.shift_reg.data = tx_byte & 0xFF;
    bus->master.shift_reg.bit_count = 0;
    slave->shift_reg.data = slave->tx_data & 0xFF;
    slave->shift_reg.bit_count = 0;

    // Shift 8 bits
    for (i = 0; i < 8; i++) {
        // Master sends MSB on MOSI
        mosi_bit = (bus->master.shift_reg.data >> 7) & 1;
        bus->mosi = mosi_bit;

        // Slave sends MSB on MISO
        miso_bit = (slave->shift_reg.data >> 7) & 1;
        bus->miso = miso_bit;

        // Both shift in received bit
        bus->master.shift_reg.data = ((bus->master.shift_reg.data << 1) & 0xFF) | miso_bit;
        slave->shift_reg.data = ((slave->shift_reg.data << 1) & 0xFF) | mosi_bit;

        bus->master.shift_reg.bit_count = bus->master.shift_reg.bit_count + 1;
        slave->shift_reg.bit_count = slave->shift_reg.bit_count + 1;
    }

    bus->master.rx_data = bus->master.shift_reg.data;
    slave->rx_data = slave->shift_reg.data;
    bus->master.transfer_count = bus->master.transfer_count + 1;
    slave->transfer_count = slave->transfer_count + 1;

    return bus->master.rx_data;
}

// Write to slave register (command byte + data byte)
int spi_write_register(struct SpiBus *bus, int slave_idx, int reg, int data) {
    struct SpiDevice *slave;
    if (slave_idx < 0 || slave_idx >= bus->num_slaves) return -1;
    slave = &(bus->slaves[slave_idx]);

    spi_select_slave(bus, slave_idx);
    slave->tx_data = 0;  // Slave sends dummy during write

    // Send register address (bit 7 = 0 for write)
    spi_transfer_byte(bus, slave_idx, reg & 0x7F);

    // Store data in slave register
    if (reg < slave->reg_count) {
        slave->register_file[reg] = data & 0xFF;
    }

    // Send data byte
    spi_transfer_byte(bus, slave_idx, data);
    spi_deselect_all(bus);
    return 0;
}

// Read from slave register
int spi_read_register(struct SpiBus *bus, int slave_idx, int reg) {
    struct SpiDevice *slave;
    int result;
    if (slave_idx < 0 || slave_idx >= bus->num_slaves) return -1;
    slave = &(bus->slaves[slave_idx]);

    spi_select_slave(bus, slave_idx);

    // Send register address with read bit (bit 7 = 1)
    slave->tx_data = 0;
    spi_transfer_byte(bus, slave_idx, reg | 0x80);

    // Slave loads register value for next transfer
    if (reg < slave->reg_count) {
        slave->tx_data = slave->register_file[reg];
    } else {
        slave->tx_data = 0xFF;
    }

    result = spi_transfer_byte(bus, slave_idx, 0x00);  // dummy write
    spi_deselect_all(bus);
    return result;
}

int main() {
    struct SpiBus bus;
    int rx;
    int i;
    int val;

    spi_bus_init(&bus);
    printf("SPI bus initialized\n");
    // EXPECT: SPI bus initialized

    // Add 3 slaves
    spi_add_slave(&bus, SPI_MODE0);
    spi_add_slave(&bus, SPI_MODE0);
    spi_add_slave(&bus, SPI_MODE0);
    printf("Slaves on bus: %d\n", bus.num_slaves);
    // EXPECT: Slaves on bus: 3

    // Basic full-duplex transfer with slave 0
    spi_select_slave(&bus, 0);
    bus.slaves[0].tx_data = 0xAB;  // 171
    rx = spi_transfer_byte(&bus, 0, 0x55);  // 85 sent
    printf("Master sent 85, received: %d\n", rx);
    // EXPECT: Master sent 85, received: 171
    printf("Slave received: %d\n", bus.slaves[0].rx_data);
    // EXPECT: Slave received: 85
    spi_deselect_all(&bus);

    // Transfer with slave 1
    spi_select_slave(&bus, 1);
    bus.slaves[1].tx_data = 0xFF;  // 255
    rx = spi_transfer_byte(&bus, 1, 0x00);  // 0 sent
    printf("From slave 1: %d\n", rx);
    // EXPECT: From slave 1: 255
    printf("Slave 1 got: %d\n", bus.slaves[1].rx_data);
    // EXPECT: Slave 1 got: 0
    spi_deselect_all(&bus);

    // Register write/read on slave 0
    spi_write_register(&bus, 0, 0, 42);
    spi_write_register(&bus, 0, 1, 100);
    spi_write_register(&bus, 0, 2, 200);
    printf("Wrote registers 0-2\n");
    // EXPECT: Wrote registers 0-2

    val = spi_read_register(&bus, 0, 0);
    printf("Reg 0: %d\n", val);
    // EXPECT: Reg 0: 42

    val = spi_read_register(&bus, 0, 1);
    printf("Reg 1: %d\n", val);
    // EXPECT: Reg 1: 100

    val = spi_read_register(&bus, 0, 2);
    printf("Reg 2: %d\n", val);
    // EXPECT: Reg 2: 200

    // Write to multiple slaves
    spi_write_register(&bus, 1, 0, 11);
    spi_write_register(&bus, 2, 0, 22);

    val = spi_read_register(&bus, 1, 0);
    printf("Slave 1 reg 0: %d\n", val);
    // EXPECT: Slave 1 reg 0: 11

    val = spi_read_register(&bus, 2, 0);
    printf("Slave 2 reg 0: %d\n", val);
    // EXPECT: Slave 2 reg 0: 22

    // Transfer count
    printf("Master transfers: %d\n", bus.master.transfer_count);
    // EXPECT: Master transfers: 22

    // Test shift register directly
    {
        struct SpiShiftReg sr;
        int out;
        sr.data = 0xA5;  // 10100101
        sr.bit_count = 0;

        out = spi_shift_bit(&sr, 0);
        printf("Shift out MSB: %d\n", out);
        // EXPECT: Shift out MSB: 1

        out = spi_shift_bit(&sr, 1);
        printf("Shift out bit 6: %d\n", out);
        // EXPECT: Shift out bit 6: 0

        printf("Bits shifted: %d\n", sr.bit_count);
        // EXPECT: Bits shifted: 2
    }

    // Deselected slave transfer should fail
    spi_deselect_all(&bus);
    rx = spi_transfer_byte(&bus, 0, 0x55);
    printf("Deselected transfer: %d\n", rx);
    // EXPECT: Deselected transfer: -1

    printf("SPI test complete\n");
    // EXPECT: SPI test complete

    return 0;
}
