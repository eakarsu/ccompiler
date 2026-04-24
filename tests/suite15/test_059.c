int printf(const char *fmt, ...);

// Supply chain simulation with multiple warehouses
// Factory -> 3 Warehouses -> Customers
// Each warehouse has stock, reorder point, and demand pattern

typedef struct {
    int stock;
    int reorder_point;
    int order_qty;
    int pending;
    int transit_days;
    int total_demand;
    int total_fulfilled;
    int total_orders_placed;
} Warehouse;

typedef struct {
    int production_rate;
    int stock;
    int total_produced;
    int total_shipped;
} Factory;

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

void wh_init(Warehouse *w, int stock, int rop, int qty) {
    w->stock = stock;
    w->reorder_point = rop;
    w->order_qty = qty;
    w->pending = 0;
    w->transit_days = 0;
    w->total_demand = 0;
    w->total_fulfilled = 0;
    w->total_orders_placed = 0;
}

void factory_init(Factory *f, int rate) {
    f->production_rate = rate;
    f->stock = 0;
    f->total_produced = 0;
    f->total_shipped = 0;
}

void factory_produce(Factory *f) {
    f->stock = f->stock + f->production_rate;
    f->total_produced = f->total_produced + f->production_rate;
}

int factory_ship(Factory *f, int amount) {
    if (f->stock >= amount) {
        f->stock = f->stock - amount;
        f->total_shipped = f->total_shipped + amount;
        return amount;
    }
    int shipped = f->stock;
    f->total_shipped = f->total_shipped + shipped;
    f->stock = 0;
    return shipped;
}

void wh_step(Warehouse *w, Factory *f, int demand) {
    // Receive pending shipment
    if (w->pending > 0) {
        w->transit_days = w->transit_days - 1;
        if (w->transit_days <= 0) {
            w->stock = w->stock + w->pending;
            w->pending = 0;
        }
    }

    // Serve demand
    w->total_demand = w->total_demand + demand;
    if (w->stock >= demand) {
        w->stock = w->stock - demand;
        w->total_fulfilled = w->total_fulfilled + demand;
    } else {
        w->total_fulfilled = w->total_fulfilled + w->stock;
        w->stock = 0;
    }

    // Reorder if below reorder point and no pending
    if (w->stock <= w->reorder_point && w->pending == 0) {
        int shipped = factory_ship(f, w->order_qty);
        if (shipped > 0) {
            w->pending = shipped;
            w->transit_days = 2;
            w->total_orders_placed++;
        }
    }
}

int main(void) {
    Factory factory;
    Warehouse w0;
    Warehouse w1;
    Warehouse w2;
    RNG rng0;
    RNG rng1;
    RNG rng2;

    factory_init(&factory, 15);
    wh_init(&w0, 50, 20, 30);
    wh_init(&w1, 30, 10, 20);
    wh_init(&w2, 20, 5, 15);

    rng_init(&rng0, 42);
    rng_init(&rng1, 77);
    rng_init(&rng2, 13);

    int day;
    for (day = 0; day < 30; day++) {
        factory_produce(&factory);

        int d0 = rng_next(&rng0) % 8;
        int d1 = rng_next(&rng1) % 6;
        int d2 = rng_next(&rng2) % 4;

        wh_step(&w0, &factory, d0);
        wh_step(&w1, &factory, d1);
        wh_step(&w2, &factory, d2);
    }

    printf("Factory produced: %d\n", factory.total_produced);
    // EXPECT: Factory produced: 450
    printf("Factory shipped: %d\n", factory.total_shipped);
    // EXPECT: Factory shipped: 195
    printf("Factory stock: %d\n", factory.stock);
    // EXPECT: Factory stock: 255

    printf("W0 stock: %d\n", w0.stock);
    // EXPECT: W0 stock: 40
    printf("W0 demand: %d\n", w0.total_demand);
    // EXPECT: W0 demand: 100
    printf("W0 fulfilled: %d\n", w0.total_fulfilled);
    // EXPECT: W0 fulfilled: 100
    printf("W0 orders: %d\n", w0.total_orders_placed);
    // EXPECT: W0 orders: 3

    printf("W1 stock: %d\n", w1.stock);
    // EXPECT: W1 stock: 12
    printf("W1 demand: %d\n", w1.total_demand);
    // EXPECT: W1 demand: 78
    printf("W1 fulfilled: %d\n", w1.total_fulfilled);
    // EXPECT: W1 fulfilled: 78
    printf("W1 orders: %d\n", w1.total_orders_placed);
    // EXPECT: W1 orders: 3

    printf("W2 stock: %d\n", w2.stock);
    // EXPECT: W2 stock: 10
    printf("W2 demand: %d\n", w2.total_demand);
    // EXPECT: W2 demand: 55
    printf("W2 fulfilled: %d\n", w2.total_fulfilled);
    // EXPECT: W2 fulfilled: 55
    printf("W2 orders: %d\n", w2.total_orders_placed);
    // EXPECT: W2 orders: 3

    // Service levels (percent)
    int svc0 = w0.total_fulfilled * 100 / w0.total_demand;
    int svc1 = w1.total_fulfilled * 100 / w1.total_demand;
    int svc2 = w2.total_fulfilled * 100 / w2.total_demand;
    printf("Service W0: %d\n", svc0);
    // EXPECT: Service W0: 100
    printf("Service W1: %d\n", svc1);
    // EXPECT: Service W1: 100
    printf("Service W2: %d\n", svc2);
    // EXPECT: Service W2: 100

    int total_demand = w0.total_demand + w1.total_demand + w2.total_demand;
    int total_fulfilled = w0.total_fulfilled + w1.total_fulfilled + w2.total_fulfilled;
    printf("Total demand: %d\n", total_demand);
    // EXPECT: Total demand: 233
    printf("Total fulfilled: %d\n", total_fulfilled);
    // EXPECT: Total fulfilled: 233

    int total_stock = w0.stock + w1.stock + w2.stock;
    printf("Total warehouse stock: %d\n", total_stock);
    // EXPECT: Total warehouse stock: 62

    return 0;
}
