int printf(const char *fmt, ...);
// EXPECT: T1:sz=5 f=10 b=50 pf=10 pf=20 pb=50 sz=2 f=30 b=40 pf=30 pf=40 empty=1 T2:sz=3 f=300 b=100 pf=300 pb=100 sz=1 pf=200 empty=1 T3:sz=4 v=4 v=2 v=1 v=3 empty=1 sz=6 v=60 v=50 v=40 v=30 v=20 v=10 empty=1 FWD:0 10 20 30 40 50 60 70 sz=8 BWD:35 30 25 20 15 10 5 0 sz=8 WRAP:pb=1 pb=2 sz=6 5 4 3 10 20 30 EO:pf=-999 pb=-999 kf=-999 kb=-999 empty=1 CAP:full=1 sz=16 pb=0 pf=0 empty=1 last=15 DQ2:sz=4 f=25 b=200 pf=25 pb=200 pf=50 pb=100 empty=1 PAL:p1=1 p2=0 p3=1 SW:8 6 10 15 16 17 13 14 IL:sz=10 8 6 4 2 0 10 30 50 70 90 empty=1 REV:66 55 44 33 22 11 ROT:3 4 5 6 1 2 6 1 2 3 4 5 SUM:55 MM:min=1 max=9 DONE

/* =========================================================================
 * Test 052: Double-Ended Queue (Deque) Implementation
 * =========================================================================
 * Tests deque with push front/back, pop front/back, peek, iteration
 * forward and backward, multiple deques.
 * ========================================================================= */

/* Deque using circular array, capacity 16 */
int dq_data[17];
int dq_front;
int dq_back;
int dq_cap;
int dq_sz;

void dq_init(void) {
    int i;
    dq_front = 0;
    dq_back = 0;
    dq_cap = 16;
    dq_sz = 0;
    for (i = 0; i < 17; i = i + 1) {
        dq_data[i] = 0;
    }
}

int dq_empty(void) {
    return dq_sz == 0;
}

int dq_full(void) {
    return dq_sz == dq_cap;
}

int dq_size(void) {
    return dq_sz;
}

int dq_push_back(int val) {
    if (dq_full()) {
        return 0;
    }
    dq_data[dq_back] = val;
    dq_back = (dq_back + 1) % (dq_cap + 1);
    dq_sz = dq_sz + 1;
    return 1;
}

int dq_push_front(int val) {
    if (dq_full()) {
        return 0;
    }
    dq_front = (dq_front - 1 + dq_cap + 1) % (dq_cap + 1);
    dq_data[dq_front] = val;
    dq_sz = dq_sz + 1;
    return 1;
}

int dq_pop_front(void) {
    int val;
    if (dq_empty()) {
        return -999;
    }
    val = dq_data[dq_front];
    dq_front = (dq_front + 1) % (dq_cap + 1);
    dq_sz = dq_sz - 1;
    return val;
}

int dq_pop_back(void) {
    int val;
    if (dq_empty()) {
        return -999;
    }
    dq_back = (dq_back - 1 + dq_cap + 1) % (dq_cap + 1);
    val = dq_data[dq_back];
    dq_sz = dq_sz - 1;
    return val;
}

int dq_peek_front(void) {
    if (dq_empty()) {
        return -999;
    }
    return dq_data[dq_front];
}

int dq_peek_back(void) {
    int idx;
    if (dq_empty()) {
        return -999;
    }
    idx = (dq_back - 1 + dq_cap + 1) % (dq_cap + 1);
    return dq_data[idx];
}

int dq_at(int index) {
    int pos;
    if (index < 0 || index >= dq_sz) {
        return -999;
    }
    pos = (dq_front + index) % (dq_cap + 1);
    return dq_data[pos];
}

/* Second deque for independent testing */
int dq2_data[9];
int dq2_front;
int dq2_back;
int dq2_sz;
int dq2_cap;

void dq2_init(void) {
    int i;
    dq2_front = 0;
    dq2_back = 0;
    dq2_sz = 0;
    dq2_cap = 8;
    for (i = 0; i < 9; i = i + 1) {
        dq2_data[i] = 0;
    }
}

