int printf(const char *fmt, ...);

enum Fruit {
    APPLE = 0,
    BANANA = 1,
    CHERRY = 2,
    DATE = 3,
    ELDERBERRY = 4
};

enum Season {
    SPRING = 0,
    SUMMER = 1,
    AUTUMN = 2,
    WINTER = 3
};

int fruit_prices[5];
int season_temps[4];
char *fruit_names[5];
char *season_names[4];

void init_fruit_data() {
    fruit_prices[APPLE] = 120;
    fruit_prices[BANANA] = 80;
    fruit_prices[CHERRY] = 350;
    fruit_prices[DATE] = 500;
    fruit_prices[ELDERBERRY] = 700;

    fruit_names[APPLE] = "apple";
    fruit_names[BANANA] = "banana";
    fruit_names[CHERRY] = "cherry";
    fruit_names[DATE] = "date";
    fruit_names[ELDERBERRY] = "elderberry";
}

void init_season_data() {
    season_temps[SPRING] = 15;
    season_temps[SUMMER] = 30;
    season_temps[AUTUMN] = 12;
    season_temps[WINTER] = -5;

    season_names[SPRING] = "spring";
    season_names[SUMMER] = "summer";
    season_names[AUTUMN] = "autumn";
    season_names[WINTER] = "winter";
}

void print_fruits() {
    int i;
    for (i = APPLE; i <= ELDERBERRY; i++) {
        // EXPECT: fruit apple price=120
        // EXPECT: fruit banana price=80
        // EXPECT: fruit cherry price=350
        // EXPECT: fruit date price=500
        // EXPECT: fruit elderberry price=700
        printf("fruit %s price=%d\n", fruit_names[i], fruit_prices[i]);
    }
}

void print_seasons() {
    int i;
    for (i = SPRING; i <= WINTER; i++) {
        // EXPECT: season spring temp=15
        // EXPECT: season summer temp=30
        // EXPECT: season autumn temp=12
        // EXPECT: season winter temp=-5
        printf("season %s temp=%d\n", season_names[i], season_temps[i]);
    }
}

void test_lookup() {
    enum Fruit f = CHERRY;
    // EXPECT: lookup cherry=350
    printf("lookup %s=%d\n", fruit_names[f], fruit_prices[f]);

    enum Season s = WINTER;
    // EXPECT: lookup winter=-5
    printf("lookup %s=%d\n", season_names[s], season_temps[s]);
}

void test_sum_prices() {
    int total = 0;
    int i;
    for (i = APPLE; i <= ELDERBERRY; i++) {
        total = total + fruit_prices[i];
    }
    // EXPECT: total_price=1750
    printf("total_price=%d\n", total);
}

void test_max_temp() {
    int max = season_temps[SPRING];
    int max_idx = SPRING;
    int i;
    for (i = SUMMER; i <= WINTER; i++) {
        if (season_temps[i] > max) {
            max = season_temps[i];
            max_idx = i;
        }
    }
    // EXPECT: hottest summer temp=30
    printf("hottest %s temp=%d\n", season_names[max_idx], max);
}

void test_enum_offset_index() {
    int grid[5];
    grid[0] = 10;
    grid[1] = 20;
    grid[2] = 30;
    grid[3] = 40;
    grid[4] = 50;

    enum Fruit f;
    int sum = 0;
    for (f = APPLE; f <= ELDERBERRY; f++) {
        sum = sum + grid[f];
    }
    // EXPECT: grid_sum=150
    printf("grid_sum=%d\n", sum);
}

int main() {
    init_fruit_data();
    init_season_data();
    print_fruits();
    print_seasons();
    test_lookup();
    test_sum_prices();
    test_max_temp();
    test_enum_offset_index();
    // EXPECT: done
    printf("done\n");
    return 0;
}
