int printf(const char *fmt, ...);

/* Cards: 0-51, suit = card/13, rank = card%13 (0=Ace..12=King) */

int deck[52];
int hand1[5];
int hand2[5];

int card_suit(int card) {
    return card / 13;
}

int card_rank(int card) {
    return card % 13;
}

int card_value(int card) {
    /* Ace=11, face cards=10, others face value+1 */
    int r = card_rank(card);
    if (r == 0) return 11;
    if (r >= 10) return 10;
    return r + 1;
}

void init_deck(void) {
    int i;
    for (i = 0; i < 52; i = i + 1) {
        deck[i] = i;
    }
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Deterministic pseudo-shuffle using swap pairs */
void shuffle_deck(int seed) {
    int i;
    int j;
    for (i = 51; i > 0; i = i - 1) {
        seed = (seed * 1103515245 + 12345);
        if (seed < 0) seed = 0 - seed;
        j = seed % (i + 1);
        swap(&deck[i], &deck[j]);
    }
}

void deal_hand(int *hand, int start) {
    int i;
    for (i = 0; i < 5; i = i + 1) {
        hand[i] = deck[start + i];
    }
}

int hand_value(int *hand) {
    int sum = 0;
    int i;
    for (i = 0; i < 5; i = i + 1) {
        sum = sum + card_value(hand[i]);
    }
    return sum;
}

int count_suit_in_hand(int *hand, int suit) {
    int count = 0;
    int i;
    for (i = 0; i < 5; i = i + 1) {
        if (card_suit(hand[i]) == suit) {
            count = count + 1;
        }
    }
    return count;
}

int has_pair(int *hand) {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = i + 1; j < 5; j = j + 1) {
            if (card_rank(hand[i]) == card_rank(hand[j])) {
                return 1;
            }
        }
    }
    return 0;
}

int is_flush(int *hand) {
    int s = card_suit(hand[0]);
    int i;
    for (i = 1; i < 5; i = i + 1) {
        if (card_suit(hand[i]) != s) return 0;
    }
    return 1;
}

int max_rank_in_hand(int *hand) {
    int mx = card_rank(hand[0]);
    int i;
    for (i = 1; i < 5; i = i + 1) {
        int r = card_rank(hand[i]);
        if (r > mx) mx = r;
    }
    return mx;
}

int min_rank_in_hand(int *hand) {
    int mn = card_rank(hand[0]);
    int i;
    for (i = 1; i < 5; i = i + 1) {
        int r = card_rank(hand[i]);
        if (r < mn) mn = r;
    }
    return mn;
}

void sort_hand(int *hand) {
    int i;
    int j;
    for (i = 0; i < 4; i = i + 1) {
        for (j = i + 1; j < 5; j = j + 1) {
            if (hand[i] > hand[j]) {
                swap(&hand[i], &hand[j]);
            }
        }
    }
}

int main(void) {
    init_deck();

    /* Verify initial deck */
    // EXPECT: Deck 0: 0
    printf("Deck 0: %d\n", deck[0]);
    // EXPECT: Deck 51: 51
    printf("Deck 51: %d\n", deck[51]);

    /* Card properties */
    // EXPECT: Card0 suit: 0
    printf("Card0 suit: %d\n", card_suit(0));
    // EXPECT: Card0 rank: 0
    printf("Card0 rank: %d\n", card_rank(0));
    // EXPECT: Card0 value: 11
    printf("Card0 value: %d\n", card_value(0));
    // EXPECT: Card14 suit: 1
    printf("Card14 suit: %d\n", card_suit(14));
    // EXPECT: Card14 rank: 1
    printf("Card14 rank: %d\n", card_rank(14));
    // EXPECT: Card14 value: 2
    printf("Card14 value: %d\n", card_value(14));
    // EXPECT: Card51 value: 10
    printf("Card51 value: %d\n", card_value(51));

    /* Shuffle and deal */
    shuffle_deck(42);

    /* Deck should be shuffled - first card changed */
    int shuffled = (deck[0] != 0 || deck[1] != 1);
    // EXPECT: Shuffled: 1
    printf("Shuffled: %d\n", shuffled);

    deal_hand(hand1, 0);
    deal_hand(hand2, 5);

    /* Hand values */
    int v1 = hand_value(hand1);
    int v2 = hand_value(hand2);
    // EXPECT: Hand1 val: 37
    printf("Hand1 val: %d\n", v1);
    // EXPECT: Hand2 val: 34
    printf("Hand2 val: %d\n", v2);

    /* Compare hands - higher total wins */
    int winner;
    if (v1 > v2) winner = 1;
    else if (v2 > v1) winner = 2;
    else winner = 0;
    // EXPECT: Winner: 1
    printf("Winner: %d\n", winner);

    /* Sort hand1 and check ordering */
    sort_hand(hand1);
    int sorted = 1;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        if (hand1[i] > hand1[i + 1]) sorted = 0;
    }
    // EXPECT: H1 sorted: 1
    printf("H1 sorted: %d\n", sorted);

    /* Construct a known flush hand for testing */
    int flush_hand[5];
    flush_hand[0] = 0;
    flush_hand[1] = 3;
    flush_hand[2] = 5;
    flush_hand[3] = 7;
    flush_hand[4] = 9;
    // EXPECT: Flush: 1
    printf("Flush: %d\n", is_flush(flush_hand));

    /* Construct pair hand */
    int pair_hand[5];
    pair_hand[0] = 0;
    pair_hand[1] = 13;
    pair_hand[2] = 5;
    pair_hand[3] = 7;
    pair_hand[4] = 9;
    // EXPECT: Has pair: 1
    printf("Has pair: %d\n", has_pair(pair_hand));

    return 0;
}
