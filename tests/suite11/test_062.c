int printf(const char *fmt, ...);

// Loan amortization calculator using integer cents
// Monthly payment = P * r * (1+r)^n / ((1+r)^n - 1)
// Simplified with integer math using basis points

struct Loan {
    int principal_cents;
    int annual_rate_bps;
    int term_months;
};

struct Payment {
    int month;
    int payment_cents;
    int interest_cents;
    int principal_cents;
    int balance_cents;
};

// Calculate monthly payment using iterative approach
// Instead of the formula, we use a binary search for the correct payment
int calculate_monthly_payment(struct Loan *loan) {
    int monthly_rate_bps = loan->annual_rate_bps / 12;
    int low = loan->principal_cents / loan->term_months;
    int high = low * 3;
    int mid;
    int balance;
    int interest;
    int i;
    int found;

    found = 0;
    while (low <= high && !found) {
        mid = (low + high) / 2;
        balance = loan->principal_cents;

        for (i = 0; i < loan->term_months; i++) {
            interest = (balance * monthly_rate_bps) / 10000;
            balance = balance - (mid - interest);
            if (balance < -mid) break;
        }

        if (balance > mid) {
            low = mid + 1;
        } else if (balance < -mid) {
            high = mid - 1;
        } else {
            found = 1;
        }
    }
    return mid;
}

void compute_schedule(struct Loan *loan, int monthly_payment, int num_months) {
    int balance = loan->principal_cents;
    int monthly_rate_bps = loan->annual_rate_bps / 12;
    int interest;
    int princ;
    int total_interest;
    int total_principal;
    int i;

    total_interest = 0;
    total_principal = 0;

    for (i = 1; i <= num_months && balance > 0; i++) {
        interest = (balance * monthly_rate_bps) / 10000;
        princ = monthly_payment - interest;
        if (princ > balance) {
            princ = balance;
        }
        balance = balance - princ;
        total_interest = total_interest + interest;
        total_principal = total_principal + princ;

        printf("Month %d: Pay=%d Int=%d Princ=%d Bal=%d\n",
               i, monthly_payment, interest, princ, balance);
    }

    printf("Total interest: %d cents\n", total_interest);
    printf("Total principal: %d cents\n", total_principal);

}

int main() {
    struct Loan loan1;
    struct Loan loan2;
    struct Loan loan3;
    int payment;
    int balance;
    int interest;
    int i;
    int monthly_rate_bps;

    // Test 1: $1000 loan at 5% for 12 months
    loan1.principal_cents = 100000;
    loan1.annual_rate_bps = 500;
    loan1.term_months = 12;
    payment = calculate_monthly_payment(&loan1);
    printf("Loan 1: $1000 at 5%% for 12 months\n");
    // EXPECT: Loan 1: $1000 at 5% for 12 months
    printf("Monthly payment: %d cents\n", payment);
    // EXPECT: Monthly payment: 8852 cents

    // Test 2: Show first 3 months of amortization
    printf("--- Amortization Schedule ---\n");
    // EXPECT: --- Amortization Schedule ---
    compute_schedule(&loan1, payment, 3);
    // EXPECT: Month 1: Pay=8852 Int=410 Princ=8442 Bal=91558
    // EXPECT: Month 2: Pay=8852 Int=375 Princ=8477 Bal=83081
    // EXPECT: Month 3: Pay=8852 Int=340 Princ=8512 Bal=74569
    // EXPECT: Total interest: 1125 cents
    // EXPECT: Total principal: 25431 cents

    // Test 3: Small loan $500 at 10% for 6 months
    loan2.principal_cents = 50000;
    loan2.annual_rate_bps = 1000;
    loan2.term_months = 6;
    payment = calculate_monthly_payment(&loan2);
    printf("Loan 2: $500 at 10%% for 6 months\n");
    // EXPECT: Loan 2: $500 at 10% for 6 months
    printf("Monthly payment: %d cents\n", payment);
    // EXPECT: Monthly payment: 9373 cents

    // Test 4: Verify total repayment
    balance = loan2.principal_cents;
    monthly_rate_bps = loan2.annual_rate_bps / 12;
    int total_paid = 0;
    for (i = 0; i < loan2.term_months; i++) {
        interest = (balance * monthly_rate_bps) / 10000;
        int princ = payment - interest;
        if (princ > balance) princ = balance;
        balance = balance - princ;
        total_paid = total_paid + payment;
    }
    printf("Total paid: %d cents\n", total_paid);
    // EXPECT: Total paid: 56238 cents
    printf("Total interest paid: %d cents\n", total_paid - loan2.principal_cents);
    // EXPECT: Total interest paid: 6238 cents

    // Test 5: Large loan $10000 at 3% for 24 months
    loan3.principal_cents = 1000000;
    loan3.annual_rate_bps = 300;
    loan3.term_months = 24;
    payment = calculate_monthly_payment(&loan3);
    printf("Loan 3: $10000 at 3%% for 24 months\n");
    // EXPECT: Loan 3: $10000 at 3% for 24 months
    printf("Monthly payment: %d cents\n", payment);
    // EXPECT: Monthly payment: 44269 cents

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
