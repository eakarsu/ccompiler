int printf(const char *fmt, ...);

struct Item {
    char name[32];
    int price;
    int quantity;
};

struct Cart {
    struct Item items[20];
    int count;
    int discount_pct;
};

void str_copy(char *dst, const char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(const char *a, const char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void init_cart(struct Cart *cart) {
    cart->count = 0;
    cart->discount_pct = 0;
}

int find_item(struct Cart *cart, const char *name) {
    int i;
    for (i = 0; i < cart->count; i = i + 1) {
        if (str_eq(cart->items[i].name, name)) return i;
    }
    return -1;
}

void add_item(struct Cart *cart, const char *name, int price, int qty) {
    int idx;
    idx = find_item(cart, name);
    if (idx >= 0) {
        cart->items[idx].quantity = cart->items[idx].quantity + qty;
    } else {
        idx = cart->count;
        str_copy(cart->items[idx].name, name);
        cart->items[idx].price = price;
        cart->items[idx].quantity = qty;
        cart->count = cart->count + 1;
    }
}

void remove_item(struct Cart *cart, const char *name) {
    int idx;
    int i;
    idx = find_item(cart, name);
    if (idx < 0) return;
    for (i = idx; i < cart->count - 1; i = i + 1) {
        cart->items[i] = cart->items[i + 1];
    }
    cart->count = cart->count - 1;
}

void update_qty(struct Cart *cart, const char *name, int qty) {
    int idx;
    idx = find_item(cart, name);
    if (idx >= 0) {
        cart->items[idx].quantity = qty;
    }
}

int subtotal(struct Cart *cart) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < cart->count; i = i + 1) {
        sum = sum + cart->items[i].price * cart->items[i].quantity;
    }
    return sum;
}

int total_after_discount(struct Cart *cart) {
    int sub;
    sub = subtotal(cart);
    return sub - (sub * cart->discount_pct / 100);
}

int total_items(struct Cart *cart) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < cart->count; i = i + 1) {
        c = c + cart->items[i].quantity;
    }
    return c;
}

void print_cart(struct Cart *cart) {
    int i;
    for (i = 0; i < cart->count; i = i + 1) {
        printf("  %s x%d @ %d = %d\n",
               cart->items[i].name,
               cart->items[i].quantity,
               cart->items[i].price,
               cart->items[i].price * cart->items[i].quantity);
    }
}

int most_expensive_item(struct Cart *cart) {
    int i;
    int best;
    int best_idx;
    best = 0;
    best_idx = 0;
    for (i = 0; i < cart->count; i = i + 1) {
        if (cart->items[i].price > best) {
            best = cart->items[i].price;
            best_idx = i;
        }
    }
    return best_idx;
}

int main() {
    struct Cart cart;
    int sub;
    int tot;
    int mi;
    init_cart(&cart);

    // EXPECT: === Shopping Cart Test ===
    printf("=== Shopping Cart Test ===\n");

    add_item(&cart, "Apple", 150, 3);
    add_item(&cart, "Bread", 350, 1);
    add_item(&cart, "Milk", 450, 2);
    add_item(&cart, "Cheese", 600, 1);
    add_item(&cart, "Eggs", 300, 2);

    // EXPECT: Cart contents:
    printf("Cart contents:\n");
    // EXPECT:   Apple x3 @ 150 = 450
    // EXPECT:   Bread x1 @ 350 = 350
    // EXPECT:   Milk x2 @ 450 = 900
    // EXPECT:   Cheese x1 @ 600 = 600
    // EXPECT:   Eggs x2 @ 300 = 600
    print_cart(&cart);

    // EXPECT: Subtotal: 2900
    printf("Subtotal: %d\n", subtotal(&cart));
    // EXPECT: Total items: 9
    printf("Total items: %d\n", total_items(&cart));

    add_item(&cart, "Apple", 150, 2);
    // EXPECT: Apple after add 2 more: qty=5
    printf("Apple after add 2 more: qty=%d\n", cart.items[0].quantity);

    remove_item(&cart, "Bread");
    // EXPECT: After removing Bread:
    printf("After removing Bread:\n");
    // EXPECT:   Apple x5 @ 150 = 750
    // EXPECT:   Milk x2 @ 450 = 900
    // EXPECT:   Cheese x1 @ 600 = 600
    // EXPECT:   Eggs x2 @ 300 = 600
    print_cart(&cart);

    // EXPECT: Subtotal: 2850
    printf("Subtotal: %d\n", subtotal(&cart));

    cart.discount_pct = 10;
    tot = total_after_discount(&cart);
    // EXPECT: After 10pct discount: 2565
    printf("After 10pct discount: %d\n", tot);

    mi = most_expensive_item(&cart);
    // EXPECT: Most expensive: Cheese at 600
    printf("Most expensive: %s at %d\n", cart.items[mi].name, cart.items[mi].price);

    update_qty(&cart, "Milk", 5);
    // EXPECT: Milk qty updated to 5
    printf("Milk qty updated to %d\n", cart.items[find_item(&cart, "Milk")].quantity);
    // EXPECT: New subtotal: 4200
    printf("New subtotal: %d\n", subtotal(&cart));

    // EXPECT: === Done ===
    printf("=== Done ===\n");
    return 0;
}
