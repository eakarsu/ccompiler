int printf(const char *fmt, ...);

// Currency converter with fixed exchange rates
// All amounts in integer cents of their respective currency
// Exchange rates stored as ratio: multiply by numerator, divide by denominator

struct ExchangeRate {
    int from_id;
    int to_id;
    int numerator;
    int denominator;
};

struct CurrencyInfo {
    char *name;
    char *symbol;
};

int convert(int amount_cents, struct ExchangeRate *rate) {
    // Use long-style multiplication to avoid overflow for small amounts
    // amount * numerator / denominator
    int result = (amount_cents / rate->denominator) * rate->numerator;
    int remainder = amount_cents % rate->denominator;
    result = result + (remainder * rate->numerator) / rate->denominator;
    return result;
}

int convert_chain(int amount, struct ExchangeRate *r1, struct ExchangeRate *r2) {
    int mid = convert(amount, r1);
    return convert(mid, r2);
}

void print_amount(int cents) {
    int whole = cents / 100;
    int frac = cents % 100;
    if (frac < 10) {
        printf("%d.0%d", whole, frac);
    } else {
        printf("%d.%d", whole, frac);
    }
}

int main() {
    struct CurrencyInfo currencies[5];
    struct ExchangeRate rates[6];
    int amount;
    int result;
    int i;

    // Currency IDs: 0=USD, 1=EUR, 2=GBP, 3=JPY (in yen*100), 4=CAD
    currencies[0].name = "USD";
    currencies[0].symbol = "$";
    currencies[1].name = "EUR";
    currencies[1].symbol = "E";
    currencies[2].name = "GBP";
    currencies[2].symbol = "L";
    currencies[3].name = "JPY";
    currencies[3].symbol = "Y";
    currencies[4].name = "CAD";
    currencies[4].symbol = "C$";

    // Exchange rates (approximate fixed rates)
    // USD -> EUR: 0.92 (multiply by 92, divide by 100)
    rates[0].from_id = 0; rates[0].to_id = 1;
    rates[0].numerator = 92; rates[0].denominator = 100;

    // USD -> GBP: 0.79
    rates[1].from_id = 0; rates[1].to_id = 2;
    rates[1].numerator = 79; rates[1].denominator = 100;

    // USD -> JPY: 150.0 (stored as cents, so 15000/100)
    rates[2].from_id = 0; rates[2].to_id = 3;
    rates[2].numerator = 150; rates[2].denominator = 1;

    // EUR -> USD: 1.087 (1087/1000)
    rates[3].from_id = 1; rates[3].to_id = 0;
    rates[3].numerator = 1087; rates[3].denominator = 1000;

    // GBP -> USD: 1.266 (1266/1000)
    rates[4].from_id = 2; rates[4].to_id = 0;
    rates[4].numerator = 1266; rates[4].denominator = 1000;

    // USD -> CAD: 1.36 (136/100)
    rates[5].from_id = 0; rates[5].to_id = 4;
    rates[5].numerator = 136; rates[5].denominator = 100;

    printf("=== Currency Converter ===\n");
    // EXPECT: === Currency Converter ===

    // Test 1: $100 USD to EUR
    amount = 10000;
    result = convert(amount, &rates[0]);
    printf("100 USD -> %d EUR cents\n", result);
    // EXPECT: 100 USD -> 9200 EUR cents

    // Test 2: $100 USD to GBP
    result = convert(amount, &rates[1]);
    printf("100 USD -> %d GBP cents\n", result);
    // EXPECT: 100 USD -> 7900 GBP cents

    // Test 3: $100 USD to JPY (yen cents)
    result = convert(amount, &rates[2]);
    printf("100 USD -> %d JPY cents\n", result);
    // EXPECT: 100 USD -> 1500000 JPY cents

    // Test 4: $250 USD to EUR
    amount = 25000;
    result = convert(amount, &rates[0]);
    printf("250 USD -> EUR: ");
    print_amount(result);
    printf("\n");
    // EXPECT: 250 USD -> EUR: 230.00

    // Test 5: $250 USD to CAD
    result = convert(amount, &rates[5]);
    printf("250 USD -> CAD: ");
    print_amount(result);
    printf("\n");
    // EXPECT: 250 USD -> CAD: 340.00

    // Test 6: Round trip USD -> EUR -> USD
    printf("--- Round Trip Test ---\n");
    // EXPECT: --- Round Trip Test ---
    amount = 10000; // $100
    result = convert_chain(amount, &rates[0], &rates[3]);
    printf("100 USD -> EUR -> USD: %d cents\n", result);
    // EXPECT: 100 USD -> EUR -> USD: 10000 cents

    // Test 7: Convert multiple amounts
    printf("--- Conversion Table (USD to EUR) ---\n");
    // EXPECT: --- Conversion Table (USD to EUR) ---
    int amounts[5];
    amounts[0] = 1000;
    amounts[1] = 5000;
    amounts[2] = 10000;
    amounts[3] = 50000;
    amounts[4] = 100000;
    for (i = 0; i < 5; i++) {
        result = convert(amounts[i], &rates[0]);
        printf("%d USD cents = %d EUR cents\n", amounts[i], result);
    }
    // EXPECT: 1000 USD cents = 920 EUR cents
    // EXPECT: 5000 USD cents = 4600 EUR cents
    // EXPECT: 10000 USD cents = 9200 EUR cents
    // EXPECT: 50000 USD cents = 46000 EUR cents
    // EXPECT: 100000 USD cents = 92000 EUR cents

    // Test 8: Cross rate GBP -> USD -> CAD
    printf("--- Cross Rate GBP -> CAD ---\n");
    // EXPECT: --- Cross Rate GBP -> CAD ---
    amount = 20000; // 200 GBP
    result = convert_chain(amount, &rates[4], &rates[5]);
    printf("200 GBP -> CAD: %d cents\n", result);
    // EXPECT: 200 GBP -> CAD: 34435 cents

    // Test 9: Small amount precision
    amount = 1; // 1 cent
    result = convert(amount, &rates[0]);
    printf("1 USD cent -> %d EUR cents\n", result);
    // EXPECT: 1 USD cent -> 0 EUR cents
    amount = 100; // $1
    result = convert(amount, &rates[0]);
    printf("100 USD cents -> %d EUR cents\n", result);
    // EXPECT: 100 USD cents -> 92 EUR cents

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
