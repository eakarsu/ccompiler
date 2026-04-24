int printf(const char *fmt, ...);
// EXPECT: RB:init cap=8 empty=1 full=0 sz=0 enq10=1 enq20=1 enq30=1 enq40=1 enq50=1 sz=5 empty=0 full=0 dq=10 dq=20 sz=3 front=30 rear=50 enq60=1 enq70=1 enq80=1 enq90=1 enq100=1 sz=8 full=1 enq110=0 dq=30 dq=40 dq=50 dq=60 dq=70 dq=80 dq=90 dq=100 empty=1 dqE=-1 WRAP:init enq1=1 enq2=1 enq3=1 dq=1 dq=2 enq4=1 enq5=1 enq6=1 enq7=1 enq8=1 enq9=1 full=0 dq=3 dq=4 dq=5 dq=6 dq=7 dq=8 dq=9 empty=1 FILL:0 1 2 3 4 5 6 7 DRAIN:0 1 2 3 4 5 6 7 CYC:c0e100d100 c1e101d101 c2e102d102 c3e103d103 c4e104d104 c5e105d105 c6e106d106 c7e107d107 c8e108d108 c9e109d109 c10e110d110 c11e111d111 c12e112d112 c13e113d113 c14e114d114 c15e115d115 c16e116d116 c17e117d117 c18e118d118 c19e119d119 empty=1 PF:e200 e201 e202 d200 e203 e204 e205 d201 d202 d203 e206 d204 d205 d206 empty=1 SUM:sum=28 cnt=8 OW:init3 e10 e20 e30 full=1 ow40 ow50 front=30 d30 d40 d50 empty=1 MRB:r0e1 r0e2 r1e10 r1e20 r1e30 r0d=1 r1d=10 r0sz=1 r1sz=2 STR:e0 e1 e2 e3 e4 e5 e6 e7 d0 d1 d2 d3 d4 d5 d6 d7 e10 e11 e12 e13 e14 e15 e16 e17 d10 d11 d12 d13 d14 d15 d16 d17 DONE

/* =========================================================================
 * Test 051: Ring Buffer (Circular Buffer) Implementation
 * =========================================================================
 * Tests circular buffer using arrays with enqueue/dequeue operations,
 * full/empty detection, wraparound handling, overwrite mode, and
 * multiple independent ring buffers.
 * ========================================================================= */

/* ------- Ring buffer 1: primary buffer with capacity 8 ------- */
int rb_data[9];
int rb_head;
int rb_tail;
int rb_cap;

void rb_init(int cap) {
    int i;
    rb_head = 0;
    rb_tail = 0;
    rb_cap = cap;
    for (i = 0; i < cap + 1; i = i + 1) {
        rb_data[i] = 0;
    }
}

int rb_empty(void) {
    return rb_head == rb_tail;
}

int rb_full(void) {
    int next;
    next = (rb_tail + 1) % (rb_cap + 1);
    return next == rb_head;
}

int rb_size(void) {
    int s;
    s = rb_tail - rb_head;
    if (s < 0) {
        s = s + (rb_cap + 1);
    }
    return s;
}

int rb_enqueue(int val) {
    if (rb_full()) {
        return 0;
    }
    rb_data[rb_tail] = val;
    rb_tail = (rb_tail + 1) % (rb_cap + 1);
    return 1;
}

int rb_dequeue(void) {
    int val;
    if (rb_empty()) {
        return -1;
    }
    val = rb_data[rb_head];
    rb_head = (rb_head + 1) % (rb_cap + 1);
    return val;
}

int rb_front(void) {
    if (rb_empty()) {
        return -1;
    }
    return rb_data[rb_head];
}

int rb_rear(void) {
    int idx;
    if (rb_empty()) {
        return -1;
    }
    idx = rb_tail - 1;
    if (idx < 0) {
        idx = idx + (rb_cap + 1);
    }
    return rb_data[idx];
}

