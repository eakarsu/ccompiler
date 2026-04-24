int printf(const char *fmt, ...);

int pack_rgb(int r, int g, int b) {
    return ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

int unpack_r(int rgb) {
    return (rgb >> 16) & 0xFF;
}

int unpack_g(int rgb) {
    return (rgb >> 8) & 0xFF;
}

int unpack_b(int rgb) {
    return rgb & 0xFF;
}

int pack_date(int year, int month, int day) {
    return ((year & 0xFFF) << 9) | ((month & 0xF) << 5) | (day & 0x1F);
}

int unpack_year(int date) {
    return (date >> 9) & 0xFFF;
}

int unpack_month(int date) {
    return (date >> 5) & 0xF;
}

int unpack_day(int date) {
    return date & 0x1F;
}

int pack_flags(int readable, int writable, int executable, int hidden) {
    int flags = 0;
    if (readable) flags = flags | 1;
    if (writable) flags = flags | 2;
    if (executable) flags = flags | 4;
    if (hidden) flags = flags | 8;
    return flags;
}

int is_readable(int flags) { return (flags >> 0) & 1; }
int is_writable(int flags) { return (flags >> 1) & 1; }
int is_executable(int flags) { return (flags >> 2) & 1; }
int is_hidden(int flags) { return (flags >> 3) & 1; }

int pack_coord(int x, int y) {
    return ((x & 0xFFFF) << 16) | (y & 0xFFFF);
}

int unpack_x(int coord) {
    return (coord >> 16) & 0xFFFF;
}

int unpack_y(int coord) {
    return coord & 0xFFFF;
}

int pack_version(int major, int minor, int patch) {
    return ((major & 0xFF) << 16) | ((minor & 0xFF) << 8) | (patch & 0xFF);
}

int unpack_major(int ver) { return (ver >> 16) & 0xFF; }
int unpack_minor(int ver) { return (ver >> 8) & 0xFF; }
int unpack_patch(int ver) { return ver & 0xFF; }

int version_compare(int v1, int v2) {
    int maj1 = unpack_major(v1);
    int maj2 = unpack_major(v2);
    if (maj1 != maj2) {
        if (maj1 < maj2) return -1;
        return 1;
    }
    int min1 = unpack_minor(v1);
    int min2 = unpack_minor(v2);
    if (min1 != min2) {
        if (min1 < min2) return -1;
        return 1;
    }
    int pat1 = unpack_patch(v1);
    int pat2 = unpack_patch(v2);
    if (pat1 < pat2) return -1;
    if (pat1 > pat2) return 1;
    return 0;
}

int blend_colors(int c1, int c2) {
    int r = (unpack_r(c1) + unpack_r(c2)) / 2;
    int g = (unpack_g(c1) + unpack_g(c2)) / 2;
    int b = (unpack_b(c1) + unpack_b(c2)) / 2;
    return pack_rgb(r, g, b);
}

int pack_card(int suit, int rank) {
    return ((suit & 0x3) << 4) | (rank & 0xF);
}

int card_suit(int card) { return (card >> 4) & 0x3; }
int card_rank(int card) { return card & 0xF; }

int pack_ip_byte(int a, int b, int c, int d) {
    return ((a & 0xFF) << 24) | ((b & 0xFF) << 16) | ((c & 0xFF) << 8) | (d & 0xFF);
}

int ip_octet(int ip, int n) {
    return (ip >> ((3 - n) * 8)) & 0xFF;
}

int main(void) {
    int red = pack_rgb(255, 0, 0);
    // EXPECT: red R=255 G=0 B=0
    printf("red R=%d G=%d B=%d\n", unpack_r(red), unpack_g(red), unpack_b(red));

    int color = pack_rgb(100, 150, 200);
    // EXPECT: color R=100 G=150 B=200
    printf("color R=%d G=%d B=%d\n", unpack_r(color), unpack_g(color), unpack_b(color));

    int blended = blend_colors(pack_rgb(200, 100, 0), pack_rgb(0, 100, 200));
    // EXPECT: blend R=100 G=100 B=100
    printf("blend R=%d G=%d B=%d\n", unpack_r(blended), unpack_g(blended), unpack_b(blended));

    int date = pack_date(2024, 12, 25);
    // EXPECT: date: 2024/12/25
    printf("date: %d/%d/%d\n", unpack_year(date), unpack_month(date), unpack_day(date));

    int date2 = pack_date(2000, 1, 1);
    // EXPECT: date2: 2000/1/1
    printf("date2: %d/%d/%d\n", unpack_year(date2), unpack_month(date2), unpack_day(date2));

    int fl = pack_flags(1, 1, 0, 1);
    // EXPECT: flags: r=1 w=1 x=0 h=1
    printf("flags: r=%d w=%d x=%d h=%d\n",
        is_readable(fl), is_writable(fl), is_executable(fl), is_hidden(fl));

    int fl2 = pack_flags(0, 0, 1, 0);
    // EXPECT: flags2: r=0 w=0 x=1 h=0
    printf("flags2: r=%d w=%d x=%d h=%d\n",
        is_readable(fl2), is_writable(fl2), is_executable(fl2), is_hidden(fl2));

    int coord = pack_coord(100, 200);
    // EXPECT: coord: x=100 y=200
    printf("coord: x=%d y=%d\n", unpack_x(coord), unpack_y(coord));

    int v1 = pack_version(1, 2, 3);
    // EXPECT: version: 1.2.3
    printf("version: %d.%d.%d\n", unpack_major(v1), unpack_minor(v1), unpack_patch(v1));

    int v2 = pack_version(2, 0, 0);
    // EXPECT: v1 < v2: -1
    printf("v1 < v2: %d\n", version_compare(v1, v2));

    int v3 = pack_version(1, 2, 3);
    // EXPECT: v1 == v3: 0
    printf("v1 == v3: %d\n", version_compare(v1, v3));

    int v4 = pack_version(1, 2, 4);
    // EXPECT: v4 > v1: 1
    printf("v4 > v1: %d\n", version_compare(v4, v1));

    int card = pack_card(2, 13);
    // EXPECT: card: suit=2 rank=13
    printf("card: suit=%d rank=%d\n", card_suit(card), card_rank(card));

    int card2 = pack_card(0, 1);
    // EXPECT: card2: suit=0 rank=1
    printf("card2: suit=%d rank=%d\n", card_suit(card2), card_rank(card2));

    int ip = pack_ip_byte(192, 168, 1, 100);
    // EXPECT: ip: 192.168.1.100
    printf("ip: %d.%d.%d.%d\n", ip_octet(ip, 0), ip_octet(ip, 1), ip_octet(ip, 2), ip_octet(ip, 3));

    int ip2 = pack_ip_byte(10, 0, 0, 1);
    // EXPECT: ip2: 10.0.0.1
    printf("ip2: %d.%d.%d.%d\n", ip_octet(ip2, 0), ip_octet(ip2, 1), ip_octet(ip2, 2), ip_octet(ip2, 3));

    return 0;
}
