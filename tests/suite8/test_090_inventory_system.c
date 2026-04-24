int printf(const char *fmt, ...);

enum ItemCategory { CAT_FOOD, CAT_DRINK, CAT_TOOL, CAT_MATERIAL };

struct Item {
    int id;
    char *name;
    int category;
    int quantity;
    int price;
    int reorder_level;
    int total_sold;
};

struct Inventory {
    struct Item items[10];
    int item_count;
    int total_revenue;
    int total_restocks;
    int alerts_raised;
};

void init_inventory(struct Inventory *inv) {
    inv->item_count = 0;
    inv->total_revenue = 0;
    inv->total_restocks = 0;
    inv->alerts_raised = 0;
}

int add_item(struct Inventory *inv, int id, char *name, int category,
             int quantity, int price, int reorder_level) {
    if (inv->item_count >= 10) return -1;
    int idx = inv->item_count;
    inv->items[idx].id = id;
    inv->items[idx].name = name;
    inv->items[idx].category = category;
    inv->items[idx].quantity = quantity;
    inv->items[idx].price = price;
    inv->items[idx].reorder_level = reorder_level;
    inv->items[idx].total_sold = 0;
    inv->item_count = inv->item_count + 1;
    return idx;
}

int find_item(struct Inventory *inv, int id) {
    int i;
    for (i = 0; i < inv->item_count; i = i + 1) {
        if (inv->items[i].id == id) return i;
    }
    return -1;
}

const char *category_name(int cat) {
    if (cat == CAT_FOOD) return "Food";
    if (cat == CAT_DRINK) return "Drink";
    if (cat == CAT_TOOL) return "Tool";
    if (cat == CAT_MATERIAL) return "Material";
    return "Unknown";
}

int sell_item(struct Inventory *inv, int id, int quantity) {
    int idx = find_item(inv, id);
    if (idx < 0) {
        printf("Sell FAIL: item %d not found\n", id);
        return 0;
    }
    if (quantity > inv->items[idx].quantity) {
        printf("Sell FAIL: %s insufficient stock (%d available, %d requested)\n",
               inv->items[idx].name, inv->items[idx].quantity, quantity);
        return 0;
    }
    inv->items[idx].quantity = inv->items[idx].quantity - quantity;
    inv->items[idx].total_sold = inv->items[idx].total_sold + quantity;
    int revenue = quantity * inv->items[idx].price;
    inv->total_revenue = inv->total_revenue + revenue;
    printf("Sold %d %s for %d\n", quantity, inv->items[idx].name, revenue);

    if (inv->items[idx].quantity <= inv->items[idx].reorder_level) {
        printf("ALERT: %s low stock (%d remaining)\n", inv->items[idx].name, inv->items[idx].quantity);
        inv->alerts_raised = inv->alerts_raised + 1;
    }
    return 1;
}

void restock_item(struct Inventory *inv, int id, int quantity) {
    int idx = find_item(inv, id);
    if (idx < 0) {
        printf("Restock FAIL: item %d not found\n", id);
        return;
    }
    inv->items[idx].quantity = inv->items[idx].quantity + quantity;
    inv->total_restocks = inv->total_restocks + 1;
    printf("Restocked %s: +%d, now %d\n", inv->items[idx].name, quantity, inv->items[idx].quantity);
}

void print_item(struct Inventory *inv, int id) {
    int idx = find_item(inv, id);
    if (idx >= 0) {
        printf("  [%d] %s (%s): qty=%d price=%d sold=%d\n",
               inv->items[idx].id, inv->items[idx].name,
               category_name(inv->items[idx].category),
               inv->items[idx].quantity, inv->items[idx].price,
               inv->items[idx].total_sold);
    }
}

void print_all(struct Inventory *inv) {
    int i;
    for (i = 0; i < inv->item_count; i = i + 1) {
        print_item(inv, inv->items[i].id);
    }
}

int count_by_category(struct Inventory *inv, int cat) {
    int count = 0;
    int i;
    for (i = 0; i < inv->item_count; i = i + 1) {
        if (inv->items[i].category == cat)
            count = count + inv->items[i].quantity;
    }
    return count;
}

