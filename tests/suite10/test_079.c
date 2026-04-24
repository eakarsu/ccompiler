int printf(const char *fmt, ...);

// Test 079: Error recovery state machine - system with states and error transitions

enum SystemState {
    ST_IDLE,
    ST_INIT,
    ST_RUNNING,
    ST_DEGRADED,
    ST_RECOVERING,
    ST_ERROR,
    ST_SHUTDOWN
};

enum Event {
    EV_START,
    EV_INIT_OK,
    EV_INIT_FAIL,
    EV_COMPONENT_FAIL,
    EV_RECOVERY_OK,
    EV_RECOVERY_FAIL,
    EV_CRITICAL_FAIL,
    EV_STOP
};

struct SystemContext {
    int state;
    int prev_state;
    int error_count;
    int recovery_attempts;
    int max_recovery;
    int components_active;
    int total_components;
    int transitions;
    int history[30];
    int history_count;
};

char *state_str(int s) {
    if (s == ST_IDLE) return "IDLE";
    if (s == ST_INIT) return "INIT";
    if (s == ST_RUNNING) return "RUNNING";
    if (s == ST_DEGRADED) return "DEGRADED";
    if (s == ST_RECOVERING) return "RECOVERING";
    if (s == ST_ERROR) return "ERROR";
    if (s == ST_SHUTDOWN) return "SHUTDOWN";
    return "UNKNOWN";
}

char *event_str(int e) {
    if (e == EV_START) return "START";
    if (e == EV_INIT_OK) return "INIT_OK";
    if (e == EV_INIT_FAIL) return "INIT_FAIL";
    if (e == EV_COMPONENT_FAIL) return "COMP_FAIL";
    if (e == EV_RECOVERY_OK) return "RECOV_OK";
    if (e == EV_RECOVERY_FAIL) return "RECOV_FAIL";
    if (e == EV_CRITICAL_FAIL) return "CRIT_FAIL";
    if (e == EV_STOP) return "STOP";
    return "UNKNOWN";
}

void ctx_init(struct SystemContext *ctx) {
    ctx->state = ST_IDLE;
    ctx->prev_state = ST_IDLE;
    ctx->error_count = 0;
    ctx->recovery_attempts = 0;
    ctx->max_recovery = 3;
    ctx->components_active = 0;
    ctx->total_components = 4;
    ctx->transitions = 0;
    ctx->history_count = 0;
}

void transition(struct SystemContext *ctx, int new_state) {
    ctx->prev_state = ctx->state;
    ctx->state = new_state;
    ctx->transitions++;
    if (ctx->history_count < 30) {
        ctx->history[ctx->history_count] = new_state;
        ctx->history_count++;
    }
}

int process_event(struct SystemContext *ctx, int event) {
    int old_state = ctx->state;

    switch (ctx->state) {
        case ST_IDLE:
            if (event == EV_START) {
                transition(ctx, ST_INIT);
                return 1;
            }
            break;

        case ST_INIT:
            if (event == EV_INIT_OK) {
                ctx->components_active = ctx->total_components;
                transition(ctx, ST_RUNNING);
                return 1;
            }
            if (event == EV_INIT_FAIL) {
                ctx->error_count++;
                transition(ctx, ST_ERROR);
                return 1;
            }
            break;

        case ST_RUNNING:
            if (event == EV_COMPONENT_FAIL) {
                ctx->components_active--;
                ctx->error_count++;
                if (ctx->components_active <= ctx->total_components / 2) {
                    transition(ctx, ST_DEGRADED);
                }
                return 1;
            }
            if (event == EV_CRITICAL_FAIL) {
                ctx->error_count++;
                transition(ctx, ST_ERROR);
                return 1;
            }
            if (event == EV_STOP) {
                transition(ctx, ST_SHUTDOWN);
                return 1;
            }
            break;

        case ST_DEGRADED:
            if (event == EV_COMPONENT_FAIL) {
                ctx->components_active--;
                ctx->error_count++;
                if (ctx->components_active == 0) {
                    transition(ctx, ST_ERROR);
                } else {
                    transition(ctx, ST_RECOVERING);
                    ctx->recovery_attempts = 0;
                }
                return 1;
            }
            if (event == EV_RECOVERY_OK) {
                ctx->components_active = ctx->total_components;
                transition(ctx, ST_RUNNING);
                return 1;
            }
            if (event == EV_STOP) {
                transition(ctx, ST_SHUTDOWN);
                return 1;
            }
            break;

        case ST_RECOVERING:
            if (event == EV_RECOVERY_OK) {
                ctx->components_active = ctx->total_components;
                ctx->recovery_attempts = 0;
                transition(ctx, ST_RUNNING);
                return 1;
            }
            if (event == EV_RECOVERY_FAIL) {
                ctx->recovery_attempts++;
                if (ctx->recovery_attempts >= ctx->max_recovery) {
                    transition(ctx, ST_ERROR);
                }
                return 1;
            }
            break;

        case ST_ERROR:
            if (event == EV_START) {
                ctx->recovery_attempts = 0;
                transition(ctx, ST_INIT);
                return 1;
            }
            if (event == EV_STOP) {
                transition(ctx, ST_SHUTDOWN);
                return 1;
            }
            break;

        case ST_SHUTDOWN:
            break;
    }

    return 0;
}

