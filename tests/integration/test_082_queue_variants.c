int printf(const char *fmt, ...);

// ============================================================
// test_082_queue_variants.c
// Implements 5 queue variants using arrays:
//   1. Priority Queue (array-based, min-priority)
//   2. Circular Queue
//   3. Deque (Double-ended queue)
//   4. Queue with min tracking
//   5. Multi-level queue (3 priority levels)
// ============================================================

// ---- Priority Queue (min-priority, array-based) ----
// Uses a simple sorted-insert approach for priority queue
int pq_data[50];
int pq_priority[50];
int pq_size;

void pq_init() {
    pq_size = 0;
}

// Insert into priority queue with given priority (lower = higher priority)
void pq_insert(int data, int pri) {
    // Find correct position to maintain sorted order by priority
    int i = pq_size - 1;
    while (i >= 0 && pq_priority[i] > pri) {
        pq_data[i + 1] = pq_data[i];
        pq_priority[i + 1] = pq_priority[i];
        i--;
    }
    pq_data[i + 1] = data;
    pq_priority[i + 1] = pri;
    pq_size++;
}

// Remove and return highest priority (lowest number) element
int pq_dequeue() {
    if (pq_size == 0) return -1;
    int val = pq_data[0];
    int i;
    for (i = 0; i < pq_size - 1; i++) {
        pq_data[i] = pq_data[i + 1];
        pq_priority[i] = pq_priority[i + 1];
    }
    pq_size--;
    return val;
}

// Peek at highest priority element
int pq_peek() {
    if (pq_size == 0) return -1;
    return pq_data[0];
}

// Check if priority queue is empty
int pq_is_empty() {
    return pq_size == 0;
}

// Get the priority of the front element
int pq_front_priority() {
    if (pq_size == 0) return -1;
    return pq_priority[0];
}

// ---- Circular Queue ----
int cq_data[20];
int cq_front;
int cq_rear;
int cq_count;
int cq_capacity;

void cq_init(int cap) {
    cq_front = 0;
    cq_rear = -1;
    cq_count = 0;
    cq_capacity = cap;
}

int cq_is_full() {
    return cq_count == cq_capacity;
}

int cq_is_empty() {
    return cq_count == 0;
}

int cq_enqueue(int val) {
    if (cq_is_full()) return 0; // failure
    cq_rear = (cq_rear + 1) % cq_capacity;
    cq_data[cq_rear] = val;
    cq_count++;
    return 1; // success
}

int cq_dequeue() {
    if (cq_is_empty()) return -9999;
    int val = cq_data[cq_front];
    cq_front = (cq_front + 1) % cq_capacity;
    cq_count--;
    return val;
}

int cq_peek_front() {
    if (cq_is_empty()) return -9999;
    return cq_data[cq_front];
}

int cq_peek_rear() {
    if (cq_is_empty()) return -9999;
    return cq_data[cq_rear];
}

int cq_get_size() {
    return cq_count;
}

// ---- Deque (Double-ended queue) ----
int dq_data[30];
int dq_front;
int dq_rear;
int dq_count;
int dq_capacity;

void dq_init(int cap) {
    dq_front = cap / 2; // Start in middle for room on both sides
    dq_rear = cap / 2 - 1;
    dq_count = 0;
    dq_capacity = cap;
}

int dq_is_empty() {
    return dq_count == 0;
}

int dq_is_full() {
    return dq_count == dq_capacity;
}

int dq_push_front(int val) {
    if (dq_is_full()) return 0;
    dq_front = (dq_front - 1 + dq_capacity) % dq_capacity;
    dq_data[dq_front] = val;
    dq_count++;
    return 1;
}

int dq_push_back(int val) {
    if (dq_is_full()) return 0;
    dq_rear = (dq_rear + 1) % dq_capacity;
    dq_data[dq_rear] = val;
    dq_count++;
    return 1;
}

int dq_pop_front() {
    if (dq_is_empty()) return -9999;
    int val = dq_data[dq_front];
    dq_front = (dq_front + 1) % dq_capacity;
    dq_count--;
    return val;
}