int find_most_sold(struct Inventory *inv) {
    int best_idx = 0;
    int i;
    for (i = 1; i < inv->item_count; i = i + 1) {
        if (inv->items[i].total_sold > inv->items[best_idx].total_sold) {
            best_idx = i;
        }
    }
    return inv->items[best_idx].id;
}

int total_stock_value(struct Inventory *inv) {
    int total = 0;
    int i;
    for (i = 0; i < inv->item_count; i = i + 1) {
        total = total + inv->items[i].quantity * inv->items[i].price;
    }
    return total;
}

int main() {
    struct Inventory inv;
    init_inventory(&inv);

    add_item(&inv, 101, "Bread", CAT_FOOD, 50, 3, 10);
    add_item(&inv, 102, "Water", CAT_DRINK, 100, 1, 20);
    add_item(&inv, 103, "Hammer", CAT_TOOL, 15, 25, 5);
    add_item(&inv, 104, "Nails", CAT_MATERIAL, 200, 1, 50);
    add_item(&inv, 105, "Cheese", CAT_FOOD, 30, 5, 8);

    printf("=== Initial Inventory ===\n");
    // EXPECT: === Initial Inventory ===
    print_all(&inv);
    // EXPECT:   [101] Bread (Food): qty=50 price=3 sold=0
    // EXPECT:   [102] Water (Drink): qty=100 price=1 sold=0
    // EXPECT:   [103] Hammer (Tool): qty=15 price=25 sold=0
    // EXPECT:   [104] Nails (Material): qty=200 price=1 sold=0
    // EXPECT:   [105] Cheese (Food): qty=30 price=5 sold=0

    printf("=== Sales ===\n");
    // EXPECT: === Sales ===
    sell_item(&inv, 101, 20);
    // EXPECT: Sold 20 Bread for 60
    sell_item(&inv, 102, 50);
    // EXPECT: Sold 50 Water for 50
    sell_item(&inv, 103, 12);
    // EXPECT: Sold 12 Hammer for 300
    // EXPECT: ALERT: Hammer low stock (3 remaining)
    sell_item(&inv, 104, 100);
    // EXPECT: Sold 100 Nails for 100
    sell_item(&inv, 105, 25);
    // EXPECT: Sold 25 Cheese for 125
    // EXPECT: ALERT: Cheese low stock (5 remaining)

    printf("=== Failed sale ===\n");
    // EXPECT: === Failed sale ===
    sell_item(&inv, 103, 10);
    // EXPECT: Sell FAIL: Hammer insufficient stock (3 available, 10 requested)
    sell_item(&inv, 999, 1);
    // EXPECT: Sell FAIL: item 999 not found

    printf("=== Restock ===\n");
    // EXPECT: === Restock ===
    restock_item(&inv, 103, 20);
    // EXPECT: Restocked Hammer: +20, now 23
    restock_item(&inv, 105, 30);
    // EXPECT: Restocked Cheese: +30, now 35

    printf("=== Final Inventory ===\n");
    // EXPECT: === Final Inventory ===
    print_all(&inv);
    // EXPECT:   [101] Bread (Food): qty=30 price=3 sold=20
    // EXPECT:   [102] Water (Drink): qty=50 price=1 sold=50
    // EXPECT:   [103] Hammer (Tool): qty=23 price=25 sold=12
    // EXPECT:   [104] Nails (Material): qty=100 price=1 sold=100
    // EXPECT:   [105] Cheese (Food): qty=35 price=5 sold=25

    printf("Revenue: %d\n", inv.total_revenue);
    // EXPECT: Revenue: 635
    printf("Restocks: %d\n", inv.total_restocks);
    // EXPECT: Restocks: 2
    printf("Alerts: %d\n", inv.alerts_raised);
    // EXPECT: Alerts: 2
    printf("Food stock: %d\n", count_by_category(&inv, CAT_FOOD));
    // EXPECT: Food stock: 65
    printf("Most sold item: %d\n", find_most_sold(&inv));
    // EXPECT: Most sold item: 104
    printf("Stock value: %d\n", total_stock_value(&inv));
    // EXPECT: Stock value: 990

    return 0;
}
