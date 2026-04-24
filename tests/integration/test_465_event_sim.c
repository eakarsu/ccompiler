int printf(const char *fmt, ...);
// EXPECT: === Queue Simulation ===\nT=0: Customer 1 arrives, served immediately\nT=3: Customer 2 arrives, waits 1\nT=4: Customer 1 done\nT=4: Customer 2 arrives, served immediately\nT=5: Customer 3 arrives, waits 3\nT=7: Customer 4 arrives, waits 1\nT=8: Customer 3 arrives, served immediately\nT=8: Customer 4 arrives, waits 4\nT=8: Customer 5 arrives, waits 4\nT=8: Customer 2 done\nT=12: Customer 5 arrives, served immediately\nT=12: Customer 3 done\nT=12: Customer 6 arrives, served immediately\nT=12: Customer 4 arrives, waits 4\nT=15: Customer 7 arrives, waits 1\nT=16: Customer 4 arrives, served immediately\nT=16: Customer 7 arrives, waits 4\nT=16: Customer 6 done\nT=16: Customer 5 done\nT=18: Customer 8 arrives, served immediately\nT=20: Customer 7 arrives, waits 2\nT=20: Customer 4 done\nT=22: Customer 7 arrives, served immediately\nT=22: Customer 8 done\nT=26: Customer 7 done\nCustomers served: 8\nTotal wait: 24
// Event-driven simulator with priority queue

struct Event {
    int time;
    int type;
    int data;
};

struct EventQueue {
    struct Event events[32];
    int size;
};

void eq_init(struct EventQueue *eq) {
    eq->size = 0;
}

void eq_swap(struct Event *a, struct Event *b) {
    struct Event tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void eq_push(struct EventQueue *eq, int time, int type, int data) {
    int i;
    if (eq->size >= 32) return;
    i = eq->size;
    eq->events[i].time = time;
    eq->events[i].type = type;
    eq->events[i].data = data;
    eq->size++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (eq->events[parent].time > eq->events[i].time) {
            eq_swap(&eq->events[parent], &eq->events[i]);
            i = parent;
        } else {
            break;
        }
    }
}

struct Event eq_pop(struct EventQueue *eq) {
    struct Event top;
    int i;
    int left;
    int right;
    int smallest;
    top = eq->events[0];
    eq->size--;
    eq->events[0] = eq->events[eq->size];
    i = 0;
    while (1) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;
        if (left < eq->size && eq->events[left].time < eq->events[smallest].time)
            smallest = left;
        if (right < eq->size && eq->events[right].time < eq->events[smallest].time)
            smallest = right;
        if (smallest != i) {
            eq_swap(&eq->events[i], &eq->events[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
    return top;
}

int main(void) {
    struct EventQueue eq;
    struct Event e;
    int server_busy;
    int customers_served;
    int total_wait;
    int current_time;
    int service_end;
    int i;

    eq_init(&eq);
    server_busy = 0;
    customers_served = 0;
    total_wait = 0;
    current_time = 0;
    service_end = 0;

    eq_push(&eq, 0, 0, 1);
    eq_push(&eq, 3, 0, 2);
    eq_push(&eq, 5, 0, 3);
    eq_push(&eq, 7, 0, 4);
    eq_push(&eq, 8, 0, 5);
    eq_push(&eq, 12, 0, 6);
    eq_push(&eq, 15, 0, 7);
    eq_push(&eq, 18, 0, 8);

    printf("=== Queue Simulation ===\n");
    while (eq.size > 0) {
        e = eq_pop(&eq);
        current_time = e.time;

        if (e.type == 0) {
            printf("T=%d: Customer %d arrives", current_time, e.data);
            if (!server_busy || current_time >= service_end) {
                server_busy = 1;
                service_end = current_time + 4;
                printf(", served immediately\n");
                eq_push(&eq, service_end, 1, e.data);
            } else {
                int wait = service_end - current_time;
                total_wait += wait;
                printf(", waits %d\n", wait);
                eq_push(&eq, service_end, 0, e.data);
            }
        } else {
            printf("T=%d: Customer %d done\n", current_time, e.data);
            customers_served++;
            server_busy = 0;
        }
    }
    printf("Customers served: %d\n", customers_served);
    printf("Total wait: %d\n", total_wait);

    return 0;
}
