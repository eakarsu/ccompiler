int printf(const char *fmt, ...);
// EXPECT: hand: AH KH QH JH 10H\ntotal: 41\nhas pair: 0\nsame suit: 1\nhand2: AS AD 7C 3H 9S\ntotal2: 21\nhas pair2: 1\nsame suit2: 0\ncompare AH vs KH: -1\ncompare AS vs AD: 1
// Test: enum-based card game

enum Suit {
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
};

enum Rank {
    ACE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13
};

struct Card {
    enum Suit suit;
    enum Rank rank;
};

int card_value(struct Card *c) {
    if (c->rank >= 10) return 10;
    return c->rank;
}

void print_suit(enum Suit s) {
    switch (s) {
        case CLUBS:    printf("C"); break;
        case DIAMONDS: printf("D"); break;
        case HEARTS:   printf("H"); break;
        case SPADES:   printf("S"); break;
    }
}

void print_rank(enum Rank r) {
    switch (r) {
        case ACE:   printf("A"); break;
        case JACK:  printf("J"); break;
        case QUEEN: printf("Q"); break;
        case KING:  printf("K"); break;
        default:    printf("%d", r); break;
    }
}

void print_card(struct Card *c) {
    print_rank(c->rank);
    print_suit(c->suit);
}

int hand_total(struct Card hand[], int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        total = total + card_value(&hand[i]);
    }
    return total;
}

int has_pair(struct Card hand[], int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (hand[i].rank == hand[j].rank) return 1;
        }
    }
    return 0;
}

int same_suit(struct Card hand[], int n) {
    int i;
    for (i = 1; i < n; i++) {
        if (hand[i].suit != hand[0].suit) return 0;
    }
    return 1;
}

int card_compare(struct Card *a, struct Card *b) {
    if (a->rank != b->rank) return a->rank - b->rank;
    return a->suit - b->suit;
}

int main(void) {
    struct Card hand[5];
    hand[0].suit = HEARTS;   hand[0].rank = ACE;
    hand[1].suit = HEARTS;   hand[1].rank = KING;
    hand[2].suit = HEARTS;   hand[2].rank = QUEEN;
    hand[3].suit = HEARTS;   hand[3].rank = JACK;
    hand[4].suit = HEARTS;   hand[4].rank = TEN;

    printf("hand: ");
    int i;
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        print_card(&hand[i]);
    }
    printf("\n");

    printf("total: %d\n", hand_total(hand, 5));
    printf("has pair: %d\n", has_pair(hand, 5));
    printf("same suit: %d\n", same_suit(hand, 5));

    struct Card hand2[5];
    hand2[0].suit = SPADES;   hand2[0].rank = ACE;
    hand2[1].suit = DIAMONDS; hand2[1].rank = ACE;
    hand2[2].suit = CLUBS;    hand2[2].rank = SEVEN;
    hand2[3].suit = HEARTS;   hand2[3].rank = THREE;
    hand2[4].suit = SPADES;   hand2[4].rank = NINE;

    printf("hand2: ");
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        print_card(&hand2[i]);
    }
    printf("\n");

    printf("total2: %d\n", hand_total(hand2, 5));
    printf("has pair2: %d\n", has_pair(hand2, 5));
    printf("same suit2: %d\n", same_suit(hand2, 5));

    printf("compare AH vs KH: %d\n", card_compare(&hand[0], &hand[1]) < 0 ? -1 : 1);
    printf("compare AS vs AD: %d\n", card_compare(&hand2[0], &hand2[1]) > 0 ? 1 : -1);

    return 0;
}
