int printf(const char *fmt, ...);

// Min-stack: stack with O(1) minimum tracking
// Uses two parallel stacks: one for values, one for tracking minimums

struct MinStack {
    int vals[64];
    int mins[64];
    int top;
};

void ms_init(struct MinStack *ms) {
    ms->top = -1;
}

int ms_empty(struct MinStack *ms) {
    return ms->top == -1;
}

int ms_size(struct MinStack *ms) {
    return ms->top + 1;
}

void ms_push(struct MinStack *ms, int val) {
    ms->top = ms->top + 1;
    ms->vals[ms->top] = val;
    if (ms->top == 0) {
        ms->mins[ms->top] = val;
    } else {
        int prev_min = ms->mins[ms->top - 1];
        if (val < prev_min) {
            ms->mins[ms->top] = val;
        } else {
            ms->mins[ms->top] = prev_min;
        }
    }
}

int ms_pop(struct MinStack *ms) {
    int val = ms->vals[ms->top];
    ms->top = ms->top - 1;
    return val;
}

int ms_peek(struct MinStack *ms) {
    return ms->vals[ms->top];
}

int ms_get_min(struct MinStack *ms) {
    if (ms->top < 0) return 999999;
    return ms->mins[ms->top];
}

// Max-stack variant: track maximum
struct MaxStack {
    int vals[64];
    int maxs[64];
    int top;
};

void mxs_init(struct MaxStack *ms) {
    ms->top = -1;
}

int mxs_empty(struct MaxStack *ms) {
    return ms->top == -1;
}

void mxs_push(struct MaxStack *ms, int val) {
    ms->top = ms->top + 1;
    ms->vals[ms->top] = val;
    if (ms->top == 0) {
        ms->maxs[ms->top] = val;
    } else {
        int prev_max = ms->maxs[ms->top - 1];
        if (val > prev_max) {
            ms->maxs[ms->top] = val;
        } else {
            ms->maxs[ms->top] = prev_max;
        }
    }
}

int mxs_pop(struct MaxStack *ms) {
    int val = ms->vals[ms->top];
    ms->top = ms->top - 1;
    return val;
}

int mxs_get_max(struct MaxStack *ms) {
    if (ms->top < 0) return -999999;
    return ms->maxs[ms->top];
}

// MinMaxStack: tracks both min and max
struct MinMaxStack {
    int vals[64];
    int mins[64];
    int maxs[64];
    int top;
};

void mms_init(struct MinMaxStack *s) {
    s->top = -1;
}

int mms_empty(struct MinMaxStack *s) {
    return s->top == -1;
}

void mms_push(struct MinMaxStack *s, int val) {
    s->top = s->top + 1;
    s->vals[s->top] = val;
    if (s->top == 0) {
        s->mins[s->top] = val;
        s->maxs[s->top] = val;
    } else {
        int pm = s->mins[s->top - 1];
        int px = s->maxs[s->top - 1];
        if (val < pm) s->mins[s->top] = val;
        else s->mins[s->top] = pm;
        if (val > px) s->maxs[s->top] = val;
        else s->maxs[s->top] = px;
    }
}

int mms_pop(struct MinMaxStack *s) {
    int val = s->vals[s->top];
    s->top = s->top - 1;
    return val;
}

int mms_get_min(struct MinMaxStack *s) {
    return s->mins[s->top];
}

int mms_get_max(struct MinMaxStack *s) {
    return s->maxs[s->top];
}

int mms_get_range(struct MinMaxStack *s) {
    return s->maxs[s->top] - s->mins[s->top];
}

