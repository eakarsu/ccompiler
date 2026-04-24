int printf(const char *fmt, ...);

// Mega test: structs + enums + pointers + loops + functions combined

enum ItemType {
    ITEM_WEAPON,
    ITEM_ARMOR,
    ITEM_POTION,
    ITEM_SCROLL,
    ITEM_RING,
    ITEM_AMULET,
    ITEM_FOOD,
    ITEM_KEY,
    ITEM_GOLD,
    ITEM_GEM
};

enum Rarity {
    RARITY_COMMON,
    RARITY_UNCOMMON,
    RARITY_RARE,
    RARITY_EPIC,
    RARITY_LEGENDARY
};

struct Item {
    int id;
    enum ItemType type;
    enum Rarity rarity;
    int value;
    int weight;
    int durability;
};

struct Inventory {
    struct Item items[20];
    int count;
    int max_weight;
    int current_weight;
};

struct Stats {
    int strength;
    int dexterity;
    int intelligence;
    int vitality;
    int luck;
};

struct Character {
    int hp;
    int max_hp;
    int level;
    struct Stats stats;
    struct Inventory inv;
};

void init_item(struct Item *item, int id, enum ItemType type, enum Rarity rarity,
               int value, int weight, int durability) {
    item->id = id;
    item->type = type;
    item->rarity = rarity;
    item->value = value;
    item->weight = weight;
    item->durability = durability;
}

int item_score(struct Item *item) {
    int base = item->value * item->durability;
    int rarity_mult = 1;
    switch (item->rarity) {
        case RARITY_COMMON: rarity_mult = 1; break;
        case RARITY_UNCOMMON: rarity_mult = 2; break;
        case RARITY_RARE: rarity_mult = 4; break;
        case RARITY_EPIC: rarity_mult = 8; break;
        case RARITY_LEGENDARY: rarity_mult = 16; break;
    }
    return base * rarity_mult;
}

int type_bonus(enum ItemType type, struct Stats *stats) {
    switch (type) {
        case ITEM_WEAPON: return stats->strength * 3;
        case ITEM_ARMOR: return stats->vitality * 2;
        case ITEM_POTION: return stats->intelligence;
        case ITEM_SCROLL: return stats->intelligence * 2;
        case ITEM_RING: return stats->luck * 3;
        case ITEM_AMULET: return stats->luck * 2;
        case ITEM_FOOD: return stats->vitality;
        case ITEM_KEY: return 0;
        case ITEM_GOLD: return stats->luck;
        case ITEM_GEM: return stats->luck * 5;
        default: return 0;
    }
}

void init_character(struct Character *ch, int hp, int str, int dex, int intel,
                    int vit, int luck) {
    ch->hp = hp;
    ch->max_hp = hp;
    ch->level = 1;
    ch->stats.strength = str;
    ch->stats.dexterity = dex;
    ch->stats.intelligence = intel;
    ch->stats.vitality = vit;
    ch->stats.luck = luck;
    ch->inv.count = 0;
    ch->inv.max_weight = str * 10;
    ch->inv.current_weight = 0;
}

int add_item(struct Character *ch, struct Item *item) {
    if (ch->inv.count >= 20) return 0;
    if (ch->inv.current_weight + item->weight > ch->inv.max_weight) return 0;
    ch->inv.items[ch->inv.count] = *item;
    ch->inv.count = ch->inv.count + 1;
    ch->inv.current_weight = ch->inv.current_weight + item->weight;
    return 1;
}

int inventory_total_value(struct Character *ch) {
    int total = 0;
    int i = 0;
    while (i < ch->inv.count) {
        total = total + ch->inv.items[i].value;
        i = i + 1;
    }
    return total;
}

int inventory_total_score(struct Character *ch) {
    int total = 0;
    int i = 0;
    while (i < ch->inv.count) {
        total = total + item_score(&ch->inv.items[i]);
        total = total + type_bonus(ch->inv.items[i].type, &ch->stats);
        i = i + 1;
    }
    return total;
}

