int printf(const char *fmt, ...);

// Rhythm Pattern Generator
// Generates and manipulates drum patterns using bit patterns
// Each pattern is a 16-step sequence stored as integers (0 or 1)

struct Pattern {
    int steps[16];
    int length;
    int name_id;   // 0=kick, 1=snare, 2=hihat, 3=clap
};

void pattern_clear(struct Pattern *p) {
    int i;
    for (i = 0; i < 16; i = i + 1) {
        p->steps[i] = 0;
    }
}

void pattern_set(struct Pattern *p, int step, int val) {
    if (step >= 0 && step < p->length) {
        p->steps[step] = val;
    }
}

void pattern_from_hits(struct Pattern *p, int *hits, int num_hits) {
    pattern_clear(p);
    int i;
    for (i = 0; i < num_hits; i = i + 1) {
        if (hits[i] >= 0 && hits[i] < p->length) {
            p->steps[hits[i]] = 1;
        }
    }
}

void print_pattern(struct Pattern *p) {
    int i;
    for (i = 0; i < p->length; i = i + 1) {
        if (p->steps[i]) printf("X");
        else printf(".");
    }
    printf("\n");
}

int pattern_density(struct Pattern *p) {
    int count = 0;
    int i;
    for (i = 0; i < p->length; i = i + 1) {
        if (p->steps[i]) count = count + 1;
    }
    // Return percentage * 10
    return (count * 1000) / p->length;
}

void pattern_shift(struct Pattern *p, int amount) {
    int temp[16];
    int i;
    for (i = 0; i < p->length; i = i + 1) {
        int src = (i - amount + p->length * 4) % p->length;
        temp[i] = p->steps[src];
    }
    for (i = 0; i < p->length; i = i + 1) {
        p->steps[i] = temp[i];
    }
}

void pattern_invert(struct Pattern *p) {
    int i;
    for (i = 0; i < p->length; i = i + 1) {
        p->steps[i] = 1 - p->steps[i];
    }
}

void pattern_or(struct Pattern *a, struct Pattern *b, struct Pattern *result) {
    int i;
    result->length = a->length;
    for (i = 0; i < a->length; i = i + 1) {
        result->steps[i] = (a->steps[i] || b->steps[i]) ? 1 : 0;
    }
}

void pattern_and(struct Pattern *a, struct Pattern *b, struct Pattern *result) {
    int i;
    result->length = a->length;
    for (i = 0; i < a->length; i = i + 1) {
        result->steps[i] = (a->steps[i] && b->steps[i]) ? 1 : 0;
    }
}

// Generate Euclidean rhythm: distribute n hits evenly across k steps
void euclidean_rhythm(struct Pattern *p, int hits, int steps) {
    int i;
    p->length = steps;
    pattern_clear(p);
    if (hits <= 0 || steps <= 0) return;
    if (hits >= steps) {
        for (i = 0; i < steps; i = i + 1) p->steps[i] = 1;
        return;
    }
    // Bresenham-style distribution
    int error = 0;
    for (i = 0; i < steps; i = i + 1) {
        error = error + hits;
        if (error >= steps) {
            p->steps[i] = 1;
            error = error - steps;
        }
    }
}

int longest_gap(struct Pattern *p) {
    int max_gap = 0;
    int gap = 0;
    int i;
    // Check two passes for wrap-around
    for (i = 0; i < p->length * 2; i = i + 1) {
        int idx = i % p->length;
        if (p->steps[idx] == 0) {
            gap = gap + 1;
        } else {
            if (gap > max_gap) max_gap = gap;
            gap = 0;
        }
    }
    if (gap > max_gap) max_gap = gap;
    if (max_gap > p->length) max_gap = p->length;
    return max_gap;
}

int main() {
    // Basic 4-on-the-floor kick pattern
    struct Pattern kick;
    kick.length = 16;
    pattern_clear(&kick);
    int kick_hits[4];
    kick_hits[0] = 0; kick_hits[1] = 4; kick_hits[2] = 8; kick_hits[3] = 12;
    pattern_from_hits(&kick, kick_hits, 4);

    printf("Kick: ");
    print_pattern(&kick);
    // EXPECT: Kick: X...X...X...X...

    // Backbeat snare
    struct Pattern snare;
    snare.length = 16;
    pattern_clear(&snare);
    int snare_hits[2];
    snare_hits[0] = 4; snare_hits[1] = 12;
    pattern_from_hits(&snare, snare_hits, 2);

    printf("Snare: ");
    print_pattern(&snare);
    // EXPECT: Snare: ....X.......X...

    // 8th note hihat
    struct Pattern hihat;
    hihat.length = 16;
    pattern_clear(&hihat);
    int i;
    for (i = 0; i < 16; i = i + 2) {
        pattern_set(&hihat, i, 1);
    }

    printf("Hihat: ");
    print_pattern(&hihat);
    // EXPECT: Hihat: X.X.X.X.X.X.X.X.

    // Pattern density
    int d_kick = pattern_density(&kick);
    printf("Kick density: %d\n", d_kick);
    // EXPECT: Kick density: 250

    int d_hihat = pattern_density(&hihat);
    printf("Hihat density: %d\n", d_hihat);
    // EXPECT: Hihat density: 500

    // OR combination
    struct Pattern combined;
    combined.length = 16;
    pattern_or(&kick, &snare, &combined);
    printf("Kick|Snare: ");
    print_pattern(&combined);
    // EXPECT: Kick|Snare: X...X...X...X...

    // AND combination
    struct Pattern common;
    common.length = 16;
    pattern_and(&kick, &hihat, &common);
    printf("Kick&Hihat: ");
    print_pattern(&common);
    // EXPECT: Kick&Hihat: X...X...X...X...

    // Shift kick right by 2
    struct Pattern shifted;
    shifted.length = 16;
    int j;
    for (j = 0; j < 16; j = j + 1) shifted.steps[j] = kick.steps[j];
    pattern_shift(&shifted, 2);
    printf("Kick>>2: ");
    print_pattern(&shifted);
    // EXPECT: Kick>>2: ..X...X...X...X.

    // Invert snare
    struct Pattern inv_snare;
    inv_snare.length = 16;
    for (j = 0; j < 16; j = j + 1) inv_snare.steps[j] = snare.steps[j];
    pattern_invert(&inv_snare);
    printf("~Snare: ");
    print_pattern(&inv_snare);
    // EXPECT: ~Snare: XXXX.XXXXXXX.XXX

    // Euclidean rhythms
    struct Pattern euc1;
    euclidean_rhythm(&euc1, 3, 8);
    printf("E(3,8): ");
    print_pattern(&euc1);
    // EXPECT: E(3,8): ..X..X.X

    struct Pattern euc2;
    euclidean_rhythm(&euc2, 5, 16);
    printf("E(5,16): ");
    print_pattern(&euc2);
    // EXPECT: E(5,16): ...X..X..X..X..X

    struct Pattern euc3;
    euclidean_rhythm(&euc3, 4, 12);
    printf("E(4,12): ");
    print_pattern(&euc3);
    // EXPECT: E(4,12): ..X..X..X..X

    // Longest gap
    int gap_kick = longest_gap(&kick);
    printf("Kick gap: %d\n", gap_kick);
    // EXPECT: Kick gap: 3

    int gap_snare = longest_gap(&snare);
    printf("Snare gap: %d\n", gap_snare);
    // EXPECT: Snare gap: 7

    int gap_euc = longest_gap(&euc1);
    printf("E(3,8) gap: %d\n", gap_euc);
    // EXPECT: E(3,8) gap: 2

    return 0;
}
