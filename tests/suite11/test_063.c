int printf(const char *fmt, ...);

// Tax bracket calculator using integer cents
// Progressive tax system with multiple brackets

struct TaxBracket {
    int upper_limit_cents;
    int rate_bps;
};

struct TaxResult {
    int taxable_income;
    int total_tax;
    int effective_rate_bps;
};

// US-like simplified tax brackets (in cents):
// 0 - 1000000 (0-$10k): 10%
// 1000000 - 4000000 ($10k-$40k): 12%
// 4000000 - 8500000 ($40k-$85k): 22%
// 8500000 - 16300000 ($85k-$163k): 24%
// 16300000+ : 32%

int bracket_limits[5];
int bracket_rates[5];

void init_brackets() {
    bracket_limits[0] = 1000000;
    bracket_limits[1] = 4000000;
    bracket_limits[2] = 8500000;
    bracket_limits[3] = 16300000;
    bracket_limits[4] = 99999999;

    bracket_rates[0] = 1000;
    bracket_rates[1] = 1200;
    bracket_rates[2] = 2200;
    bracket_rates[3] = 2400;
    bracket_rates[4] = 3200;
}

// Safe multiply to avoid overflow: (amount * rate) / 10000
// Split: amount = (amount/10000)*10000 + (amount%10000)
int safe_tax_calc(int amount, int rate) {
    int hi = (amount / 10000) * rate;
    int lo = (amount % 10000) * rate / 10000;
    return hi + lo;
}

void calculate_tax(int income_cents, struct TaxResult *result) {
    int remaining = income_cents;
    int tax = 0;
    int prev_limit = 0;
    int bracket_amount;
    int bracket_tax;
    int i;

    result->taxable_income = income_cents;

    for (i = 0; i < 5 && remaining > 0; i++) {
        bracket_amount = bracket_limits[i] - prev_limit;
        if (remaining < bracket_amount) {
            bracket_amount = remaining;
        }
        bracket_tax = safe_tax_calc(bracket_amount, bracket_rates[i]);
        tax = tax + bracket_tax;
        remaining = remaining - bracket_amount;
        prev_limit = bracket_limits[i];
    }

    result->total_tax = tax;
    if (income_cents > 0) {
        result->effective_rate_bps = (tax / (income_cents / 10000));
    } else {
        result->effective_rate_bps = 0;
    }
}

void print_result(struct TaxResult *r) {
    printf("Income: %d cents, Tax: %d cents, Effective rate: %d bps\n",
           r->taxable_income, r->total_tax, r->effective_rate_bps);
}

int calculate_deduction(int income_cents, int deduction_cents) {
    if (deduction_cents > income_cents) return 0;
    return income_cents - deduction_cents;
}

int main() {
    struct TaxResult r;
    int income;
    int after_deduction;
    int marginal;
    int i;

    init_brackets();

    // Test 1: Income in first bracket ($5000 = 500000 cents)
    printf("=== Tax Bracket Calculator ===\n");
    // EXPECT: === Tax Bracket Calculator ===
    calculate_tax(500000, &r);
    print_result(&r);
    // EXPECT: Income: 500000 cents, Tax: 50000 cents, Effective rate: 1000 bps

    // Test 2: Income spanning two brackets ($25000)
    calculate_tax(2500000, &r);
    print_result(&r);
    // EXPECT: Income: 2500000 cents, Tax: 280000 cents, Effective rate: 1120 bps

    // Test 3: Income spanning three brackets ($50000)
    calculate_tax(5000000, &r);
    print_result(&r);
    // EXPECT: Income: 5000000 cents, Tax: 680000 cents, Effective rate: 1360 bps

    // Test 4: High income ($100000)
    calculate_tax(10000000, &r);
    print_result(&r);
    // EXPECT: Income: 10000000 cents, Tax: 1810000 cents, Effective rate: 1810 bps

    // Test 5: Very high income ($200000)
    calculate_tax(20000000, &r);
    print_result(&r);
    // EXPECT: Income: 20000000 cents, Tax: 4506000 cents, Effective rate: 2253 bps

    // Test 6: Zero income
    calculate_tax(0, &r);
    printf("Zero income tax: %d\n", r.total_tax);
    // EXPECT: Zero income tax: 0

    // Test 7: Exactly at bracket boundary ($10000)
    calculate_tax(1000000, &r);
    printf("At first bracket limit: tax=%d\n", r.total_tax);
    // EXPECT: At first bracket limit: tax=100000

    // Test 8: With standard deduction
    printf("--- With Deductions ---\n");
    // EXPECT: --- With Deductions ---
    income = 7500000; // $75000
    after_deduction = calculate_deduction(income, 1290000); // $12900 standard deduction
    printf("Gross income: %d cents\n", income);
    // EXPECT: Gross income: 7500000 cents
    printf("After deduction: %d cents\n", after_deduction);
    // EXPECT: After deduction: 6210000 cents
    calculate_tax(after_deduction, &r);
    printf("Tax after deduction: %d cents\n", r.total_tax);
    // EXPECT: Tax after deduction: 946200 cents

    // Test 9: Marginal vs effective comparison
    printf("--- Marginal vs Effective ---\n");
    // EXPECT: --- Marginal vs Effective ---
    int incomes[4];
    incomes[0] = 500000;
    incomes[1] = 2000000;
    incomes[2] = 5000000;
    incomes[3] = 10000000;
    for (i = 0; i < 4; i++) {
        calculate_tax(incomes[i], &r);
        // Find marginal bracket
        marginal = 0;
        int prev = 0;
        int j;
        for (j = 0; j < 5; j++) {
            if (incomes[i] <= bracket_limits[j]) {
                marginal = bracket_rates[j];
                break;
            }
        }
        printf("Income %d: marginal=%d eff=%d\n",
               incomes[i], marginal, r.effective_rate_bps);
    }
    // EXPECT: Income 500000: marginal=1000 eff=1000
    // EXPECT: Income 2000000: marginal=1200 eff=1100
    // EXPECT: Income 5000000: marginal=2200 eff=1360
    // EXPECT: Income 10000000: marginal=2400 eff=1810

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
