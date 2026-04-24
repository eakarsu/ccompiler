int printf(const char *fmt, ...);

// Payroll calculator with tax withholding, deductions, and overtime

enum PayType { HOURLY, SALARIED };

struct Employee {
    int id;
    char *name;
    int pay_type;
    int base_rate_cents;   // hourly rate or annual salary in cents
    int hours_worked;      // for hourly employees (in tenths of hours)
    int overtime_hours;    // in tenths of hours
    int health_deduction;  // monthly health insurance cents
    int retirement_pct;    // retirement contribution in bps
    int tax_bracket_bps;   // simplified flat tax rate
};

struct PayStub {
    int gross_pay;
    int overtime_pay;
    int health_ded;
    int retirement_ded;
    int tax_withheld;
    int net_pay;
};

void calculate_pay(struct Employee *emp, struct PayStub *stub) {
    int regular_pay;
    int ot_pay;
    int gross;
    int pre_tax_deductions;
    int taxable;

    if (emp->pay_type == HOURLY) {
        // Regular pay: hours * rate (hours in tenths)
        regular_pay = (emp->hours_worked * emp->base_rate_cents) / 10;
        // Overtime at 1.5x
        ot_pay = (emp->overtime_hours * emp->base_rate_cents * 15) / 100;
        gross = regular_pay + ot_pay;
    } else {
        // Salaried: annual / 24 for semi-monthly
        regular_pay = emp->base_rate_cents / 24;
        ot_pay = 0;
        gross = regular_pay;
    }

    stub->gross_pay = gross;
    stub->overtime_pay = ot_pay;
    stub->health_ded = emp->health_deduction;

    // Retirement is pre-tax
    stub->retirement_ded = (gross * emp->retirement_pct) / 10000;

    pre_tax_deductions = stub->health_ded + stub->retirement_ded;
    taxable = gross - pre_tax_deductions;
    if (taxable < 0) taxable = 0;

    stub->tax_withheld = (taxable * emp->tax_bracket_bps) / 10000;
    stub->net_pay = gross - pre_tax_deductions - stub->tax_withheld;
}

void print_paystub(struct Employee *emp, struct PayStub *stub) {
    printf("--- Pay Stub: %s (#%d) ---\n", emp->name, emp->id);
    printf("Gross pay: %d cents\n", stub->gross_pay);
    if (stub->overtime_pay > 0) {
        printf("  (includes OT: %d cents)\n", stub->overtime_pay);
    }
    printf("Health insurance: -%d cents\n", stub->health_ded);
    printf("Retirement (%d bps): -%d cents\n", emp->retirement_pct, stub->retirement_ded);
    printf("Tax withheld (%d bps): -%d cents\n", emp->tax_bracket_bps, stub->tax_withheld);
    printf("Net pay: %d cents\n", stub->net_pay);
}

int main() {
    struct Employee emp1;
    struct Employee emp2;
    struct Employee emp3;
    struct PayStub stub;
    int total_payroll;
    int total_tax;

    printf("=== Payroll Calculator ===\n");
    // EXPECT: === Payroll Calculator ===

    // Employee 1: Hourly worker with overtime
    emp1.id = 101;
    emp1.name = "Alice";
    emp1.pay_type = HOURLY;
    emp1.base_rate_cents = 2500;  // $25/hour
    emp1.hours_worked = 400;      // 40.0 hours
    emp1.overtime_hours = 80;     // 8.0 hours overtime
    emp1.health_deduction = 15000;
    emp1.retirement_pct = 600;    // 6%
    emp1.tax_bracket_bps = 2200;  // 22%

    calculate_pay(&emp1, &stub);
    print_paystub(&emp1, &stub);
    // EXPECT: --- Pay Stub: Alice (#101) ---
    // EXPECT: Gross pay: 130000 cents
    // EXPECT:   (includes OT: 30000 cents)
    // EXPECT: Health insurance: -15000 cents
    // EXPECT: Retirement (600 bps): -7800 cents
    // EXPECT: Tax withheld (2200 bps): -23584 cents
    // EXPECT: Net pay: 83616 cents

    printf("---\n");
    // EXPECT: ---

    // Employee 2: Salaried employee
    emp2.id = 102;
    emp2.name = "Bob";
    emp2.pay_type = SALARIED;
    emp2.base_rate_cents = 7500000;  // $75,000/year
    emp2.hours_worked = 0;
    emp2.overtime_hours = 0;
    emp2.health_deduction = 25000;
    emp2.retirement_pct = 800;     // 8%
    emp2.tax_bracket_bps = 2400;   // 24%

    calculate_pay(&emp2, &stub);
    print_paystub(&emp2, &stub);
    // EXPECT: --- Pay Stub: Bob (#102) ---
    // EXPECT: Gross pay: 312500 cents
    // EXPECT: Health insurance: -25000 cents
    // EXPECT: Retirement (800 bps): -25000 cents
    // EXPECT: Tax withheld (2400 bps): -63000 cents
    // EXPECT: Net pay: 199500 cents

    printf("---\n");
    // EXPECT: ---

    // Employee 3: Hourly, no overtime
    emp3.id = 103;
    emp3.name = "Carol";
    emp3.pay_type = HOURLY;
    emp3.base_rate_cents = 1800;   // $18/hour
    emp3.hours_worked = 350;       // 35.0 hours
    emp3.overtime_hours = 0;
    emp3.health_deduction = 10000;
    emp3.retirement_pct = 400;     // 4%
    emp3.tax_bracket_bps = 1200;   // 12%

    calculate_pay(&emp3, &stub);
    print_paystub(&emp3, &stub);
    // EXPECT: --- Pay Stub: Carol (#103) ---
    // EXPECT: Gross pay: 63000 cents
    // EXPECT: Health insurance: -10000 cents
    // EXPECT: Retirement (400 bps): -2520 cents
    // EXPECT: Tax withheld (1200 bps): -6057 cents
    // EXPECT: Net pay: 44423 cents

    // Payroll summary
    printf("--- Payroll Summary ---\n");
    // EXPECT: --- Payroll Summary ---

    struct PayStub stubs[3];
    calculate_pay(&emp1, &stubs[0]);
    calculate_pay(&emp2, &stubs[1]);
    calculate_pay(&emp3, &stubs[2]);

    total_payroll = 0;
    total_tax = 0;
    int total_health = 0;
    int total_retire = 0;
    int i;
    for (i = 0; i < 3; i++) {
        total_payroll = total_payroll + stubs[i].gross_pay;
        total_tax = total_tax + stubs[i].tax_withheld;
        total_health = total_health + stubs[i].health_ded;
        total_retire = total_retire + stubs[i].retirement_ded;
    }

    printf("Total gross payroll: %d cents\n", total_payroll);
    // EXPECT: Total gross payroll: 505500 cents
    printf("Total tax withheld: %d cents\n", total_tax);
    // EXPECT: Total tax withheld: 92641 cents
    printf("Total health: %d cents\n", total_health);
    // EXPECT: Total health: 50000 cents
    printf("Total retirement: %d cents\n", total_retire);
    // EXPECT: Total retirement: 35320 cents

    int total_net = 0;
    for (i = 0; i < 3; i++) {
        total_net = total_net + stubs[i].net_pay;
    }
    printf("Total net payroll: %d cents\n", total_net);
    // EXPECT: Total net payroll: 327539 cents

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
