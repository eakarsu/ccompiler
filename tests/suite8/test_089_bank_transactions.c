int printf(const char *fmt, ...);

enum TxType { TX_DEPOSIT, TX_WITHDRAW, TX_TRANSFER };

struct Transaction {
    int type;
    int from_account;
    int to_account;
    int amount;
    int success;
};

struct Account {
    int id;
    int balance;
    int total_deposits;
    int total_withdrawals;
    int tx_count;
    int is_frozen;
};

struct Bank {
    struct Account accounts[6];
    int account_count;
    struct Transaction history[30];
    int history_count;
    int total_transactions;
    int failed_transactions;
};

void init_bank(struct Bank *bank) {
    bank->account_count = 0;
    bank->history_count = 0;
    bank->total_transactions = 0;
    bank->failed_transactions = 0;
}

int create_account(struct Bank *bank, int id, int initial_balance) {
    if (bank->account_count >= 6) return -1;
    int idx = bank->account_count;
    bank->accounts[idx].id = id;
    bank->accounts[idx].balance = initial_balance;
    bank->accounts[idx].total_deposits = initial_balance;
    bank->accounts[idx].total_withdrawals = 0;
    bank->accounts[idx].tx_count = 0;
    bank->accounts[idx].is_frozen = 0;
    bank->account_count = bank->account_count + 1;
    return idx;
}

int find_account(struct Bank *bank, int id) {
    int i;
    for (i = 0; i < bank->account_count; i = i + 1) {
        if (bank->accounts[i].id == id) return i;
    }
    return -1;
}

void record_tx(struct Bank *bank, int type, int from, int to, int amount, int success) {
    if (bank->history_count < 30) {
        int idx = bank->history_count;
        bank->history[idx].type = type;
        bank->history[idx].from_account = from;
        bank->history[idx].to_account = to;
        bank->history[idx].amount = amount;
        bank->history[idx].success = success;
        bank->history_count = bank->history_count + 1;
    }
    bank->total_transactions = bank->total_transactions + 1;
    if (!success) bank->failed_transactions = bank->failed_transactions + 1;
}

int deposit(struct Bank *bank, int account_id, int amount) {
    int idx = find_account(bank, account_id);
    if (idx < 0) {
        printf("Deposit FAIL: account %d not found\n", account_id);
        record_tx(bank, TX_DEPOSIT, -1, account_id, amount, 0);
        return 0;
    }
    if (bank->accounts[idx].is_frozen) {
        printf("Deposit FAIL: account %d is frozen\n", account_id);
        record_tx(bank, TX_DEPOSIT, -1, account_id, amount, 0);
        return 0;
    }
    if (amount <= 0) {
        printf("Deposit FAIL: invalid amount %d\n", amount);
        record_tx(bank, TX_DEPOSIT, -1, account_id, amount, 0);
        return 0;
    }
    bank->accounts[idx].balance = bank->accounts[idx].balance + amount;
    bank->accounts[idx].total_deposits = bank->accounts[idx].total_deposits + amount;
    bank->accounts[idx].tx_count = bank->accounts[idx].tx_count + 1;
    printf("Deposit %d to account %d, balance=%d\n", amount, account_id, bank->accounts[idx].balance);
    record_tx(bank, TX_DEPOSIT, -1, account_id, amount, 1);
    return 1;
}

int withdraw(struct Bank *bank, int account_id, int amount) {
    int idx = find_account(bank, account_id);
    if (idx < 0) {
        printf("Withdraw FAIL: account %d not found\n", account_id);
        record_tx(bank, TX_WITHDRAW, account_id, -1, amount, 0);
        return 0;
    }
    if (bank->accounts[idx].is_frozen) {
        printf("Withdraw FAIL: account %d is frozen\n", account_id);
        record_tx(bank, TX_WITHDRAW, account_id, -1, amount, 0);
        return 0;
    }
    if (amount <= 0 || amount > bank->accounts[idx].balance) {
        printf("Withdraw FAIL: insufficient funds (have %d, want %d)\n",
               bank->accounts[idx].balance, amount);
        record_tx(bank, TX_WITHDRAW, account_id, -1, amount, 0);
        return 0;
    }
    bank->accounts[idx].balance = bank->accounts[idx].balance - amount;
    bank->accounts[idx].total_withdrawals = bank->accounts[idx].total_withdrawals + amount;
    bank->accounts[idx].tx_count = bank->accounts[idx].tx_count + 1;
    printf("Withdraw %d from account %d, balance=%d\n", amount, account_id, bank->accounts[idx].balance);
    record_tx(bank, TX_WITHDRAW, account_id, -1, amount, 1);
    return 1;
}

