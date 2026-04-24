int printf(const char *fmt, ...);

struct Transaction {
    int from_id;
    int to_id;
    int amount;
    int type;
};

struct Account {
    int id;
    int balance;
    int active;
    char name[16];
};

int str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
    return i;
}

int str_eq(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int create_account(struct Account *accounts, int *count, char *name, int initial) {
    int idx;
    idx = *count;
    if (idx >= 10) return -1;
    accounts[idx].id = idx + 1;
    accounts[idx].balance = initial;
    accounts[idx].active = 1;
    str_copy(accounts[idx].name, name);
    *count = *count + 1;
    return accounts[idx].id;
}

int find_account(struct Account *accounts, int count, int id) {
    int i;
    i = 0;
    while (i < count) {
        if (accounts[i].id == id && accounts[i].active) return i;
        i = i + 1;
    }
    return -1;
}

int deposit(struct Account *accounts, int count, int id, int amount,
            struct Transaction *log, int *log_count) {
    int idx;
    idx = find_account(accounts, count, id);
    if (idx < 0 || amount <= 0) return -1;
    accounts[idx].balance = accounts[idx].balance + amount;
    log[*log_count].from_id = 0;
    log[*log_count].to_id = id;
    log[*log_count].amount = amount;
    log[*log_count].type = 1;
    *log_count = *log_count + 1;
    return accounts[idx].balance;
}

int withdraw(struct Account *accounts, int count, int id, int amount,
             struct Transaction *log, int *log_count) {
    int idx;
    idx = find_account(accounts, count, id);
    if (idx < 0 || amount <= 0) return -1;
    if (accounts[idx].balance < amount) return -2;
    accounts[idx].balance = accounts[idx].balance - amount;
    log[*log_count].from_id = id;
    log[*log_count].to_id = 0;
    log[*log_count].amount = amount;
    log[*log_count].type = 2;
    *log_count = *log_count + 1;
    return accounts[idx].balance;
}

int transfer(struct Account *accounts, int count, int from_id, int to_id, int amount,
             struct Transaction *log, int *log_count) {
    int from_idx;
    int to_idx;
    from_idx = find_account(accounts, count, from_id);
    to_idx = find_account(accounts, count, to_id);
    if (from_idx < 0 || to_idx < 0 || amount <= 0) return -1;
    if (accounts[from_idx].balance < amount) return -2;
    accounts[from_idx].balance = accounts[from_idx].balance - amount;
    accounts[to_idx].balance = accounts[to_idx].balance + amount;
    log[*log_count].from_id = from_id;
    log[*log_count].to_id = to_id;
    log[*log_count].amount = amount;
    log[*log_count].type = 3;
    *log_count = *log_count + 1;
    return 0;
}

int total_balance(struct Account *accounts, int count) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < count) {
        if (accounts[i].active) {
            total = total + accounts[i].balance;
        }
        i = i + 1;
    }
    return total;
}

int count_transactions_for(struct Transaction *log, int log_count, int id) {
    int i;
    int c;
    c = 0;
    i = 0;
    while (i < log_count) {
        if (log[i].from_id == id || log[i].to_id == id) {
            c = c + 1;
        }
        i = i + 1;
    }
    return c;
}

int close_account(struct Account *accounts, int count, int id) {
    int idx;
    idx = find_account(accounts, count, id);
    if (idx < 0) return -1;
    accounts[idx].active = 0;
    return accounts[idx].balance;
}

int count_active(struct Account *accounts, int count) {
    int i;
    int c;
    c = 0;
    i = 0;
    while (i < count) {
        if (accounts[i].active) c = c + 1;
        i = i + 1;
    }
    return c;
}

int main(void) {
    struct Account accounts[10];
    struct Transaction log[50];
    int acct_count;
    int log_count;
    int id1;
    int id2;
    int id3;
    int result;

    acct_count = 0;
    log_count = 0;

    id1 = create_account(accounts, &acct_count, "Alice", 1000);
    id2 = create_account(accounts, &acct_count, "Bob", 500);
    id3 = create_account(accounts, &acct_count, "Charlie", 250);

    // EXPECT: 1
    printf("%d\n", id1);
    // EXPECT: 2
    printf("%d\n", id2);
    // EXPECT: 3
    printf("%d\n", id3);
    // EXPECT: 3
    printf("%d\n", acct_count);

    /* Total balance: 1000 + 500 + 250 = 1750 */
    // EXPECT: 1750
    printf("%d\n", total_balance(accounts, acct_count));

    /* Deposit 200 to Alice: 1000 + 200 = 1200 */
    result = deposit(accounts, acct_count, 1, 200, log, &log_count);
    // EXPECT: 1200
    printf("%d\n", result);

    /* Withdraw 100 from Bob: 500 - 100 = 400 */
    result = withdraw(accounts, acct_count, 2, 100, log, &log_count);
    // EXPECT: 400
    printf("%d\n", result);

    /* Transfer 300 from Alice to Charlie */
    result = transfer(accounts, acct_count, 1, 3, 300, log, &log_count);
    // EXPECT: 0
    printf("%d\n", result);

    /* Alice: 1200 - 300 = 900 */
    // EXPECT: 900
    printf("%d\n", accounts[find_account(accounts, acct_count, 1)].balance);

    /* Charlie: 250 + 300 = 550 */
    // EXPECT: 550
    printf("%d\n", accounts[find_account(accounts, acct_count, 3)].balance);

    /* Total: 900 + 400 + 550 = 1850 */
    // EXPECT: 1850
    printf("%d\n", total_balance(accounts, acct_count));

    // EXPECT: 3
    printf("%d\n", log_count);

    /* Transactions for Alice (deposit + transfer = 2) */
    // EXPECT: 2
    printf("%d\n", count_transactions_for(log, log_count, 1));

    /* Overdraw attempt: Bob has 400, try 500 */
    result = withdraw(accounts, acct_count, 2, 500, log, &log_count);
    // EXPECT: -2
    printf("%d\n", result);

    /* Close Bob's account */
    result = close_account(accounts, acct_count, 2);
    // EXPECT: 400
    printf("%d\n", result);

    // EXPECT: 2
    printf("%d\n", count_active(accounts, acct_count));

    /* Deposit to closed account fails */
    result = deposit(accounts, acct_count, 2, 100, log, &log_count);
    // EXPECT: -1
    printf("%d\n", result);

    /* Transfer from closed account fails */
    result = transfer(accounts, acct_count, 2, 1, 50, log, &log_count);
    // EXPECT: -1
    printf("%d\n", result);

    /* Active balance: 900 + 550 = 1450 */
    // EXPECT: 1450
    printf("%d\n", total_balance(accounts, acct_count));

    /* Deposit to invalid account */
    result = deposit(accounts, acct_count, 99, 100, log, &log_count);
    // EXPECT: -1
    printf("%d\n", result);

    return 0;
}