int count_by_type(struct Character *ch, enum ItemType type) {
    int cnt = 0;
    int i = 0;
    while (i < ch->inv.count) {
        if (ch->inv.items[i].type == type) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int count_by_rarity(struct Character *ch, enum Rarity rarity) {
    int cnt = 0;
    int i = 0;
    while (i < ch->inv.count) {
        if (ch->inv.items[i].rarity == rarity) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int best_item_score(struct Character *ch) {
    if (ch->inv.count == 0) return 0;
    int best = item_score(&ch->inv.items[0]);
    int i = 1;
    while (i < ch->inv.count) {
        int s = item_score(&ch->inv.items[i]);
        if (s > best) best = s;
        i = i + 1;
    }
    return best;
}

void level_up(struct Character *ch) {
    ch->level = ch->level + 1;
    ch->stats.strength = ch->stats.strength + 2;
    ch->stats.dexterity = ch->stats.dexterity + 1;
    ch->stats.intelligence = ch->stats.intelligence + 1;
    ch->stats.vitality = ch->stats.vitality + 2;
    ch->stats.luck = ch->stats.luck + 1;
    ch->max_hp = ch->max_hp + ch->stats.vitality * 2;
    ch->hp = ch->max_hp;
    ch->inv.max_weight = ch->stats.strength * 10;
}

int combat_power(struct Character *ch) {
    int base = ch->stats.strength * 3 + ch->stats.dexterity * 2 +
               ch->stats.intelligence + ch->stats.vitality * 2 +
               ch->stats.luck;
    int weapon_bonus = 0;
    int armor_bonus = 0;
    int i = 0;
    while (i < ch->inv.count) {
        if (ch->inv.items[i].type == ITEM_WEAPON) {
            int s = item_score(&ch->inv.items[i]);
            if (s > weapon_bonus) weapon_bonus = s;
        }
        if (ch->inv.items[i].type == ITEM_ARMOR) {
            armor_bonus = armor_bonus + ch->inv.items[i].durability;
        }
        i = i + 1;
    }
    return base + weapon_bonus + armor_bonus + ch->level * 10;
}

int main() {
    struct Character hero;
    init_character(&hero, 100, 10, 8, 6, 12, 5);

    printf("hp: %d\n", hero.hp);
    // EXPECT: hp: 100
    printf("str: %d\n", hero.stats.strength);
    // EXPECT: str: 10
    printf("max weight: %d\n", hero.inv.max_weight);
    // EXPECT: max weight: 100

    struct Item sword;
    init_item(&sword, 1, ITEM_WEAPON, RARITY_RARE, 50, 15, 80);
    struct Item shield;
    init_item(&shield, 2, ITEM_ARMOR, RARITY_UNCOMMON, 30, 20, 100);
    struct Item potion;
    init_item(&potion, 3, ITEM_POTION, RARITY_COMMON, 10, 1, 1);
    struct Item ring;
    init_item(&ring, 4, ITEM_RING, RARITY_EPIC, 200, 1, 50);
    struct Item gem;
    init_item(&gem, 5, ITEM_GEM, RARITY_LEGENDARY, 500, 2, 100);

    printf("sword score: %d\n", item_score(&sword));
    // EXPECT: sword score: 16000
    printf("ring score: %d\n", item_score(&ring));
    // EXPECT: ring score: 80000
    printf("gem score: %d\n", item_score(&gem));
    // EXPECT: gem score: 800000

    int r1 = add_item(&hero, &sword);
    int r2 = add_item(&hero, &shield);
    int r3 = add_item(&hero, &potion);
    int r4 = add_item(&hero, &ring);
    int r5 = add_item(&hero, &gem);

    printf("added all: %d\n", r1 + r2 + r3 + r4 + r5);
    // EXPECT: added all: 5
    printf("inv count: %d\n", hero.inv.count);
    // EXPECT: inv count: 5
    printf("inv weight: %d\n", hero.inv.current_weight);
    // EXPECT: inv weight: 39
    printf("total value: %d\n", inventory_total_value(&hero));
    // EXPECT: total value: 790

    printf("weapons: %d\n", count_by_type(&hero, ITEM_WEAPON));
    // EXPECT: weapons: 1
    printf("potions: %d\n", count_by_type(&hero, ITEM_POTION));
    // EXPECT: potions: 1

    printf("common: %d\n", count_by_rarity(&hero, RARITY_COMMON));
    // EXPECT: common: 1
    printf("rare: %d\n", count_by_rarity(&hero, RARITY_RARE));
    // EXPECT: rare: 1
    printf("legendary: %d\n", count_by_rarity(&hero, RARITY_LEGENDARY));
    // EXPECT: legendary: 1

    printf("best score: %d\n", best_item_score(&hero));
    // EXPECT: best score: 800000

    printf("total score: %d\n", inventory_total_score(&hero));
    // EXPECT: total score: 902110

    printf("combat power: %d\n", combat_power(&hero));
    // EXPECT: combat power: 16191

    level_up(&hero);
    level_up(&hero);
    level_up(&hero);
    printf("level: %d\n", hero.level);
    // EXPECT: level: 4
    printf("hp after lvl: %d\n", hero.hp);
    // EXPECT: hp after lvl: 196
    printf("str after lvl: %d\n", hero.stats.strength);
    // EXPECT: str after lvl: 16

    printf("combat after lvl: %d\n", combat_power(&hero));
    // EXPECT: combat after lvl: 16263

    // Add more items
    struct Item scroll;
    init_item(&scroll, 6, ITEM_SCROLL, RARITY_RARE, 75, 1, 5);
    struct Item amulet;
    init_item(&amulet, 7, ITEM_AMULET, RARITY_EPIC, 150, 3, 60);
    add_item(&hero, &scroll);
    add_item(&hero, &amulet);

    printf("final count: %d\n", hero.inv.count);
    // EXPECT: final count: 7
    printf("final value: %d\n", inventory_total_value(&hero));
    // EXPECT: final value: 1015
    printf("final weight: %d\n", hero.inv.current_weight);
    // EXPECT: final weight: 43

    return 0;
}
