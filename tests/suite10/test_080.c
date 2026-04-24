int printf(const char *fmt, ...);

// Test 080: Graceful degradation - service with fallback levels

enum ServiceLevel {
    SVC_FULL,
    SVC_REDUCED,
    SVC_MINIMAL,
    SVC_OFFLINE
};

struct Component {
    int id;
    int healthy;
    int priority;
    int fallback_id;
};

struct Service {
    struct Component components[8];
    int num_components;
    int level;
    int requests_served;
    int requests_degraded;
    int requests_rejected;
};

char *level_str(int lv) {
    if (lv == SVC_FULL) return "FULL";
    if (lv == SVC_REDUCED) return "REDUCED";
    if (lv == SVC_MINIMAL) return "MINIMAL";
    if (lv == SVC_OFFLINE) return "OFFLINE";
    return "UNKNOWN";
}

void svc_init(struct Service *s) {
    s->num_components = 0;
    s->level = SVC_FULL;
    s->requests_served = 0;
    s->requests_degraded = 0;
    s->requests_rejected = 0;
}

void svc_add_component(struct Service *s, int id, int priority, int fallback_id) {
    if (s->num_components >= 8) return;
    int i = s->num_components;
    s->components[i].id = id;
    s->components[i].healthy = 1;
    s->components[i].priority = priority;
    s->components[i].fallback_id = fallback_id;
    s->num_components++;
}

int count_healthy(struct Service *s) {
    int c = 0;
    for (int i = 0; i < s->num_components; i++) {
        if (s->components[i].healthy) c++;
    }
    return c;
}

int count_critical_healthy(struct Service *s) {
    int c = 0;
    for (int i = 0; i < s->num_components; i++) {
        if (s->components[i].healthy && s->components[i].priority >= 3) c++;
    }
    return c;
}

void recalc_level(struct Service *s) {
    int healthy = count_healthy(s);
    int critical = count_critical_healthy(s);
    int total = s->num_components;

    if (healthy == total) {
        s->level = SVC_FULL;
    } else if (critical >= 2 && healthy >= total / 2) {
        s->level = SVC_REDUCED;
    } else if (critical >= 1) {
        s->level = SVC_MINIMAL;
    } else {
        s->level = SVC_OFFLINE;
    }
}

int find_component(struct Service *s, int id) {
    for (int i = 0; i < s->num_components; i++) {
        if (s->components[i].id == id) return i;
    }
    return -1;
}

void fail_component(struct Service *s, int id) {
    int idx = find_component(s, id);
    if (idx >= 0) {
        s->components[idx].healthy = 0;
        recalc_level(s);
    }
}

void restore_component(struct Service *s, int id) {
    int idx = find_component(s, id);
    if (idx >= 0) {
        s->components[idx].healthy = 1;
        recalc_level(s);
    }
}

int handle_request(struct Service *s, int required_component) {
    if (s->level == SVC_OFFLINE) {
        s->requests_rejected++;
        return -1;
    }

    int idx = find_component(s, required_component);
    if (idx < 0) {
        s->requests_rejected++;
        return -2;
    }

    if (s->components[idx].healthy) {
        s->requests_served++;
        return required_component;
    }

    int fb = s->components[idx].fallback_id;
    if (fb >= 0) {
        int fb_idx = find_component(s, fb);
        if (fb_idx >= 0 && s->components[fb_idx].healthy) {
            s->requests_degraded++;
            return fb;
        }
    }

    if (s->level == SVC_MINIMAL) {
        s->requests_degraded++;
        return 0;
    }

    s->requests_rejected++;
    return -3;
}

int main() {
    struct Service svc;
    svc_init(&svc);

    svc_add_component(&svc, 1, 3, -1);
    svc_add_component(&svc, 2, 3, 1);
    svc_add_component(&svc, 3, 2, 2);
    svc_add_component(&svc, 4, 2, 3);
    svc_add_component(&svc, 5, 1, 4);
    svc_add_component(&svc, 6, 1, 5);

    printf("Initial: level=%s healthy=%d\n", level_str(svc.level), count_healthy(&svc));
    // EXPECT: Initial: level=FULL healthy=6

    int r = handle_request(&svc, 1);
    printf("Request comp 1: served_by=%d\n", r);
    // EXPECT: Request comp 1: served_by=1

    r = handle_request(&svc, 3);
    printf("Request comp 3: served_by=%d\n", r);
    // EXPECT: Request comp 3: served_by=3

    fail_component(&svc, 5);
    printf("After fail 5: level=%s healthy=%d\n", level_str(svc.level), count_healthy(&svc));
    // EXPECT: After fail 5: level=REDUCED healthy=5

    fail_component(&svc, 6);
    printf("After fail 6: level=%s healthy=%d\n", level_str(svc.level), count_healthy(&svc));
    // EXPECT: After fail 6: level=REDUCED healthy=4

    r = handle_request(&svc, 5);
    printf("Request failed comp 5 (fallback to 4): served_by=%d\n", r);
    // EXPECT: Request failed comp 5 (fallback to 4): served_by=4

    r = handle_request(&svc, 6);
    printf("Request failed comp 6 (fallback 5 also down): served_by=%d\n", r);
    // EXPECT: Request failed comp 6 (fallback 5 also down): served_by=-3

    fail_component(&svc, 3);
    fail_component(&svc, 4);
    printf("After fail 3,4: level=%s healthy=%d critical=%d\n",
           level_str(svc.level), count_healthy(&svc), count_critical_healthy(&svc));
    // EXPECT: After fail 3,4: level=MINIMAL healthy=2 critical=2

    r = handle_request(&svc, 4);
    printf("Request comp 4 in minimal: served_by=%d\n", r);
    // EXPECT: Request comp 4 in minimal: served_by=0

    fail_component(&svc, 1);
    printf("After fail 1: level=%s critical=%d\n", level_str(svc.level), count_critical_healthy(&svc));
    // EXPECT: After fail 1: level=MINIMAL critical=1

    fail_component(&svc, 2);
    printf("After fail 2: level=%s\n", level_str(svc.level));
    // EXPECT: After fail 2: level=OFFLINE

    r = handle_request(&svc, 1);
    printf("Request when offline: %d\n", r);
    // EXPECT: Request when offline: -1

    restore_component(&svc, 1);
    restore_component(&svc, 2);
    printf("After restore 1,2: level=%s healthy=%d\n", level_str(svc.level), count_healthy(&svc));
    // EXPECT: After restore 1,2: level=MINIMAL healthy=2

    restore_component(&svc, 3);
    restore_component(&svc, 4);
    restore_component(&svc, 5);
    restore_component(&svc, 6);
    printf("After full restore: level=%s healthy=%d\n", level_str(svc.level), count_healthy(&svc));
    // EXPECT: After full restore: level=FULL healthy=6

    printf("Stats: served=%d degraded=%d rejected=%d\n",
           svc.requests_served, svc.requests_degraded, svc.requests_rejected);
    // EXPECT: Stats: served=2 degraded=2 rejected=2

    printf("Graceful degradation done\n");
    // EXPECT: Graceful degradation done
    return 0;
}