int rb_sum(void) {
    int sum;
    int i;
    int idx;
    sum = 0;
    for (i = 0; i < rb_size(); i = i + 1) {
        idx = (rb_head + i) % (rb_cap + 1);
        sum = sum + rb_data[idx];
    }
    return sum;
}

int rb_count(void) {
    int count;
    int pos;
    count = 0;
    pos = rb_head;
    while (pos != rb_tail) {
        count = count + 1;
        pos = (pos + 1) % (rb_cap + 1);
    }
    return count;
}

/* ------- Overwrite ring buffer: capacity 3 ------- */
int ow_data[3];
int ow_head;
int ow_tail;
int ow_count;
int ow_cap;

void ow_init(int cap) {
    int i;
    ow_head = 0;
    ow_tail = 0;
    ow_count = 0;
    ow_cap = cap;
    for (i = 0; i < cap; i = i + 1) {
        ow_data[i] = 0;
    }
}

int ow_full(void) {
    return ow_count == ow_cap;
}

int ow_empty(void) {
    return ow_count == 0;
}

void ow_enqueue(int val) {
    ow_data[ow_tail] = val;
    ow_tail = (ow_tail + 1) % ow_cap;
    if (ow_count == ow_cap) {
        ow_head = (ow_head + 1) % ow_cap;
    } else {
        ow_count = ow_count + 1;
    }
}

int ow_dequeue(void) {
    int val;
    if (ow_empty()) {
        return -1;
    }
    val = ow_data[ow_head];
    ow_head = (ow_head + 1) % ow_cap;
    ow_count = ow_count - 1;
    return val;
}

int ow_front(void) {
    if (ow_empty()) {
        return -1;
    }
    return ow_data[ow_head];
}

/* ------- Second independent ring buffer ------- */
int rb2_data[5];
int rb2_head;
int rb2_tail;

void rb2_init(void) {
    int i;
    rb2_head = 0;
    rb2_tail = 0;
    for (i = 0; i < 5; i = i + 1) {
        rb2_data[i] = 0;
    }
}

int rb2_empty(void) {
    return rb2_head == rb2_tail;
}

int rb2_full(void) {
    return ((rb2_tail + 1) % 5) == rb2_head;
}

int rb2_enqueue(int val) {
    if (rb2_full()) {
        return 0;
    }
    rb2_data[rb2_tail] = val;
    rb2_tail = (rb2_tail + 1) % 5;
    return 1;
}

int rb2_dequeue(void) {
    int val;
    if (rb2_empty()) {
        return -1;
    }
    val = rb2_data[rb2_head];
    rb2_head = (rb2_head + 1) % 5;
    return val;
}

int rb2_size(void) {
    int s;
    s = rb2_tail - rb2_head;
    if (s < 0) {
        s = s + 5;
    }
    return s;
}

/* ------- Third independent ring buffer ------- */
int rb3_data[9];
int rb3_head;
int rb3_tail;

void rb3_init(void) {
    int i;
    rb3_head = 0;
    rb3_tail = 0;
    for (i = 0; i < 9; i = i + 1) {
        rb3_data[i] = 0;
    }
}

int rb3_enqueue(int val) {
    int next;
    next = (rb3_tail + 1) % 9;
    if (next == rb3_head) {
        return 0;
    }
    rb3_data[rb3_tail] = val;
    rb3_tail = next;
    return 1;
}

int rb3_dequeue(void) {
    int val;
    if (rb3_head == rb3_tail) {
        return -1;
    }
    val = rb3_data[rb3_head];
    rb3_head = (rb3_head + 1) % 9;
    return val;
}

/* =========================================================================
 * Test functions
 * ========================================================================= */

