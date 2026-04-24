int printf(const char *fmt, ...);

struct Fragment {
    int pkt_id;
    int frag_offset;
    int frag_length;
    int more_fragments;
    int data[8];
    int valid;
};

struct ReassemblyBuffer {
    int pkt_id;
    int total_length;
    int received_bytes;
    int complete;
    int data[32];
    int received_map[32];
    int frag_count;
};

int frag_init(struct Fragment *f) {
    f->pkt_id = 0;
    f->frag_offset = 0;
    f->frag_length = 0;
    f->more_fragments = 0;
    f->valid = 0;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        f->data[i] = 0;
    }
    return 0;
}

int rb_init(struct ReassemblyBuffer *rb, int pkt_id) {
    rb->pkt_id = pkt_id;
    rb->total_length = 0;
    rb->received_bytes = 0;
    rb->complete = 0;
    rb->frag_count = 0;
    int i;
    for (i = 0; i < 32; i = i + 1) {
        rb->data[i] = 0;
        rb->received_map[i] = 0;
    }
    return 0;
}

int fragment_packet(struct Fragment *frags, int pkt_id, int *data, int data_len, int mtu) {
    int frag_idx = 0;
    int offset = 0;
    while (offset < data_len) {
        int chunk = mtu;
        if (offset + chunk > data_len) {
            chunk = data_len - offset;
        }
        frags[frag_idx].pkt_id = pkt_id;
        frags[frag_idx].frag_offset = offset;
        frags[frag_idx].frag_length = chunk;
        if (offset + chunk < data_len) {
            frags[frag_idx].more_fragments = 1;
        } else {
            frags[frag_idx].more_fragments = 0;
        }
        frags[frag_idx].valid = 1;
        int i;
        for (i = 0; i < chunk; i = i + 1) {
            frags[frag_idx].data[i] = data[offset + i];
        }
        printf("Fragment: id=%d offset=%d len=%d MF=%d\n",
               pkt_id, offset, chunk, frags[frag_idx].more_fragments);
        frag_idx = frag_idx + 1;
        offset = offset + chunk;
    }
    return frag_idx;
}

int reassemble_add(struct ReassemblyBuffer *rb, struct Fragment *f) {
    if (f->pkt_id != rb->pkt_id) {
        printf("Wrong packet id: %d vs %d\n", f->pkt_id, rb->pkt_id);
        return -1;
    }
    int i;
    int already = 1;
    for (i = 0; i < f->frag_length; i = i + 1) {
        int pos = f->frag_offset + i;
        if (pos < 32) {
            if (rb->received_map[pos] == 0) {
                already = 0;
            }
            rb->data[pos] = f->data[i];
            rb->received_map[pos] = 1;
        }
    }
    if (already) {
        printf("Reassemble: duplicate fragment offset=%d\n", f->frag_offset);
        return 0;
    }
    rb->received_bytes = rb->received_bytes + f->frag_length;
    rb->frag_count = rb->frag_count + 1;
    if (f->more_fragments == 0) {
        rb->total_length = f->frag_offset + f->frag_length;
    }
    printf("Reassemble: added offset=%d len=%d (received=%d",
           f->frag_offset, f->frag_length, rb->received_bytes);
    if (rb->total_length > 0) {
        printf("/%d", rb->total_length);
    }
    printf(")\n");
    if (rb->total_length > 0 && rb->received_bytes >= rb->total_length) {
        rb->complete = 1;
        printf("Reassembly complete! total=%d\n", rb->total_length);
    }
    return 1;
}

int main() {
    int original[10];
    int i;
    for (i = 0; i < 10; i = i + 1) {
        original[i] = (i + 1) * 10;
    }

    printf("=== Packet Fragmentation/Reassembly ===\n");
    // EXPECT: === Packet Fragmentation/Reassembly ===

    printf("--- Fragment packet (MTU=4) ---\n");
    // EXPECT: --- Fragment packet (MTU=4) ---
    struct Fragment frags[4];
    int nfrags = fragment_packet(frags, 1001, original, 10, 4);
    // EXPECT: Fragment: id=1001 offset=0 len=4 MF=1
    // EXPECT: Fragment: id=1001 offset=4 len=4 MF=1
    // EXPECT: Fragment: id=1001 offset=8 len=2 MF=0
    printf("Number of fragments: %d\n", nfrags);
    // EXPECT: Number of fragments: 3

    printf("--- Reassemble in order ---\n");
    // EXPECT: --- Reassemble in order ---
    struct ReassemblyBuffer rb;
    rb_init(&rb, 1001);
    reassemble_add(&rb, &frags[0]);
    // EXPECT: Reassemble: added offset=0 len=4 (received=4)
    reassemble_add(&rb, &frags[1]);
    // EXPECT: Reassemble: added offset=4 len=4 (received=8)
    reassemble_add(&rb, &frags[2]);
    // EXPECT: Reassemble: added offset=8 len=2 (received=10/10)
    // EXPECT: Reassembly complete! total=10

    printf("Data: ");
    for (i = 0; i < 10; i = i + 1) {
        printf("%d ", rb.data[i]);
    }
    printf("\n");
    // EXPECT: Data: 10 20 30 40 50 60 70 80 90 100 

    printf("--- Reassemble out of order ---\n");
    // EXPECT: --- Reassemble out of order ---
    struct ReassemblyBuffer rb2;
    rb_init(&rb2, 1001);
    reassemble_add(&rb2, &frags[2]);
    // EXPECT: Reassemble: added offset=8 len=2 (received=2/10)
    reassemble_add(&rb2, &frags[0]);
    // EXPECT: Reassemble: added offset=0 len=4 (received=6/10)
    reassemble_add(&rb2, &frags[1]);
    // EXPECT: Reassemble: added offset=4 len=4 (received=10/10)
    // EXPECT: Reassembly complete! total=10
    printf("Complete: %d\n", rb2.complete);
    // EXPECT: Complete: 1

    printf("--- Duplicate fragment ---\n");
    // EXPECT: --- Duplicate fragment ---
    reassemble_add(&rb2, &frags[0]);
    // EXPECT: Reassemble: duplicate fragment offset=0

    printf("--- Small MTU test ---\n");
    // EXPECT: --- Small MTU test ---
    int small_data[6];
    for (i = 0; i < 6; i = i + 1) {
        small_data[i] = i + 1;
    }
    struct Fragment sf[4];
    int sn = fragment_packet(sf, 2002, small_data, 6, 2);
    // EXPECT: Fragment: id=2002 offset=0 len=2 MF=1
    // EXPECT: Fragment: id=2002 offset=2 len=2 MF=1
    // EXPECT: Fragment: id=2002 offset=4 len=2 MF=0
    printf("Small frags: %d\n", sn);
    // EXPECT: Small frags: 3

    struct ReassemblyBuffer rb3;
    rb_init(&rb3, 2002);
    for (i = 0; i < sn; i = i + 1) {
        reassemble_add(&rb3, &sf[i]);
    }
    // EXPECT: Reassemble: added offset=0 len=2 (received=2)
    // EXPECT: Reassemble: added offset=2 len=2 (received=4)
    // EXPECT: Reassemble: added offset=4 len=2 (received=6/6)
    // EXPECT: Reassembly complete! total=6

    printf("Result: %d %d %d %d %d %d\n", rb3.data[0], rb3.data[1], rb3.data[2], rb3.data[3], rb3.data[4], rb3.data[5]);
    // EXPECT: Result: 1 2 3 4 5 6

    return 0;
}