int transfer(struct Bank *bank, int from_id, int to_id, int amount) {
    int from_idx = find_account(bank, from_id);
    int to_idx = find_account(bank, to_id);
    if (from_idx < 0 || to_idx < 0) {
        printf("Transfer FAIL: account not found\n");
        record_tx(bank, TX_TRANSFER, from_id, to_id, amount, 0);
        return 0;
    }
    if (bank->accounts[from_idx].is_frozen || bank->accounts[to_idx].is_frozen) {
        printf("Transfer FAIL: frozen account\n");
        record_tx(bank, TX_TRANSFER, from_id, to_id, amount, 0);
        return 0;
    }
    if (amount <= 0 || amount > bank->accounts[from_idx].balance) {
        printf("Transfer FAIL: insufficient funds\n");
        record_tx(bank, TX_TRANSFER, from_id, to_id, amount, 0);
        return 0;
    }
    bank->accounts[from_idx].balance = bank->accounts[from_idx].balance - amount;
    bank->accounts[from_idx].total_withdrawals = bank->accounts[from_idx].total_withdrawals + amount;
    bank->accounts[from_idx].tx_count = bank->accounts[from_idx].tx_count + 1;
    bank->accounts[to_idx].balance = bank->accounts[to_idx].balance + amount;
    bank->accounts[to_idx].total_deposits = bank->accounts[to_idx].total_deposits + amount;
    bank->accounts[to_idx].tx_count = bank->accounts[to_idx].tx_count + 1;
    printf("Transfer %d: account %d -> account %d\n", amount, from_id, to_id);
    record_tx(bank, TX_TRANSFER, from_id, to_id, amount, 1);
    return 1;
}

void freeze_account(struct Bank *bank, int account_id) {
    int idx = find_account(bank, account_id);
    if (idx >= 0) {
        bank->accounts[idx].is_frozen = 1;
        printf("Account %d frozen\n", account_id);
    }
}

void print_account(struct Bank *bank, int account_id) {
    int idx = find_account(bank, account_id);
    if (idx >= 0) {
        printf("Account %d: balance=%d deposits=%d withdrawals=%d txns=%d\n",
               account_id, bank->accounts[idx].balance,
               bank->accounts[idx].total_deposits,
               bank->accounts[idx].total_withdrawals,
               bank->accounts[idx].tx_count);
    }
}

int main() {
    struct Bank bank;
    init_bank(&bank);

    create_account(&bank, 1001, 500);
    create_account(&bank, 1002, 300);
    create_account(&bank, 1003, 100);

    printf("=== Deposits ===\n");
    // EXPECT: === Deposits ===
    deposit(&bank, 1001, 200);
    // EXPECT: Deposit 200 to account 1001, balance=700
    deposit(&bank, 1002, 150);
    // EXPECT: Deposit 150 to account 1002, balance=450

    printf("=== Withdrawals ===\n");
    // EXPECT: === Withdrawals ===
    withdraw(&bank, 1001, 100);
    // EXPECT: Withdraw 100 from account 1001, balance=600
    withdraw(&bank, 1003, 200);
    // EXPECT: Withdraw FAIL: insufficient funds (have 100, want 200)

    printf("=== Transfers ===\n");
    // EXPECT: === Transfers ===
    transfer(&bank, 1001, 1003, 250);
    // EXPECT: Transfer 250: account 1001 -> account 1003
    transfer(&bank, 1002, 1001, 100);
    // EXPECT: Transfer 100: account 1002 -> account 1001

    printf("=== Freeze test ===\n");
    // EXPECT: === Freeze test ===
    freeze_account(&bank, 1002);
    // EXPECT: Account 1002 frozen
    deposit(&bank, 1002, 50);
    // EXPECT: Deposit FAIL: account 1002 is frozen
    withdraw(&bank, 1002, 50);
    // EXPECT: Withdraw FAIL: account 1002 is frozen

    printf("=== Account Summary ===\n");
    // EXPECT: === Account Summary ===
    print_account(&bank, 1001);
    // EXPECT: Account 1001: balance=450 deposits=800 withdrawals=350 txns=4
    print_account(&bank, 1002);
    // EXPECT: Account 1002: balance=350 deposits=450 withdrawals=100 txns=2
    print_account(&bank, 1003);
    // EXPECT: Account 1003: balance=350 deposits=350 withdrawals=0 txns=1

    printf("Total txns: %d, Failed: %d\n", bank.total_transactions, bank.failed_transactions);
    // EXPECT: Total txns: 8, Failed: 3

    return 0;
}
