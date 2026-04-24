int printf(const char *fmt, ...);

// Stock portfolio tracker using integer cents
// Tracks buy/sell transactions, computes gains/losses

enum TxnType { BUY, SELL };

struct Transaction {
    int stock_id;
    int type;
    int quantity;
    int price_cents;
};

struct Holding {
    int stock_id;
    int quantity;
    int avg_cost_cents;
    int total_cost_cents;
};

struct Portfolio {
    struct Holding holdings[5];
    int num_holdings;
    int cash_cents;
    int realized_gain;
};

int find_holding(struct Portfolio *p, int stock_id) {
    int i;
    for (i = 0; i < p->num_holdings; i++) {
        if (p->holdings[i].stock_id == stock_id) return i;
    }
    return -1;
}

void process_buy(struct Portfolio *p, struct Transaction *txn) {
    int idx = find_holding(p, txn->stock_id);
    int cost = txn->quantity * txn->price_cents;

    if (idx < 0) {
        idx = p->num_holdings;
        p->num_holdings = p->num_holdings + 1;
        p->holdings[idx].stock_id = txn->stock_id;
        p->holdings[idx].quantity = 0;
        p->holdings[idx].total_cost_cents = 0;
    }

    p->holdings[idx].quantity = p->holdings[idx].quantity + txn->quantity;
    p->holdings[idx].total_cost_cents = p->holdings[idx].total_cost_cents + cost;
    p->holdings[idx].avg_cost_cents = p->holdings[idx].total_cost_cents / p->holdings[idx].quantity;
    p->cash_cents = p->cash_cents - cost;
}

void process_sell(struct Portfolio *p, struct Transaction *txn) {
    int idx = find_holding(p, txn->stock_id);
    int revenue;
    int cost_basis;
    int gain;

    if (idx < 0) return;

    revenue = txn->quantity * txn->price_cents;
    cost_basis = txn->quantity * p->holdings[idx].avg_cost_cents;
    gain = revenue - cost_basis;

    p->holdings[idx].quantity = p->holdings[idx].quantity - txn->quantity;
    p->holdings[idx].total_cost_cents = p->holdings[idx].total_cost_cents - cost_basis;
    p->cash_cents = p->cash_cents + revenue;
    p->realized_gain = p->realized_gain + gain;
}

int portfolio_value(struct Portfolio *p, int *prices) {
    int total = p->cash_cents;
    int i;
    for (i = 0; i < p->num_holdings; i++) {
        total = total + p->holdings[i].quantity * prices[p->holdings[i].stock_id];
    }
    return total;
}

int unrealized_gain(struct Portfolio *p, int *prices) {
    int gain = 0;
    int i;
    int market_val;
    for (i = 0; i < p->num_holdings; i++) {
        market_val = p->holdings[i].quantity * prices[p->holdings[i].stock_id];
        gain = gain + (market_val - p->holdings[i].total_cost_cents);
    }
    return gain;
}

char *stock_name(int id) {
    if (id == 0) return "AAPL";
    if (id == 1) return "GOOG";
    if (id == 2) return "MSFT";
    return "UNKN";
}

