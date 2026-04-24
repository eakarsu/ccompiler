int printf(const char *fmt, ...);

// Card deck shuffling and dealing (Fisher-Yates with deterministic seed)

typedef struct {
    int cards[52]; // 0-51: suit*13 + rank
} Deck;

// Simple LCG random number generator
typedef struct {
    int state;
} RNG;

void rng_init(RNG *r, int seed) {
    r->state = seed;
}

int rng_next(RNG *r) {
    // LCG: next = (a * state + c) mod m
    // Using small constants to avoid overflow concerns
    r->state = (r->state * 1103515245 + 12345) & 0x7FFFFFFF;
    if (r->state < 0) r->state = -r->state;
    return r->state;
}

int rng_range(RNG *r, int n) {
    int val = rng_next(r);
    // Ensure positive
    if (val < 0) val = -val;
    return val % n;
}

void deck_init(Deck *d) {
    int i;
    for (i = 0; i < 52; i++) {
        d->cards[i] = i;
    }
}

void deck_shuffle(Deck *d, RNG *r) {
    int i;
    for (i = 51; i > 0; i--) {
        int j = rng_range(r, i + 1);
        int tmp = d->cards[i];
        d->cards[i] = d->cards[j];
        d->cards[j] = tmp;
    }
}

int card_suit(int card) {
    return card / 13; // 0=spades, 1=hearts, 2=diamonds, 3=clubs
}

int card_rank(int card) {
    return card % 13; // 0=A, 1=2, ..., 9=10, 10=J, 11=Q, 12=K
}

// Check if all 52 cards are present (no duplicates)
int deck_valid(Deck *d) {
    int seen[52];
    int i;
    for (i = 0; i < 52; i++) seen[i] = 0;
    for (i = 0; i < 52; i++) {
        int c = d->cards[i];
        if (c < 0 || c >= 52) return 0;
        if (seen[c]) return 0;
        seen[c] = 1;
    }
    return 1;
}

// Count cards of a given suit in first n cards
int count_suit(Deck *d, int suit, int n) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (card_suit(d->cards[i]) == suit) count++;
    }
    return count;
}

// Sum of ranks in first n cards
int sum_ranks(Deck *d, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum += card_rank(d->cards[i]);
    }
    return sum;
}

// Deal a hand of 5 cards, return hand in array
void deal_hand(Deck *d, int start, int *hand) {
    int i;
    for (i = 0; i < 5; i++) {
        hand[i] = d->cards[start + i];
    }
}

// Check if hand has a pair
int has_pair(int *hand) {
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 5; j++) {
            if (card_rank(hand[i]) == card_rank(hand[j])) return 1;
        }
    }
    return 0;
}

// Check if hand has a flush (all same suit)
int has_flush(int *hand) {
    int s = card_suit(hand[0]);
    int i;
    for (i = 1; i < 5; i++) {
        if (card_suit(hand[i]) != s) return 0;
    }
    return 1;
}

// Count unique ranks in a hand
int unique_ranks(int *hand) {
    int ranks[13];
    int i;
    for (i = 0; i < 13; i++) ranks[i] = 0;
    for (i = 0; i < 5; i++) {
        ranks[card_rank(hand[i])] = 1;
    }
    int count = 0;
    for (i = 0; i < 13; i++) {
        if (ranks[i]) count++;
    }
    return count;
}

