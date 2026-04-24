int printf(const char *fmt, ...);
// EXPECT: Unit Conversions:\n  100.00 C = 212.00 F\n  212.00 F = 100.00 C\n  175.00 m = 574.00 ft\n  70.00 kg = 154.00 lbs\n  42.00 km = 26.04 mi\nRound-trip check:\n  100.00 -> 212.00 -> 100.00 C\n  212.00 -> 100.00 -> 212.00 F\n  175.00 -> 574.00 -> 175.00 m\n  70.00 -> 154.00 -> 70.00 kg\n  42.00 -> 26.04 -> 42.00 km\nConversions done: 5

enum Unit {
    UNIT_CELSIUS = 0,
    UNIT_FAHRENHEIT = 1,
    UNIT_METERS = 2,
    UNIT_FEET = 3,
    UNIT_KG = 4,
    UNIT_LBS = 5,
    UNIT_KM = 6,
    UNIT_MILES = 7
};

struct Conversion {
    int from_unit;
    int to_unit;
    int input_val_x100;
    int output_val_x100;
};

char *unit_name(int u) {
    if (u == UNIT_CELSIUS) return "C";
    if (u == UNIT_FAHRENHEIT) return "F";
    if (u == UNIT_METERS) return "m";
    if (u == UNIT_FEET) return "ft";
    if (u == UNIT_KG) return "kg";
    if (u == UNIT_LBS) return "lbs";
    if (u == UNIT_KM) return "km";
    if (u == UNIT_MILES) return "mi";
    return "?";
}

int convert_x100(int val_x100, int from, int to) {
    if (from == UNIT_CELSIUS && to == UNIT_FAHRENHEIT) {
        return val_x100 * 9 / 5 + 3200;
    }
    if (from == UNIT_FAHRENHEIT && to == UNIT_CELSIUS) {
        return (val_x100 - 3200) * 5 / 9;
    }
    if (from == UNIT_METERS && to == UNIT_FEET) {
        return val_x100 * 328 / 100;
    }
    if (from == UNIT_FEET && to == UNIT_METERS) {
        return val_x100 * 100 / 328;
    }
    if (from == UNIT_KG && to == UNIT_LBS) {
        return val_x100 * 220 / 100;
    }
    if (from == UNIT_LBS && to == UNIT_KG) {
        return val_x100 * 100 / 220;
    }
    if (from == UNIT_KM && to == UNIT_MILES) {
        return val_x100 * 62 / 100;
    }
    if (from == UNIT_MILES && to == UNIT_KM) {
        return val_x100 * 100 / 62;
    }
    return val_x100;
}

void print_val(int val_x100) {
    int whole = val_x100 / 100;
    int frac = val_x100 % 100;
    if (frac < 0) frac = -frac;
    if (val_x100 < 0 && whole == 0) {
        printf("-%d.%02d", whole, frac);
    } else {
        printf("%d.%02d", whole, frac);
    }
}

int main(void) {
    struct Conversion conversions[10];
    int count = 0;

    int inputs_x100[5];
    int from_units[5];
    int to_units[5];
    inputs_x100[0] = 10000; from_units[0] = UNIT_CELSIUS; to_units[0] = UNIT_FAHRENHEIT;
    inputs_x100[1] = 21200; from_units[1] = UNIT_FAHRENHEIT; to_units[1] = UNIT_CELSIUS;
    inputs_x100[2] = 17500; from_units[2] = UNIT_METERS; to_units[2] = UNIT_FEET;
    inputs_x100[3] = 7000;  from_units[3] = UNIT_KG; to_units[3] = UNIT_LBS;
    inputs_x100[4] = 4200;  from_units[4] = UNIT_KM; to_units[4] = UNIT_MILES;

    printf("Unit Conversions:\n");
    int i;
    for (i = 0; i < 5; i++) {
        int result = convert_x100(inputs_x100[i], from_units[i], to_units[i]);
        conversions[count].from_unit = from_units[i];
        conversions[count].to_unit = to_units[i];
        conversions[count].input_val_x100 = inputs_x100[i];
        conversions[count].output_val_x100 = result;
        count++;

        printf("  ");
        print_val(inputs_x100[i]);
        printf(" %s = ", unit_name(from_units[i]));
        print_val(result);
        printf(" %s\n", unit_name(to_units[i]));
    }

    printf("Round-trip check:\n");
    for (i = 0; i < count; i++) {
        int back = convert_x100(conversions[i].output_val_x100,
                                conversions[i].to_unit,
                                conversions[i].from_unit);
        printf("  ");
        print_val(conversions[i].input_val_x100);
        printf(" -> ");
        print_val(conversions[i].output_val_x100);
        printf(" -> ");
        print_val(back);
        printf(" %s\n", unit_name(conversions[i].from_unit));
    }

    printf("Conversions done: %d\n", count);

    return 0;
}