int main() {
    struct Portfolio port;
    struct Transaction txn;
    int current_prices[3];
    int total;
    int i;

    // Initialize portfolio with $100,000 cash
    port.num_holdings = 0;
    port.cash_cents = 10000000;
    port.realized_gain = 0;

    printf("=== Stock Portfolio Tracker ===\n");
    // EXPECT: === Stock Portfolio Tracker ===
    printf("Starting cash: %d cents\n", port.cash_cents);
    // EXPECT: Starting cash: 10000000 cents

    // Buy 10 shares AAPL at $150 (15000 cents)
    txn.stock_id = 0; txn.type = BUY; txn.quantity = 10; txn.price_cents = 15000;
    process_buy(&port, &txn);
    printf("Buy 10 AAPL at 15000: cash=%d\n", port.cash_cents);
    // EXPECT: Buy 10 AAPL at 15000: cash=9850000

    // Buy 5 shares GOOG at $280 (28000 cents)
    txn.stock_id = 1; txn.type = BUY; txn.quantity = 5; txn.price_cents = 28000;
    process_buy(&port, &txn);
    printf("Buy 5 GOOG at 28000: cash=%d\n", port.cash_cents);
    // EXPECT: Buy 5 GOOG at 28000: cash=9710000

    // Buy 15 shares MSFT at $100 (10000 cents)
    txn.stock_id = 2; txn.type = BUY; txn.quantity = 15; txn.price_cents = 10000;
    process_buy(&port, &txn);
    printf("Buy 15 MSFT at 10000: cash=%d\n", port.cash_cents);
    // EXPECT: Buy 15 MSFT at 10000: cash=9560000

    // Print holdings
    printf("--- Holdings ---\n");
    // EXPECT: --- Holdings ---
    for (i = 0; i < port.num_holdings; i++) {
        printf("%s: qty=%d avg_cost=%d total=%d\n",
               stock_name(port.holdings[i].stock_id),
               port.holdings[i].quantity,
               port.holdings[i].avg_cost_cents,
               port.holdings[i].total_cost_cents);
    }
    // EXPECT: AAPL: qty=10 avg_cost=15000 total=150000
    // EXPECT: GOOG: qty=5 avg_cost=28000 total=140000
    // EXPECT: MSFT: qty=15 avg_cost=10000 total=150000

    // Buy 5 more AAPL at $160 (dollar cost averaging)
    txn.stock_id = 0; txn.type = BUY; txn.quantity = 5; txn.price_cents = 16000;
    process_buy(&port, &txn);
    printf("Buy 5 more AAPL at 16000: avg=%d\n", port.holdings[0].avg_cost_cents);
    // EXPECT: Buy 5 more AAPL at 16000: avg=15333

    // Set current market prices
    current_prices[0] = 17000; // AAPL up
    current_prices[1] = 27000; // GOOG down
    current_prices[2] = 11000; // MSFT up

    // Portfolio value
    total = portfolio_value(&port, current_prices);
    printf("Portfolio value: %d cents\n", total);
    // EXPECT: Portfolio value: 10035000 cents

    // Unrealized gain
    int ugain = unrealized_gain(&port, current_prices);
    printf("Unrealized gain: %d cents\n", ugain);
    // EXPECT: Unrealized gain: 35000 cents

    // Sell 5 AAPL at $170 (profit)
    txn.stock_id = 0; txn.type = SELL; txn.quantity = 5; txn.price_cents = 17000;
    process_sell(&port, &txn);
    printf("Sell 5 AAPL: realized=%d cash=%d\n", port.realized_gain, port.cash_cents);
    // EXPECT: Sell 5 AAPL: realized=8335 cash=9565000

    // Sell 3 GOOG at $270 (loss)
    txn.stock_id = 1; txn.type = SELL; txn.quantity = 3; txn.price_cents = 27000;
    process_sell(&port, &txn);
    printf("Sell 3 GOOG: realized=%d cash=%d\n", port.realized_gain, port.cash_cents);
    // EXPECT: Sell 3 GOOG: realized=5335 cash=9646000

    // Final summary
    printf("--- Final Summary ---\n");
    // EXPECT: --- Final Summary ---
    printf("Cash: %d cents\n", port.cash_cents);
    // EXPECT: Cash: 9646000 cents
    printf("Realized gain: %d cents\n", port.realized_gain);
    // EXPECT: Realized gain: 5335 cents
    for (i = 0; i < port.num_holdings; i++) {
        if (port.holdings[i].quantity > 0) {
            printf("%s: %d shares\n",
                   stock_name(port.holdings[i].stock_id),
                   port.holdings[i].quantity);
        }
    }
    // EXPECT: AAPL: 10 shares
    // EXPECT: GOOG: 2 shares
    // EXPECT: MSFT: 15 shares

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
