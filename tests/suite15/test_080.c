int printf(const char *fmt, ...);

// DMA transfer simulation

enum dma_direction {
    DMA_MEM_TO_MEM,
    DMA_MEM_TO_PERIPH,
    DMA_PERIPH_TO_MEM
};

enum dma_width {
    DMA_WIDTH_BYTE,
    DMA_WIDTH_HALF,
    DMA_WIDTH_WORD
};

enum dma_state {
    DMA_IDLE,
    DMA_CONFIGURED,
    DMA_RUNNING,
    DMA_COMPLETE,
    DMA_ERROR
};

struct dma_channel {
    int src_addr;
    int dst_addr;
    int transfer_size;
    int direction;
    int width;
    int state;
    int bytes_transferred;
    int priority;
    int circular;
    int half_transfer_flag;
    int complete_flag;
    int error_flag;
};

struct dma_controller {
    struct dma_channel channels[4];
    int num_channels;
    int active_transfers;
    int total_transfers;
    int bus_busy;
};

int src_memory[32];
int dst_memory[32];
int periph_buffer[16];

void dma_channel_init(struct dma_channel *ch) {
    ch->src_addr = 0;
    ch->dst_addr = 0;
    ch->transfer_size = 0;
    ch->direction = DMA_MEM_TO_MEM;
    ch->width = DMA_WIDTH_BYTE;
    ch->state = DMA_IDLE;
    ch->bytes_transferred = 0;
    ch->priority = 0;
    ch->circular = 0;
    ch->half_transfer_flag = 0;
    ch->complete_flag = 0;
    ch->error_flag = 0;
}

void dma_ctrl_init(struct dma_controller *ctrl, int num_ch) {
    int i;
    ctrl->num_channels = num_ch;
    ctrl->active_transfers = 0;
    ctrl->total_transfers = 0;
    ctrl->bus_busy = 0;
    for (i = 0; i < 4; i++) {
        dma_channel_init(&ctrl->channels[i]);
    }
}

int dma_configure(struct dma_channel *ch, int src, int dst, int size, int dir, int width) {
    if (ch->state == DMA_RUNNING) return -1;
    if (size <= 0) return -2;

    ch->src_addr = src;
    ch->dst_addr = dst;
    ch->transfer_size = size;
    ch->direction = dir;
    ch->width = width;
    ch->state = DMA_CONFIGURED;
    ch->bytes_transferred = 0;
    ch->half_transfer_flag = 0;
    ch->complete_flag = 0;
    ch->error_flag = 0;
    return 0;
}

int dma_get_width_bytes(int width) {
    if (width == DMA_WIDTH_BYTE) return 1;
    if (width == DMA_WIDTH_HALF) return 2;
    return 4;
}

int dma_start(struct dma_channel *ch) {
    if (ch->state != DMA_CONFIGURED) return -1;
    ch->state = DMA_RUNNING;
    return 0;
}

int dma_transfer_step(struct dma_channel *ch, int *src_buf, int *dst_buf, int src_size, int dst_size) {
    int width_bytes;
    int src_idx;
    int dst_idx;
    int total_bytes;

    if (ch->state != DMA_RUNNING) return -1;

    width_bytes = dma_get_width_bytes(ch->width);
    total_bytes = ch->transfer_size * width_bytes;

    src_idx = (ch->src_addr + ch->bytes_transferred / width_bytes) % src_size;
    dst_idx = (ch->dst_addr + ch->bytes_transferred / width_bytes) % dst_size;

    if (src_idx < src_size && dst_idx < dst_size) {
        dst_buf[dst_idx] = src_buf[src_idx];
        ch->bytes_transferred = ch->bytes_transferred + width_bytes;
    } else {
        ch->state = DMA_ERROR;
        ch->error_flag = 1;
        return -2;
    }

    if (ch->bytes_transferred >= total_bytes / 2 && !ch->half_transfer_flag) {
        ch->half_transfer_flag = 1;
    }

    if (ch->bytes_transferred >= total_bytes) {
        if (ch->circular) {
            ch->bytes_transferred = 0;
            ch->half_transfer_flag = 0;
        } else {
            ch->state = DMA_COMPLETE;
            ch->complete_flag = 1;
        }
        return 1;
    }
    return 0;
}

