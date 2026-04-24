int printf(const char *fmt, ...);

// Invoice system with line items, discounts, and tax

struct LineItem {
    char *description;
    int quantity;
    int unit_price_cents;
};

struct Invoice {
    int invoice_id;
    char *customer_name;
    struct LineItem items[8];
    int num_items;
    int discount_bps;
    int tax_rate_bps;
};

int line_total(struct LineItem *item) {
    return item->quantity * item->unit_price_cents;
}

int invoice_subtotal(struct Invoice *inv) {
    int total = 0;
    int i;
    for (i = 0; i < inv->num_items; i++) {
        total = total + line_total(&inv->items[i]);
    }
    return total;
}

int invoice_discount(struct Invoice *inv) {
    int sub = invoice_subtotal(inv);
    return (sub * inv->discount_bps) / 10000;
}

int invoice_after_discount(struct Invoice *inv) {
    return invoice_subtotal(inv) - invoice_discount(inv);
}

int invoice_tax(struct Invoice *inv) {
    int after_disc = invoice_after_discount(inv);
    return (after_disc * inv->tax_rate_bps) / 10000;
}

int invoice_total(struct Invoice *inv) {
    return invoice_after_discount(inv) + invoice_tax(inv);
}

void print_invoice(struct Invoice *inv) {
    int i;
    int lt;

    printf("Invoice #%d for %s\n", inv->invoice_id, inv->customer_name);
    printf("----------------------------\n");

    for (i = 0; i < inv->num_items; i++) {
        lt = line_total(&inv->items[i]);
        printf("  %s x%d @ %d = %d\n",
               inv->items[i].description,
               inv->items[i].quantity,
               inv->items[i].unit_price_cents,
               lt);
    }

    printf("Subtotal: %d\n", invoice_subtotal(inv));
    printf("Discount (%d bps): -%d\n", inv->discount_bps, invoice_discount(inv));
    printf("After discount: %d\n", invoice_after_discount(inv));
    printf("Tax (%d bps): %d\n", inv->tax_rate_bps, invoice_tax(inv));
    printf("TOTAL: %d\n", invoice_total(inv));
}

int main() {
    struct Invoice inv1;
    struct Invoice inv2;
    int sub;
    int total;

    printf("=== Invoice System ===\n");
    // EXPECT: === Invoice System ===

    // Invoice 1: Office supplies
    inv1.invoice_id = 1001;
    inv1.customer_name = "Acme Corp";
    inv1.num_items = 4;
    inv1.discount_bps = 500; // 5% discount
    inv1.tax_rate_bps = 875; // 8.75% tax

    inv1.items[0].description = "Pens";
    inv1.items[0].quantity = 100;
    inv1.items[0].unit_price_cents = 150;

    inv1.items[1].description = "Paper";
    inv1.items[1].quantity = 50;
    inv1.items[1].unit_price_cents = 899;

    inv1.items[2].description = "Stapler";
    inv1.items[2].quantity = 5;
    inv1.items[2].unit_price_cents = 1299;

    inv1.items[3].description = "Tape";
    inv1.items[3].quantity = 20;
    inv1.items[3].unit_price_cents = 349;

    print_invoice(&inv1);
    // EXPECT: Invoice #1001 for Acme Corp
    // EXPECT: ----------------------------
    // EXPECT:   Pens x100 @ 150 = 15000
    // EXPECT:   Paper x50 @ 899 = 44950
    // EXPECT:   Stapler x5 @ 1299 = 6495
    // EXPECT:   Tape x20 @ 349 = 6980
    // EXPECT: Subtotal: 73425
    // EXPECT: Discount (500 bps): -3671
    // EXPECT: After discount: 69754
    // EXPECT: Tax (875 bps): 6103
    // EXPECT: TOTAL: 75857

    printf("---\n");
    // EXPECT: ---

    // Invoice 2: Tech equipment
    inv2.invoice_id = 1002;
    inv2.customer_name = "Tech Solutions";
    inv2.num_items = 3;
    inv2.discount_bps = 1000; // 10% discount
    inv2.tax_rate_bps = 1000; // 10% tax

    inv2.items[0].description = "Laptop";
    inv2.items[0].quantity = 3;
    inv2.items[0].unit_price_cents = 99999;

    inv2.items[1].description = "Monitor";
    inv2.items[1].quantity = 6;
    inv2.items[1].unit_price_cents = 34999;

    inv2.items[2].description = "Keyboard";
    inv2.items[2].quantity = 6;
    inv2.items[2].unit_price_cents = 7999;

    print_invoice(&inv2);
    // EXPECT: Invoice #1002 for Tech Solutions
    // EXPECT: ----------------------------
    // EXPECT:   Laptop x3 @ 99999 = 299997
    // EXPECT:   Monitor x6 @ 34999 = 209994
    // EXPECT:   Keyboard x6 @ 7999 = 47994
    // EXPECT: Subtotal: 557985
    // EXPECT: Discount (1000 bps): -55798
    // EXPECT: After discount: 502187
    // EXPECT: Tax (1000 bps): 50218
    // EXPECT: TOTAL: 552405

    // Test: Compare invoices
    printf("--- Comparison ---\n");
    // EXPECT: --- Comparison ---
    int t1 = invoice_total(&inv1);
    int t2 = invoice_total(&inv2);
    printf("Invoice 1001 total: %d cents\n", t1);
    // EXPECT: Invoice 1001 total: 75857 cents
    printf("Invoice 1002 total: %d cents\n", t2);
    // EXPECT: Invoice 1002 total: 552405 cents
    printf("Combined: %d cents\n", t1 + t2);
    // EXPECT: Combined: 628262 cents

    // Test: Zero discount invoice
    inv1.discount_bps = 0;
    inv1.num_items = 1;
    sub = invoice_subtotal(&inv1);
    total = invoice_total(&inv1);
    printf("No discount subtotal: %d\n", sub);
    // EXPECT: No discount subtotal: 15000
    printf("No discount total: %d\n", total);
    // EXPECT: No discount total: 16312

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
