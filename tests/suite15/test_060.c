int printf(const char *fmt, ...);

// Weather model using Markov chain
// States: 0=Sunny, 1=Cloudy, 2=Rainy
// Transition probabilities scaled by 100:
//   From Sunny:  70% Sunny, 20% Cloudy, 10% Rainy
//   From Cloudy: 30% Sunny, 40% Cloudy, 30% Rainy
//   From Rainy:  20% Sunny, 30% Cloudy, 50% Rainy

typedef struct {
    int state;
} RNG;

void rng_init(RNG *r, int seed) {
    r->state = seed % 32749;
}

int rng_next(RNG *r) {
    r->state = (1021 * r->state + 13) % 32749;
    return r->state;
}

int transition(int current, RNG *rng) {
    int r = rng_next(rng) % 100;

    if (current == 0) {
        if (r < 70) return 0;
        if (r < 90) return 1;
        return 2;
    } else if (current == 1) {
        if (r < 30) return 0;
        if (r < 70) return 1;
        return 2;
    } else {
        if (r < 20) return 0;
        if (r < 50) return 1;
        return 2;
    }
}

void simulate(int *weather, int n, int start, RNG *rng) {
    weather[0] = start;
    int i;
    for (i = 1; i < n; i++) {
        weather[i] = transition(weather[i - 1], rng);
    }
}

int count_state(int *weather, int n, int state) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (weather[i] == state) count++;
    }
    return count;
}

int longest_streak(int *weather, int n, int state) {
    int max_streak = 0;
    int current_streak = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (weather[i] == state) {
            current_streak++;
            if (current_streak > max_streak) max_streak = current_streak;
        } else {
            current_streak = 0;
        }
    }
    return max_streak;
}

int count_transitions(int *weather, int n, int from, int to) {
    int count = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (weather[i - 1] == from && weather[i] == to) count++;
    }
    return count;
}

// Comfort score: sunny=3, cloudy=1, rainy=-2
int comfort_score(int *weather, int n) {
    int score = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (weather[i] == 0) score = score + 3;
        else if (weather[i] == 1) score = score + 1;
        else score = score - 2;
    }
    return score;
}

int dominant_state(int *weather, int n) {
    int counts[3];
    counts[0] = count_state(weather, n, 0);
    counts[1] = count_state(weather, n, 1);
    counts[2] = count_state(weather, n, 2);
    int best = 0;
    if (counts[1] > counts[best]) best = 1;
    if (counts[2] > counts[best]) best = 2;
    return best;
}

int main(void) {
    RNG rng;
    int weather[60];

    // Simulation 1: start sunny
    rng_init(&rng, 42);
    simulate(weather, 60, 0, &rng);

    printf("First 10 days:");
    int i;
    for (i = 0; i < 10; i++) {
        printf(" %d", weather[i]);
    }
    printf("\n");
    // EXPECT: First 10 days: 0 0 2 1 1 2 0 0 0 0

    int sunny = count_state(weather, 60, 0);
    int cloudy = count_state(weather, 60, 1);
    int rainy = count_state(weather, 60, 2);
    printf("Sunny: %d\n", sunny);
    // EXPECT: Sunny: 25
    printf("Cloudy: %d\n", cloudy);
    // EXPECT: Cloudy: 18
    printf("Rainy: %d\n", rainy);
    // EXPECT: Rainy: 17

    int ls = longest_streak(weather, 60, 0);
    printf("Longest sunny streak: %d\n", ls);
    // EXPECT: Longest sunny streak: 11

    int lr = longest_streak(weather, 60, 2);
    printf("Longest rainy streak: %d\n", lr);
    // EXPECT: Longest rainy streak: 3

    int s_to_r = count_transitions(weather, 60, 0, 2);
    printf("Sunny to Rainy: %d\n", s_to_r);
    // EXPECT: Sunny to Rainy: 4

    int r_to_s = count_transitions(weather, 60, 2, 0);
    printf("Rainy to Sunny: %d\n", r_to_s);
    // EXPECT: Rainy to Sunny: 4

    int cs = comfort_score(weather, 60);
    printf("Comfort score: %d\n", cs);
    // EXPECT: Comfort score: 59

    int dom = dominant_state(weather, 60);
    printf("Dominant state: %d\n", dom);
    // EXPECT: Dominant state: 0

    // Simulation 2: start rainy, different seed
    RNG rng2;
    int weather2[60];
    rng_init(&rng2, 99);
    simulate(weather2, 60, 2, &rng2);

    int sunny2 = count_state(weather2, 60, 0);
    int rainy2 = count_state(weather2, 60, 2);
    printf("Sim2 sunny: %d\n", sunny2);
    // EXPECT: Sim2 sunny: 15
    printf("Sim2 rainy: %d\n", rainy2);
    // EXPECT: Sim2 rainy: 20

    int cs2 = comfort_score(weather2, 60);
    printf("Sim2 comfort: %d\n", cs2);
    // EXPECT: Sim2 comfort: 30

    int better = (cs > cs2) ? 1 : 2;
    printf("More comfortable: sim%d\n", better);
    // EXPECT: More comfortable: sim1

    // Stability check: count total transitions (state changes)
    int changes1 = 0;
    int changes2 = 0;
    for (i = 1; i < 60; i++) {
        if (weather[i] != weather[i - 1]) changes1++;
        if (weather2[i] != weather2[i - 1]) changes2++;
    }
    printf("Sim1 changes: %d\n", changes1);
    // EXPECT: Sim1 changes: 28
    printf("Sim2 changes: %d\n", changes2);
    // EXPECT: Sim2 changes: 31

    return 0;
}
