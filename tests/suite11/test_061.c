int printf(const char *fmt, ...);

// Compound interest calculator using integer cents
// Formula: A = P * (1 + r/n)^(nt) computed with integer arithmetic
// We use basis points (1/10000) for rates to avoid floating point

struct Account {
    int principal_cents;
    int rate_bps;
    int compounds_per_year;
    int years;
};

// Multiply amount by (1 + rate_bps / (compounds * 10000))
// To keep precision, we do: amount = amount * (base + rate_per_period) / base
// where base = compounds_per_year * 10000
int compute_compound_interest(struct Account *acct) {
    int amount = acct->principal_cents;
    int base = acct->compounds_per_year * 10000;
    int rate_per_period = acct->rate_bps;
    int total_periods = acct->compounds_per_year * acct->years;
    int i;

    for (i = 0; i < total_periods; i++) {
        amount = amount + (amount * rate_per_period) / base;
    }
    return amount;
}

int cents_to_dollars(int cents) {
    if (cents < 0) {
        cents = -cents;
        return -(cents / 100);
    }
    return cents / 100;
}

int cents_to_remainder(int cents) {
    if (cents < 0) cents = -cents;
    return cents % 100;
}

void print_money(int cents) {
    int dollars = cents_to_dollars(cents);
    int rem = cents_to_remainder(cents);
    if (rem < 10) {
        printf("%d.0%d", dollars, rem);
    } else {
        printf("%d.%d", dollars, rem);
    }
}

int main() {
    struct Account a1;
    struct Account a2;
    struct Account a3;
    struct Account a4;
    int result;
    int interest;
    int dollars;
    int rem;

    // Test 1: $1000 at 5% annual, compounded yearly for 1 year
    a1.principal_cents = 100000;
    a1.rate_bps = 500;
    a1.compounds_per_year = 1;
    a1.years = 1;
    result = compute_compound_interest(&a1);
    printf("Test 1: Principal 1000, Rate 5%%, 1 year\n");
    // EXPECT: Test 1: Principal 1000, Rate 5%, 1 year
    interest = result - a1.principal_cents;
    printf("Final: %d cents, Interest: %d cents\n", result, interest);
    // EXPECT: Final: 105000 cents, Interest: 5000 cents

    // Test 2: $1000 at 5% compounded quarterly for 1 year
    a2.principal_cents = 100000;
    a2.rate_bps = 500;
    a2.compounds_per_year = 4;
    a2.years = 1;
    result = compute_compound_interest(&a2);
    printf("Test 2: Quarterly compounding\n");
    // EXPECT: Test 2: Quarterly compounding
    printf("Final: %d cents\n", result);
    // EXPECT: Final: 105093 cents

    // Test 3: $5000 at 8% compounded monthly for 3 years
    a3.principal_cents = 500000;
    a3.rate_bps = 800;
    a3.compounds_per_year = 12;
    a3.years = 3;
    result = compute_compound_interest(&a3);
    printf("Test 3: Monthly compounding 3 years\n");
    // EXPECT: Test 3: Monthly compounding 3 years
    printf("Final: %d cents\n", result);
    // EXPECT: Final: 635097 cents
    interest = result - a3.principal_cents;
    printf("Interest earned: %d cents\n", interest);
    // EXPECT: Interest earned: 135097 cents

    // Test 4: Money formatting
    printf("Format test: ");
    print_money(12345);
    printf("\n");
    // EXPECT: Format test: 123.45
    print_money(100);
    printf("\n");
    // EXPECT: 1.00
    print_money(99);
    printf("\n");
    // EXPECT: 0.99
    print_money(5);
    printf("\n");
    // EXPECT: 0.05

    // Test 5: Multiple accounts comparison
    printf("--- Account Comparison ---\n");
    // EXPECT: --- Account Comparison ---
    a4.principal_cents = 200000;
    a4.rate_bps = 300;
    a4.compounds_per_year = 1;
    a4.years = 5;
    result = compute_compound_interest(&a4);
    printf("3%% for 5 years: %d cents\n", result);
    // EXPECT: 3% for 5 years: 231854 cents

    a4.rate_bps = 700;
    result = compute_compound_interest(&a4);
    printf("7%% for 5 years: %d cents\n", result);
    // EXPECT: 7% for 5 years: 280509 cents

    a4.rate_bps = 1000;
    result = compute_compound_interest(&a4);
    printf("10%% for 5 years: %d cents\n", result);
    // EXPECT: 10% for 5 years: 322102 cents

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