int dq2_push_back(int val) {
    if (dq2_sz == dq2_cap) { return 0; }
    dq2_data[dq2_back] = val;
    dq2_back = (dq2_back + 1) % 9;
    dq2_sz = dq2_sz + 1;
    return 1;
}

int dq2_push_front(int val) {
    if (dq2_sz == dq2_cap) { return 0; }
    dq2_front = (dq2_front - 1 + 9) % 9;
    dq2_data[dq2_front] = val;
    dq2_sz = dq2_sz + 1;
    return 1;
}

int dq2_pop_front(void) {
    int val;
    if (dq2_sz == 0) { return -999; }
    val = dq2_data[dq2_front];
    dq2_front = (dq2_front + 1) % 9;
    dq2_sz = dq2_sz - 1;
    return val;
}

int dq2_pop_back(void) {
    int val;
    if (dq2_sz == 0) { return -999; }
    dq2_back = (dq2_back - 1 + 9) % 9;
    val = dq2_data[dq2_back];
    dq2_sz = dq2_sz - 1;
    return val;
}

int dq2_peek_front(void) {
    if (dq2_sz == 0) { return -999; }
    return dq2_data[dq2_front];
}

int dq2_peek_back(void) {
    int idx;
    if (dq2_sz == 0) { return -999; }
    idx = (dq2_back - 1 + 9) % 9;
    return dq2_data[idx];
}

/* =========================================================================
 * Test functions
 * ========================================================================= */

void test_basic_pushback(void) {
    int v;
    dq_init();
    printf("T1:");
    dq_push_back(10);
    dq_push_back(20);
    dq_push_back(30);
    dq_push_back(40);
    dq_push_back(50);
    printf("sz=%d", dq_size());
    printf(" f=%d", dq_peek_front());
    printf(" b=%d", dq_peek_back());

    v = dq_pop_front(); printf(" pf=%d", v);
    v = dq_pop_front(); printf(" pf=%d", v);
    v = dq_pop_back();  printf(" pb=%d", v);
    printf(" sz=%d", dq_size());
    printf(" f=%d", dq_peek_front());
    printf(" b=%d", dq_peek_back());

    v = dq_pop_front(); printf(" pf=%d", v);
    v = dq_pop_front(); printf(" pf=%d", v);
    printf(" empty=%d", dq_empty());
}

void test_push_front(void) {
    int v;
    dq_init();
    printf(" T2:");
    dq_push_front(100);
    dq_push_front(200);
    dq_push_front(300);
    printf("sz=%d", dq_size());
    printf(" f=%d", dq_peek_front());
    printf(" b=%d", dq_peek_back());

    v = dq_pop_front(); printf(" pf=%d", v);
    v = dq_pop_back();  printf(" pb=%d", v);
    printf(" sz=%d", dq_size());
    v = dq_pop_front(); printf(" pf=%d", v);
    printf(" empty=%d", dq_empty());
}

void test_mixed_ops(void) {
    int v;
    dq_init();
    printf(" T3:");

    dq_push_back(1);
    dq_push_front(2);
    dq_push_back(3);
    dq_push_front(4);
    /* Order: 4, 2, 1, 3 */
    printf("sz=%d", dq_size());

    v = dq_pop_front(); printf(" v=%d", v);
    v = dq_pop_front(); printf(" v=%d", v);
    v = dq_pop_front(); printf(" v=%d", v);
    v = dq_pop_front(); printf(" v=%d", v);
    printf(" empty=%d", dq_empty());

    /* Push alternating front/back */
    dq_push_back(10);
    dq_push_front(20);
    dq_push_back(30);
    dq_push_front(40);
    dq_push_back(50);
    dq_push_front(60);
    /* Order: 60, 40, 20, 10, 30, 50 */
    printf(" sz=%d", dq_size());

    v = dq_pop_front(); printf(" v=%d", v);
    v = dq_pop_back();  printf(" v=%d", v);
    v = dq_pop_front(); printf(" v=%d", v);
    v = dq_pop_back();  printf(" v=%d", v);
    v = dq_pop_front(); printf(" v=%d", v);
    v = dq_pop_back();  printf(" v=%d", v);
    printf(" empty=%d", dq_empty());
}