int dma_run_transfer(struct dma_channel *ch, int *src_buf, int *dst_buf, int src_size, int dst_size) {
    int steps = 0;
    int result;

    if (dma_start(ch) != 0) return -1;

    while (1) {
        result = dma_transfer_step(ch, src_buf, dst_buf, src_size, dst_size);
        steps++;
        if (result == 1) break;
        if (result < 0) return -1;
        if (steps > 1000) return -1;
    }
    return steps;
}

int dma_get_remaining(struct dma_channel *ch) {
    int width_bytes = dma_get_width_bytes(ch->width);
    int total = ch->transfer_size * width_bytes;
    int remaining = total - ch->bytes_transferred;
    if (remaining < 0) remaining = 0;
    return remaining / width_bytes;
}

int dma_get_progress_percent(struct dma_channel *ch) {
    int width_bytes = dma_get_width_bytes(ch->width);
    int total = ch->transfer_size * width_bytes;
    if (total == 0) return 0;
    return (ch->bytes_transferred * 100) / total;
}

int dma_ctrl_find_highest_priority(struct dma_controller *ctrl) {
    int i;
    int best = -1;
    int best_prio = -1;

    for (i = 0; i < ctrl->num_channels; i++) {
        if (ctrl->channels[i].state == DMA_RUNNING) {
            if (ctrl->channels[i].priority > best_prio) {
                best_prio = ctrl->channels[i].priority;
                best = i;
            }
        }
    }
    return best;
}

int dma_calc_bandwidth(int transfer_size, int width, int clock_mhz) {
    int bytes_per_transfer = dma_get_width_bytes(width);
    return transfer_size * bytes_per_transfer * clock_mhz;
}

void memory_fill_pattern(int *buf, int size, int seed) {
    int i;
    for (i = 0; i < size; i++) {
        buf[i] = (seed + i * 3) & 0xFF;
    }
}

int memory_verify(int *buf1, int *buf2, int size) {
    int i;
    int errors = 0;
    for (i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) errors++;
    }
    return errors;
}

