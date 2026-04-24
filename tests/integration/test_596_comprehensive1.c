int printf(const char *fmt, ...);
// EXPECT: Starting state:\n=== Adventurer (Lv1) ===\nHP: 100/100 ATK: 10 DEF: 5 XP: 0\nGold: 100 Items: 4 Weight: 15 Value: 140\n  [0] Iron Sword (Weapon) val=50 wt=5 [E]\n  [1] Leather Armor (Armor) val=30 wt=8 [E]\n  [2] Health Potion (Potion) val=20 wt=1\n  [3] Fire Scroll (Scroll) val=40 wt=1\n\nUsing health potion:\nHealed 20 HP\nUsing strength scroll:\nATK +5\n\nGaining XP:\nGained 150 XP, leveled up 1 times\nGained 250 XP, leveled up 1 times\n\nFinal state:\n=== Adventurer (Lv3) ===\nHP: 70/120 ATK: 19 DEF: 7 XP: 100\nGold: 100 Items: 4 Weight: 15 Value: 140\n  [0] Iron Sword (Weapon) val=50 wt=5 [E]\n  [1] Leather Armor (Armor) val=30 wt=8 [E]\n  [2] Health Potion (Potion) val=20 wt=1\n  [3] Fire Scroll (Scroll) val=40 wt=1
#define MAX_ITEMS 8
#define MAX_NAME 20

enum ItemType { WEAPON = 0, ARMOR = 1, POTION = 2, SCROLL = 3 };

struct Item {
    char *name;
    int type;
    int value;
    int weight;
    int equipped;
};

struct Inventory {
    struct Item items[MAX_ITEMS];
    int count;
    int gold;
};

struct Stats {
    int hp;
    int max_hp;
    int attack;
    int defense;
    int level;
    int xp;
};

struct Character {
    char *name;
    struct Stats stats;
    struct Inventory inv;
};

typedef int (*EffectFn)(struct Character *);

int heal_effect(struct Character *c) {
    int healed = 20;
    c->stats.hp = c->stats.hp + healed;
    if (c->stats.hp > c->stats.max_hp) c->stats.hp = c->stats.max_hp;
    return healed;
}

int strength_effect(struct Character *c) {
    c->stats.attack = c->stats.attack + 5;
    return 5;
}

char *type_name(int t) {
    switch (t) {
        case WEAPON: return "Weapon";
        case ARMOR: return "Armor";
        case POTION: return "Potion";
        case SCROLL: return "Scroll";
    }
    return "Unknown";
}

void inv_init(struct Inventory *inv) {
    inv->count = 0;
    inv->gold = 100;
}

int inv_add(struct Inventory *inv, char *name, int type, int value, int weight) {
    if (inv->count >= MAX_ITEMS) return -1;
    inv->items[inv->count].name = name;
    inv->items[inv->count].type = type;
    inv->items[inv->count].value = value;
    inv->items[inv->count].weight = weight;
    inv->items[inv->count].equipped = 0;
    inv->count++;
    return inv->count - 1;
}

int inv_total_weight(struct Inventory *inv) {
    int w = 0;
    int i;
    for (i = 0; i < inv->count; i++) w = w + inv->items[i].weight;
    return w;
}

int inv_total_value(struct Inventory *inv) {
    int v = 0;
    int i;
    for (i = 0; i < inv->count; i++) v = v + inv->items[i].value;
    return v;
}

void char_init(struct Character *c, char *name) {
    c->name = name;
    c->stats.hp = 100; c->stats.max_hp = 100;
    c->stats.attack = 10; c->stats.defense = 5;
    c->stats.level = 1; c->stats.xp = 0;
    inv_init(&c->inv);
}

int gain_xp(struct Character *c, int xp) {
    c->stats.xp = c->stats.xp + xp;
    int leveled = 0;
    while (c->stats.xp >= c->stats.level * 100) {
        c->stats.xp = c->stats.xp - c->stats.level * 100;
        c->stats.level++;
        c->stats.max_hp = c->stats.max_hp + 10;
        c->stats.attack = c->stats.attack + 2;
        c->stats.defense = c->stats.defense + 1;
        leveled++;
    }
    return leveled;
}

void char_print(struct Character *c) {
    printf("=== %s (Lv%d) ===\n", c->name, c->stats.level);
    printf("HP: %d/%d ATK: %d DEF: %d XP: %d\n",
           c->stats.hp, c->stats.max_hp, c->stats.attack,
           c->stats.defense, c->stats.xp);
    printf("Gold: %d Items: %d Weight: %d Value: %d\n",
           c->inv.gold, c->inv.count,
           inv_total_weight(&c->inv), inv_total_value(&c->inv));
    int i;
    for (i = 0; i < c->inv.count; i++) {
        printf("  [%d] %s (%s) val=%d wt=%d%s\n", i,
               c->inv.items[i].name, type_name(c->inv.items[i].type),
               c->inv.items[i].value, c->inv.items[i].weight,
               c->inv.items[i].equipped ? " [E]" : "");
    }
}

int main(void) {
    struct Character hero;
    char_init(&hero, "Adventurer");

    inv_add(&hero.inv, "Iron Sword", WEAPON, 50, 5);
    inv_add(&hero.inv, "Leather Armor", ARMOR, 30, 8);
    inv_add(&hero.inv, "Health Potion", POTION, 20, 1);
    inv_add(&hero.inv, "Fire Scroll", SCROLL, 40, 1);

    hero.inv.items[0].equipped = 1;
    hero.inv.items[1].equipped = 1;

    printf("Starting state:\n");
    char_print(&hero);

    hero.stats.hp = 50;
    printf("\nUsing health potion:\n");
    EffectFn effect = heal_effect;
    int result = effect(&hero);
    printf("Healed %d HP\n", result);

    printf("Using strength scroll:\n");
    effect = strength_effect;
    result = effect(&hero);
    printf("ATK +%d\n", result);

    printf("\nGaining XP:\n");
    int levels = gain_xp(&hero, 150);
    printf("Gained 150 XP, leveled up %d times\n", levels);
    levels = gain_xp(&hero, 250);
    printf("Gained 250 XP, leveled up %d times\n", levels);

    printf("\nFinal state:\n");
    char_print(&hero);

    return 0;
}
