int printf(const char *fmt, ...);
// EXPECT: === Banking System Simulation ===\nInitial balances: 5000 3000 1000\nTxn 0: DEPOSIT amount=500 -> OK\nTxn 1: WITHDRAW amount=200 -> OK\nTxn 2: TRANSFER amount=1000 -> OK\nTxn 3: WITHDRAW amount=5000 -> FAILED\nTxn 4: DEPOSIT amount=750 -> OK\nTxn 5: TRANSFER amount=800 -> OK\nTxn 6: WITHDRAW amount=300 -> OK\nTxn 7: DEPOSIT amount=1200 -> OK\n--- Final State ---\nAccount 1001: balance=5000 txns=4\nAccount 1002: balance=3200 txns=3\nAccount 1003: balance=2750 txns=2\nSuccess: 7, Failed: 1

enum TxnType { DEPOSIT, WITHDRAW, TRANSFER };

struct Account {
    int id;
    int balance;
    int num_txns;
};

struct Transaction {
    enum TxnType type;
    int from_acct;
    int to_acct;
    int amount;
    int success;
};

void init_account(struct Account *a, int id, int balance) {
    a->id = id;
    a->balance = balance;
    a->num_txns = 0;
}

int deposit(struct Account *a, int amount) {
    if (amount <= 0) return 0;
    a->balance = a->balance + amount;
    a->num_txns++;
    return 1;
}

int withdraw(struct Account *a, int amount) {
    if (amount <= 0 || a->balance < amount) return 0;
    a->balance = a->balance - amount;
    a->num_txns++;
    return 1;
}

int transfer(struct Account *from, struct Account *to, int amount) {
    if (amount <= 0 || from->balance < amount) return 0;
    from->balance = from->balance - amount;
    to->balance = to->balance + amount;
    from->num_txns++;
    to->num_txns++;
    return 1;
}

const char *txn_name(enum TxnType t) {
    if (t == DEPOSIT) return "DEPOSIT";
    if (t == WITHDRAW) return "WITHDRAW";
    return "TRANSFER";
}

int main(void) {
    struct Account accounts[3];
    init_account(&accounts[0], 1001, 5000);
    init_account(&accounts[1], 1002, 3000);
    init_account(&accounts[2], 1003, 1000);

    struct Transaction txns[8];
    txns[0].type = DEPOSIT;   txns[0].from_acct = 0; txns[0].to_acct = 0; txns[0].amount = 500;
    txns[1].type = WITHDRAW;  txns[1].from_acct = 1; txns[1].to_acct = 1; txns[1].amount = 200;
    txns[2].type = TRANSFER;  txns[2].from_acct = 0; txns[2].to_acct = 2; txns[2].amount = 1000;
    txns[3].type = WITHDRAW;  txns[3].from_acct = 2; txns[3].to_acct = 2; txns[3].amount = 5000;
    txns[4].type = DEPOSIT;   txns[4].from_acct = 2; txns[4].to_acct = 2; txns[4].amount = 750;
    txns[5].type = TRANSFER;  txns[5].from_acct = 1; txns[5].to_acct = 0; txns[5].amount = 800;
    txns[6].type = WITHDRAW;  txns[6].from_acct = 0; txns[6].to_acct = 0; txns[6].amount = 300;
    txns[7].type = DEPOSIT;   txns[7].from_acct = 1; txns[7].to_acct = 1; txns[7].amount = 1200;

    printf("=== Banking System Simulation ===\n");
    printf("Initial balances: %d %d %d\n",
           accounts[0].balance, accounts[1].balance, accounts[2].balance);

    int i;
    int success_count = 0;
    int fail_count = 0;

    for (i = 0; i < 8; i++) {
        int ok = 0;
        if (txns[i].type == DEPOSIT) {
            ok = deposit(&accounts[txns[i].from_acct], txns[i].amount);
        } else if (txns[i].type == WITHDRAW) {
            ok = withdraw(&accounts[txns[i].from_acct], txns[i].amount);
        } else {
            ok = transfer(&accounts[txns[i].from_acct], &accounts[txns[i].to_acct], txns[i].amount);
        }
        txns[i].success = ok;
        if (ok) success_count++;
        else fail_count++;

        printf("Txn %d: %s amount=%d -> %s\n",
               i, txn_name(txns[i].type), txns[i].amount,
               ok ? "OK" : "FAILED");
    }

    printf("--- Final State ---\n");
    for (i = 0; i < 3; i++) {
        printf("Account %d: balance=%d txns=%d\n",
               accounts[i].id, accounts[i].balance, accounts[i].num_txns);
    }
    printf("Success: %d, Failed: %d\n", success_count, fail_count);

    return 0;
}
