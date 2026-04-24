int printf(const char *fmt, ...);

/* Simple hash functions: djb2, sdbm, and a custom polynomial hash */

int djb2_hash(char *str) {
    int hash;
    int c;
    hash = 5381;
    while (*str) {
        c = *str;
        hash = ((hash << 5) + hash) + c;
        str = str + 1;
    }
    if (hash < 0) hash = -hash;
    return hash;
}

int sdbm_hash(char *str) {
    int hash;
    int c;
    hash = 0;
    while (*str) {
        c = *str;
        hash = c + (hash << 6) + (hash << 16) - hash;
        str = str + 1;
    }
    if (hash < 0) hash = -hash;
    return hash;
}

int poly_hash(char *str, int base, int mod) {
    int hash;
    int power;
    hash = 0;
    power = 1;
    while (*str) {
        int c;
        c = *str;
        hash = (hash + (c * power) % mod) % mod;
        power = (power * base) % mod;
        str = str + 1;
    }
    if (hash < 0) hash = -hash;
    return hash;
}

int test_collision(char *a, char *b) {
    int ha;
    int hb;
    ha = djb2_hash(a);
    hb = djb2_hash(b);
    if (ha == hb) {
        return 1;
    }
    return 0;
}

int hash_bucket(char *str, int num_buckets) {
    int h;
    h = djb2_hash(str);
    return h % num_buckets;
}

void test_basic_hashes(void) {
    int h1;
    int h2;
    int h3;
    h1 = djb2_hash("hello");
    h2 = djb2_hash("world");
    h3 = djb2_hash("hello");
    // EXPECT: djb2 hello=261238937
    printf("djb2 hello=%d\n", h1);
    // EXPECT: djb2 world=279393645
    printf("djb2 world=%d\n", h2);
    // EXPECT: djb2 consistent=1
    printf("djb2 consistent=%d\n", h1 == h3);
    // EXPECT: djb2 different=1
    printf("djb2 different=%d\n", h1 != h2);
}

void test_sdbm_hashes(void) {
    int h1;
    int h2;
    h1 = sdbm_hash("abc");
    h2 = sdbm_hash("xyz");
    // EXPECT: sdbm abc != sdbm xyz: 1
    printf("sdbm abc != sdbm xyz: %d\n", h1 != h2);
    // EXPECT: sdbm nonempty: 1
    printf("sdbm nonempty: %d\n", h1 != 0);
}

void test_poly_hashes(void) {
    int h1;
    int h2;
    int h3;
    h1 = poly_hash("test", 31, 1000007);
    h2 = poly_hash("test", 31, 1000007);
    h3 = poly_hash("tset", 31, 1000007);
    // EXPECT: poly consistent=1
    printf("poly consistent=%d\n", h1 == h2);
    // EXPECT: poly different=1
    printf("poly different=%d\n", h1 != h3);
}

void test_buckets(void) {
    int b1;
    int b2;
    int b3;
    b1 = hash_bucket("apple", 16);
    b2 = hash_bucket("banana", 16);
    b3 = hash_bucket("cherry", 16);
    // EXPECT: bucket apple=7
    printf("bucket apple=%d\n", b1);
    // EXPECT: bucket banana=10
    printf("bucket banana=%d\n", b2);
    // EXPECT: bucket cherry=14
    printf("bucket cherry=%d\n", b3);
}

void test_empty_string(void) {
    int h;
    h = djb2_hash("");
    // EXPECT: empty hash=5381
    printf("empty hash=%d\n", h);
}

int main(void) {
    int collision;
    test_basic_hashes();
    test_sdbm_hashes();
    test_poly_hashes();
    test_buckets();
    test_empty_string();
    collision = test_collision("hello", "world");
    // EXPECT: collision=0
    printf("collision=%d\n", collision);
    // EXPECT: done
    printf("done\n");
    return 0;
}
