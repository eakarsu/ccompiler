int printf(const char *fmt, ...);

/* Basic union */
union IntOrChar {
    int  ival;
    char cval;
};

/* Union with two ints */
union TwoInts {
    int a;
    int b;
};

/* Union whose size = size of largest member (int = 4) */
union Small {
    char  c;
    short s;
    int   i;
};

/* Struct containing a union */
struct Tagged {
    int         tag;    /* 0=int, 1=char */
    union IntOrChar data;
};

/* Union containing a struct */
struct Coords { int x; int y; };
union CoordOrInt {
    struct Coords coords;
    int           raw[2];
};

/* Type-punning: store bytes of an int and read back */
union Pun {
    int      ival;
    char     bytes[4];
};

int main(void) {
    /* --- Basic union write and read --- */
    union IntOrChar u;
    u.ival = 65;
    /* Reading ival gives 65 */
    printf("%d\n", u.ival);                  // EXPECT: 65
    /* Reading cval: least-significant byte of 65 is 'A'=65 */
    printf("%d\n", (int)u.cval);             // EXPECT: 65

    u.ival = 0;
    printf("%d\n", u.ival);                  // EXPECT: 0

    /* --- TwoInts: both members overlap at same address --- */
    union TwoInts t;
    t.a = 42;
    printf("%d\n", t.b);                     // EXPECT: 42
    t.b = 100;
    printf("%d\n", t.a);                     // EXPECT: 100

    /* --- Small: size is 4 (int) --- */
    union Small s;
    s.i = 300;
    printf("%d\n", s.i);                     // EXPECT: 300
    /* Overwrite with char */
    s.c = 7;
    printf("%d\n", (int)s.c);               // EXPECT: 7

    /* --- Struct with union (tagged union pattern) --- */
    struct Tagged tg;
    tg.tag = 0;
    tg.data.ival = 12345;
    printf("%d\n", tg.tag);                  // EXPECT: 0
    printf("%d\n", tg.data.ival);            // EXPECT: 12345

    tg.tag = 1;
    tg.data.cval = 'Z';                      /* ASCII 90 */
    printf("%d\n", tg.tag);                  // EXPECT: 1
    printf("%d\n", (int)tg.data.cval);       // EXPECT: 90

    /* --- Array of tagged unions --- */
    struct Tagged tags[3];
    tags[0].tag = 0; tags[0].data.ival = 10;
    tags[1].tag = 1; tags[1].data.cval = 'A'; /* 65 */
    tags[2].tag = 0; tags[2].data.ival = 30;

    int i, sum = 0;
    for (i = 0; i < 3; i++) {
        if (tags[i].tag == 0) {
            sum += tags[i].data.ival;
        } else {
            sum += (int)tags[i].data.cval;
        }
    }
    /* 10 + 65 + 30 = 105 */
    printf("%d\n", sum);                     // EXPECT: 105

    /* --- Union containing struct --- */
    union CoordOrInt ci;
    ci.coords.x = 3;
    ci.coords.y = 7;
    printf("%d\n", ci.coords.x);            // EXPECT: 3
    printf("%d\n", ci.coords.y);            // EXPECT: 7

    /* Overwrite via raw[0] and check coords.x */
    ci.raw[0] = 99;
    printf("%d\n", ci.coords.x);            // EXPECT: 99

    /* --- Type-punning to inspect bytes --- */
    union Pun pun;
    pun.ival = 0x01020304;
    /* On little-endian: bytes[0]=4, bytes[1]=3, bytes[2]=2, bytes[3]=1 */
    int b0 = (int)(unsigned char)pun.bytes[0];
    int b1 = (int)(unsigned char)pun.bytes[1];
    int b2 = (int)(unsigned char)pun.bytes[2];
    int b3 = (int)(unsigned char)pun.bytes[3];
    /* b0+b1+b2+b3 = 4+3+2+1 = 10 regardless of endian */
    printf("%d\n", b0 + b1 + b2 + b3);     // EXPECT: 10

    /* --- Write int zero, confirm all bytes zero --- */
    pun.ival = 0;
    printf("%d\n", (int)pun.bytes[0]);      // EXPECT: 0
    printf("%d\n", (int)pun.bytes[1]);      // EXPECT: 0
    printf("%d\n", (int)pun.bytes[2]);      // EXPECT: 0
    printf("%d\n", (int)pun.bytes[3]);      // EXPECT: 0

    /* --- Union assignment copies the full union --- */
    union Small s2;
    s2.i = 777;
    union Small s3 = s2;
    printf("%d\n", s3.i);                   // EXPECT: 777

    /* Modifying s3 does not affect s2 */
    s3.i = 1;
    printf("%d\n", s2.i);                   // EXPECT: 777
    printf("%d\n", s3.i);                   // EXPECT: 1

    /* --- Overwrite a union member multiple times --- */
    union TwoInts tt;
    int k;
    for (k = 0; k < 5; k++) {
        tt.a = k * k;
    }
    /* Last iteration k=4: 4*4=16 */
    printf("%d\n", tt.a);                   // EXPECT: 16
    printf("%d\n", tt.b);                   // EXPECT: 16

    return 0;
}
