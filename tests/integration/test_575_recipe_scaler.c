int printf(const char *fmt, ...);
// EXPECT: Recipe: Pancakes (serves 4)\n  2 cups flour\n  0.5 tbsp sugar\n  2 pcs eggs\n  1.5 cups milk\n  1 tbsp butter\n  0.5 tsp salt\n\nScaled to 8 servings:\nRecipe: Pancakes (serves 8)\n  4 cups flour\n  1 tbsp sugar\n  4 pcs eggs\n  3 cups milk\n  2 tbsp butter\n  1 tsp salt\n\nScaled to 2 servings:\nRecipe: Pancakes (serves 2)\n  1 cups flour\n  0.2 tbsp sugar\n  1 pcs eggs\n  0.7 cups milk\n  0.5 tbsp butter\n  0.2 tsp salt\n\nScaled to 12 servings:\nRecipe: Pancakes (serves 12)\n  6 cups flour\n  1.5 tbsp sugar\n  6 pcs eggs\n  4.5 cups milk\n  3 tbsp butter\n  1.5 tsp salt\n\nTotal amounts (x10):\n  Original (4): 75\n  Scaled (8): 150
#define MAX_INGREDIENTS 8

struct Ingredient {
    char *name;
    int amount_x10;
    char *unit;
};

struct Recipe {
    char *name;
    int servings;
    struct Ingredient ingredients[MAX_INGREDIENTS];
    int num_ingredients;
};

void recipe_init(struct Recipe *r, char *name, int servings) {
    r->name = name;
    r->servings = servings;
    r->num_ingredients = 0;
}

void recipe_add(struct Recipe *r, char *name, int amount_x10, char *unit) {
    if (r->num_ingredients < MAX_INGREDIENTS) {
        r->ingredients[r->num_ingredients].name = name;
        r->ingredients[r->num_ingredients].amount_x10 = amount_x10;
        r->ingredients[r->num_ingredients].unit = unit;
        r->num_ingredients++;
    }
}

void print_amount(int val_x10) {
    int whole = val_x10 / 10;
    int frac = val_x10 % 10;
    if (frac == 0) {
        printf("%d", whole);
    } else {
        printf("%d.%d", whole, frac);
    }
}

void recipe_print(struct Recipe *r) {
    printf("Recipe: %s (serves %d)\n", r->name, r->servings);
    int i;
    for (i = 0; i < r->num_ingredients; i++) {
        printf("  ");
        print_amount(r->ingredients[i].amount_x10);
        printf(" %s %s\n", r->ingredients[i].unit, r->ingredients[i].name);
    }
}

void recipe_scale(struct Recipe *src, struct Recipe *dst, int new_servings) {
    dst->name = src->name;
    dst->servings = new_servings;
    dst->num_ingredients = src->num_ingredients;
    int i;
    for (i = 0; i < src->num_ingredients; i++) {
        dst->ingredients[i].name = src->ingredients[i].name;
        dst->ingredients[i].unit = src->ingredients[i].unit;
        dst->ingredients[i].amount_x10 =
            src->ingredients[i].amount_x10 * new_servings / src->servings;
    }
}

int recipe_total_amount(struct Recipe *r) {
    int total = 0;
    int i;
    for (i = 0; i < r->num_ingredients; i++) {
        total = total + r->ingredients[i].amount_x10;
    }
    return total;
}

int main(void) {
    struct Recipe pancakes;
    recipe_init(&pancakes, "Pancakes", 4);
    recipe_add(&pancakes, "flour", 20, "cups");
    recipe_add(&pancakes, "sugar", 5, "tbsp");
    recipe_add(&pancakes, "eggs", 20, "pcs");
    recipe_add(&pancakes, "milk", 15, "cups");
    recipe_add(&pancakes, "butter", 10, "tbsp");
    recipe_add(&pancakes, "salt", 5, "tsp");

    recipe_print(&pancakes);

    struct Recipe scaled;
    recipe_scale(&pancakes, &scaled, 8);
    printf("\nScaled to 8 servings:\n");
    recipe_print(&scaled);

    recipe_scale(&pancakes, &scaled, 2);
    printf("\nScaled to 2 servings:\n");
    recipe_print(&scaled);

    recipe_scale(&pancakes, &scaled, 12);
    printf("\nScaled to 12 servings:\n");
    recipe_print(&scaled);

    printf("\nTotal amounts (x10):\n");
    printf("  Original (4): %d\n", recipe_total_amount(&pancakes));
    recipe_scale(&pancakes, &scaled, 8);
    printf("  Scaled (8): %d\n", recipe_total_amount(&scaled));

    return 0;
}
