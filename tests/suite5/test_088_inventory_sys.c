int printf(const char *fmt, ...);

struct Item {
    char name[16];
    int quantity;
    int value;
    int active;
};

int str_eq(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) {
            return 0;
        }
        i = i + 1;
    }
    if (a[i] != b[i]) {
        return 0;
    }
    return 1;
}

int str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
    return i;
}

int add_item(struct Item *inv, int *count, char *name, int qty, int val) {
    int idx;
    idx = *count;
    if (idx >= 20) {
        return -1;
    }
    str_copy(inv[idx].name, name);
    inv[idx].quantity = qty;
    inv[idx].value = val;
    inv[idx].active = 1;
    *count = *count + 1;
    return idx;
}

int find_item(struct Item *inv, int count, char *name) {
    int i;
    i = 0;
    while (i < count) {
        if (inv[i].active && str_eq(inv[i].name, name)) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int remove_item(struct Item *inv, int count, char *name) {
    int idx;
    idx = find_item(inv, count, name);
    if (idx < 0) {
        return 0;
    }
    inv[idx].active = 0;
    return 1;
}

int stack_quantity(struct Item *inv, int count, char *name, int add_qty) {
    int idx;
    idx = find_item(inv, count, name);
    if (idx < 0) {
        return -1;
    }
    inv[idx].quantity = inv[idx].quantity + add_qty;
    return inv[idx].quantity;
}

int total_value(struct Item *inv, int count) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < count) {
        if (inv[i].active) {
            total = total + inv[i].quantity * inv[i].value;
        }
        i = i + 1;
    }
    return total;
}

int count_active(struct Item *inv, int count) {
    int i;
    int active;
    active = 0;
    i = 0;
    while (i < count) {
        if (inv[i].active) {
            active = active + 1;
        }
        i = i + 1;
    }
    return active;
}

int sort_by_value_desc(struct Item *inv, int count) {
    int i;
    int j;
    struct Item temp;
    int swapped;

    i = 0;
    while (i < count - 1) {
        j = 0;
        swapped = 0;
        while (j < count - 1 - i) {
            if (inv[j].value < inv[j + 1].value) {
                temp = inv[j];
                inv[j] = inv[j + 1];
                inv[j + 1] = temp;
                swapped = 1;
            }
            j = j + 1;
        }
        if (swapped == 0) {
            break;
        }
        i = i + 1;
    }
    return 0;
}

int find_most_valuable(struct Item *inv, int count) {
    int i;
    int best;
    int best_val;
    best = -1;
    best_val = -1;
    i = 0;
    while (i < count) {
        if (inv[i].active && inv[i].value > best_val) {
            best_val = inv[i].value;
            best = i;
        }
        i = i + 1;
    }
    return best;
}

int total_quantity(struct Item *inv, int count) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < count) {
        if (inv[i].active) {
            total = total + inv[i].quantity;
        }
        i = i + 1;
    }
    return total;
}

int main(void) {
    struct Item inventory[20];
    int item_count;
    int idx;
    int result;

    item_count = 0;

    /* Add items */
    add_item(inventory, &item_count, "Sword", 1, 100);
    add_item(inventory, &item_count, "Shield", 2, 50);
    add_item(inventory, &item_count, "Potion", 5, 10);
    add_item(inventory, &item_count, "Arrow", 20, 2);
    add_item(inventory, &item_count, "Gem", 3, 200);

    // EXPECT: 5
    printf("%d\n", item_count);

    // EXPECT: 5
    printf("%d\n", count_active(inventory, item_count));

    /* Total value: 1*100 + 2*50 + 5*10 + 20*2 + 3*200 = 100+100+50+40+600 = 890 */
    // EXPECT: 890
    printf("%d\n", total_value(inventory, item_count));

    /* Find items */
    idx = find_item(inventory, item_count, "Potion");
    // EXPECT: 2
    printf("%d\n", idx);

    idx = find_item(inventory, item_count, "Dragon");
    // EXPECT: -1
    printf("%d\n", idx);

    /* Stack quantities */
    result = stack_quantity(inventory, item_count, "Potion", 3);
    // EXPECT: 8
    printf("%d\n", result);

    result = stack_quantity(inventory, item_count, "Arrow", 10);
    // EXPECT: 30
    printf("%d\n", result);

    /* New total value: 1*100 + 2*50 + 8*10 + 30*2 + 3*200 = 100+100+80+60+600 = 940 */
    // EXPECT: 940
    printf("%d\n", total_value(inventory, item_count));

    /* Remove an item */
    result = remove_item(inventory, item_count, "Shield");
    // EXPECT: 1
    printf("%d\n", result);

    // EXPECT: 4
    printf("%d\n", count_active(inventory, item_count));

    /* Can't find removed item */
    idx = find_item(inventory, item_count, "Shield");
    // EXPECT: -1
    printf("%d\n", idx);

    /* Total value without shield: 100 + 80 + 60 + 600 = 840 */
    // EXPECT: 840
    printf("%d\n", total_value(inventory, item_count));

    /* Remove non-existent item */
    result = remove_item(inventory, item_count, "Wand");
    // EXPECT: 0
    printf("%d\n", result);

    /* Total quantity of active items: 1 + 8 + 30 + 3 = 42 */
    // EXPECT: 42
    printf("%d\n", total_quantity(inventory, item_count));

    /* Find most valuable active item (Gem at index 4, value 200) */
    idx = find_most_valuable(inventory, item_count);
    // EXPECT: 4
    printf("%d\n", idx);

    // EXPECT: 200
    printf("%d\n", inventory[idx].value);

    /* Sort by value descending (operates on all slots including inactive) */
    sort_by_value_desc(inventory, item_count);

    /* After sort: Gem(200), Sword(100), Shield(50,inactive), Potion(10), Arrow(2) */
    // EXPECT: 200
    printf("%d\n", inventory[0].value);

    // EXPECT: 100
    printf("%d\n", inventory[1].value);

    // EXPECT: 10
    printf("%d\n", inventory[3].value);

    // EXPECT: 2
    printf("%d\n", inventory[4].value);

    /* Add another item after sort */
    add_item(inventory, &item_count, "Ring", 1, 150);

    // EXPECT: 6
    printf("%d\n", item_count);

    /* Total value: Gem 3*200=600, Sword 1*100=100, Potion 8*10=80, Arrow 30*2=60, Ring 1*150=150 = 990 */
    // EXPECT: 990
    printf("%d\n", total_value(inventory, item_count));

    /* Stack quantity on non-existent */
    result = stack_quantity(inventory, item_count, "Ghost", 5);
    // EXPECT: -1
    printf("%d\n", result);

    return 0;
}
