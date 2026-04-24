int printf(const char *fmt, ...);
// EXPECT: === Poker Hand Evaluator ===\nHand 1: 10H JH QH KH AH -> Straight Flush\nHand 2: 7C 7H 7D 7S KH -> Four of a Kind\nHand 3: 3C 3H 3D KS KH -> Full House\nHand 4: 2S 5S 9S JS AS -> Flush\nHand 5: 4H 5C 6D 7H 8S -> Straight\nHand 6: AH 3C 7D 9S JH -> High Card

enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
enum Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
            JACK, QUEEN, KING, ACE };

struct Card {
    enum Rank rank;
    enum Suit suit;
};

struct Hand {
    struct Card cards[5];
};

const char *suit_name(enum Suit s) {
    if (s == HEARTS) return "H";
    if (s == DIAMONDS) return "D";
    if (s == CLUBS) return "C";
    return "S";
}

const char *rank_name(enum Rank r) {
    if (r == ACE) return "A";
    if (r == KING) return "K";
    if (r == QUEEN) return "Q";
    if (r == JACK) return "J";
    if (r == TEN) return "10";
    if (r == NINE) return "9";
    if (r == EIGHT) return "8";
    if (r == SEVEN) return "7";
    if (r == SIX) return "6";
    if (r == FIVE) return "5";
    if (r == FOUR) return "4";
    if (r == THREE) return "3";
    return "2";
}

int is_flush(struct Hand *h) {
    enum Suit s = h->cards[0].suit;
    int i;
    for (i = 1; i < 5; i++) {
        if (h->cards[i].suit != s) return 0;
    }
    return 1;
}

void sort_hand(struct Hand *h) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = i + 1; j < 5; j++) {
            if (h->cards[j].rank < h->cards[i].rank) {
                struct Card tmp = h->cards[i];
                h->cards[i] = h->cards[j];
                h->cards[j] = tmp;
            }
        }
    }
}

int is_straight(struct Hand *h) {
    sort_hand(h);
    int i;
    for (i = 0; i < 4; i++) {
        if (h->cards[i + 1].rank != h->cards[i].rank + 1) return 0;
    }
    return 1;
}

int count_rank(struct Hand *h, enum Rank r) {
    int count = 0;
    int i;
    for (i = 0; i < 5; i++) {
        if (h->cards[i].rank == r) count++;
    }
    return count;
}

int has_n_of_kind(struct Hand *h, int n) {
    int i;
    for (i = 0; i < 5; i++) {
        if (count_rank(h, h->cards[i].rank) == n) return 1;
    }
    return 0;
}

int count_pairs(struct Hand *h) {
    int pairs = 0;
    int checked[15];
    int i;
    for (i = 0; i < 15; i++) checked[i] = 0;
    for (i = 0; i < 5; i++) {
        int r = h->cards[i].rank;
        if (!checked[r] && count_rank(h, r) == 2) {
            pairs++;
            checked[r] = 1;
        }
    }
    return pairs;
}

const char *evaluate_hand(struct Hand *h) {
    int flush = is_flush(h);
    int straight = is_straight(h);
    if (flush && straight) return "Straight Flush";
    if (has_n_of_kind(h, 4)) return "Four of a Kind";
    if (has_n_of_kind(h, 3) && has_n_of_kind(h, 2)) return "Full House";
    if (flush) return "Flush";
    if (straight) return "Straight";
    if (has_n_of_kind(h, 3)) return "Three of a Kind";
    if (count_pairs(h) == 2) return "Two Pair";
    if (count_pairs(h) == 1) return "One Pair";
    return "High Card";
}

void print_hand(struct Hand *h) {
    int i;
    for (i = 0; i < 5; i++) {
        printf("%s%s", rank_name(h->cards[i].rank), suit_name(h->cards[i].suit));
        if (i < 4) printf(" ");
    }
}

int main(void) {
    printf("=== Poker Hand Evaluator ===\n");

    struct Hand hands[6];

    hands[0].cards[0].rank = TEN;   hands[0].cards[0].suit = HEARTS;
    hands[0].cards[1].rank = JACK;  hands[0].cards[1].suit = HEARTS;
    hands[0].cards[2].rank = QUEEN; hands[0].cards[2].suit = HEARTS;
    hands[0].cards[3].rank = KING;  hands[0].cards[3].suit = HEARTS;
    hands[0].cards[4].rank = ACE;   hands[0].cards[4].suit = HEARTS;

    hands[1].cards[0].rank = SEVEN; hands[1].cards[0].suit = CLUBS;
    hands[1].cards[1].rank = SEVEN; hands[1].cards[1].suit = HEARTS;
    hands[1].cards[2].rank = SEVEN; hands[1].cards[2].suit = DIAMONDS;
    hands[1].cards[3].rank = SEVEN; hands[1].cards[3].suit = SPADES;
    hands[1].cards[4].rank = KING;  hands[1].cards[4].suit = HEARTS;

    hands[2].cards[0].rank = THREE; hands[2].cards[0].suit = CLUBS;
    hands[2].cards[1].rank = THREE; hands[2].cards[1].suit = HEARTS;
    hands[2].cards[2].rank = THREE; hands[2].cards[2].suit = DIAMONDS;
    hands[2].cards[3].rank = KING;  hands[2].cards[3].suit = SPADES;
    hands[2].cards[4].rank = KING;  hands[2].cards[4].suit = HEARTS;

    hands[3].cards[0].rank = TWO;   hands[3].cards[0].suit = SPADES;
    hands[3].cards[1].rank = FIVE;  hands[3].cards[1].suit = SPADES;
    hands[3].cards[2].rank = NINE;  hands[3].cards[2].suit = SPADES;
    hands[3].cards[3].rank = JACK;  hands[3].cards[3].suit = SPADES;
    hands[3].cards[4].rank = ACE;   hands[3].cards[4].suit = SPADES;

    hands[4].cards[0].rank = FOUR;  hands[4].cards[0].suit = HEARTS;
    hands[4].cards[1].rank = FIVE;  hands[4].cards[1].suit = CLUBS;
    hands[4].cards[2].rank = SIX;   hands[4].cards[2].suit = DIAMONDS;
    hands[4].cards[3].rank = SEVEN; hands[4].cards[3].suit = HEARTS;
    hands[4].cards[4].rank = EIGHT; hands[4].cards[4].suit = SPADES;

    hands[5].cards[0].rank = ACE;   hands[5].cards[0].suit = HEARTS;
    hands[5].cards[1].rank = THREE; hands[5].cards[1].suit = CLUBS;
    hands[5].cards[2].rank = SEVEN; hands[5].cards[2].suit = DIAMONDS;
    hands[5].cards[3].rank = NINE;  hands[5].cards[3].suit = SPADES;
    hands[5].cards[4].rank = JACK;  hands[5].cards[4].suit = HEARTS;

    int i;
    for (i = 0; i < 6; i++) {
        printf("Hand %d: ", i + 1);
        print_hand(&hands[i]);
        printf(" -> %s\n", evaluate_hand(&hands[i]));
    }

    return 0;
}
