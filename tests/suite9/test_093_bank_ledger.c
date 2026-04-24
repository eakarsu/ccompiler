int printf(const char *fmt, ...);

enum TxnType { DEPOSIT, WITHDRAWAL, TRANSFER };

struct Transaction {
    int id;
    int type;
    int amount;
    int from_acct;
    int to_acct;
};

struct Account {
    int id;
    char name[32];
    int balance;
    int active;
};

struct Ledger {
    struct Account accounts[10];
    int num_accounts;
    struct Transaction log[50];
    int num_txns;
    int next_txn_id;
};

void str_copy(char *dst, const char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void init_ledger(struct Ledger *led) {
    led->num_accounts = 0;
    led->num_txns = 0;
    led->next_txn_id = 1000;
}

int create_account(struct Ledger *led, const char *name, int initial) {
    int idx;
    idx = led->num_accounts;
    led->accounts[idx].id = idx + 1;
    str_copy(led->accounts[idx].name, name);
    led->accounts[idx].balance = initial;
    led->accounts[idx].active = 1;
    led->num_accounts = led->num_accounts + 1;
    return led->accounts[idx].id;
}

int deposit(struct Ledger *led, int acct_id, int amount) {
    int idx;
    int tid;
    idx = acct_id - 1;
    if (idx < 0 || idx >= led->num_accounts) return -1;
    if (!led->accounts[idx].active) return -1;
    led->accounts[idx].balance = led->accounts[idx].balance + amount;
    tid = led->next_txn_id;
    led->log[led->num_txns].id = tid;
    led->log[led->num_txns].type = DEPOSIT;
    led->log[led->num_txns].amount = amount;
    led->log[led->num_txns].from_acct = 0;
    led->log[led->num_txns].to_acct = acct_id;
    led->num_txns = led->num_txns + 1;
    led->next_txn_id = led->next_txn_id + 1;
    return tid;
}

int withdraw(struct Ledger *led, int acct_id, int amount) {
    int idx;
    int tid;
    idx = acct_id - 1;
    if (idx < 0 || idx >= led->num_accounts) return -1;
    if (!led->accounts[idx].active) return -1;
    if (led->accounts[idx].balance < amount) return -2;
    led->accounts[idx].balance = led->accounts[idx].balance - amount;
    tid = led->next_txn_id;
    led->log[led->num_txns].id = tid;
    led->log[led->num_txns].type = WITHDRAWAL;
    led->log[led->num_txns].amount = amount;
    led->log[led->num_txns].from_acct = acct_id;
    led->log[led->num_txns].to_acct = 0;
    led->num_txns = led->num_txns + 1;
    led->next_txn_id = led->next_txn_id + 1;
    return tid;
}

int transfer(struct Ledger *led, int from_id, int to_id, int amount) {
    int fi;
    int ti;
    int tid;
    fi = from_id - 1;
    ti = to_id - 1;
    if (fi < 0 || fi >= led->num_accounts) return -1;
    if (ti < 0 || ti >= led->num_accounts) return -1;
    if (led->accounts[fi].balance < amount) return -2;
    led->accounts[fi].balance = led->accounts[fi].balance - amount;
    led->accounts[ti].balance = led->accounts[ti].balance + amount;
    tid = led->next_txn_id;
    led->log[led->num_txns].id = tid;
    led->log[led->num_txns].type = TRANSFER;
    led->log[led->num_txns].amount = amount;
    led->log[led->num_txns].from_acct = from_id;
    led->log[led->num_txns].to_acct = to_id;
    led->num_txns = led->num_txns + 1;
    led->next_txn_id = led->next_txn_id + 1;
    return tid;
}

void print_balances(struct Ledger *led) {
    int i;
    for (i = 0; i < led->num_accounts; i = i + 1) {
        if (led->accounts[i].active) {
            printf("  [%d] %s: %d\n", led->accounts[i].id,
                   led->accounts[i].name, led->accounts[i].balance);
        }
    }
}

int total_assets(struct Ledger *led) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < led->num_accounts; i = i + 1) {
        if (led->accounts[i].active) {
            sum = sum + led->accounts[i].balance;
        }
    }
    return sum;
}

int main() {
    struct Ledger led;
    int a1;
    int a2;
    int a3;
    int tid;
    init_ledger(&led);

    // EXPECT: === Bank Ledger Test ===
    printf("=== Bank Ledger Test ===\n");

    a1 = create_account(&led, "Alice", 1000);
    a2 = create_account(&led, "Bob", 500);
    a3 = create_account(&led, "Charlie", 2000);

    // EXPECT: Accounts created: 1 2 3
    printf("Accounts created: %d %d %d\n", a1, a2, a3);

    // EXPECT: Initial balances:
    printf("Initial balances:\n");
    // EXPECT:   [1] Alice: 1000
    // EXPECT:   [2] Bob: 500
    // EXPECT:   [3] Charlie: 2000
    print_balances(&led);

    tid = deposit(&led, a1, 500);
    // EXPECT: Deposit txn: 1000
    printf("Deposit txn: %d\n", tid);

    tid = withdraw(&led, a2, 200);
    // EXPECT: Withdraw txn: 1001
    printf("Withdraw txn: %d\n", tid);

    tid = transfer(&led, a3, a1, 750);
    // EXPECT: Transfer txn: 1002
    printf("Transfer txn: %d\n", tid);

    // EXPECT: After transactions:
    printf("After transactions:\n");
    // EXPECT:   [1] Alice: 2250
    // EXPECT:   [2] Bob: 300
    // EXPECT:   [3] Charlie: 1250
    print_balances(&led);

    // EXPECT: Total assets: 3800
    printf("Total assets: %d\n", total_assets(&led));

    tid = withdraw(&led, a2, 999);
    // EXPECT: Overdraft result: -2
    printf("Overdraft result: %d\n", tid);

    // EXPECT: Transaction count: 3
    printf("Transaction count: %d\n", led.num_txns);

    // EXPECT: === Done ===
    printf("=== Done ===\n");
    return 0;
}