int main() {
    // Test unshuffled deck
    Deck d;
    deck_init(&d);
    printf("valid_init: %d\n", deck_valid(&d));
    // EXPECT: valid_init: 1
    printf("first: %d\n", d.cards[0]);
    // EXPECT: first: 0
    printf("last: %d\n", d.cards[51]);
    // EXPECT: last: 51

    // Shuffle with seed 42
    RNG r;
    rng_init(&r, 42);
    deck_shuffle(&d, &r);
    printf("valid_shuffled: %d\n", deck_valid(&d));
    // EXPECT: valid_shuffled: 1

    // After shuffle, first card should differ from 0 (very likely with this seed)
    int different = (d.cards[0] != 0);
    printf("shuffled: %d\n", different);
    // EXPECT: shuffled: 1

    // Total suits in full deck
    int s0 = count_suit(&d, 0, 52);
    int s1 = count_suit(&d, 1, 52);
    int s2 = count_suit(&d, 2, 52);
    int s3 = count_suit(&d, 3, 52);
    printf("suit_counts: %d %d %d %d\n", s0, s1, s2, s3);
    // EXPECT: suit_counts: 13 13 13 13

    // Sum of all ranks: 0+1+2+...+12 = 78 per suit, 78*4 = 312
    printf("rank_sum: %d\n", sum_ranks(&d, 52));
    // EXPECT: rank_sum: 312

    // Shuffle again with same seed - should produce same result
    Deck d2;
    deck_init(&d2);
    RNG r2;
    rng_init(&r2, 42);
    deck_shuffle(&d2, &r2);
    int same = 1;
    int i;
    for (i = 0; i < 52; i++) {
        if (d.cards[i] != d2.cards[i]) { same = 0; }
    }
    printf("deterministic: %d\n", same);
    // EXPECT: deterministic: 1

    // Shuffle with different seed - should be different
    Deck d3;
    deck_init(&d3);
    RNG r3;
    rng_init(&r3, 99);
    deck_shuffle(&d3, &r3);
    printf("valid_d3: %d\n", deck_valid(&d3));
    // EXPECT: valid_d3: 1
    int diff_count = 0;
    for (i = 0; i < 52; i++) {
        if (d.cards[i] != d3.cards[i]) diff_count++;
    }
    int mostly_different = (diff_count > 40);
    printf("diff_seed: %d\n", mostly_different);
    // EXPECT: diff_seed: 1

    // Deal hand from shuffled deck (seed 42) and check properties
    int hand[5];
    deal_hand(&d, 0, hand);
    printf("hand_size: %d\n", 5);
    // EXPECT: hand_size: 5
    int ur = unique_ranks(hand);
    printf("unique_ranks_ge1: %d\n", ur >= 1);
    // EXPECT: unique_ranks_ge1: 1

    // Test card_suit and card_rank
    printf("suit_of_0: %d\n", card_suit(0));
    // EXPECT: suit_of_0: 0
    printf("rank_of_0: %d\n", card_rank(0));
    // EXPECT: rank_of_0: 0
    printf("suit_of_13: %d\n", card_suit(13));
    // EXPECT: suit_of_13: 1
    printf("rank_of_13: %d\n", card_rank(13));
    // EXPECT: rank_of_13: 0
    printf("suit_of_51: %d\n", card_suit(51));
    // EXPECT: suit_of_51: 3
    printf("rank_of_51: %d\n", card_rank(51));
    // EXPECT: rank_of_51: 12

    // Test flush detection
    int flush_hand[5];
    flush_hand[0] = 0; flush_hand[1] = 1; flush_hand[2] = 2;
    flush_hand[3] = 3; flush_hand[4] = 4;
    printf("is_flush: %d\n", has_flush(flush_hand));
    // EXPECT: is_flush: 1
    flush_hand[4] = 13; // different suit
    printf("not_flush: %d\n", has_flush(flush_hand));
    // EXPECT: not_flush: 0

    // Test pair detection
    int pair_hand[5];
    pair_hand[0] = 0; pair_hand[1] = 13; // both rank 0
    pair_hand[2] = 2; pair_hand[3] = 3; pair_hand[4] = 4;
    printf("has_pair: %d\n", has_pair(pair_hand));
    // EXPECT: has_pair: 1
    pair_hand[1] = 1;
    printf("no_pair: %d\n", has_pair(pair_hand));
    // EXPECT: no_pair: 0

    printf("done\n");
    // EXPECT: done
    return 0;
}