int main() {
    struct SystemContext ctx;
    ctx_init(&ctx);

    printf("Initial: %s\n", state_str(ctx.state));
    // EXPECT: Initial: IDLE

    process_event(&ctx, EV_START);
    printf("After START: %s\n", state_str(ctx.state));
    // EXPECT: After START: INIT

    process_event(&ctx, EV_INIT_OK);
    printf("After INIT_OK: %s active=%d\n", state_str(ctx.state), ctx.components_active);
    // EXPECT: After INIT_OK: RUNNING active=4

    process_event(&ctx, EV_COMPONENT_FAIL);
    printf("After 1st fail: %s active=%d\n", state_str(ctx.state), ctx.components_active);
    // EXPECT: After 1st fail: RUNNING active=3

    process_event(&ctx, EV_COMPONENT_FAIL);
    printf("After 2nd fail: %s active=%d\n", state_str(ctx.state), ctx.components_active);
    // EXPECT: After 2nd fail: DEGRADED active=2

    process_event(&ctx, EV_RECOVERY_OK);
    printf("After recovery: %s active=%d\n", state_str(ctx.state), ctx.components_active);
    // EXPECT: After recovery: RUNNING active=4

    process_event(&ctx, EV_CRITICAL_FAIL);
    printf("After critical: %s errors=%d\n", state_str(ctx.state), ctx.error_count);
    // EXPECT: After critical: ERROR errors=3

    process_event(&ctx, EV_START);
    process_event(&ctx, EV_INIT_OK);
    printf("Restarted: %s\n", state_str(ctx.state));
    // EXPECT: Restarted: RUNNING

    printf("Transitions: %d\n", ctx.transitions);
    // EXPECT: Transitions: 7

    printf("History:");
    for (int i = 0; i < ctx.history_count; i++) {
        printf(" %s", state_str(ctx.history[i]));
    }
    printf("\n");
    // EXPECT: History: INIT RUNNING DEGRADED RUNNING ERROR INIT RUNNING

    struct SystemContext ctx2;
    ctx_init(&ctx2);
    process_event(&ctx2, EV_START);
    process_event(&ctx2, EV_INIT_OK);
    process_event(&ctx2, EV_COMPONENT_FAIL);
    process_event(&ctx2, EV_COMPONENT_FAIL);
    process_event(&ctx2, EV_COMPONENT_FAIL);
    printf("Recovery scenario: %s active=%d\n", state_str(ctx2.state), ctx2.components_active);
    // EXPECT: Recovery scenario: RECOVERING active=1

    process_event(&ctx2, EV_RECOVERY_FAIL);
    process_event(&ctx2, EV_RECOVERY_FAIL);
    printf("After 2 recovery fails: %s attempts=%d\n", state_str(ctx2.state), ctx2.recovery_attempts);
    // EXPECT: After 2 recovery fails: RECOVERING attempts=2

    process_event(&ctx2, EV_RECOVERY_FAIL);
    printf("After 3rd recovery fail: %s\n", state_str(ctx2.state));
    // EXPECT: After 3rd recovery fail: ERROR

    process_event(&ctx2, EV_STOP);
    printf("Final: %s\n", state_str(ctx2.state));
    // EXPECT: Final: SHUTDOWN

    printf("State machine done\n");
    // EXPECT: State machine done
    return 0;
}