int dq_pop_back() {
    if (dq_is_empty()) return -9999;
    int val = dq_data[dq_rear];
    dq_rear = (dq_rear - 1 + dq_capacity) % dq_capacity;
    dq_count--;
    return val;
}

int dq_peek_front() {
    if (dq_is_empty()) return -9999;
    return dq_data[dq_front];
}

int dq_peek_back() {
    if (dq_is_empty()) return -9999;
    return dq_data[dq_rear];
}

int dq_get_size() {
    return dq_count;
}

// ---- Queue with Min Tracking ----
// Uses a main queue and an auxiliary min-tracking structure
int mq_data[30];
int mq_front;
int mq_rear;
int mq_count;

// Min-deque for tracking minimums
int mq_min_dq[30];
int mq_min_front;
int mq_min_rear;
int mq_min_count;

void mq_init() {
    mq_front = 0;
    mq_rear = -1;
    mq_count = 0;
    mq_min_front = 0;
    mq_min_rear = -1;
    mq_min_count = 0;
}

void mq_enqueue(int val) {
    // Add to main queue
    mq_rear++;
    mq_data[mq_rear] = val;
    mq_count++;

    // Maintain min-deque: remove from back anything > val
    while (mq_min_count > 0 && mq_min_dq[mq_min_rear] > val) {
        mq_min_rear--;
        mq_min_count--;
    }
    mq_min_rear++;
    mq_min_dq[mq_min_rear] = val;
    mq_min_count++;
}

int mq_dequeue() {
    if (mq_count == 0) return -9999;
    int val = mq_data[mq_front];
    mq_front++;
    mq_count--;

    // If dequeued value is the current min, remove from min-deque
    if (mq_min_count > 0 && mq_min_dq[mq_min_front] == val) {
        mq_min_front++;
        mq_min_count--;
    }
    return val;
}

int mq_get_min() {
    if (mq_min_count == 0) return -9999;
    return mq_min_dq[mq_min_front];
}

int mq_get_size() {
    return mq_count;
}

int mq_is_empty() {
    return mq_count == 0;
}

// ---- Multi-level Queue (3 priority levels) ----
// Level 0 = highest priority, Level 2 = lowest priority
int mlq_data[3][20];
int mlq_front[3];
int mlq_rear[3];
int mlq_count[3];

void mlq_init() {
    int i;
    for (i = 0; i < 3; i++) {
        mlq_front[i] = 0;
        mlq_rear[i] = -1;
        mlq_count[i] = 0;
    }
}

int mlq_enqueue(int level, int val) {
    if (level < 0 || level > 2) return 0;
    if (mlq_count[level] >= 20) return 0;
    mlq_rear[level]++;
    mlq_data[level][mlq_rear[level]] = val;
    mlq_count[level]++;
    return 1;
}

// Dequeue from highest priority non-empty level
int mlq_dequeue() {
    int i;
    for (i = 0; i < 3; i++) {
        if (mlq_count[i] > 0) {
            int val = mlq_data[i][mlq_front[i]];
            mlq_front[i]++;
            mlq_count[i]--;
            return val;
        }
    }
    return -9999; // all empty
}

// Dequeue from a specific level
int mlq_dequeue_level(int level) {
    if (level < 0 || level > 2) return -9999;
    if (mlq_count[level] == 0) return -9999;
    int val = mlq_data[level][mlq_front[level]];
    mlq_front[level]++;
    mlq_count[level]--;
    return val;
}

int mlq_total_size() {
    return mlq_count[0] + mlq_count[1] + mlq_count[2];
}

int mlq_level_size(int level) {
    if (level < 0 || level > 2) return 0;
    return mlq_count[level];
}

int mlq_is_empty() {
    return mlq_total_size() == 0;
}

// Peek at next element that would be dequeued
int mlq_peek() {
    int i;
    for (i = 0; i < 3; i++) {
        if (mlq_count[i] > 0) {
            return mlq_data[i][mlq_front[i]];
        }
    }
    return -9999;
}

