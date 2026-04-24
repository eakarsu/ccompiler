int printf(const char *fmt, ...);

typedef int Meters;
typedef int Seconds;
typedef int Kilograms;
typedef int MetersPerSec;
typedef int Celsius;
typedef int Fahrenheit;
typedef int Dollars;
typedef int Cents;

Meters meters(int val) { return val; }
Seconds seconds(int val) { return val; }
Kilograms kilograms(int val) { return val; }

MetersPerSec compute_speed(Meters dist, Seconds time) {
    if (time == 0) return 0;
    return dist / time;
}

Meters compute_distance(MetersPerSec speed, Seconds time) {
    return speed * time;
}

Seconds compute_time(Meters dist, MetersPerSec speed) {
    if (speed == 0) return 0;
    return dist / speed;
}

int momentum(Kilograms mass, MetersPerSec velocity) {
    return mass * velocity;
}

int kinetic_energy_x2(Kilograms mass, MetersPerSec velocity) {
    return mass * velocity * velocity;
}

Fahrenheit celsius_to_fahrenheit(Celsius c) {
    return c * 9 / 5 + 32;
}

Celsius fahrenheit_to_celsius(Fahrenheit f) {
    return (f - 32) * 5 / 9;
}

Cents dollars_to_cents(Dollars d) {
    return d * 100;
}

Dollars cents_to_dollars(Cents c) {
    return c / 100;
}

Cents add_tax(Cents price, int tax_percent) {
    return price + price * tax_percent / 100;
}

Cents apply_discount(Cents price, int discount_percent) {
    return price - price * discount_percent / 100;
}

typedef struct {
    Meters x;
    Meters y;
} Position;

typedef struct {
    MetersPerSec vx;
    MetersPerSec vy;
} Velocity;

typedef struct {
    Position pos;
    Velocity vel;
    Kilograms mass;
} Body;

void body_init(Body *b, Meters x, Meters y, MetersPerSec vx, MetersPerSec vy, Kilograms m) {
    b->pos.x = x;
    b->pos.y = y;
    b->vel.vx = vx;
    b->vel.vy = vy;
    b->mass = m;
}

void body_update(Body *b, Seconds dt) {
    b->pos.x = b->pos.x + b->vel.vx * dt;
    b->pos.y = b->pos.y + b->vel.vy * dt;
}

Meters body_distance_sq(Body *a, Body *b) {
    Meters dx = a->pos.x - b->pos.x;
    Meters dy = a->pos.y - b->pos.y;
    return dx * dx + dy * dy;
}

int body_total_momentum_x(Body bodies[], int count) {
    int total = 0;
    int i;
    for (i = 0; i < count; i = i + 1) {
        total = total + bodies[i].mass * bodies[i].vel.vx;
    }
    return total;
}

int body_total_mass(Body bodies[], int count) {
    int total = 0;
    int i;
    for (i = 0; i < count; i = i + 1) {
        total = total + bodies[i].mass;
    }
    return total;
}

Meters meters_max(Meters a, Meters b) {
    if (a > b) return a;
    return b;
}

Meters meters_min(Meters a, Meters b) {
    if (a < b) return a;
    return b;
}

Meters meters_abs(Meters a) {
    if (a < 0) return -a;
    return a;
}

int main(void) {
    Meters d = meters(100);
    Seconds t = seconds(10);
    // EXPECT: speed: 10
    printf("speed: %d\n", compute_speed(d, t));

    MetersPerSec spd = compute_speed(d, t);
    // EXPECT: distance: 150
    printf("distance: %d\n", compute_distance(spd, seconds(15)));

    // EXPECT: time: 20
    printf("time: %d\n", compute_time(meters(200), spd));

    Kilograms mass = kilograms(5);
    // EXPECT: momentum: 50
    printf("momentum: %d\n", momentum(mass, spd));

    // EXPECT: KE x2: 500
    printf("KE x2: %d\n", kinetic_energy_x2(mass, spd));

    // EXPECT: 0C to F: 32
    printf("0C to F: %d\n", celsius_to_fahrenheit(0));

    // EXPECT: 100C to F: 212
    printf("100C to F: %d\n", celsius_to_fahrenheit(100));

    // EXPECT: 32F to C: 0
    printf("32F to C: %d\n", fahrenheit_to_celsius(32));

    // EXPECT: 212F to C: 100
    printf("212F to C: %d\n", fahrenheit_to_celsius(212));

    // EXPECT: $5 to cents: 500
    printf("$5 to cents: %d\n", dollars_to_cents(5));

    // EXPECT: 750c to $: 7
    printf("750c to $: %d\n", cents_to_dollars(750));

    Cents price = dollars_to_cents(100);
    // EXPECT: with 8% tax: 10800
    printf("with 8%% tax: %d\n", add_tax(price, 8));

    // EXPECT: with 20% discount: 8000
    printf("with 20%% discount: %d\n", apply_discount(price, 20));

    Body b1;
    body_init(&b1, 0, 0, 3, 4, 10);
    // EXPECT: b1 pos: 0 0
    printf("b1 pos: %d %d\n", b1.pos.x, b1.pos.y);

    body_update(&b1, 2);
    // EXPECT: b1 after 2s: 6 8
    printf("b1 after 2s: %d %d\n", b1.pos.x, b1.pos.y);

    body_update(&b1, 3);
    // EXPECT: b1 after 3s more: 15 20
    printf("b1 after 3s more: %d %d\n", b1.pos.x, b1.pos.y);

    Body b2;
    body_init(&b2, 10, 10, -1, 0, 5);
    // EXPECT: dist sq: 125
    printf("dist sq: %d\n", body_distance_sq(&b1, &b2));

    Body bodies[2];
    bodies[0] = b1;
    bodies[1] = b2;
    // EXPECT: total mass: 15
    printf("total mass: %d\n", body_total_mass(bodies, 2));

    // EXPECT: total px: 25
    printf("total px: %d\n", body_total_momentum_x(bodies, 2));

    // EXPECT: max 5 10: 10
    printf("max 5 10: %d\n", meters_max(5, 10));

    // EXPECT: min 5 10: 5
    printf("min 5 10: %d\n", meters_min(5, 10));

    // EXPECT: abs -42: 42
    printf("abs -42: %d\n", meters_abs(-42));

    // EXPECT: speed 0 time: 0
    printf("speed 0 time: %d\n", compute_speed(100, 0));

    return 0;
}
