int printf(const char *fmt, ...);

struct Todo {
    char desc[24];
    int priority;
    int completed;
    int id;
};

int str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
    return i;
}

int str_eq(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int next_id;

int add_todo(struct Todo *list, int *count, char *desc, int priority) {
    int idx;
    idx = *count;
    if (idx >= 20) return -1;
    str_copy(list[idx].desc, desc);
    list[idx].priority = priority;
    list[idx].completed = 0;
    next_id = next_id + 1;
    list[idx].id = next_id;
    *count = *count + 1;
    return list[idx].id;
}

int find_by_id(struct Todo *list, int count, int id) {
    int i;
    i = 0;
    while (i < count) {
        if (list[i].id == id) return i;
        i = i + 1;
    }
    return -1;
}

int complete_todo(struct Todo *list, int count, int id) {
    int idx;
    idx = find_by_id(list, count, id);
    if (idx < 0) return -1;
    if (list[idx].completed) return -2;
    list[idx].completed = 1;
    return 0;
}

int count_pending(struct Todo *list, int count) {
    int i;
    int c;
    c = 0;
    i = 0;
    while (i < count) {
        if (list[i].completed == 0) c = c + 1;
        i = i + 1;
    }
    return c;
}

int count_completed(struct Todo *list, int count) {
    return count - count_pending(list, count);
}

int count_by_priority(struct Todo *list, int count, int priority) {
    int i;
    int c;
    c = 0;
    i = 0;
    while (i < count) {
        if (list[i].priority == priority && list[i].completed == 0) c = c + 1;
        i = i + 1;
    }
    return c;
}

int highest_priority_pending(struct Todo *list, int count) {
    int i;
    int best;
    int best_pri;
    best = -1;
    best_pri = -1;
    i = 0;
    while (i < count) {
        if (list[i].completed == 0 && list[i].priority > best_pri) {
            best_pri = list[i].priority;
            best = i;
        }
        i = i + 1;
    }
    return best;
}

/* Sort by priority descending (bubble sort) */
int sort_by_priority(struct Todo *list, int count) {
    int i;
    int j;
    struct Todo temp;
    i = 0;
    while (i < count - 1) {
        j = 0;
        while (j < count - 1 - i) {
            if (list[j].priority < list[j + 1].priority) {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return 0;
}

int sum_priorities(struct Todo *list, int count) {
    int i;
    int sum;
    sum = 0;
    i = 0;
    while (i < count) {
        if (list[i].completed == 0) {
            sum = sum + list[i].priority;
        }
        i = i + 1;
    }
    return sum;
}

int delete_completed(struct Todo *list, int *count) {
    int i;
    int j;
    int removed;
    removed = 0;
    i = 0;
    while (i < *count) {
        if (list[i].completed) {
            j = i;
            while (j < *count - 1) {
                list[j] = list[j + 1];
                j = j + 1;
            }
            *count = *count - 1;
            removed = removed + 1;
        } else {
            i = i + 1;
        }
    }
    return removed;
}

int main(void) {
    struct Todo list[20];
    int count;
    int id1;
    int id2;
    int id3;
    int id4;
    int id5;
    int result;
    int idx;

    count = 0;
    next_id = 0;

    id1 = add_todo(list, &count, "Buy groceries", 2);
    id2 = add_todo(list, &count, "Write report", 5);
    id3 = add_todo(list, &count, "Clean house", 3);
    id4 = add_todo(list, &count, "Fix bug", 5);
    id5 = add_todo(list, &count, "Read book", 1);

    // EXPECT: 5
    printf("%d\n", count);
    // EXPECT: 1
    printf("%d\n", id1);
    // EXPECT: 5
    printf("%d\n", id5);

    // EXPECT: 5
    printf("%d\n", count_pending(list, count));
    // EXPECT: 0
    printf("%d\n", count_completed(list, count));

    /* Priority 5 items: Write report, Fix bug */
    // EXPECT: 2
    printf("%d\n", count_by_priority(list, count, 5));

    /* Priority 1 items: Read book */
    // EXPECT: 1
    printf("%d\n", count_by_priority(list, count, 1));

    /* Sum of priorities: 2+5+3+5+1 = 16 */
    // EXPECT: 16
    printf("%d\n", sum_priorities(list, count));

    /* Highest priority pending */
    idx = highest_priority_pending(list, count);
    // EXPECT: 5
    printf("%d\n", list[idx].priority);

    /* Complete "Write report" (id2=2) */
    result = complete_todo(list, count, id2);
    // EXPECT: 0
    printf("%d\n", result);

    // EXPECT: 4
    printf("%d\n", count_pending(list, count));
    // EXPECT: 1
    printf("%d\n", count_completed(list, count));

    /* Complete again fails */
    result = complete_todo(list, count, id2);
    // EXPECT: -2
    printf("%d\n", result);

    /* Complete non-existent */
    result = complete_todo(list, count, 99);
    // EXPECT: -1
    printf("%d\n", result);

    /* Priority 5 pending now: just Fix bug */
    // EXPECT: 1
    printf("%d\n", count_by_priority(list, count, 5));

    /* Sum of pending priorities: 2+3+5+1 = 11 */
    // EXPECT: 11
    printf("%d\n", sum_priorities(list, count));

    /* Complete "Buy groceries" (id1=1) */
    complete_todo(list, count, id1);

    /* Delete completed (Write report + Buy groceries) */
    result = delete_completed(list, &count);
    // EXPECT: 2
    printf("%d\n", result);

    // EXPECT: 3
    printf("%d\n", count);

    /* Sort remaining by priority */
    sort_by_priority(list, count);

    /* After sort: Fix bug(5), Clean house(3), Read book(1) */
    // EXPECT: 5
    printf("%d\n", list[0].priority);
    // EXPECT: 3
    printf("%d\n", list[1].priority);
    // EXPECT: 1
    printf("%d\n", list[2].priority);

    return 0;
}
