int printf(const char *fmt, ...);

// Break-even analysis: find point where revenue equals costs
// Supports fixed costs, variable costs per unit, and selling price per unit

struct Product {
    char *name;
    int selling_price_cents;
    int variable_cost_cents;
    int fixed_costs_cents;
};

struct BreakEvenResult {
    int break_even_units;
    int break_even_revenue;
    int contribution_margin;
    int cm_ratio_bps;
};

int contribution_margin(struct Product *p) {
    return p->selling_price_cents - p->variable_cost_cents;
}

void calculate_break_even(struct Product *p, struct BreakEvenResult *result) {
    int cm = contribution_margin(p);
    result->contribution_margin = cm;

    if (cm > 0) {
        // Break-even units = fixed costs / contribution margin per unit
        result->break_even_units = (p->fixed_costs_cents + cm - 1) / cm; // ceiling division
        result->break_even_revenue = result->break_even_units * p->selling_price_cents;
        result->cm_ratio_bps = (cm * 10000) / p->selling_price_cents;
    } else {
        result->break_even_units = -1; // impossible
        result->break_even_revenue = -1;
        result->cm_ratio_bps = 0;
    }
}

int profit_at_units(struct Product *p, int units) {
    int revenue = units * p->selling_price_cents;
    int total_cost = p->fixed_costs_cents + (units * p->variable_cost_cents);
    return revenue - total_cost;
}

int units_for_target_profit(struct Product *p, int target_profit_cents) {
    int cm = contribution_margin(p);
    if (cm <= 0) return -1;
    return (p->fixed_costs_cents + target_profit_cents + cm - 1) / cm;
}

int margin_of_safety_bps(struct Product *p, int actual_units) {
    struct BreakEvenResult be;
    int excess;
    calculate_break_even(p, &be);
    if (actual_units <= 0) return 0;
    excess = actual_units - be.break_even_units;
    if (excess < 0) return 0;
    return (excess * 10000) / actual_units;
}

void print_analysis(struct Product *p, struct BreakEvenResult *r) {
    printf("Product: %s\n", p->name);
    printf("  Price: %d cents, Variable cost: %d cents\n",
           p->selling_price_cents, p->variable_cost_cents);
    printf("  Fixed costs: %d cents\n", p->fixed_costs_cents);
    printf("  Contribution margin: %d cents\n", r->contribution_margin);
    printf("  CM ratio: %d bps\n", r->cm_ratio_bps);
    printf("  Break-even units: %d\n", r->break_even_units);
    printf("  Break-even revenue: %d cents\n", r->break_even_revenue);
}