int main() {
    // Basic MinStack tests
    struct MinStack ms;
    ms_init(&ms);

    printf("%d\n", ms_empty(&ms));
    // EXPECT: 1

    ms_push(&ms, 5);
    printf("%d\n", ms_get_min(&ms));
    // EXPECT: 5

    ms_push(&ms, 3);
    printf("%d\n", ms_get_min(&ms));
    // EXPECT: 3

    ms_push(&ms, 7);
    printf("%d\n", ms_get_min(&ms));
    // EXPECT: 3

    ms_push(&ms, 1);
    printf("%d\n", ms_get_min(&ms));
    // EXPECT: 1

    ms_push(&ms, 4);
    printf("%d\n", ms_get_min(&ms));
    // EXPECT: 1

    // Pop and check min updates
    ms_pop(&ms); // remove 4
    printf("%d\n", ms_get_min(&ms));
    // EXPECT: 1

    ms_pop(&ms); // remove 1
    printf("%d\n", ms_get_min(&ms));
    // EXPECT: 3

    ms_pop(&ms); // remove 7
    printf("%d\n", ms_get_min(&ms));
    // EXPECT: 3

    ms_pop(&ms); // remove 3
    printf("%d\n", ms_get_min(&ms));
    // EXPECT: 5

    ms_pop(&ms); // remove 5
    printf("%d\n", ms_empty(&ms));
    // EXPECT: 1

    // Push sequence and track all minimums
    struct MinStack ms2;
    ms_init(&ms2);
    int vals[10];
    vals[0] = 8; vals[1] = 4; vals[2] = 6; vals[3] = 2;
    vals[4] = 9; vals[5] = 1; vals[6] = 7; vals[7] = 3;
    vals[8] = 5; vals[9] = 10;

    int i = 0;
    while (i < 10) {
        ms_push(&ms2, vals[i]);
        i = i + 1;
    }
    // mins after each push: 8,4,4,2,2,1,1,1,1,1
    printf("%d\n", ms_get_min(&ms2));
    // EXPECT: 1

    // Pop down and check min at each step
    ms_pop(&ms2); // remove 10, min still 1
    printf("%d\n", ms_get_min(&ms2));
    // EXPECT: 1
    ms_pop(&ms2); // remove 5, min still 1
    printf("%d\n", ms_get_min(&ms2));
    // EXPECT: 1
    ms_pop(&ms2); // remove 3, min still 1
    printf("%d\n", ms_get_min(&ms2));
    // EXPECT: 1
    ms_pop(&ms2); // remove 7, min still 1
    printf("%d\n", ms_get_min(&ms2));
    // EXPECT: 1
    ms_pop(&ms2); // remove 1, min now 2
    printf("%d\n", ms_get_min(&ms2));
    // EXPECT: 2
    ms_pop(&ms2); // remove 9, min still 2
    printf("%d\n", ms_get_min(&ms2));
    // EXPECT: 2
    ms_pop(&ms2); // remove 2, min now 4
    printf("%d\n", ms_get_min(&ms2));
    // EXPECT: 4

    // MaxStack test
    struct MaxStack mxs;
    mxs_init(&mxs);
    mxs_push(&mxs, 2);
    mxs_push(&mxs, 8);
    mxs_push(&mxs, 5);
    mxs_push(&mxs, 10);
    mxs_push(&mxs, 3);
    printf("%d\n", mxs_get_max(&mxs));
    // EXPECT: 10
    mxs_pop(&mxs); // remove 3
    printf("%d\n", mxs_get_max(&mxs));
    // EXPECT: 10
    mxs_pop(&mxs); // remove 10
    printf("%d\n", mxs_get_max(&mxs));
    // EXPECT: 8

    // MinMaxStack range test
    struct MinMaxStack mms;
    mms_init(&mms);
    mms_push(&mms, 10);
    printf("%d\n", mms_get_range(&mms));
    // EXPECT: 0
    mms_push(&mms, 5);
    printf("%d\n", mms_get_range(&mms));
    // EXPECT: 5
    mms_push(&mms, 20);
    printf("%d\n", mms_get_range(&mms));
    // EXPECT: 15
    mms_push(&mms, 3);
    printf("%d\n", mms_get_range(&mms));
    // EXPECT: 17
    printf("%d %d\n", mms_get_min(&mms), mms_get_max(&mms));
    // EXPECT: 3 20
    mms_pop(&mms); // remove 3
    printf("%d %d\n", mms_get_min(&mms), mms_get_max(&mms));
    // EXPECT: 5 20
    mms_pop(&mms); // remove 20
    printf("%d %d\n", mms_get_min(&mms), mms_get_max(&mms));
    // EXPECT: 5 10

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
