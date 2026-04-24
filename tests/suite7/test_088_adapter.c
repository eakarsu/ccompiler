int printf(const char *fmt, ...);

struct PointCartesian {
    int x;
    int y;
};

struct PointPolar {
    int radius;
    int angle_deg;
};

struct RGBColor {
    int r;
    int g;
    int b;
};

struct HSVColor {
    int hue;
    int saturation;
    int value;
};

struct TempCelsius {
    int degrees;
};

struct TempFahr {
    int degrees;
};

int isqrt(int n) {
    int x;
    int next;
    if (n <= 0) return 0;
    x = n;
    next = (x + n / x) / 2;
    while (next < x) {
        x = next;
        next = (x + n / x) / 2;
    }
    return x;
}

struct PointCartesian polar_to_cartesian(struct PointPolar *p) {
    struct PointCartesian c;
    int cos_val;
    int sin_val;
    if (p->angle_deg == 0) { cos_val = 100; sin_val = 0; }
    else if (p->angle_deg == 30) { cos_val = 87; sin_val = 50; }
    else if (p->angle_deg == 45) { cos_val = 71; sin_val = 71; }
    else if (p->angle_deg == 60) { cos_val = 50; sin_val = 87; }
    else if (p->angle_deg == 90) { cos_val = 0; sin_val = 100; }
    else if (p->angle_deg == 180) { cos_val = -100; sin_val = 0; }
    else { cos_val = 100; sin_val = 0; }
    c.x = (p->radius * cos_val) / 100;
    c.y = (p->radius * sin_val) / 100;
    return c;
}

int cartesian_distance(struct PointCartesian *a, struct PointCartesian *b) {
    int dx;
    int dy;
    dx = a->x - b->x;
    dy = a->y - b->y;
    return isqrt(dx * dx + dy * dy);
}

struct TempFahr celsius_to_fahr(struct TempCelsius *tc) {
    struct TempFahr tf;
    tf.degrees = (tc->degrees * 9) / 5 + 32;
    return tf;
}

struct TempCelsius fahr_to_celsius(struct TempFahr *tf) {
    struct TempCelsius tc;
    tc.degrees = ((tf->degrees - 32) * 5) / 9;
    return tc;
}

int rgb_brightness(struct RGBColor *c) {
    return (c->r + c->g + c->b) / 3;
}

struct RGBColor grayscale_adapter(struct RGBColor *c) {
    struct RGBColor gray;
    int avg;
    avg = rgb_brightness(c);
    gray.r = avg;
    gray.g = avg;
    gray.b = avg;
    return gray;
}

struct RGBColor invert_adapter(struct RGBColor *c) {
    struct RGBColor inv;
    inv.r = 255 - c->r;
    inv.g = 255 - c->g;
    inv.b = 255 - c->b;
    return inv;
}

int colors_equal(struct RGBColor *a, struct RGBColor *b) {
    return a->r == b->r && a->g == b->g && a->b == b->b;
}

int main(void) {
    struct PointPolar pp;
    struct PointCartesian pc;
    struct PointCartesian pc2;
    struct TempCelsius tc;
    struct TempFahr tf;
    struct TempCelsius tc2;
    struct RGBColor rgb;
    struct RGBColor gray;
    struct RGBColor inv;
    struct RGBColor inv2;
    int dist;

    pp.radius = 100;
    pp.angle_deg = 0;
    pc = polar_to_cartesian(&pp);
    // EXPECT: polar(100,0): x=100 y=0
    printf("polar(100,0): x=%d y=%d\n", pc.x, pc.y);

    pp.radius = 100;
    pp.angle_deg = 90;
    pc = polar_to_cartesian(&pp);
    // EXPECT: polar(100,90): x=0 y=100
    printf("polar(100,90): x=%d y=%d\n", pc.x, pc.y);

    pp.radius = 100;
    pp.angle_deg = 45;
    pc = polar_to_cartesian(&pp);
    // EXPECT: polar(100,45): x=71 y=71
    printf("polar(100,45): x=%d y=%d\n", pc.x, pc.y);

    pp.radius = 50;
    pp.angle_deg = 30;
    pc = polar_to_cartesian(&pp);
    // EXPECT: polar(50,30): x=43 y=25
    printf("polar(50,30): x=%d y=%d\n", pc.x, pc.y);

    pp.radius = 200;
    pp.angle_deg = 60;
    pc = polar_to_cartesian(&pp);
    // EXPECT: polar(200,60): x=100 y=174
    printf("polar(200,60): x=%d y=%d\n", pc.x, pc.y);

    pc.x = 3;
    pc.y = 0;
    pc2.x = 0;
    pc2.y = 4;
    dist = cartesian_distance(&pc, &pc2);
    // EXPECT: dist(3,0)-(0,4): 5
    printf("dist(3,0)-(0,4): %d\n", dist);

    tc.degrees = 100;
    tf = celsius_to_fahr(&tc);
    // EXPECT: 100C = 212F
    printf("100C = %dF\n", tf.degrees);

    tc.degrees = 0;
    tf = celsius_to_fahr(&tc);
    // EXPECT: 0C = 32F
    printf("0C = %dF\n", tf.degrees);

    tf.degrees = 212;
    tc2 = fahr_to_celsius(&tf);
    // EXPECT: 212F = 100C
    printf("212F = %dC\n", tc2.degrees);

    tf.degrees = 32;
    tc2 = fahr_to_celsius(&tf);
    // EXPECT: 32F = 0C
    printf("32F = %dC\n", tc2.degrees);

    rgb.r = 100;
    rgb.g = 150;
    rgb.b = 200;
    // EXPECT: brightness: 150
    printf("brightness: %d\n", rgb_brightness(&rgb));

    gray = grayscale_adapter(&rgb);
    // EXPECT: grayscale: 150 150 150
    printf("grayscale: %d %d %d\n", gray.r, gray.g, gray.b);

    inv = invert_adapter(&rgb);
    // EXPECT: inverted: 155 105 55
    printf("inverted: %d %d %d\n", inv.r, inv.g, inv.b);

    inv2 = invert_adapter(&inv);
    // EXPECT: double invert equal: 1
    printf("double invert equal: %d\n", colors_equal(&rgb, &inv2));

    // EXPECT: inv brightness: 105
    printf("inv brightness: %d\n", rgb_brightness(&inv));

    return 0;
}