int main() {
    struct Product p1;
    struct Product p2;
    struct Product p3;
    struct BreakEvenResult r;
    int profit;
    int units;
    int mos;
    int i;

    printf("=== Break-Even Analysis ===\n");
    // EXPECT: === Break-Even Analysis ===

    // Product 1: Widget
    p1.name = "Widget";
    p1.selling_price_cents = 2500;    // $25.00
    p1.variable_cost_cents = 1500;    // $15.00
    p1.fixed_costs_cents = 5000000;   // $50,000

    calculate_break_even(&p1, &r);
    print_analysis(&p1, &r);
    // EXPECT: Product: Widget
    // EXPECT:   Price: 2500 cents, Variable cost: 1500 cents
    // EXPECT:   Fixed costs: 5000000 cents
    // EXPECT:   Contribution margin: 1000 cents
    // EXPECT:   CM ratio: 4000 bps
    // EXPECT:   Break-even units: 5000
    // EXPECT:   Break-even revenue: 12500000 cents

    // Profit at various volumes
    printf("--- Profit Analysis ---\n");
    // EXPECT: --- Profit Analysis ---
    int volumes[5];
    volumes[0] = 3000;
    volumes[1] = 5000;
    volumes[2] = 7000;
    volumes[3] = 10000;
    volumes[4] = 15000;
    for (i = 0; i < 5; i++) {
        profit = profit_at_units(&p1, volumes[i]);
        printf("  %d units: profit=%d cents\n", volumes[i], profit);
    }
    // EXPECT:   3000 units: profit=-2000000 cents
    // EXPECT:   5000 units: profit=0 cents
    // EXPECT:   7000 units: profit=2000000 cents
    // EXPECT:   10000 units: profit=5000000 cents
    // EXPECT:   15000 units: profit=10000000 cents

    // Target profit
    units = units_for_target_profit(&p1, 2000000);
    printf("Units for $20k profit: %d\n", units);
    // EXPECT: Units for $20k profit: 7000

    // Margin of safety at 8000 units
    mos = margin_of_safety_bps(&p1, 8000);
    printf("Margin of safety at 8000 units: %d bps\n", mos);
    // EXPECT: Margin of safety at 8000 units: 3750 bps

    printf("---\n");
    // EXPECT: ---

    // Product 2: Premium gadget
    p2.name = "Gadget";
    p2.selling_price_cents = 9999;
    p2.variable_cost_cents = 4500;
    p2.fixed_costs_cents = 20000000;  // $200,000

    calculate_break_even(&p2, &r);
    print_analysis(&p2, &r);
    // EXPECT: Product: Gadget
    // EXPECT:   Price: 9999 cents, Variable cost: 4500 cents
    // EXPECT:   Fixed costs: 20000000 cents
    // EXPECT:   Contribution margin: 5499 cents
    // EXPECT:   CM ratio: 5499 bps
    // EXPECT:   Break-even units: 3638
    // EXPECT:   Break-even revenue: 36376362 cents

    profit = profit_at_units(&p2, 5000);
    printf("Gadget at 5000 units: %d cents profit\n", profit);
    // EXPECT: Gadget at 5000 units: 7495000 cents profit

    printf("---\n");
    // EXPECT: ---

    // Product 3: Low margin item
    p3.name = "BasicItem";
    p3.selling_price_cents = 500;
    p3.variable_cost_cents = 420;
    p3.fixed_costs_cents = 1000000;

    calculate_break_even(&p3, &r);
    print_analysis(&p3, &r);
    // EXPECT: Product: BasicItem
    // EXPECT:   Price: 500 cents, Variable cost: 420 cents
    // EXPECT:   Fixed costs: 1000000 cents
    // EXPECT:   Contribution margin: 80 cents
    // EXPECT:   CM ratio: 1600 bps
    // EXPECT:   Break-even units: 12500
    // EXPECT:   Break-even revenue: 6250000 cents

    // Scenario: what if we raise price by 50 cents?
    printf("--- Price Increase Scenario ---\n");
    // EXPECT: --- Price Increase Scenario ---
    p3.selling_price_cents = 550;
    calculate_break_even(&p3, &r);
    printf("New price 550: BE units=%d, CM=%d\n", r.break_even_units, r.contribution_margin);
    // EXPECT: New price 550: BE units=7693, CM=130

    // Scenario: what if variable cost drops by 20 cents?
    p3.selling_price_cents = 500;
    p3.variable_cost_cents = 400;
    calculate_break_even(&p3, &r);
    printf("Lower VC 400: BE units=%d, CM=%d\n", r.break_even_units, r.contribution_margin);
    // EXPECT: Lower VC 400: BE units=10000, CM=100

    // Compare all three products
    printf("--- Comparison ---\n");
    // EXPECT: --- Comparison ---
    p3.variable_cost_cents = 420;
    struct Product *products[3];
    products[0] = &p1;
    products[1] = &p2;
    products[2] = &p3;
    for (i = 0; i < 3; i++) {
        calculate_break_even(products[i], &r);
        printf("%s: BE=%d units, CM ratio=%d bps\n",
               products[i]->name, r.break_even_units, r.cm_ratio_bps);
    }
    // EXPECT: Widget: BE=5000 units, CM ratio=4000 bps
    // EXPECT: Gadget: BE=3638 units, CM ratio=5499 bps
    // EXPECT: BasicItem: BE=12500 units, CM ratio=1600 bps

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
