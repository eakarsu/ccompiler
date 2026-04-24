int printf(const char *fmt, ...);

enum VendState { IDLE, ACCEPTING, DISPENSING, RETURNING_CHANGE };

struct Product {
    char *name;
    int price;
    int stock;
};

struct VendingMachine {
    int state;
    int balance;
    int total_sales;
    int total_revenue;
    struct Product products[5];
    int product_count;
};

void init_machine(struct VendingMachine *vm) {
    vm->state = IDLE;
    vm->balance = 0;
    vm->total_sales = 0;
    vm->total_revenue = 0;
    vm->product_count = 0;
}

void add_product(struct VendingMachine *vm, char *name, int price, int stock) {
    if (vm->product_count < 5) {
        int idx = vm->product_count;
        vm->products[idx].name = name;
        vm->products[idx].price = price;
        vm->products[idx].stock = stock;
        vm->product_count = vm->product_count + 1;
    }
}

void insert_coin(struct VendingMachine *vm, int amount) {
    if (vm->state == IDLE) {
        vm->state = ACCEPTING;
    }
    if (vm->state == ACCEPTING) {
        vm->balance = vm->balance + amount;
        printf("Inserted %d, balance: %d\n", amount, vm->balance);
    }
}

int select_product(struct VendingMachine *vm, int idx) {
    if (idx < 0 || idx >= vm->product_count) {
        printf("Invalid selection\n");
        return -1;
    }
    if (vm->products[idx].stock <= 0) {
        printf("Out of stock: %s\n", vm->products[idx].name);
        return -2;
    }
    if (vm->balance < vm->products[idx].price) {
        printf("Insufficient funds for %s (need %d, have %d)\n",
               vm->products[idx].name, vm->products[idx].price, vm->balance);
        return -3;
    }

    vm->state = DISPENSING;
    vm->balance = vm->balance - vm->products[idx].price;
    vm->products[idx].stock = vm->products[idx].stock - 1;
    vm->total_sales = vm->total_sales + 1;
    vm->total_revenue = vm->total_revenue + vm->products[idx].price;
    printf("Dispensing: %s\n", vm->products[idx].name);

    if (vm->balance > 0) {
        vm->state = RETURNING_CHANGE;
        printf("Change returned: %d\n", vm->balance);
        vm->balance = 0;
    }
    vm->state = IDLE;
    return 0;
}

void cancel_transaction(struct VendingMachine *vm) {
    if (vm->balance > 0) {
        printf("Refunding: %d\n", vm->balance);
        vm->balance = 0;
    }
    vm->state = IDLE;
}

void print_inventory(struct VendingMachine *vm) {
    int i;
    for (i = 0; i < vm->product_count; i = i + 1) {
        printf("  [%d] %s: price=%d stock=%d\n", i,
               vm->products[i].name, vm->products[i].price, vm->products[i].stock);
    }
}

void print_stats(struct VendingMachine *vm) {
    printf("Sales: %d, Revenue: %d\n", vm->total_sales, vm->total_revenue);
}

int main() {
    struct VendingMachine vm;
    init_machine(&vm);

    add_product(&vm, "Cola", 150, 3);
    add_product(&vm, "Chips", 100, 2);
    add_product(&vm, "Water", 75, 5);
    add_product(&vm, "Candy", 50, 1);

    printf("--- Inventory ---\n");
    // EXPECT: --- Inventory ---
    print_inventory(&vm);
    // EXPECT:   [0] Cola: price=150 stock=3
    // EXPECT:   [1] Chips: price=100 stock=2
    // EXPECT:   [2] Water: price=75 stock=5
    // EXPECT:   [3] Candy: price=50 stock=1

    printf("--- Buy Cola ---\n");
    // EXPECT: --- Buy Cola ---
    insert_coin(&vm, 100);
    // EXPECT: Inserted 100, balance: 100
    insert_coin(&vm, 50);
    // EXPECT: Inserted 50, balance: 150
    select_product(&vm, 0);
    // EXPECT: Dispensing: Cola

    printf("--- Buy Chips with change ---\n");
    // EXPECT: --- Buy Chips with change ---
    insert_coin(&vm, 50);
    // EXPECT: Inserted 50, balance: 50
    insert_coin(&vm, 50);
    // EXPECT: Inserted 50, balance: 100
    insert_coin(&vm, 25);
    // EXPECT: Inserted 25, balance: 125
    select_product(&vm, 1);
    // EXPECT: Dispensing: Chips
    // EXPECT: Change returned: 25

    printf("--- Insufficient funds ---\n");
    // EXPECT: --- Insufficient funds ---
    insert_coin(&vm, 25);
    // EXPECT: Inserted 25, balance: 25
    select_product(&vm, 0);
    // EXPECT: Insufficient funds for Cola (need 150, have 25)
    cancel_transaction(&vm);
    // EXPECT: Refunding: 25

    printf("--- Buy last candy ---\n");
    // EXPECT: --- Buy last candy ---
    insert_coin(&vm, 50);
    // EXPECT: Inserted 50, balance: 50
    select_product(&vm, 3);
    // EXPECT: Dispensing: Candy

    printf("--- Try out of stock ---\n");
    // EXPECT: --- Try out of stock ---
    insert_coin(&vm, 50);
    // EXPECT: Inserted 50, balance: 50
    select_product(&vm, 3);
    // EXPECT: Out of stock: Candy
    cancel_transaction(&vm);
    // EXPECT: Refunding: 50

    printf("--- Final stats ---\n");
    // EXPECT: --- Final stats ---
    print_stats(&vm);
    // EXPECT: Sales: 3, Revenue: 300

    return 0;
}