// ============================================================
// Test functions
// ============================================================

void test_priority_queue() {
    printf("=== Priority Queue Tests ===\n");
    pq_init();

    // Insert elements with various priorities
    pq_insert(100, 5);  // data=100, priority=5
    pq_insert(200, 2);  // data=200, priority=2 (higher)
    pq_insert(300, 8);  // data=300, priority=8
    pq_insert(400, 1);  // data=400, priority=1 (highest)
    pq_insert(500, 3);  // data=500, priority=3

    printf("PQ size: %d\n", pq_size);
    printf("PQ peek: %d\n", pq_peek());
    printf("PQ front_pri: %d\n", pq_front_priority());

    // Dequeue should come out in priority order
    printf("PQ dequeue1: %d\n", pq_dequeue());
    printf("PQ dequeue2: %d\n", pq_dequeue());
    printf("PQ dequeue3: %d\n", pq_dequeue());
    printf("PQ dequeue4: %d\n", pq_dequeue());
    printf("PQ dequeue5: %d\n", pq_dequeue());
    printf("PQ empty: %d\n", pq_is_empty());

    // Test with same priorities
    pq_init();
    pq_insert(10, 3);
    pq_insert(20, 3);
    pq_insert(30, 3);
    printf("PQ same_pri_size: %d\n", pq_size);
    printf("PQ same_pri_d1: %d\n", pq_dequeue());
    printf("PQ same_pri_d2: %d\n", pq_dequeue());
    printf("PQ same_pri_d3: %d\n", pq_dequeue());
}

void test_circular_queue() {
    printf("=== Circular Queue Tests ===\n");
    cq_init(5);

    printf("CQ empty: %d\n", cq_is_empty());
    cq_enqueue(10);
    cq_enqueue(20);
    cq_enqueue(30);
    printf("CQ size: %d\n", cq_get_size());
    printf("CQ front: %d\n", cq_peek_front());
    printf("CQ rear: %d\n", cq_peek_rear());

    printf("CQ dequeue: %d\n", cq_dequeue());
    printf("CQ dequeue: %d\n", cq_dequeue());
    printf("CQ size_after: %d\n", cq_get_size());

    // Fill up to test circular behavior
    cq_enqueue(40);
    cq_enqueue(50);
    cq_enqueue(60);
    cq_enqueue(70);
    printf("CQ full: %d\n", cq_is_full());
    printf("CQ size_full: %d\n", cq_get_size());

    // Try to enqueue when full
    int ok = cq_enqueue(80);
    printf("CQ overflow: %d\n", ok);

    // Dequeue all and verify circular wrap
    printf("CQ d1: %d\n", cq_dequeue());
    printf("CQ d2: %d\n", cq_dequeue());
    printf("CQ d3: %d\n", cq_dequeue());
    printf("CQ d4: %d\n", cq_dequeue());
    printf("CQ d5: %d\n", cq_dequeue());
    printf("CQ empty_after: %d\n", cq_is_empty());

    // Test wrap-around more thoroughly
    cq_init(3);
    cq_enqueue(1);
    cq_enqueue(2);
    cq_enqueue(3);
    cq_dequeue();
    cq_dequeue();
    cq_enqueue(4);
    cq_enqueue(5);
    printf("CQ wrap_front: %d\n", cq_peek_front());
    printf("CQ wrap_rear: %d\n", cq_peek_rear());
    printf("CQ wrap_size: %d\n", cq_get_size());
}