void test_iterate_forward(void) {
    int i;
    dq_init();
    printf(" FWD:");

    for (i = 0; i < 8; i = i + 1) {
        dq_push_back(i * 10);
    }

    for (i = 0; i < dq_size(); i = i + 1) {
        printf("%d ", dq_at(i));
    }
    printf("sz=%d", dq_size());
}

void test_iterate_backward(void) {
    int i;
    printf(" BWD:");

    dq_init();
    for (i = 0; i < 8; i = i + 1) {
        dq_push_back(i * 5);
    }

    for (i = dq_size() - 1; i >= 0; i = i - 1) {
        printf("%d ", dq_at(i));
    }
    printf("sz=%d", dq_size());
}

void test_wrap_around(void) {
    int i;
    int v;
    dq_init();
    printf(" WRAP:");

    for (i = 0; i < 5; i = i + 1) {
        dq_push_front(i + 1);
    }
    /* Order: 5, 4, 3, 2, 1 */

    v = dq_pop_back(); printf("pb=%d ", v);
    v = dq_pop_back(); printf("pb=%d ", v);

    dq_push_back(10);
    dq_push_back(20);
    dq_push_back(30);
    /* Order: 5, 4, 3, 10, 20, 30 */

    printf("sz=%d ", dq_size());

    for (i = 0; i < dq_size(); i = i + 1) {
        printf("%d ", dq_at(i));
    }
}

void test_empty_ops(void) {
    int v;
    dq_init();
    printf("EO:");

    v = dq_pop_front(); printf("pf=%d ", v);
    v = dq_pop_back();  printf("pb=%d ", v);
    v = dq_peek_front(); printf("kf=%d ", v);
    v = dq_peek_back();  printf("kb=%d ", v);
    printf("empty=%d", dq_empty());
}

void test_fill_to_capacity(void) {
    int i;
    int v;
    dq_init();
    printf(" CAP:");

    for (i = 0; i < 16; i = i + 1) {
        dq_push_back(i);
    }
    printf("full=%d ", dq_full());
    printf("sz=%d ", dq_size());

    v = dq_push_back(99);
    printf("pb=%d ", v);
    v = dq_push_front(99);
    printf("pf=%d ", v);

    for (i = 0; i < 16; i = i + 1) {
        v = dq_pop_front();
    }
    printf("empty=%d", dq_empty());
    printf(" last=%d", v);
}

void test_second_deque(void) {
    int v;
    printf(" DQ2:");

    dq2_init();
    dq2_push_back(100);
    dq2_push_back(200);
    dq2_push_front(50);
    dq2_push_front(25);
    /* Order: 25, 50, 100, 200 */
    printf("sz=%d", dq2_sz);
    printf(" f=%d", dq2_peek_front());
    printf(" b=%d", dq2_peek_back());

    v = dq2_pop_front(); printf(" pf=%d", v);
    v = dq2_pop_back();  printf(" pb=%d", v);
    v = dq2_pop_front(); printf(" pf=%d", v);
    v = dq2_pop_back();  printf(" pb=%d", v);
    printf(" empty=%d", dq2_sz == 0);
}

void test_palindrome_check(void) {
    int front;
    int back;
    int is_pal;

    printf(" PAL:");

    /* Test: 1 2 3 2 1 - palindrome */
    dq_init();
    dq_push_back(1);
    dq_push_back(2);
    dq_push_back(3);
    dq_push_back(2);
    dq_push_back(1);

    is_pal = 1;
    while (dq_size() > 1) {
        front = dq_pop_front();
        back = dq_pop_back();
        if (front != back) {
            is_pal = 0;
        }
    }
    printf("p1=%d", is_pal);

    /* Test: 1 2 3 4 5 - not palindrome */
    dq_init();
    dq_push_back(1);
    dq_push_back(2);
    dq_push_back(3);
    dq_push_back(4);
    dq_push_back(5);

    is_pal = 1;
    while (dq_size() > 1) {
        front = dq_pop_front();
        back = dq_pop_back();
        if (front != back) {
            is_pal = 0;
        }
    }
    printf(" p2=%d", is_pal);

    /* Test: 7 7 7 7 - palindrome */
    dq_init();
    dq_push_back(7);
    dq_push_back(7);
    dq_push_back(7);
    dq_push_back(7);

    is_pal = 1;
    while (dq_size() > 1) {
        front = dq_pop_front();
        back = dq_pop_back();
        if (front != back) {
            is_pal = 0;
        }
    }
    printf(" p3=%d", is_pal);
}