int main(void) {
    struct dma_controller ctrl;
    struct dma_channel ch;
    int i;
    int result;

    dma_ctrl_init(&ctrl, 4);
    printf("%d\n", ctrl.num_channels);
    // EXPECT: 4
    printf("%d\n", ctrl.active_transfers);
    // EXPECT: 0

    dma_channel_init(&ch);
    printf("%d\n", ch.state);
    // EXPECT: 0

    result = dma_configure(&ch, 0, 0, 8, DMA_MEM_TO_MEM, DMA_WIDTH_WORD);
    printf("%d\n", result);
    // EXPECT: 0
    printf("%d\n", ch.state);
    // EXPECT: 1

    result = dma_configure(&ch, 0, 0, 0, DMA_MEM_TO_MEM, DMA_WIDTH_WORD);
    printf("%d\n", result);
    // EXPECT: -2

    memory_fill_pattern(src_memory, 32, 42);
    printf("%d\n", src_memory[0]);
    // EXPECT: 42
    printf("%d\n", src_memory[1]);
    // EXPECT: 45
    printf("%d\n", src_memory[2]);
    // EXPECT: 48

    dma_configure(&ch, 0, 0, 8, DMA_MEM_TO_MEM, DMA_WIDTH_WORD);
    int steps = dma_run_transfer(&ch, src_memory, dst_memory, 32, 32);
    printf("%d\n", steps);
    // EXPECT: 8
    printf("%d\n", ch.state);
    // EXPECT: 3
    printf("%d\n", ch.complete_flag);
    // EXPECT: 1

    int errors = memory_verify(src_memory, dst_memory, 8);
    printf("%d\n", errors);
    // EXPECT: 0

    printf("%d\n", dst_memory[0]);
    // EXPECT: 42
    printf("%d\n", dst_memory[3]);
    // EXPECT: 51
    printf("%d\n", dst_memory[7]);
    // EXPECT: 63

    struct dma_channel ch2;
    dma_channel_init(&ch2);
    dma_configure(&ch2, 0, 0, 4, DMA_MEM_TO_MEM, DMA_WIDTH_WORD);
    dma_start(&ch2);

    dma_transfer_step(&ch2, src_memory, periph_buffer, 32, 16);
    printf("%d\n", ch2.bytes_transferred);
    // EXPECT: 4
    printf("%d\n", dma_get_remaining(&ch2));
    // EXPECT: 3
    printf("%d\n", dma_get_progress_percent(&ch2));
    // EXPECT: 25

    dma_transfer_step(&ch2, src_memory, periph_buffer, 32, 16);
    printf("%d\n", ch2.half_transfer_flag);
    // EXPECT: 1
    printf("%d\n", dma_get_progress_percent(&ch2));
    // EXPECT: 50

    dma_transfer_step(&ch2, src_memory, periph_buffer, 32, 16);
    dma_transfer_step(&ch2, src_memory, periph_buffer, 32, 16);
    printf("%d\n", ch2.state);
    // EXPECT: 3
    printf("%d\n", ch2.complete_flag);
    // EXPECT: 1
    printf("%d\n", dma_get_remaining(&ch2));
    // EXPECT: 0

    printf("%d\n", periph_buffer[0]);
    // EXPECT: 42
    printf("%d\n", periph_buffer[1]);
    // EXPECT: 45

    struct dma_channel circ_ch;
    dma_channel_init(&circ_ch);
    circ_ch.circular = 1;
    dma_configure(&circ_ch, 0, 0, 2, DMA_MEM_TO_MEM, DMA_WIDTH_WORD);
    circ_ch.circular = 1;
    dma_start(&circ_ch);

    dma_transfer_step(&circ_ch, src_memory, dst_memory, 32, 32);
    dma_transfer_step(&circ_ch, src_memory, dst_memory, 32, 32);
    printf("%d\n", circ_ch.state);
    // EXPECT: 2
    printf("%d\n", circ_ch.bytes_transferred);
    // EXPECT: 0

    ctrl.channels[0].state = DMA_RUNNING;
    ctrl.channels[0].priority = 2;
    ctrl.channels[1].state = DMA_RUNNING;
    ctrl.channels[1].priority = 5;
    ctrl.channels[2].state = DMA_IDLE;
    ctrl.channels[2].priority = 10;
    int highest = dma_ctrl_find_highest_priority(&ctrl);
    printf("%d\n", highest);
    // EXPECT: 1

    printf("%d\n", dma_get_width_bytes(DMA_WIDTH_BYTE));
    // EXPECT: 1
    printf("%d\n", dma_get_width_bytes(DMA_WIDTH_HALF));
    // EXPECT: 2
    printf("%d\n", dma_get_width_bytes(DMA_WIDTH_WORD));
    // EXPECT: 4

    int bw = dma_calc_bandwidth(256, DMA_WIDTH_WORD, 100);
    printf("%d\n", bw);
    // EXPECT: 102400

    bw = dma_calc_bandwidth(128, DMA_WIDTH_HALF, 50);
    printf("%d\n", bw);
    // EXPECT: 12800

    int verify_src[4];
    int verify_dst[4];
    verify_src[0] = 10;
    verify_src[1] = 20;
    verify_src[2] = 30;
    verify_src[3] = 40;
    verify_dst[0] = 10;
    verify_dst[1] = 20;
    verify_dst[2] = 99;
    verify_dst[3] = 40;
    errors = memory_verify(verify_src, verify_dst, 4);
    printf("%d\n", errors);
    // EXPECT: 1

    printf("%s\n", "dma_done");
    // EXPECT: dma_done

    return 0;
}
