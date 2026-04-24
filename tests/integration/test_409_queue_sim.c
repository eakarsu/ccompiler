int printf(const char *fmt, ...);
// EXPECT: Size: 3\nFront: 10\nDequeued: 10\nDequeued: 20\nSize: 1\n\nBank simulation:\nCustomer 0: arrived=0, served=0, wait=0\nCustomer 1: arrived=2, served=3, wait=1\nCustomer 2: arrived=3, served=7, wait=4\nCustomer 3: arrived=5, served=9, wait=4\nCustomer 4: arrived=7, served=10, wait=3\nCustomer 5: arrived=8, served=13, wait=5\nCustomer 6: arrived=10, served=15, wait=5\nCustomer 7: arrived=12, served=19, wait=7\nTotal customers: 8\nTotal wait: 29
// Test: Queue simulation (circular buffer queue)

int q_data[16];
int q_front;
int q_rear;
int q_count;
int q_capacity;

void queue_init(int cap) {
    q_front = 0;
    q_rear = 0;
    q_count = 0;
    q_capacity = cap;
}

int queue_is_empty(void) {
    return q_count == 0;
}

int queue_is_full(void) {
    return q_count == q_capacity;
}

int enqueue(int val) {
    if (queue_is_full()) return 0;
    q_data[q_rear] = val;
    q_rear = (q_rear + 1) % q_capacity;
    q_count++;
    return 1;
}

int dequeue(int *val) {
    if (queue_is_empty()) return 0;
    *val = q_data[q_front];
    q_front = (q_front + 1) % q_capacity;
    q_count--;
    return 1;
}

int queue_peek(void) {
    return q_data[q_front];
}

void simulate_bank(void) {
    int arrivals[] = {0, 2, 3, 5, 7, 8, 10, 12};
    int service[] =  {3, 4, 2, 1, 3, 2, 4, 1};
    int n = 8;
    int time = 0;
    int served = 0;
    int total_wait = 0;
    int current_end = 0;
    int i = 0;

    queue_init(16);

    while (served < n) {
        while (i < n && arrivals[i] <= time) {
            enqueue(i);
            i++;
        }
        if (!queue_is_empty() && time >= current_end) {
            int cust;
            dequeue(&cust);
            int wait = time - arrivals[cust];
            total_wait = total_wait + wait;
            current_end = time + service[cust];
            printf("Customer %d: arrived=%d, served=%d, wait=%d\n",
                   cust, arrivals[cust], time, wait);
            served++;
        }
        time++;
    }
    printf("Total customers: %d\n", served);
    printf("Total wait: %d\n", total_wait);
}

int main(void) {
    queue_init(8);
    enqueue(10);
    enqueue(20);
    enqueue(30);
    printf("Size: %d\n", q_count);
    printf("Front: %d\n", queue_peek());

    int val;
    dequeue(&val);
    printf("Dequeued: %d\n", val);
    dequeue(&val);
    printf("Dequeued: %d\n", val);
    printf("Size: %d\n", q_count);

    printf("\nBank simulation:\n");
    simulate_bank();

    return 0;
}
