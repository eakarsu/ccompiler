int printf(const char *fmt, ...);

// Polynomial addition using linked list representation
// Each node stores coefficient and exponent, sorted by descending exponent

struct PolyNode {
    int coeff;
    int exp;
    int next;
};

struct Polynomial {
    struct PolyNode pool[30];
    int head;
    int free_idx;
    int size;
};

void poly_init(struct Polynomial *p) {
    int i;
    p->head = -1;
    p->size = 0;
    p->free_idx = 0;
    for (i = 0; i < 29; i++) {
        p->pool[i].next = i + 1;
        p->pool[i].coeff = 0;
        p->pool[i].exp = 0;
    }
    p->pool[29].next = -1;
    p->pool[29].coeff = 0;
    p->pool[29].exp = 0;
}

int poly_alloc(struct Polynomial *p) {
    int idx;
    if (p->free_idx == -1) return -1;
    idx = p->free_idx;
    p->free_idx = p->pool[idx].next;
    p->pool[idx].next = -1;
    return idx;
}

// Insert term in sorted order (descending exponent)
// If exponent already exists, add coefficients
void poly_add_term(struct Polynomial *p, int coeff, int exp) {
    int idx;
    int cur;
    int prev;

    if (coeff == 0) return;

    // Check if exponent already exists
    cur = p->head;
    prev = -1;
    while (cur != -1 && p->pool[cur].exp > exp) {
        prev = cur;
        cur = p->pool[cur].next;
    }

    if (cur != -1 && p->pool[cur].exp == exp) {
        // Add to existing term
        p->pool[cur].coeff = p->pool[cur].coeff + coeff;
        // Remove if coefficient becomes 0
        if (p->pool[cur].coeff == 0) {
            if (prev == -1) {
                p->head = p->pool[cur].next;
            } else {
                p->pool[prev].next = p->pool[cur].next;
            }
            p->pool[cur].next = p->free_idx;
            p->free_idx = cur;
            p->size--;
        }
        return;
    }

    // Insert new term
    idx = poly_alloc(p);
    if (idx == -1) return;
    p->pool[idx].coeff = coeff;
    p->pool[idx].exp = exp;

    if (prev == -1) {
        p->pool[idx].next = p->head;
        p->head = idx;
    } else {
        p->pool[idx].next = p->pool[prev].next;
        p->pool[prev].next = idx;
    }
    p->size++;
}

// Print polynomial in human-readable form
void poly_print(struct Polynomial *p) {
    int cur = p->head;
    int first = 1;

    if (cur == -1) {
        printf("0");
        printf("\n");
        return;
    }

    while (cur != -1) {
        if (!first) {
            if (p->pool[cur].coeff > 0) {
                printf(" + ");
            } else {
                printf(" - ");
            }
        }

        if (first && p->pool[cur].coeff < 0) {
            printf("-");
        }

        if (p->pool[cur].exp == 0) {
            if (p->pool[cur].coeff < 0 && !first) {
                printf("%d", -p->pool[cur].coeff);
            } else if (p->pool[cur].coeff < 0 && first) {
                printf("%d", -p->pool[cur].coeff);
            } else {
                printf("%d", p->pool[cur].coeff);
            }
        } else if (p->pool[cur].exp == 1) {
            if (p->pool[cur].coeff < 0) {
                int c = -p->pool[cur].coeff;
                if (c == 1) printf("x");
                else printf("%dx", c);
            } else {
                if (p->pool[cur].coeff == 1) printf("x");
                else printf("%dx", p->pool[cur].coeff);
            }
        } else {
            if (p->pool[cur].coeff < 0) {
                int c = -p->pool[cur].coeff;
                if (c == 1) printf("x^%d", p->pool[cur].exp);
                else printf("%dx^%d", c, p->pool[cur].exp);
            } else {
                if (p->pool[cur].coeff == 1) printf("x^%d", p->pool[cur].exp);
                else printf("%dx^%d", p->pool[cur].coeff, p->pool[cur].exp);
            }
        }

        first = 0;
        cur = p->pool[cur].next;
    }
    printf("\n");
}

// Add two polynomials: result = a + b
void poly_add(struct Polynomial *a, struct Polynomial *b, struct Polynomial *result) {
    int cur;
    poly_init(result);

    // Copy all terms from a
    cur = a->head;
    while (cur != -1) {
        poly_add_term(result, a->pool[cur].coeff, a->pool[cur].exp);
        cur = a->pool[cur].next;
    }

    // Add all terms from b
    cur = b->head;
    while (cur != -1) {
        poly_add_term(result, b->pool[cur].coeff, b->pool[cur].exp);
        cur = b->pool[cur].next;
    }
}

// Subtract two polynomials: result = a - b
void poly_sub(struct Polynomial *a, struct Polynomial *b, struct Polynomial *result) {
    int cur;
    poly_init(result);

    cur = a->head;
    while (cur != -1) {
        poly_add_term(result, a->pool[cur].coeff, a->pool[cur].exp);
        cur = a->pool[cur].next;
    }

    cur = b->head;
    while (cur != -1) {
        poly_add_term(result, -b->pool[cur].coeff, b->pool[cur].exp);
        cur = b->pool[cur].next;
    }
}

