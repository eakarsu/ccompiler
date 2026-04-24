int printf(const char *fmt, ...);
// EXPECT: Inventory:\n  [0] Bread: $3 x50 (Food)\n  [1] Water: $1 x100 (Drink)\n  [2] Hammer: $15 x10 (Tool)\n  [3] Nails: $5 x200 (Supply)\n  [4] Cheese: $7 x30 (Food)\n  [5] Juice: $4 x40 (Drink)\nTotal value: $1770\nAfter transactions:\n  [0] Bread: $3 x40 (Food)\n  [1] Water: $1 x75 (Drink)\n  [2] Hammer: $15 x10 (Tool)\n  [3] Nails: $5 x250 (Supply)\n  [4] Cheese: $7 x30 (Food)\n  [5] Juice: $4 x40 (Drink)\nTotal value: $1965\nFood items: 2\nDrink items: 2\nTool items: 1\nSupply items: 1\nSell 100 hammers (only 10): -2

#define MAX_ITEMS 10

enum Category {
    CAT_FOOD = 0,
    CAT_DRINK = 1,
    CAT_TOOL = 2,
    CAT_SUPPLY = 3
};

struct Item {
    char *name;
    int price;
    int quantity;
    int category;
    int active;
};

struct Inventory {
    struct Item items[MAX_ITEMS];
    int count;
};

void inv_init(struct Inventory *inv) {
    inv->count = 0;
}

char *cat_name(int c) {
    if (c == CAT_FOOD) return "Food";
    if (c == CAT_DRINK) return "Drink";
    if (c == CAT_TOOL) return "Tool";
    if (c == CAT_SUPPLY) return "Supply";
    return "Unknown";
}

int inv_add(struct Inventory *inv, char *name, int price, int qty, int cat) {
    if (inv->count >= MAX_ITEMS) return -1;
    inv->items[inv->count].name = name;
    inv->items[inv->count].price = price;
    inv->items[inv->count].quantity = qty;
    inv->items[inv->count].category = cat;
    inv->items[inv->count].active = 1;
    inv->count++;
    return inv->count - 1;
}

int inv_restock(struct Inventory *inv, int idx, int qty) {
    if (idx < 0 || idx >= inv->count || !inv->items[idx].active) return -1;
    inv->items[idx].quantity = inv->items[idx].quantity + qty;
    return inv->items[idx].quantity;
}

int inv_sell(struct Inventory *inv, int idx, int qty) {
    if (idx < 0 || idx >= inv->count || !inv->items[idx].active) return -1;
    if (inv->items[idx].quantity < qty) return -2;
    inv->items[idx].quantity = inv->items[idx].quantity - qty;
    return inv->items[idx].quantity;
}

int inv_total_value(struct Inventory *inv) {
    int total = 0;
    int i;
    for (i = 0; i < inv->count; i++) {
        if (inv->items[i].active) {
            total = total + inv->items[i].price * inv->items[i].quantity;
        }
    }
    return total;
}

int inv_count_category(struct Inventory *inv, int cat) {
    int c = 0;
    int i;
    for (i = 0; i < inv->count; i++) {
        if (inv->items[i].active && inv->items[i].category == cat) c++;
    }
    return c;
}

void inv_print(struct Inventory *inv) {
    int i;
    for (i = 0; i < inv->count; i++) {
        if (inv->items[i].active) {
            printf("  [%d] %s: $%d x%d (%s)\n", i,
                   inv->items[i].name, inv->items[i].price,
                   inv->items[i].quantity, cat_name(inv->items[i].category));
        }
    }
}

int main(void) {
    struct Inventory inv;
    inv_init(&inv);

    inv_add(&inv, "Bread", 3, 50, CAT_FOOD);
    inv_add(&inv, "Water", 1, 100, CAT_DRINK);
    inv_add(&inv, "Hammer", 15, 10, CAT_TOOL);
    inv_add(&inv, "Nails", 5, 200, CAT_SUPPLY);
    inv_add(&inv, "Cheese", 7, 30, CAT_FOOD);
    inv_add(&inv, "Juice", 4, 40, CAT_DRINK);

    printf("Inventory:\n");
    inv_print(&inv);
    printf("Total value: $%d\n", inv_total_value(&inv));

    inv_sell(&inv, 0, 10);
    inv_sell(&inv, 1, 25);
    inv_restock(&inv, 3, 50);
    printf("After transactions:\n");
    inv_print(&inv);
    printf("Total value: $%d\n", inv_total_value(&inv));

    printf("Food items: %d\n", inv_count_category(&inv, CAT_FOOD));
    printf("Drink items: %d\n", inv_count_category(&inv, CAT_DRINK));
    printf("Tool items: %d\n", inv_count_category(&inv, CAT_TOOL));
    printf("Supply items: %d\n", inv_count_category(&inv, CAT_SUPPLY));

    int result = inv_sell(&inv, 2, 100);
    printf("Sell 100 hammers (only 10): %d\n", result);

    return 0;
}
