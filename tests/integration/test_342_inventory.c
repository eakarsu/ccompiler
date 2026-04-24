int printf(const char *fmt, ...);
// EXPECT: inventory:\n  item1: type=0 val=50 qty=1\n  item2: type=1 val=30 qty=1\n  item3: type=2 val=10 qty=5\n  item4: type=3 val=5 qty=3\ntotal value: 145\nweapons: 1\npotions: 5\ngold: 100\nafter using 2 potions: 3 left\nsold misc for: 2\ngold after sell: 102\nbought weapon: 1\ngold after buy: 62\nweapons now: 2
// Test: inventory system with structs

enum ItemType {
    ITEM_WEAPON,
    ITEM_ARMOR,
    ITEM_POTION,
    ITEM_MISC
};

struct Item {
    int id;
    enum ItemType type;
    int value;
    int quantity;
};

struct Inventory {
    struct Item items[12];
    int count;
    int gold;
};

void inv_init(struct Inventory *inv) {
    inv->count = 0;
    inv->gold = 100;
}

int inv_add(struct Inventory *inv, int id, enum ItemType type, int value, int qty) {
    int i;
    for (i = 0; i < inv->count; i++) {
        if (inv->items[i].id == id) {
            inv->items[i].quantity = inv->items[i].quantity + qty;
            return 1;
        }
    }
    if (inv->count >= 12) return 0;
    int idx = inv->count;
    inv->items[idx].id = id;
    inv->items[idx].type = type;
    inv->items[idx].value = value;
    inv->items[idx].quantity = qty;
    inv->count = inv->count + 1;
    return 1;
}

int inv_remove(struct Inventory *inv, int id, int qty) {
    int i;
    for (i = 0; i < inv->count; i++) {
        if (inv->items[i].id == id) {
            if (inv->items[i].quantity < qty) return 0;
            inv->items[i].quantity = inv->items[i].quantity - qty;
            return 1;
        }
    }
    return 0;
}

int inv_total_value(struct Inventory *inv) {
    int total = 0;
    int i;
    for (i = 0; i < inv->count; i++) {
        total = total + inv->items[i].value * inv->items[i].quantity;
    }
    return total;
}

int inv_count_type(struct Inventory *inv, enum ItemType type) {
    int count = 0;
    int i;
    for (i = 0; i < inv->count; i++) {
        if (inv->items[i].type == type) {
            count = count + inv->items[i].quantity;
        }
    }
    return count;
}

int inv_buy(struct Inventory *inv, int id, enum ItemType type, int price) {
    if (inv->gold < price) return 0;
    inv->gold = inv->gold - price;
    inv_add(inv, id, type, price, 1);
    return 1;
}

int inv_sell(struct Inventory *inv, int id) {
    int i;
    for (i = 0; i < inv->count; i++) {
        if (inv->items[i].id == id && inv->items[i].quantity > 0) {
            int price = inv->items[i].value / 2;
            inv->items[i].quantity = inv->items[i].quantity - 1;
            inv->gold = inv->gold + price;
            return price;
        }
    }
    return 0;
}

void inv_print(struct Inventory *inv) {
    int i;
    for (i = 0; i < inv->count; i++) {
        if (inv->items[i].quantity > 0) {
            printf("  item%d: type=%d val=%d qty=%d\n",
                   inv->items[i].id, inv->items[i].type,
                   inv->items[i].value, inv->items[i].quantity);
        }
    }
}

int main(void) {
    struct Inventory inv;
    inv_init(&inv);

    inv_add(&inv, 1, ITEM_WEAPON, 50, 1);
    inv_add(&inv, 2, ITEM_ARMOR, 30, 1);
    inv_add(&inv, 3, ITEM_POTION, 10, 5);
    inv_add(&inv, 4, ITEM_MISC, 5, 3);

    printf("inventory:\n");
    inv_print(&inv);
    printf("total value: %d\n", inv_total_value(&inv));
    printf("weapons: %d\n", inv_count_type(&inv, ITEM_WEAPON));
    printf("potions: %d\n", inv_count_type(&inv, ITEM_POTION));
    printf("gold: %d\n", inv.gold);

    inv_remove(&inv, 3, 2);
    printf("after using 2 potions: %d left\n", inv_count_type(&inv, ITEM_POTION));

    int earned = inv_sell(&inv, 4);
    printf("sold misc for: %d\n", earned);
    printf("gold after sell: %d\n", inv.gold);

    int bought = inv_buy(&inv, 5, ITEM_WEAPON, 40);
    printf("bought weapon: %d\n", bought);
    printf("gold after buy: %d\n", inv.gold);
    printf("weapons now: %d\n", inv_count_type(&inv, ITEM_WEAPON));

    return 0;
}
