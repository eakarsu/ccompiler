int printf(const char *fmt, ...);

// SPI master/slave communication simulation

enum spi_mode {
    SPI_MODE_0,
    SPI_MODE_1,
    SPI_MODE_2,
    SPI_MODE_3
};

enum spi_bit_order {
    SPI_MSB_FIRST,
    SPI_LSB_FIRST
};

struct spi_config {
    int clock_div;
    int mode;
    int bit_order;
    int bits_per_word;
};

struct spi_device {
    struct spi_config config;
    int tx_buffer[16];
    int rx_buffer[16];
    int tx_count;
    int rx_count;
    int cs_active;
    int bytes_transferred;
};

void spi_init(struct spi_device *dev, int clock_div, int mode, int bit_order) {
    int i;
    dev->config.clock_div = clock_div;
    dev->config.mode = mode;
    dev->config.bit_order = bit_order;
    dev->config.bits_per_word = 8;
    dev->tx_count = 0;
    dev->rx_count = 0;
    dev->cs_active = 0;
    dev->bytes_transferred = 0;
    for (i = 0; i < 16; i++) {
        dev->tx_buffer[i] = 0;
        dev->rx_buffer[i] = 0;
    }
}

void spi_cs_assert(struct spi_device *dev) {
    dev->cs_active = 1;
}

void spi_cs_deassert(struct spi_device *dev) {
    dev->cs_active = 0;
}

int reverse_bits(int val, int nbits) {
    int result = 0;
    int i;
    for (i = 0; i < nbits; i++) {
        result = result << 1;
        result = result | (val & 1);
        val = val >> 1;
    }
    return result;
}

int spi_transfer_byte(struct spi_device *master, struct spi_device *slave, int mosi_data) {
    int miso_data;
    int tx_val;
    int rx_val;

    if (!master->cs_active) return -1;

    tx_val = mosi_data & 0xFF;
    if (master->config.bit_order == SPI_LSB_FIRST) {
        tx_val = reverse_bits(tx_val, 8);
    }

    if (slave->tx_count > 0) {
        miso_data = slave->tx_buffer[0];
        int j;
        for (j = 0; j < slave->tx_count - 1; j++) {
            slave->tx_buffer[j] = slave->tx_buffer[j + 1];
        }
        slave->tx_count--;
    } else {
        miso_data = 0xFF;
    }

    if (slave->rx_count < 16) {
        slave->rx_buffer[slave->rx_count] = tx_val;
        slave->rx_count++;
    }

    rx_val = miso_data;
    if (master->config.bit_order == SPI_LSB_FIRST) {
        rx_val = reverse_bits(rx_val, 8);
    }

    master->bytes_transferred++;
    slave->bytes_transferred++;

    return rx_val;
}

int spi_transfer_multi(struct spi_device *master, struct spi_device *slave, int *tx, int *rx, int count) {
    int i;
    int result;

    spi_cs_assert(master);
    for (i = 0; i < count; i++) {
        result = spi_transfer_byte(master, slave, tx[i]);
        if (result < 0) {
            spi_cs_deassert(master);
            return -1;
        }
        rx[i] = result;
    }
    spi_cs_deassert(master);
    return count;
}

void spi_slave_load_tx(struct spi_device *slave, int *data, int count) {
    int i;
    for (i = 0; i < count && i < 16; i++) {
        slave->tx_buffer[i] = data[i];
    }
    slave->tx_count = count;
}

int spi_calc_clock_hz(int sys_clock, int divider) {
    if (divider <= 0) return 0;
    return sys_clock / divider;
}

int spi_crc8(int *data, int len) {
    int crc = 0;
    int i;
    int j;
    for (i = 0; i < len; i++) {
        crc = crc ^ data[i];
        for (j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = ((crc << 1) ^ 0x07) & 0xFF;
            } else {
                crc = (crc << 1) & 0xFF;
            }
        }
    }
    return crc;
}