void test_basic(void) {
    int r;
    int v;

    rb_init(8);
    printf("RB:init cap=8");
    printf(" empty=%d", rb_empty());
    printf(" full=%d", rb_full());
    printf(" sz=%d", rb_size());

    /* Enqueue 5 items */
    r = rb_enqueue(10); printf(" enq10=%d", r);
    r = rb_enqueue(20); printf(" enq20=%d", r);
    r = rb_enqueue(30); printf(" enq30=%d", r);
    r = rb_enqueue(40); printf(" enq40=%d", r);
    r = rb_enqueue(50); printf(" enq50=%d", r);

    printf(" sz=%d", rb_size());
    printf(" empty=%d", rb_empty());
    printf(" full=%d", rb_full());

    /* Dequeue 2 */
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    printf(" sz=%d", rb_size());
    printf(" front=%d", rb_front());
    printf(" rear=%d", rb_rear());

    /* Fill to capacity */
    r = rb_enqueue(60);  printf(" enq60=%d", r);
    r = rb_enqueue(70);  printf(" enq70=%d", r);
    r = rb_enqueue(80);  printf(" enq80=%d", r);
    r = rb_enqueue(90);  printf(" enq90=%d", r);
    r = rb_enqueue(100); printf(" enq100=%d", r);

    printf(" sz=%d", rb_size());
    printf(" full=%d", rb_full());

    /* Enqueue when full should fail */
    r = rb_enqueue(110); printf(" enq110=%d", r);

    /* Drain all 8 */
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);

    printf(" empty=%d", rb_empty());

    /* Dequeue from empty */
    v = rb_dequeue(); printf(" dqE=%d", v);
}

void test_wraparound(void) {
    int r;
    int v;

    printf(" WRAP:init");
    rb_init(8);

    r = rb_enqueue(1); printf(" enq1=%d", r);
    r = rb_enqueue(2); printf(" enq2=%d", r);
    r = rb_enqueue(3); printf(" enq3=%d", r);

    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);

    r = rb_enqueue(4); printf(" enq4=%d", r);
    r = rb_enqueue(5); printf(" enq5=%d", r);
    r = rb_enqueue(6); printf(" enq6=%d", r);
    r = rb_enqueue(7); printf(" enq7=%d", r);
    r = rb_enqueue(8); printf(" enq8=%d", r);
    r = rb_enqueue(9); printf(" enq9=%d", r);

    printf(" full=%d", rb_full());

    /* Should have 3,4,5,6,7,8,9 => wait no: after dq 1,2 we have 3 in buffer.
       Then enqueued 4,5,6,7,8,9. So buffer has 3,4,5,6,7,8,9 = 7 items.
       Cap is 8, so not full. Actually wait: enq9 would be 8th item total if cap=8.
       Let me recount: after init(8), enq 1,2,3 => size=3. Deq 2 => size=1.
       Enq 4,5,6,7,8,9 => size = 1+6 = 7. So full=0. But we printed full=1 in expect...
       Let me recalculate. After dequeue 1 and 2, size=1 (element 3 remains).
       Then enqueue 4,5,6,7,8,9 => size = 7. Not full (cap=8).
       Hmm, I need to adjust. Let me enqueue one more to make it 8. */

    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);
    v = rb_dequeue(); printf(" dq=%d", v);

    printf(" empty=%d", rb_empty());
}

void test_fill_drain(void) {
    int i;
    int v;

    rb_init(8);
    printf(" FILL:");
    for (i = 0; i < 8; i = i + 1) {
        rb_enqueue(i);
        printf("%d ", i);
    }

    printf("DRAIN:");
    for (i = 0; i < 8; i = i + 1) {
        v = rb_dequeue();
        printf("%d ", v);
    }
}

void test_cycles(void) {
    int i;
    int v;
    int r;

    rb_init(8);
    printf("CYC:");
    for (i = 0; i < 20; i = i + 1) {
        r = rb_enqueue(100 + i);
        v = rb_dequeue();
        printf("c%d", i);
        printf("e%d", 100 + i);
        printf("d%d ", v);
    }
    printf("empty=%d", rb_empty());
}

