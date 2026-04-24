int printf(const char *fmt, ...);

/* Vending machine state machine.
   States: IDLE=0, COIN_5=1, COIN_10=2, COIN_15=3, COIN_20=4, DISPENSING=5
   Inputs: NICKEL=0, DIME=1, QUARTER=2, CANCEL=3
   Item costs 20 cents. Machine accepts nickels, dimes, quarters.
   Returns change if overpaid. */

typedef struct {
    int state;
    int balance;
    int dispensed;    /* count of items dispensed */
    int change_given; /* last change returned */
} VendingMachine;

void vm_init(VendingMachine *vm) {
    vm->state = 0;
    vm->balance = 0;
    vm->dispensed = 0;
    vm->change_given = 0;
}

int coin_value(int coin_type) {
    if (coin_type == 0) return 5;
    if (coin_type == 1) return 10;
    if (coin_type == 2) return 25;
    return 0;
}

void vm_insert_coin(VendingMachine *vm, int coin_type) {
    int val;
    int total;

    if (coin_type == 3) {
        /* Cancel */
        vm->change_given = vm->balance;
        vm->balance = 0;
        vm->state = 0;
        return;
    }

    val = coin_value(coin_type);
    total = vm->balance + val;

    if (total >= 20) {
        /* Dispense item */
        vm->change_given = total - 20;
        vm->balance = 0;
        vm->dispensed++;
        vm->state = 5; /* dispensing */
    } else {
        vm->balance = total;
        vm->change_given = 0;
        if (total == 5) vm->state = 1;
        else if (total == 10) vm->state = 2;
        else if (total == 15) vm->state = 3;
        else vm->state = 4;
    }
}

void vm_reset_after_dispense(VendingMachine *vm) {
    if (vm->state == 5) {
        vm->state = 0;
    }
}

void test_exact_payment(void) {
    VendingMachine vm;

    printf("=== Exact Payment ===\n");
    // EXPECT: === Exact Payment ===

    /* Pay with 4 nickels */
    vm_init(&vm);
    vm_insert_coin(&vm, 0); /* 5 */
    printf("after nickel1: state=%d bal=%d\n", vm.state, vm.balance);
    // EXPECT: after nickel1: state=1 bal=5
    vm_insert_coin(&vm, 0); /* 10 */
    printf("after nickel2: state=%d bal=%d\n", vm.state, vm.balance);
    // EXPECT: after nickel2: state=2 bal=10
    vm_insert_coin(&vm, 0); /* 15 */
    printf("after nickel3: state=%d bal=%d\n", vm.state, vm.balance);
    // EXPECT: after nickel3: state=3 bal=15
    vm_insert_coin(&vm, 0); /* 20 -> dispense */
    printf("after nickel4: state=%d disp=%d change=%d\n",
           vm.state, vm.dispensed, vm.change_given);
    // EXPECT: after nickel4: state=5 disp=1 change=0

    /* Pay with 2 dimes */
    vm_init(&vm);
    vm_insert_coin(&vm, 1); /* 10 */
    vm_insert_coin(&vm, 1); /* 20 -> dispense */
    printf("2 dimes: disp=%d change=%d\n", vm.dispensed, vm.change_given);
    // EXPECT: 2 dimes: disp=1 change=0
}

