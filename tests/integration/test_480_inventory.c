int printf(const char *fmt, ...);
// EXPECT: === Inventory Management ===\nItems in stock: 5\nTotal value: 11750\nAfter sales:\n  Widget: qty=15 val=375\n  Gadget: qty=10 val=450\n  Bolt: qty=80 val=400\n  Nut: qty=50 val=150\n  Gear: qty=5 val=600\nLow stock items: 5\nTotal value after sales: 1975\nAfter restock:\n  Widget: qty=65\n  Gadget: qty=10\n  Bolt: qty=80\n  Nut: qty=50\n  Gear: qty=25\nFinal value: 5625

struct Item {
    char name[20];
    int quantity;
    int price;
    int reorder_level;
};

struct Inventory {
    struct Item items[8];
    int count;
    int total_value;
};

void copy_name(char *dst, const char *src) {
    int i = 0;
    while (src[i] != '\0' && i < 19) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

void add_item(struct Inventory *inv, const char *name, int qty, int price, int reorder) {
    if (inv->count >= 8) return;
    struct Item *it = &inv->items[inv->count];
    copy_name(it->name, name);
    it->quantity = qty;
    it->price = price;
    it->reorder_level = reorder;
    inv->count++;
}

int restock(struct Inventory *inv, int idx, int amount) {
    if (idx < 0 || idx >= inv->count) return 0;
    inv->items[idx].quantity = inv->items[idx].quantity + amount;
    return 1;
}

int sell(struct Inventory *inv, int idx, int amount) {
    if (idx < 0 || idx >= inv->count) return 0;
    if (inv->items[idx].quantity < amount) return 0;
    inv->items[idx].quantity = inv->items[idx].quantity - amount;
    return 1;
}

int calc_total_value(struct Inventory *inv) {
    int total = 0;
    int i;
    for (i = 0; i < inv->count; i++) {
        total = total + inv->items[i].quantity * inv->items[i].price;
    }
    inv->total_value = total;
    return total;
}

int count_low_stock(struct Inventory *inv) {
    int count = 0;
    int i;
    for (i = 0; i < inv->count; i++) {
        if (inv->items[i].quantity <= inv->items[i].reorder_level) {
            count++;
        }
    }
    return count;
}

int main(void) {
    struct Inventory inv;
    inv.count = 0;
    inv.total_value = 0;

    add_item(&inv, "Widget", 100, 25, 20);
    add_item(&inv, "Gadget", 50, 45, 15);
    add_item(&inv, "Bolt", 500, 5, 100);
    add_item(&inv, "Nut", 300, 3, 80);
    add_item(&inv, "Gear", 30, 120, 10);

    printf("=== Inventory Management ===\n");
    printf("Items in stock: %d\n", inv.count);
    printf("Total value: %d\n", calc_total_value(&inv));

    sell(&inv, 0, 85);
    sell(&inv, 1, 40);
    sell(&inv, 2, 420);
    sell(&inv, 3, 250);
    sell(&inv, 4, 25);

    printf("After sales:\n");
    int i;
    for (i = 0; i < inv.count; i++) {
        printf("  %s: qty=%d val=%d\n",
               inv.items[i].name, inv.items[i].quantity,
               inv.items[i].quantity * inv.items[i].price);
    }

    printf("Low stock items: %d\n", count_low_stock(&inv));
    printf("Total value after sales: %d\n", calc_total_value(&inv));

    restock(&inv, 0, 50);
    restock(&inv, 4, 20);

    printf("After restock:\n");
    for (i = 0; i < inv.count; i++) {
        printf("  %s: qty=%d\n", inv.items[i].name, inv.items[i].quantity);
    }
    printf("Final value: %d\n", calc_total_value(&inv));

    return 0;
}
