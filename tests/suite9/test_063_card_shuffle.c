int printf(const char *fmt, ...);

// Deterministic card deck shuffle using LCG PRNG
// Cards: 0-51, suit = card/13, rank = card%13

int deck[52];
int seed;

int next_rand(void) {
    seed = seed * 1103515245 + 12345;
    int val = seed;
    if (val < 0) val = -val;
    return val;
}

void init_deck(void) {
    int i;
    for (i = 0; i < 52; i++) {
        deck[i] = i;
    }
}

void shuffle_deck(void) {
    int i;
    for (i = 51; i > 0; i--) {
        int j = next_rand() % (i + 1);
        int tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }
}

char *suit_name(int card) {
    int s = card / 13;
    if (s == 0) return "Hearts";
    if (s == 1) return "Diamonds";
    if (s == 2) return "Clubs";
    return "Spades";
}

char *rank_name(int card) {
    int r = card % 13;
    if (r == 0) return "A";
    if (r == 1) return "2";
    if (r == 2) return "3";
    if (r == 3) return "4";
    if (r == 4) return "5";
    if (r == 5) return "6";
    if (r == 6) return "7";
    if (r == 7) return "8";
    if (r == 8) return "9";
    if (r == 9) return "10";
    if (r == 10) return "J";
    if (r == 11) return "Q";
    return "K";
}

int is_valid_deck(void) {
    int seen[52];
    int i;
    for (i = 0; i < 52; i++) seen[i] = 0;
    for (i = 0; i < 52; i++) {
        if (deck[i] < 0 || deck[i] > 51) return 0;
        if (seen[deck[i]]) return 0;
        seen[deck[i]] = 1;
    }
    return 1;
}

int count_in_position(void) {
    int count = 0;
    int i;
    for (i = 0; i < 52; i++) {
        if (deck[i] == i) count++;
    }
    return count;
}

// Deal n cards from top of deck, return sum of card values (rank+1)
int deal_hand(int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum += (deck[i] % 13) + 1;
    }
    return sum;
}

// Count cards of a specific suit in first n cards
int count_suit(int n, int suit) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (deck[i] / 13 == suit) count++;
    }
    return count;
}

// Find position of a specific card in deck
int find_card(int card) {
    int i;
    for (i = 0; i < 52; i++) {
        if (deck[i] == card) return i;
    }
    return -1;
}

int main(void) {
    // Test 1: Initialize deck
    init_deck();
    printf("First card: %d Last card: %d\n", deck[0], deck[51]);
    // EXPECT: First card: 0 Last card: 51

    // Test 2: Deck validity before shuffle
    int valid = is_valid_deck();
    printf("Valid before shuffle: %d\n", valid);
    // EXPECT: Valid before shuffle: 1

    // Test 3: Shuffle with seed 42
    seed = 42;
    shuffle_deck();
    valid = is_valid_deck();
    printf("Valid after shuffle: %d\n", valid);
    // EXPECT: Valid after shuffle: 1

    // Test 4: Print first 5 cards
    printf("Top 5 cards: %d %d %d %d %d\n", deck[0], deck[1], deck[2], deck[3], deck[4]);
    // EXPECT: Top 5 cards: 0 2 4 34 17

    // Test 5: Cards moved from original position
    int in_place = count_in_position();
    printf("Cards in original position: %d\n", in_place);
    // EXPECT: Cards in original position: 2

    // Test 6: Name of first card
    printf("First card: %s of %s\n", rank_name(deck[0]), suit_name(deck[0]));
    // EXPECT: First card: A of Hearts

    // Test 7: Same seed produces same shuffle
    init_deck();
    seed = 42;
    shuffle_deck();
    printf("Reproducible: %d %d %d\n", deck[0], deck[1], deck[2]);
    // EXPECT: Reproducible: 0 2 4

    // Test 8: Different seed, different result
    init_deck();
    seed = 99;
    shuffle_deck();
    printf("Different seed: %d %d %d\n", deck[0], deck[1], deck[2]);
    // EXPECT: Different seed: 6 3 1

    // Test 9: Deal 5 card hand value
    init_deck();
    seed = 42;
    shuffle_deck();
    int hand_val = deal_hand(5);
    printf("Hand value (5 cards): %d\n", hand_val);
    // EXPECT: Hand value (5 cards): 23

    // Test 10: Count hearts in top 13 cards
    int hearts = count_suit(13, 0);
    printf("Hearts in top 13: %d\n", hearts);
    // EXPECT: Hearts in top 13: 6

    // Test 11: Find ace of spades (card 39)
    int pos = find_card(39);
    printf("Ace of spades at position: %d\n", pos);
    // EXPECT: Ace of spades at position: 33

    // Test 12: Second card name
    printf("Second card: %s of %s\n", rank_name(deck[1]), suit_name(deck[1]));
    // EXPECT: Second card: 3 of Hearts

    printf("Card shuffle tests passed\n");
    // EXPECT: Card shuffle tests passed
    return 0;
}