void test_overpayment(void) {
    VendingMachine vm;

    printf("=== Overpayment ===\n");
    // EXPECT: === Overpayment ===

    /* Pay with a quarter (25 cents, item costs 20) */
    vm_init(&vm);
    vm_insert_coin(&vm, 2); /* 25 -> dispense + 5 change */
    printf("quarter: disp=%d change=%d\n", vm.dispensed, vm.change_given);
    // EXPECT: quarter: disp=1 change=5

    /* Pay with dime + quarter = 35, change = 15 */
    vm_init(&vm);
    vm_insert_coin(&vm, 1); /* 10 */
    vm_insert_coin(&vm, 2); /* 35 -> dispense + 15 change */
    printf("dime+quarter: disp=%d change=%d\n", vm.dispensed, vm.change_given);
    // EXPECT: dime+quarter: disp=1 change=15

    /* Pay with nickel + dime + quarter = 40, change = 20 */
    vm_init(&vm);
    vm_insert_coin(&vm, 0); /* 5 */
    vm_insert_coin(&vm, 1); /* 15 */
    vm_insert_coin(&vm, 2); /* 40 -> dispense + 20 change */
    printf("n+d+q: disp=%d change=%d\n", vm.dispensed, vm.change_given);
    // EXPECT: n+d+q: disp=1 change=20
}

void test_cancel(void) {
    VendingMachine vm;

    printf("=== Cancel ===\n");
    // EXPECT: === Cancel ===

    /* Insert dime then cancel */
    vm_init(&vm);
    vm_insert_coin(&vm, 1); /* 10 */
    printf("before cancel: state=%d bal=%d\n", vm.state, vm.balance);
    // EXPECT: before cancel: state=2 bal=10
    vm_insert_coin(&vm, 3); /* cancel */
    printf("after cancel: state=%d change=%d\n", vm.state, vm.change_given);
    // EXPECT: after cancel: state=0 change=10

    /* Insert nickel + dime then cancel */
    vm_init(&vm);
    vm_insert_coin(&vm, 0); /* 5 */
    vm_insert_coin(&vm, 1); /* 15 */
    vm_insert_coin(&vm, 3); /* cancel */
    printf("n+d cancel: state=%d change=%d\n", vm.state, vm.change_given);
    // EXPECT: n+d cancel: state=0 change=15
}

void test_multiple_purchases(void) {
    VendingMachine vm;

    printf("=== Multiple Purchases ===\n");
    // EXPECT: === Multiple Purchases ===

    vm_init(&vm);

    /* First purchase: 2 dimes */
    vm_insert_coin(&vm, 1);
    vm_insert_coin(&vm, 1);
    printf("purchase 1: disp=%d\n", vm.dispensed);
    // EXPECT: purchase 1: disp=1
    vm_reset_after_dispense(&vm);

    /* Second purchase: quarter */
    vm_insert_coin(&vm, 2);
    printf("purchase 2: disp=%d change=%d\n", vm.dispensed, vm.change_given);
    // EXPECT: purchase 2: disp=2 change=5
    vm_reset_after_dispense(&vm);

    /* Third purchase: 4 nickels */
    vm_insert_coin(&vm, 0);
    vm_insert_coin(&vm, 0);
    vm_insert_coin(&vm, 0);
    vm_insert_coin(&vm, 0);
    printf("purchase 3: disp=%d change=%d\n", vm.dispensed, vm.change_given);
    // EXPECT: purchase 3: disp=3 change=0
}

void test_state_names(void) {
    VendingMachine vm;
    int states[6];
    int i;

    printf("=== State Progression ===\n");
    // EXPECT: === State Progression ===

    vm_init(&vm);
    states[0] = vm.state;
    vm_insert_coin(&vm, 0); states[1] = vm.state;
    vm_insert_coin(&vm, 0); states[2] = vm.state;
    vm_insert_coin(&vm, 0); states[3] = vm.state;

    printf("progression: %d->%d->%d->%d\n",
           states[0], states[1], states[2], states[3]);
    // EXPECT: progression: 0->1->2->3

    /* Cancel from state 3 */
    vm_insert_coin(&vm, 3);
    printf("cancel from 15c: state=%d change=%d\n", vm.state, vm.change_given);
    // EXPECT: cancel from 15c: state=0 change=15
}

int main(void) {
    test_exact_payment();
    test_overpayment();
    test_cancel();
    test_multiple_purchases();
    test_state_names();

    printf("Vending machine done.\n");
    // EXPECT: Vending machine done.
    return 0;
}