int main(void) {
    struct spi_device master;
    struct spi_device slave;
    int tx_data[8];
    int rx_data[8];
    int slave_resp[4];
    int result;

    spi_init(&master, 4, SPI_MODE_0, SPI_MSB_FIRST);
    spi_init(&slave, 1, SPI_MODE_0, SPI_MSB_FIRST);

    printf("%d\n", master.config.clock_div);
    // EXPECT: 4
    printf("%d\n", master.cs_active);
    // EXPECT: 0
    printf("%d\n", master.bytes_transferred);
    // EXPECT: 0

    spi_cs_assert(&master);
    printf("%d\n", master.cs_active);
    // EXPECT: 1

    result = spi_transfer_byte(&master, &slave, 0xA5);
    printf("%d\n", result);
    // EXPECT: 255
    printf("%d\n", slave.rx_buffer[0]);
    // EXPECT: 165
    printf("%d\n", master.bytes_transferred);
    // EXPECT: 1

    spi_cs_deassert(&master);

    slave_resp[0] = 0x12;
    slave_resp[1] = 0x34;
    slave_resp[2] = 0x56;
    slave_resp[3] = 0x78;
    spi_slave_load_tx(&slave, slave_resp, 4);
    printf("%d\n", slave.tx_count);
    // EXPECT: 4

    tx_data[0] = 0xAA;
    tx_data[1] = 0xBB;
    tx_data[2] = 0xCC;
    tx_data[3] = 0xDD;
    result = spi_transfer_multi(&master, &slave, tx_data, rx_data, 4);
    printf("%d\n", result);
    // EXPECT: 4

    printf("%d\n", rx_data[0]);
    // EXPECT: 18
    printf("%d\n", rx_data[1]);
    // EXPECT: 52
    printf("%d\n", rx_data[2]);
    // EXPECT: 86
    printf("%d\n", rx_data[3]);
    // EXPECT: 120

    printf("%d\n", slave.rx_count);
    // EXPECT: 5
    printf("%d\n", slave.tx_count);
    // EXPECT: 0

    printf("%d\n", master.bytes_transferred);
    // EXPECT: 5

    printf("%d\n", reverse_bits(0x01, 8));
    // EXPECT: 128
    printf("%d\n", reverse_bits(0x80, 8));
    // EXPECT: 1
    printf("%d\n", reverse_bits(0xF0, 8));
    // EXPECT: 15
    printf("%d\n", reverse_bits(0xA5, 8));
    // EXPECT: 165

    int spi_clk = spi_calc_clock_hz(48000000, 4);
    printf("%d\n", spi_clk);
    // EXPECT: 12000000

    spi_clk = spi_calc_clock_hz(48000000, 16);
    printf("%d\n", spi_clk);
    // EXPECT: 3000000

    int crc_data[3];
    crc_data[0] = 0x01;
    crc_data[1] = 0x02;
    crc_data[2] = 0x03;
    int crc = spi_crc8(crc_data, 3);
    printf("%d\n", crc);
    // EXPECT: 72

    int crc_data2[1];
    crc_data2[0] = 0x00;
    crc = spi_crc8(crc_data2, 1);
    printf("%d\n", crc);
    // EXPECT: 0

    struct spi_device lsb_master;
    spi_init(&lsb_master, 2, SPI_MODE_0, SPI_LSB_FIRST);
    struct spi_device lsb_slave;
    spi_init(&lsb_slave, 1, SPI_MODE_0, SPI_LSB_FIRST);
    spi_cs_assert(&lsb_master);
    result = spi_transfer_byte(&lsb_master, &lsb_slave, 0x01);
    printf("%d\n", result);
    // EXPECT: 255
    printf("%d\n", lsb_slave.rx_buffer[0]);
    // EXPECT: 128
    spi_cs_deassert(&lsb_master);

    printf("%s\n", "spi_done");
    // EXPECT: spi_done

    return 0;
}