void test_deque() {
    printf("=== Deque Tests ===\n");
    dq_init(10);

    printf("DQ empty: %d\n", dq_is_empty());

    // Push to back
    dq_push_back(10);
    dq_push_back(20);
    dq_push_back(30);
    printf("DQ size: %d\n", dq_get_size());
    printf("DQ front: %d\n", dq_peek_front());
    printf("DQ back: %d\n", dq_peek_back());

    // Push to front
    dq_push_front(5);
    dq_push_front(1);
    printf("DQ size2: %d\n", dq_get_size());
    printf("DQ front2: %d\n", dq_peek_front());

    // Pop from front
    printf("DQ pop_front: %d\n", dq_pop_front());
    printf("DQ pop_front: %d\n", dq_pop_front());

    // Pop from back
    printf("DQ pop_back: %d\n", dq_pop_back());
    printf("DQ size3: %d\n", dq_get_size());
    printf("DQ front3: %d\n", dq_peek_front());
    printf("DQ back3: %d\n", dq_peek_back());

    // Use as stack (push/pop from same end)
    dq_init(10);
    dq_push_back(100);
    dq_push_back(200);
    dq_push_back(300);
    printf("DQ stack_pop: %d\n", dq_pop_back());
    printf("DQ stack_pop: %d\n", dq_pop_back());
    printf("DQ stack_pop: %d\n", dq_pop_back());
    printf("DQ stack_empty: %d\n", dq_is_empty());
}

void test_min_queue() {
    printf("=== Min Queue Tests ===\n");
    mq_init();

    mq_enqueue(5);
    printf("MQ min1: %d\n", mq_get_min());
    mq_enqueue(3);
    printf("MQ min2: %d\n", mq_get_min());
    mq_enqueue(7);
    printf("MQ min3: %d\n", mq_get_min());
    mq_enqueue(1);
    printf("MQ min4: %d\n", mq_get_min());

    printf("MQ size: %d\n", mq_get_size());

    printf("MQ deq: %d\n", mq_dequeue());
    printf("MQ min5: %d\n", mq_get_min());
    printf("MQ deq: %d\n", mq_dequeue());
    printf("MQ min6: %d\n", mq_get_min());
    printf("MQ deq: %d\n", mq_dequeue());
    printf("MQ min7: %d\n", mq_get_min());
    printf("MQ deq: %d\n", mq_dequeue());
    printf("MQ empty: %d\n", mq_is_empty());

    // Test with decreasing sequence
    mq_init();
    mq_enqueue(10);
    mq_enqueue(8);
    mq_enqueue(6);
    mq_enqueue(4);
    mq_enqueue(2);
    printf("MQ dec_min: %d\n", mq_get_min());
    printf("MQ dec_size: %d\n", mq_get_size());
}

void test_multilevel_queue() {
    printf("=== Multi-level Queue Tests ===\n");
    mlq_init();

    // Enqueue to different levels
    mlq_enqueue(2, 100); // low priority
    mlq_enqueue(2, 200); // low priority
    mlq_enqueue(0, 10);  // high priority
    mlq_enqueue(1, 50);  // medium priority
    mlq_enqueue(0, 20);  // high priority
    mlq_enqueue(1, 60);  // medium priority

    printf("MLQ total: %d\n", mlq_total_size());
    printf("MLQ level0: %d\n", mlq_level_size(0));
    printf("MLQ level1: %d\n", mlq_level_size(1));
    printf("MLQ level2: %d\n", mlq_level_size(2));
    printf("MLQ peek: %d\n", mlq_peek());

    // Dequeue - should come from highest priority first
    printf("MLQ d1: %d\n", mlq_dequeue());
    printf("MLQ d2: %d\n", mlq_dequeue());
    printf("MLQ d3: %d\n", mlq_dequeue());
    printf("MLQ d4: %d\n", mlq_dequeue());
    printf("MLQ d5: %d\n", mlq_dequeue());
    printf("MLQ d6: %d\n", mlq_dequeue());
    printf("MLQ empty: %d\n", mlq_is_empty());

    // Test dequeue from specific level
    mlq_init();
    mlq_enqueue(0, 1);
    mlq_enqueue(1, 2);
    mlq_enqueue(2, 3);
    printf("MLQ spec_d2: %d\n", mlq_dequeue_level(2));
    printf("MLQ spec_d0: %d\n", mlq_dequeue_level(0));
    printf("MLQ spec_d1: %d\n", mlq_dequeue_level(1));
    printf("MLQ all_empty: %d\n", mlq_is_empty());
}