void test_sliding_window(void) {
    int arr[10];
    int i;
    int v;

    arr[0] = 3; arr[1] = 1; arr[2] = 4; arr[3] = 1; arr[4] = 5;
    arr[5] = 9; arr[6] = 2; arr[7] = 6; arr[8] = 5; arr[9] = 3;

    printf(" SW:");
    for (i = 0; i <= 7; i = i + 1) {
        v = arr[i] + arr[i + 1] + arr[i + 2];
        printf("%d ", v);
    }
}

void test_interleave(void) {
    int v;
    int i;
    dq_init();
    printf("IL:");

    for (i = 0; i < 10; i = i + 1) {
        if (i % 2 == 0) {
            dq_push_front(i);
        } else {
            dq_push_back(i * 10);
        }
    }

    printf("sz=%d ", dq_size());
    for (i = 0; i < 10; i = i + 1) {
        v = dq_pop_front();
        printf("%d ", v);
    }
    printf("empty=%d", dq_empty());
}

void test_reverse_using_deque(void) {
    int arr[6];
    int i;
    int v;

    arr[0] = 11; arr[1] = 22; arr[2] = 33;
    arr[3] = 44; arr[4] = 55; arr[5] = 66;

    dq_init();
    printf(" REV:");

    for (i = 0; i < 6; i = i + 1) {
        dq_push_front(arr[i]);
    }

    for (i = 0; i < 6; i = i + 1) {
        v = dq_pop_front();
        printf("%d ", v);
    }
}

void test_rotate(void) {
    int i;
    int v;
    dq_init();
    printf("ROT:");

    for (i = 1; i <= 6; i = i + 1) {
        dq_push_back(i);
    }

    /* Rotate left by 2 */
    v = dq_pop_front(); dq_push_back(v);
    v = dq_pop_front(); dq_push_back(v);

    for (i = 0; i < 6; i = i + 1) {
        printf("%d ", dq_at(i));
    }

    /* Rotate right by 3 */
    v = dq_pop_back(); dq_push_front(v);
    v = dq_pop_back(); dq_push_front(v);
    v = dq_pop_back(); dq_push_front(v);

    for (i = 0; i < 6; i = i + 1) {
        printf("%d ", dq_at(i));
    }
}

void test_sum_elements(void) {
    int i;
    int sum;
    dq_init();
    printf("SUM:");

    for (i = 1; i <= 10; i = i + 1) {
        dq_push_back(i);
    }

    sum = 0;
    for (i = 0; i < dq_size(); i = i + 1) {
        sum = sum + dq_at(i);
    }
    printf("%d", sum);
}

void test_min_max(void) {
    int i;
    int mn;
    int mx;
    int val;
    dq_init();
    printf(" MM:");

    dq_push_back(5);
    dq_push_back(2);
    dq_push_back(8);
    dq_push_back(1);
    dq_push_back(9);
    dq_push_back(3);

    mn = dq_at(0);
    mx = dq_at(0);
    for (i = 1; i < dq_size(); i = i + 1) {
        val = dq_at(i);
        if (val < mn) { mn = val; }
        if (val > mx) { mx = val; }
    }
    printf("min=%d max=%d", mn, mx);

    printf(" DONE\n");
}

int main(void) {
    test_basic_pushback();
    test_push_front();
    test_mixed_ops();
    test_iterate_forward();
    test_iterate_backward();
    test_wrap_around();
    test_empty_ops();
    test_fill_to_capacity();
    test_second_deque();
    test_palindrome_check();
    test_sliding_window();
    test_interleave();
    test_reverse_using_deque();
    test_rotate();
    test_sum_elements();
    test_min_max();
    return 0;
}
