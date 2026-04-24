int printf(const char *fmt, ...);
// EXPECT: Hand 1 (Blackjack):\n  Ace of Spades (11)\n  King of Hearts (10)\n  Score: 21\nHand 2 (Two Aces):\n  Ace of Diamonds (11)\n  Ace of Clubs (11)\n  9 of Hearts (9)\n  Score: 21\nHand 3 (Bust?):\n  5 of Spades (5)\n  6 of Hearts (6)\n  Queen of Diamonds (10)\n  Score: 21\nHand 4:\n  7 of Clubs (7)\n  8 of Spades (8)\n  3 of Hearts (3)\n  Score: 18\nWinner: Hand 1 with 21

enum Suit { HEARTS = 0, DIAMONDS = 1, CLUBS = 2, SPADES = 3 };
enum Rank { ACE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6,
            SEVEN = 7, EIGHT = 8, NINE = 9, TEN = 10, JACK = 11, QUEEN = 12, KING = 13 };

struct Card {
    int rank;
    int suit;
};

char *suit_name(int s) {
    if (s == HEARTS) return "Hearts";
    if (s == DIAMONDS) return "Diamonds";
    if (s == CLUBS) return "Clubs";
    if (s == SPADES) return "Spades";
    return "?";
}

char *rank_name(int r) {
    if (r == ACE) return "Ace";
    if (r == JACK) return "Jack";
    if (r == QUEEN) return "Queen";
    if (r == KING) return "King";
    if (r >= 2 && r <= 10) {
        /* return number names for scoring display */
        if (r == 2) return "2";
        if (r == 3) return "3";
        if (r == 4) return "4";
        if (r == 5) return "5";
        if (r == 6) return "6";
        if (r == 7) return "7";
        if (r == 8) return "8";
        if (r == 9) return "9";
        if (r == 10) return "10";
    }
    return "?";
}

int card_value(struct Card *c) {
    if (c->rank >= 10) return 10;
    if (c->rank == ACE) return 11;
    return c->rank;
}

struct Hand {
    struct Card cards[5];
    int count;
    int score;
};

void hand_init(struct Hand *h) {
    h->count = 0;
    h->score = 0;
}

void hand_add(struct Hand *h, int rank, int suit) {
    if (h->count < 5) {
        h->cards[h->count].rank = rank;
        h->cards[h->count].suit = suit;
        h->count++;
    }
}

int hand_score(struct Hand *h) {
    int total = 0;
    int aces = 0;
    int i;
    for (i = 0; i < h->count; i++) {
        total = total + card_value(&h->cards[i]);
        if (h->cards[i].rank == ACE) aces++;
    }
    while (total > 21 && aces > 0) {
        total = total - 10;
        aces--;
    }
    h->score = total;
    return total;
}

void hand_print(struct Hand *h) {
    int i;
    for (i = 0; i < h->count; i++) {
        printf("  %s of %s (%d)\n",
               rank_name(h->cards[i].rank),
               suit_name(h->cards[i].suit),
               card_value(&h->cards[i]));
    }
    printf("  Score: %d\n", h->score);
}

int main(void) {
    struct Hand h1;
    hand_init(&h1);
    hand_add(&h1, ACE, SPADES);
    hand_add(&h1, KING, HEARTS);
    hand_score(&h1);
    printf("Hand 1 (Blackjack):\n");
    hand_print(&h1);

    struct Hand h2;
    hand_init(&h2);
    hand_add(&h2, ACE, DIAMONDS);
    hand_add(&h2, ACE, CLUBS);
    hand_add(&h2, NINE, HEARTS);
    hand_score(&h2);
    printf("Hand 2 (Two Aces):\n");
    hand_print(&h2);

    struct Hand h3;
    hand_init(&h3);
    hand_add(&h3, FIVE, SPADES);
    hand_add(&h3, SIX, HEARTS);
    hand_add(&h3, QUEEN, DIAMONDS);
    hand_score(&h3);
    printf("Hand 3 (Bust?):\n");
    hand_print(&h3);

    struct Hand h4;
    hand_init(&h4);
    hand_add(&h4, SEVEN, CLUBS);
    hand_add(&h4, EIGHT, SPADES);
    hand_add(&h4, THREE, HEARTS);
    hand_score(&h4);
    printf("Hand 4:\n");
    hand_print(&h4);

    printf("Winner: ");
    int best = 0;
    int best_score = 0;
    struct Hand *hands[4];
    hands[0] = &h1; hands[1] = &h2; hands[2] = &h3; hands[3] = &h4;
    int i;
    for (i = 0; i < 4; i++) {
        if (hands[i]->score <= 21 && hands[i]->score > best_score) {
            best_score = hands[i]->score;
            best = i + 1;
        }
    }
    printf("Hand %d with %d\n", best, best_score);

    return 0;
}
