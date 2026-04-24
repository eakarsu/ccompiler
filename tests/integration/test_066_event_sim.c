int printf(const char *fmt, ...);
// EXPECT: 1: 5 10 20 30 40 50 5\n2: 3 2 1 6 30\n3: 10-1 10-2 10-3 20-1 20-2 \n4: 0 -1 0\n5: 1 42 42 1 99 0\n6: 5 5 23\n7: 8 8 70\n8: 150 5\n9: 6 50\n10: 5 5 10 45\n11: 5 3 15 25\n12: 10-1 20-2 30-3 40-4 50-5 \n13: 20 1 20\n14: 6 12 4 \n15: 15 2 25\n16: 14 12 2 10\n17: 10 4 14 50\n18: 30 10 20 \n19: 550 10 55\n20: 80 90 60 65 50 25 45 35 \n21: 10 90 111\n22: 0 4 12
// ============================================================================
// Discrete Event Simulation
// ============================================================================
// This file implements a discrete event simulation using sorted arrays.
// Features:
// 1. Event scheduling and priority queue (sorted by time)
// 2. Event processing in chronological order
// 3. Time advancement
// 4. Statistics collection
// 5. Multiple event types and handlers
// ============================================================================

#define MAX_EVENTS 100
#define MAX_STATS 50

// ============================================================================
// Event structure (stored in parallel arrays)
// ============================================================================
int evt_time[MAX_EVENTS];       // Time of event
int evt_type[MAX_EVENTS];       // Type of event (1=arrival, 2=departure, 3=timeout, etc.)
int evt_data[MAX_EVENTS];       // Associated data
int evt_count;                  // Number of events in queue

// ============================================================================
// Statistics arrays
// ============================================================================
int stat_processed[MAX_STATS];  // Count of processed events per type
int stat_total_wait[MAX_STATS]; // Total wait time per type
int current_time;               // Current simulation time
int total_events_processed;     // Total events processed

// ============================================================================
// Initialize event queue and statistics
// ============================================================================
void sim_init(void) {
    int i;
    evt_count = 0;
    current_time = 0;
    total_events_processed = 0;
    for (i = 0; i < MAX_STATS; i = i + 1) {
        stat_processed[i] = 0;
        stat_total_wait[i] = 0;
    }
}

// ============================================================================
// Schedule an event (insert into sorted position by time)
// ============================================================================
void schedule_event(int time, int type, int data) {
    int i;
    int pos;

    if (evt_count >= MAX_EVENTS) return;

    // Find insertion position (sorted by time, stable)
    pos = evt_count;
    for (i = 0; i < evt_count; i = i + 1) {
        if (time < evt_time[i]) {
            pos = i;
            i = evt_count; // break
        }
    }

    // Shift elements to make room
    for (i = evt_count; i > pos; i = i - 1) {
        evt_time[i] = evt_time[i - 1];
        evt_type[i] = evt_type[i - 1];
        evt_data[i] = evt_data[i - 1];
    }

    // Insert new event
    evt_time[pos] = time;
    evt_type[pos] = type;
    evt_data[pos] = data;
    evt_count = evt_count + 1;
}

// ============================================================================
// Remove and return the next event (earliest time)
// Returns 1 if an event was dequeued, 0 if queue is empty
// Output parameters: out_time, out_type, out_data
// ============================================================================
int next_event_time;
int next_event_type;
int next_event_data;

int dequeue_event(void) {
    int i;

    if (evt_count == 0) return 0;

    next_event_time = evt_time[0];
    next_event_type = evt_type[0];
    next_event_data = evt_data[0];

    // Shift remaining events
    for (i = 1; i < evt_count; i = i + 1) {
        evt_time[i - 1] = evt_time[i];
        evt_type[i - 1] = evt_type[i];
        evt_data[i - 1] = evt_data[i];
    }
    evt_count = evt_count - 1;

    return 1;
}