void test_partial(void) {
    int i;
    int v;
    int r;

    rb_init(8);
    printf(" PF:");

    /* Enqueue 3 */
    for (i = 0; i < 3; i = i + 1) {
        r = rb_enqueue(200 + i);
        printf("e%d ", 200 + i);
    }

    /* Dequeue 1 */
    v = rb_dequeue();
    printf("d%d ", v);

    /* Enqueue 3 more */
    for (i = 3; i < 6; i = i + 1) {
        r = rb_enqueue(200 + i);
        printf("e%d ", 200 + i);
    }

    /* Dequeue 3 */
    v = rb_dequeue(); printf("d%d ", v);
    v = rb_dequeue(); printf("d%d ", v);
    v = rb_dequeue(); printf("d%d ", v);

    /* Enqueue 1 */
    r = rb_enqueue(206);
    printf("e%d ", 206);

    /* Drain remaining */
    v = rb_dequeue(); printf("d%d ", v);
    v = rb_dequeue(); printf("d%d ", v);
    v = rb_dequeue(); printf("d%d ", v);

    printf("empty=%d", rb_empty());
}

void test_sum_count(void) {
    int i;

    rb_init(8);
    printf(" SUM:");

    for (i = 0; i < 8; i = i + 1) {
        rb_enqueue(i);
    }

    printf("sum=%d", rb_sum());
    printf(" cnt=%d", rb_count());
}

void test_overwrite(void) {
    int v;

    printf(" OW:init3");
    ow_init(3);

    ow_enqueue(10); printf(" e10");
    ow_enqueue(20); printf(" e20");
    ow_enqueue(30); printf(" e30");
    printf(" full=%d", ow_full());

    /* Overwrite: push 40, drops 10 */
    ow_enqueue(40); printf(" ow40");
    /* Overwrite: push 50, drops 20 */
    ow_enqueue(50); printf(" ow50");

    printf(" front=%d", ow_front());

    v = ow_dequeue(); printf(" d%d", v);
    v = ow_dequeue(); printf(" d%d", v);
    v = ow_dequeue(); printf(" d%d", v);
    printf(" empty=%d", ow_empty());
}

void test_multi(void) {
    int v;

    printf(" MRB:");
    rb2_init();

    rb2_enqueue(1);  printf("r0e1 ");
    rb2_enqueue(2);  printf("r0e2 ");

    rb3_init();
    rb3_enqueue(10); printf("r1e10 ");
    rb3_enqueue(20); printf("r1e20 ");
    rb3_enqueue(30); printf("r1e30 ");

    v = rb2_dequeue(); printf("r0d=%d ", v);
    v = rb3_dequeue(); printf("r1d=%d ", v);

    printf("r0sz=%d ", rb2_size());
    printf("r1sz=");
    /* rb3 size: tail=3, head=1, size = 3-1 = 2 */
    {
        int s;
        s = rb3_tail - rb3_head;
        if (s < 0) { s = s + 9; }
        printf("%d", s);
    }
}

void test_stress(void) {
    int i;
    int v;

    printf(" STR:");
    rb3_init();

    for (i = 0; i < 8; i = i + 1) {
        rb3_enqueue(i);
        printf("e%d ", i);
    }
    for (i = 0; i < 8; i = i + 1) {
        v = rb3_dequeue();
        printf("d%d ", v);
    }

    /* Second round */
    for (i = 10; i < 18; i = i + 1) {
        rb3_enqueue(i);
        printf("e%d ", i);
    }
    for (i = 0; i < 8; i = i + 1) {
        v = rb3_dequeue();
        printf("d%d ", v);
    }

    printf("DONE\n");
}

int main(void) {
    test_basic();
    test_wraparound();
    test_fill_drain();
    test_cycles();
    test_partial();
    test_sum_count();
    test_overwrite();
    test_multi();
    test_stress();
    return 0;
}
