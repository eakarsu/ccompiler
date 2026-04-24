int printf(const char *fmt, ...);

// Inventory management with reorder point system
// Tracks stock levels, reorder triggers, and costs

typedef struct {
    int stock;
    int reorder_point;
    int order_quantity;
    int lead_time;       // days until order arrives
    int pending_order;   // 1 if order is in transit
    int days_until_arrival;
    int total_orders;
    int total_stockouts;
    int total_holding_cost;
    int total_order_cost;
} Inventory;

void inv_init(Inventory *inv, int initial, int rop, int qty, int lead) {
    inv->stock = initial;
    inv->reorder_point = rop;
    inv->order_quantity = qty;
    inv->lead_time = lead;
    inv->pending_order = 0;
    inv->days_until_arrival = 0;
    inv->total_orders = 0;
    inv->total_stockouts = 0;
    inv->total_holding_cost = 0;
    inv->total_order_cost = 0;
}

// Simple demand generator using LCG
typedef struct {
    int state;
} RNG;

void rng_init(RNG *r, int seed) {
    r->state = seed % 32749;
}

int rng_next(RNG *r) {
    r->state = (1021 * r->state + 13) % 32749;
    return r->state;
}

int gen_demand(RNG *r, int avg_demand) {
    int v = rng_next(r) % (avg_demand * 2 + 1);
    return v;
}

// Simulate one day
void inv_day(Inventory *inv, int demand) {
    // Check if order arrives
    if (inv->pending_order) {
        inv->days_until_arrival = inv->days_until_arrival - 1;
        if (inv->days_until_arrival <= 0) {
            inv->stock = inv->stock + inv->order_quantity;
            inv->pending_order = 0;
        }
    }

    // Fulfill demand
    if (inv->stock >= demand) {
        inv->stock = inv->stock - demand;
    } else {
        inv->total_stockouts = inv->total_stockouts + (demand - inv->stock);
        inv->stock = 0;
    }

    // Holding cost: 1 per unit per day
    inv->total_holding_cost = inv->total_holding_cost + inv->stock;

    // Check reorder
    if (inv->stock <= inv->reorder_point && inv->pending_order == 0) {
        inv->pending_order = 1;
        inv->days_until_arrival = inv->lead_time;
        inv->total_orders = inv->total_orders + 1;
        inv->total_order_cost = inv->total_order_cost + 50; // fixed order cost
    }
}

// Compute service level (percent of days without stockout x100)
int service_level(int days, int stockout_days) {
    return (days - stockout_days) * 100 / days;
}

int main(void) {
    Inventory inv;
    RNG rng;

    // Scenario 1: well-stocked, low demand
    inv_init(&inv, 100, 30, 80, 3);
    rng_init(&rng, 42);

    int demands[30];
    int i;
    for (i = 0; i < 30; i++) {
        demands[i] = gen_demand(&rng, 5);
    }

    int stockout_days = 0;
    for (i = 0; i < 30; i++) {
        int before = inv.stock;
        inv_day(&inv, demands[i]);
        if (before < demands[i]) stockout_days++;
    }

    printf("Scenario 1 final stock: %d\n", inv.stock);
    // EXPECT: Scenario 1 final stock: 39
    printf("Total orders: %d\n", inv.total_orders);
    // EXPECT: Total orders: 1
    printf("Total stockouts: %d\n", inv.total_stockouts);
    // EXPECT: Total stockouts: 0
    printf("Holding cost: %d\n", inv.total_holding_cost);
    // EXPECT: Holding cost: 1931
    printf("Order cost: %d\n", inv.total_order_cost);
    // EXPECT: Order cost: 50

    int svc = service_level(30, stockout_days);
    printf("Service level: %d\n", svc);
    // EXPECT: Service level: 100

    // Scenario 2: tight stock, higher demand
    Inventory inv2;
    RNG rng2;
    inv_init(&inv2, 30, 10, 40, 2);
    rng_init(&rng2, 77);

    int demands2[30];
    for (i = 0; i < 30; i++) {
        demands2[i] = gen_demand(&rng2, 5);
    }

    int stockout_days2 = 0;
    for (i = 0; i < 30; i++) {
        int before = inv2.stock;
        inv_day(&inv2, demands2[i]);
        if (before < demands2[i]) stockout_days2++;
    }

    printf("Scenario 2 final stock: %d\n", inv2.stock);
    // EXPECT: Scenario 2 final stock: 17
    printf("S2 total orders: %d\n", inv2.total_orders);
    // EXPECT: S2 total orders: 3
    printf("S2 total stockouts: %d\n", inv2.total_stockouts);
    // EXPECT: S2 total stockouts: 0
    printf("S2 holding cost: %d\n", inv2.total_holding_cost);
    // EXPECT: S2 holding cost: 646
    printf("S2 order cost: %d\n", inv2.total_order_cost);
    // EXPECT: S2 order cost: 150

    int svc2 = service_level(30, stockout_days2);
    printf("S2 service level: %d\n", svc2);
    // EXPECT: S2 service level: 90

    // Total cost comparison
    int total_cost_1 = inv.total_holding_cost + inv.total_order_cost;
    int total_cost_2 = inv2.total_holding_cost + inv2.total_order_cost;
    printf("Total cost S1: %d\n", total_cost_1);
    // EXPECT: Total cost S1: 1981
    printf("Total cost S2: %d\n", total_cost_2);
    // EXPECT: Total cost S2: 796

    printf("S1 cheaper: %d\n", total_cost_1 < total_cost_2 ? 1 : 0);
    // EXPECT: S1 cheaper: 0

    return 0;
}