// ============================================================================
// Peek at the next event without removing it
// Returns the time of the next event, or -1 if empty
// ============================================================================
int peek_time(void) {
    if (evt_count == 0) return -1;
    return evt_time[0];
}

// ============================================================================
// Process an event and update statistics
// ============================================================================
void process_event(void) {
    current_time = next_event_time;
    stat_processed[next_event_type] = stat_processed[next_event_type] + 1;
    total_events_processed = total_events_processed + 1;
}

// ============================================================================
// Get count of events still in queue
// ============================================================================
int queue_size(void) {
    return evt_count;
}

// ============================================================================
// Test Case 1: Basic event scheduling and processing order
// ============================================================================
void test_case_1(void) {
    printf("%d", 1);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();

    // Schedule events out of order
    schedule_event(30, 1, 100);
    schedule_event(10, 2, 200);
    schedule_event(50, 3, 300);
    schedule_event(20, 1, 400);
    schedule_event(40, 2, 500);

    printf("%d", queue_size());
    printf("%c", ' ');

    // Process all events and print times (should be sorted)
    while (dequeue_event()) {
        process_event();
        printf("%d", next_event_time);
        printf("%c", ' ');
    }
    printf("%d", total_events_processed);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 2: Event types and statistics
// ============================================================================
void test_case_2(void) {
    printf("%d", 2);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();

    schedule_event(5, 1, 10);
    schedule_event(10, 1, 20);
    schedule_event(15, 2, 30);
    schedule_event(20, 1, 40);
    schedule_event(25, 2, 50);
    schedule_event(30, 3, 60);

    while (dequeue_event()) {
        process_event();
    }

    // Print count per type
    printf("%d", stat_processed[1]);
    printf("%c", ' ');
    printf("%d", stat_processed[2]);
    printf("%c", ' ');
    printf("%d", stat_processed[3]);
    printf("%c", ' ');
    printf("%d", total_events_processed);
    printf("%c", ' ');
    printf("%d", current_time);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 3: Same time events (stability test)
// ============================================================================
void test_case_3(void) {
    printf("%d", 3);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();

    // Multiple events at the same time
    schedule_event(10, 1, 1);
    schedule_event(10, 2, 2);
    schedule_event(10, 3, 3);
    schedule_event(20, 1, 4);
    schedule_event(20, 2, 5);

    while (dequeue_event()) {
        process_event();
        printf("%d", next_event_time);
        printf("%c", '-');
        printf("%d", next_event_type);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 4: Empty queue operations
// ============================================================================
void test_case_4(void) {
    printf("%d", 4);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();

    printf("%d", queue_size());
    printf("%c", ' ');
    printf("%d", peek_time());
    printf("%c", ' ');
    printf("%d", dequeue_event());
    printf("%c", '\n');
}

// ============================================================================
// Test Case 5: Single event
// ============================================================================
void test_case_5(void) {
    printf("%d", 5);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    schedule_event(42, 1, 99);

    printf("%d", queue_size());
    printf("%c", ' ');
    printf("%d", peek_time());
    printf("%c", ' ');

    dequeue_event();
    process_event();
    printf("%d", next_event_time);
    printf("%c", ' ');
    printf("%d", next_event_type);
    printf("%c", ' ');
    printf("%d", next_event_data);
    printf("%c", ' ');
    printf("%d", queue_size());
    printf("%c", '\n');
}

// ============================================================================
// Test Case 6: Arrival-Departure simulation (simple queue model)
// Customers arrive and are served after a fixed delay
// ============================================================================
void test_case_6(void) {
    int arrivals;
    int departures;
    int service_time;
    int server_busy;
    int wait_count;
    int total_wait;
    int arrival_time;

    printf("%d", 6);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    arrivals = 0;
    departures = 0;
    service_time = 5;
    server_busy = 0;
    wait_count = 0;
    total_wait = 0;

    // Schedule 5 arrivals at times 0, 3, 8, 12, 18
    schedule_event(0, 1, 0);    // arrival
    schedule_event(3, 1, 1);
    schedule_event(8, 1, 2);
    schedule_event(12, 1, 3);
    schedule_event(18, 1, 4);

    while (dequeue_event()) {
        process_event();

        if (next_event_type == 1) {
            // Arrival
            arrivals = arrivals + 1;
            if (server_busy == 0) {
                server_busy = 1;
                // Schedule departure
                schedule_event(current_time + service_time, 2, next_event_data);
            } else {
                wait_count = wait_count + 1;
                // Schedule departure after current one
                // Simple: just schedule at current_time + service_time
                schedule_event(current_time + service_time, 2, next_event_data);
            }
        } else if (next_event_type == 2) {
            // Departure
            departures = departures + 1;
            if (queue_size() == 0) {
                server_busy = 0;
            }
        }
    }

    printf("%d", arrivals);
    printf("%c", ' ');
    printf("%d", departures);
    printf("%c", ' ');
    printf("%d", current_time);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 7: Periodic event generation
// Generate events at regular intervals
// ============================================================================
void test_case_7(void) {
    int count;
    int interval;
    int i;

    printf("%d", 7);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    interval = 10;

    // Schedule periodic events
    for (i = 0; i < 8; i = i + 1) {
        schedule_event(i * interval, 1, i);
    }

    printf("%d", queue_size());
    printf("%c", ' ');

    count = 0;
    while (dequeue_event()) {
        process_event();
        count = count + 1;
    }
    printf("%d", count);
    printf("%c", ' ');
    printf("%d", current_time);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 8: Event data tracking
// ============================================================================
void test_case_8(void) {
    int data_sum;

    printf("%d", 8);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    schedule_event(1, 1, 10);
    schedule_event(2, 1, 20);
    schedule_event(3, 1, 30);
    schedule_event(4, 1, 40);
    schedule_event(5, 1, 50);

    data_sum = 0;
    while (dequeue_event()) {
        process_event();
        data_sum = data_sum + next_event_data;
    }

    printf("%d", data_sum);
    printf("%c", ' ');
    printf("%d", total_events_processed);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 9: Cascading events (events that generate new events)
// ============================================================================
void test_case_9(void) {
    int count;

    printf("%d", 9);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    schedule_event(0, 1, 0);

    count = 0;
    while (dequeue_event()) {
        process_event();
        count = count + 1;

        // Each event generates a follow-up event, up to time 50
        if (current_time + 10 <= 50) {
            schedule_event(current_time + 10, 1, count);
        }
    }

    printf("%d", count);
    printf("%c", ' ');
    printf("%d", current_time);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 10: Multiple event types interleaved
// ============================================================================
void test_case_10(void) {
    printf("%d", 10);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();

    // Type 1 at times 0, 10, 20, 30, 40
    schedule_event(0, 1, 0);
    schedule_event(10, 1, 0);
    schedule_event(20, 1, 0);
    schedule_event(30, 1, 0);
    schedule_event(40, 1, 0);

    // Type 2 at times 5, 15, 25, 35, 45
    schedule_event(5, 2, 0);
    schedule_event(15, 2, 0);
    schedule_event(25, 2, 0);
    schedule_event(35, 2, 0);
    schedule_event(45, 2, 0);

    while (dequeue_event()) {
        process_event();
    }

    printf("%d", stat_processed[1]);
    printf("%c", ' ');
    printf("%d", stat_processed[2]);
    printf("%c", ' ');
    printf("%d", total_events_processed);
    printf("%c", ' ');
    printf("%d", current_time);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 11: Peek without dequeue
// ============================================================================
void test_case_11(void) {
    printf("%d", 11);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    schedule_event(15, 1, 0);
    schedule_event(5, 2, 0);
    schedule_event(25, 3, 0);

    // Peek should show earliest (5)
    printf("%d", peek_time());
    printf("%c", ' ');

    // Queue size should be unchanged
    printf("%d", queue_size());
    printf("%c", ' ');

    // Dequeue and peek again
    dequeue_event();
    printf("%d", peek_time());
    printf("%c", ' ');

    dequeue_event();
    printf("%d", peek_time());
    printf("%c", '\n');
}

// ============================================================================
// Test Case 12: Reverse order scheduling
// ============================================================================
void test_case_12(void) {
    int i;

    printf("%d", 12);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();

    // Schedule in reverse order
    schedule_event(50, 1, 5);
    schedule_event(40, 1, 4);
    schedule_event(30, 1, 3);
    schedule_event(20, 1, 2);
    schedule_event(10, 1, 1);

    // Should still come out in order
    while (dequeue_event()) {
        process_event();
        printf("%d", next_event_time);
        printf("%c", '-');
        printf("%d", next_event_data);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 13: Large batch scheduling
// ============================================================================
void test_case_13(void) {
    int i;
    int last_time;
    int ordered;

    printf("%d", 13);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();

    // Schedule 20 events with various times
    for (i = 0; i < 20; i = i + 1) {
        // times: 19, 17, 15, ..., 1 (for even i), 0, 2, 4, ..., 18 (for odd i)
        // Actually let's just use a simple pattern
        schedule_event((i * 7) - ((i * 7) / 20) * 20, 1, i);
    }

    printf("%d", queue_size());
    printf("%c", ' ');

    // Process all and verify they come out in non-decreasing order
    last_time = -1;
    ordered = 1;
    while (dequeue_event()) {
        process_event();
        if (next_event_time < last_time) {
            ordered = 0;
        }
        last_time = next_event_time;
    }
    printf("%d", ordered);
    printf("%c", ' ');
    printf("%d", total_events_processed);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 14: Event-driven counter simulation
// ============================================================================
void test_case_14(void) {
    int counter;

    printf("%d", 14);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    counter = 0;

    // Type 1 = increment, type 2 = decrement, type 3 = report
    schedule_event(1, 1, 5);    // increment by 5
    schedule_event(2, 1, 3);    // increment by 3
    schedule_event(3, 2, 2);    // decrement by 2
    schedule_event(4, 3, 0);    // report
    schedule_event(5, 1, 10);   // increment by 10
    schedule_event(6, 2, 4);    // decrement by 4
    schedule_event(7, 3, 0);    // report
    schedule_event(8, 2, 8);    // decrement by 8
    schedule_event(9, 3, 0);    // report

    while (dequeue_event()) {
        process_event();
        if (next_event_type == 1) {
            counter = counter + next_event_data;
        } else if (next_event_type == 2) {
            counter = counter - next_event_data;
        } else if (next_event_type == 3) {
            printf("%d", counter);
            printf("%c", ' ');
        }
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 15: Time gap analysis
// ============================================================================
void test_case_15(void) {
    int prev_time;
    int max_gap;
    int min_gap;
    int gap;

    printf("%d", 15);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    schedule_event(0, 1, 0);
    schedule_event(3, 1, 0);
    schedule_event(8, 1, 0);
    schedule_event(10, 1, 0);
    schedule_event(25, 1, 0);

    prev_time = -1;
    max_gap = 0;
    min_gap = 999999;

    while (dequeue_event()) {
        process_event();
        if (prev_time >= 0) {
            gap = next_event_time - prev_time;
            if (gap > max_gap) max_gap = gap;
            if (gap < min_gap) min_gap = gap;
        }
        prev_time = next_event_time;
    }

    printf("%d", max_gap);
    printf("%c", ' ');
    printf("%d", min_gap);
    printf("%c", ' ');
    printf("%d", current_time);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 16: Producer-consumer simulation
// ============================================================================
void test_case_16(void) {
    int buffer;
    int max_buffer;
    int produced;
    int consumed;
    int max_seen;

    printf("%d", 16);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    buffer = 0;
    max_buffer = 10;
    produced = 0;
    consumed = 0;
    max_seen = 0;

    // Type 1 = produce, type 2 = consume
    schedule_event(1, 1, 3);
    schedule_event(2, 1, 5);
    schedule_event(3, 2, 2);
    schedule_event(4, 1, 4);
    schedule_event(5, 2, 6);
    schedule_event(6, 2, 3);
    schedule_event(7, 1, 2);
    schedule_event(8, 2, 1);

    while (dequeue_event()) {
        process_event();
        if (next_event_type == 1) {
            buffer = buffer + next_event_data;
            produced = produced + next_event_data;
            if (buffer > max_buffer) buffer = max_buffer;
        } else {
            if (next_event_data > buffer) {
                consumed = consumed + buffer;
                buffer = 0;
            } else {
                buffer = buffer - next_event_data;
                consumed = consumed + next_event_data;
            }
        }
        if (buffer > max_seen) max_seen = buffer;
    }

    printf("%d", produced);
    printf("%c", ' ');
    printf("%d", consumed);
    printf("%c", ' ');
    printf("%d", buffer);
    printf("%c", ' ');
    printf("%d", max_seen);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 17: Multi-phase simulation
// Phase 1 (time 0-20): high frequency events
// Phase 2 (time 20-50): low frequency events
// ============================================================================
void test_case_17(void) {
    int phase1_count;
    int phase2_count;

    printf("%d", 17);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    phase1_count = 0;
    phase2_count = 0;

    // Phase 1 events: every 2 time units
    schedule_event(0, 1, 0);
    schedule_event(2, 1, 0);
    schedule_event(4, 1, 0);
    schedule_event(6, 1, 0);
    schedule_event(8, 1, 0);
    schedule_event(10, 1, 0);
    schedule_event(12, 1, 0);
    schedule_event(14, 1, 0);
    schedule_event(16, 1, 0);
    schedule_event(18, 1, 0);

    // Phase 2 events: every 10 time units
    schedule_event(20, 2, 0);
    schedule_event(30, 2, 0);
    schedule_event(40, 2, 0);
    schedule_event(50, 2, 0);

    while (dequeue_event()) {
        process_event();
        if (next_event_type == 1) {
            phase1_count = phase1_count + 1;
        } else {
            phase2_count = phase2_count + 1;
        }
    }

    printf("%d", phase1_count);
    printf("%c", ' ');
    printf("%d", phase2_count);
    printf("%c", ' ');
    printf("%d", total_events_processed);
    printf("%c", ' ');
    printf("%d", current_time);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 18: Event priority within same time
// ============================================================================
void test_case_18(void) {
    printf("%d", 18);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();

    // All at time 10 with different types and data
    schedule_event(10, 3, 30);
    schedule_event(10, 1, 10);
    schedule_event(10, 2, 20);

    while (dequeue_event()) {
        process_event();
        printf("%d", next_event_data);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 19: Running average computation
// ============================================================================
void test_case_19(void) {
    int sum;
    int count;

    printf("%d", 19);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    sum = 0;
    count = 0;

    schedule_event(1, 1, 10);
    schedule_event(2, 1, 20);
    schedule_event(3, 1, 30);
    schedule_event(4, 1, 40);
    schedule_event(5, 1, 50);
    schedule_event(6, 1, 60);
    schedule_event(7, 1, 70);
    schedule_event(8, 1, 80);
    schedule_event(9, 1, 90);
    schedule_event(10, 1, 100);

    while (dequeue_event()) {
        process_event();
        sum = sum + next_event_data;
        count = count + 1;
    }

    // Average of 10+20+...+100 = 550/10 = 55
    printf("%d", sum);
    printf("%c", ' ');
    printf("%d", count);
    printf("%c", ' ');
    printf("%d", sum / count);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 20: Simulation with conditional event generation
// ============================================================================
void test_case_20(void) {
    int alive;

    printf("%d", 20);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    alive = 100;

    // Type 1 = damage, type 2 = heal
    schedule_event(1, 1, 20);
    schedule_event(2, 2, 10);
    schedule_event(3, 1, 30);
    schedule_event(4, 2, 5);
    schedule_event(5, 1, 15);
    schedule_event(6, 1, 25);
    schedule_event(7, 2, 20);
    schedule_event(8, 1, 10);

    while (dequeue_event()) {
        process_event();
        if (next_event_type == 1) {
            alive = alive - next_event_data;
        } else {
            alive = alive + next_event_data;
        }
        printf("%d", alive);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 21: Throughput calculation
// ============================================================================
void test_case_21(void) {
    int start_time;
    int end_time;
    int count;

    printf("%d", 21);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    count = 0;

    // 10 events over 100 time units
    schedule_event(10, 1, 0);
    schedule_event(20, 1, 0);
    schedule_event(30, 1, 0);
    schedule_event(40, 1, 0);
    schedule_event(50, 1, 0);
    schedule_event(60, 1, 0);
    schedule_event(70, 1, 0);
    schedule_event(80, 1, 0);
    schedule_event(90, 1, 0);
    schedule_event(100, 1, 0);

    start_time = 10;
    end_time = 0;

    while (dequeue_event()) {
        process_event();
        count = count + 1;
        end_time = next_event_time;
    }

    // Throughput = count * 1000 / duration (scaled to avoid fractions)
    printf("%d", count);
    printf("%c", ' ');
    printf("%d", end_time - start_time);
    printf("%c", ' ');
    // 10 events in 90 time units = 111 per 1000 time units
    printf("%d", (count * 1000) / (end_time - start_time));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 22: Queue depth tracking over time
// ============================================================================
void test_case_22(void) {
    int depth;
    int max_depth;

    printf("%d", 22);
    printf("%c", ':');
    printf("%c", ' ');

    sim_init();
    depth = 0;
    max_depth = 0;

    // Arrivals
    schedule_event(1, 1, 0);
    schedule_event(2, 1, 0);
    schedule_event(3, 1, 0);
    schedule_event(4, 1, 0);
    // Departures
    schedule_event(5, 2, 0);
    schedule_event(6, 2, 0);
    // More arrivals
    schedule_event(7, 1, 0);
    schedule_event(8, 1, 0);
    // More departures
    schedule_event(9, 2, 0);
    schedule_event(10, 2, 0);
    schedule_event(11, 2, 0);
    schedule_event(12, 2, 0);

    while (dequeue_event()) {
        process_event();
        if (next_event_type == 1) {
            depth = depth + 1;
        } else {
            depth = depth - 1;
        }
        if (depth > max_depth) max_depth = depth;
    }

    printf("%d", depth);
    printf("%c", ' ');
    printf("%d", max_depth);
    printf("%c", ' ');
    printf("%d", total_events_processed);
    printf("%c", '\n');
}

// ============================================================================
// Main function
// ============================================================================

// 2: 3 2 1 6 30
// 3: 10-1 10-2 10-3 20-1 20-2
// 4: 0 -1 0
// 5: 1 42 42 1 99 0
// 6: 5 5 23
// 7: 8 8 70
// 8: 150 5
// 9: 6 50
// 10: 5 5 10 45
// 11: 5 3 15 25
// 12: 10-1 20-2 30-3 40-4 50-5
// 13: 20 1 20
// 14: 6 12 4
// 15: 15 2 25
// 16: 14 12 2 10
// 17: 10 4 14 50
// 18: 30 10 20
// 19: 550 10 55
// 20: 80 90 60 65 50 25 45 35
// 21: 10 90 111
// 22: 0 4 12

int main(void) {
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();
    test_case_5();
    test_case_6();
    test_case_7();
    test_case_8();
    test_case_9();
    test_case_10();
    test_case_11();
    test_case_12();
    test_case_13();
    test_case_14();
    test_case_15();
    test_case_16();
    test_case_17();
    test_case_18();
    test_case_19();
    test_case_20();
    test_case_21();
    test_case_22();
    return 0;
}