// Evaluate polynomial at x
int poly_eval(struct Polynomial *p, int x) {
    int result = 0;
    int cur = p->head;
    int term_val;
    int i;

    while (cur != -1) {
        term_val = p->pool[cur].coeff;
        for (i = 0; i < p->pool[cur].exp; i++) {
            term_val = term_val * x;
        }
        result = result + term_val;
        cur = p->pool[cur].next;
    }
    return result;
}

int main() {
    struct Polynomial a;
    struct Polynomial b;
    struct Polynomial result;

    // Test 1: Simple addition
    // a = 3x^2 + 2x + 1
    // b = 5x^2 + 4x + 3
    // sum = 8x^2 + 6x + 4
    poly_init(&a);
    poly_add_term(&a, 3, 2);
    poly_add_term(&a, 2, 1);
    poly_add_term(&a, 1, 0);

    printf("A: ");
    poly_print(&a);
    // EXPECT: A: 3x^2 + 2x + 1

    poly_init(&b);
    poly_add_term(&b, 5, 2);
    poly_add_term(&b, 4, 1);
    poly_add_term(&b, 3, 0);

    printf("B: ");
    poly_print(&b);
    // EXPECT: B: 5x^2 + 4x + 3

    poly_add(&a, &b, &result);
    printf("A+B: ");
    poly_print(&result);
    // EXPECT: A+B: 8x^2 + 6x + 4

    // Evaluate at x=2: 8*4 + 6*2 + 4 = 32 + 12 + 4 = 48
    printf("A+B at x=2: %d\n", poly_eval(&result, 2));
    // EXPECT: A+B at x=2: 48

    // Test 2: Different exponents
    // a = 4x^3 + 2x
    // b = 3x^2 + 5
    // sum = 4x^3 + 3x^2 + 2x + 5
    poly_init(&a);
    poly_add_term(&a, 4, 3);
    poly_add_term(&a, 2, 1);

    poly_init(&b);
    poly_add_term(&b, 3, 2);
    poly_add_term(&b, 5, 0);

    poly_add(&a, &b, &result);
    printf("Mixed: ");
    poly_print(&result);
    // EXPECT: Mixed: 4x^3 + 3x^2 + 2x + 5

    // Evaluate at x=1: 4 + 3 + 2 + 5 = 14
    printf("Mixed at x=1: %d\n", poly_eval(&result, 1));
    // EXPECT: Mixed at x=1: 14

    // Test 3: Cancellation
    // a = 3x^2 + 2x + 1
    // b = -3x^2 + x + 4
    // sum = 3x + 5
    poly_init(&a);
    poly_add_term(&a, 3, 2);
    poly_add_term(&a, 2, 1);
    poly_add_term(&a, 1, 0);

    poly_init(&b);
    poly_add_term(&b, -3, 2);
    poly_add_term(&b, 1, 1);
    poly_add_term(&b, 4, 0);

    poly_add(&a, &b, &result);
    printf("Cancel: ");
    poly_print(&result);
    // EXPECT: Cancel: 3x + 5

    printf("Terms: %d\n", result.size);
    // EXPECT: Terms: 2

    // Test 4: Subtraction
    // a = 5x^3 + 3x + 1
    // b = 2x^3 + 3x + 1
    // a - b = 3x^3
    poly_init(&a);
    poly_add_term(&a, 5, 3);
    poly_add_term(&a, 3, 1);
    poly_add_term(&a, 1, 0);

    poly_init(&b);
    poly_add_term(&b, 2, 3);
    poly_add_term(&b, 3, 1);
    poly_add_term(&b, 1, 0);

    poly_sub(&a, &b, &result);
    printf("A-B: ");
    poly_print(&result);
    // EXPECT: A-B: 3x^3

    // Evaluate at x=2: 3*8 = 24
    printf("A-B at x=2: %d\n", poly_eval(&result, 2));
    // EXPECT: A-B at x=2: 24

    // Test 5: Evaluation
    // p = x^4 + 2x^3 + 3x^2 + 4x + 5
    // at x=0: 5
    // at x=1: 1+2+3+4+5 = 15
    // at x=2: 16+16+12+8+5 = 57
    poly_init(&a);
    poly_add_term(&a, 1, 4);
    poly_add_term(&a, 2, 3);
    poly_add_term(&a, 3, 2);
    poly_add_term(&a, 4, 1);
    poly_add_term(&a, 5, 0);

    printf("Poly: ");
    poly_print(&a);
    // EXPECT: Poly: x^4 + 2x^3 + 3x^2 + 4x + 5

    printf("At x=0: %d\n", poly_eval(&a, 0));
    // EXPECT: At x=0: 5

    printf("At x=1: %d\n", poly_eval(&a, 1));
    // EXPECT: At x=1: 15

    printf("At x=2: %d\n", poly_eval(&a, 2));
    // EXPECT: At x=2: 57

    return 0;
}