// EXPECT: === Priority Queue Tests ===
// EXPECT: PQ size: 5
// EXPECT: PQ peek: 400
// EXPECT: PQ front_pri: 1
// EXPECT: PQ dequeue1: 400
// EXPECT: PQ dequeue2: 200
// EXPECT: PQ dequeue3: 500
// EXPECT: PQ dequeue4: 100
// EXPECT: PQ dequeue5: 300
// EXPECT: PQ empty: 1
// EXPECT: PQ same_pri_size: 3
// EXPECT: PQ same_pri_d1: 10
// EXPECT: PQ same_pri_d2: 20
// EXPECT: PQ same_pri_d3: 30
// EXPECT: === Circular Queue Tests ===
// EXPECT: CQ empty: 1
// EXPECT: CQ size: 3
// EXPECT: CQ front: 10
// EXPECT: CQ rear: 30
// EXPECT: CQ dequeue: 10
// EXPECT: CQ dequeue: 20
// EXPECT: CQ size_after: 1
// EXPECT: CQ full: 1
// EXPECT: CQ size_full: 5
// EXPECT: CQ overflow: 0
// EXPECT: CQ d1: 30
// EXPECT: CQ d2: 40
// EXPECT: CQ d3: 50
// EXPECT: CQ d4: 60
// EXPECT: CQ d5: 70
// EXPECT: CQ empty_after: 1
// EXPECT: CQ wrap_front: 3
// EXPECT: CQ wrap_rear: 5
// EXPECT: CQ wrap_size: 3
// EXPECT: === Deque Tests ===
// EXPECT: DQ empty: 1
// EXPECT: DQ size: 3
// EXPECT: DQ front: 10
// EXPECT: DQ back: 30
// EXPECT: DQ size2: 5
// EXPECT: DQ front2: 1
// EXPECT: DQ pop_front: 1
// EXPECT: DQ pop_front: 5
// EXPECT: DQ pop_back: 30
// EXPECT: DQ size3: 2
// EXPECT: DQ front3: 10
// EXPECT: DQ back3: 20
// EXPECT: DQ stack_pop: 300
// EXPECT: DQ stack_pop: 200
// EXPECT: DQ stack_pop: 100
// EXPECT: DQ stack_empty: 1
// EXPECT: === Min Queue Tests ===
// EXPECT: MQ min1: 5
// EXPECT: MQ min2: 3
// EXPECT: MQ min3: 3
// EXPECT: MQ min4: 1
// EXPECT: MQ size: 4
// EXPECT: MQ deq: 5
// EXPECT: MQ min5: 1
// EXPECT: MQ deq: 3
// EXPECT: MQ min6: 1
// EXPECT: MQ deq: 7
// EXPECT: MQ min7: 1
// EXPECT: MQ deq: 1
// EXPECT: MQ empty: 1
// EXPECT: MQ dec_min: 2
// EXPECT: MQ dec_size: 5
// EXPECT: === Multi-level Queue Tests ===
// EXPECT: MLQ total: 6
// EXPECT: MLQ level0: 2
// EXPECT: MLQ level1: 2
// EXPECT: MLQ level2: 2
// EXPECT: MLQ peek: 10
// EXPECT: MLQ d1: 10
// EXPECT: MLQ d2: 20
// EXPECT: MLQ d3: 50
// EXPECT: MLQ d4: 60
// EXPECT: MLQ d5: 100
// EXPECT: MLQ d6: 200
// EXPECT: MLQ empty: 1
// EXPECT: MLQ spec_d2: 3
// EXPECT: MLQ spec_d0: 1
// EXPECT: MLQ spec_d1: 2
// EXPECT: MLQ all_empty: 1

int main() {
    test_priority_queue();
    test_circular_queue();
    test_deque();
    test_min_queue();
    test_multilevel_queue();
    return 0;
}
