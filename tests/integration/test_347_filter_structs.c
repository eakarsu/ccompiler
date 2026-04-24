int printf(const char *fmt, ...);
// EXPECT: all products: 8\ntotal value: 3625\nprice<=75: 5\n  [1] price=50 cat=1 stock=10\n  [3] price=30 cat=1 stock=0\n  [5] price=75 cat=2 stock=8\n  [6] price=25 cat=1 stock=20\n  [8] price=45 cat=2 stock=15\ncategory 2: 3\n  [2] price=150 cat=2 stock=5\n  [5] price=75 cat=2 stock=8\n  [8] price=45 cat=2 stock=15\nin stock: 6\ncheapest in stock: 6\nprice<=100 and in stock: 4\n  [1] price=50 cat=1 stock=10\n  [5] price=75 cat=2 stock=8\n  [6] price=25 cat=1 stock=20\n  [8] price=45 cat=2 stock=15
// Test: filter structs

struct Product {
    int id;
    int price;
    int category;
    int in_stock;
};

int filter_by_price(struct Product src[], int n, struct Product dst[], int max_price) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (src[i].price <= max_price) {
            dst[count] = src[i];
            count = count + 1;
        }
    }
    return count;
}

int filter_by_category(struct Product src[], int n, struct Product dst[], int cat) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (src[i].category == cat) {
            dst[count] = src[i];
            count = count + 1;
        }
    }
    return count;
}

int filter_in_stock(struct Product src[], int n, struct Product dst[]) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (src[i].in_stock > 0) {
            dst[count] = src[i];
            count = count + 1;
        }
    }
    return count;
}

int total_value(struct Product arr[], int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + arr[i].price * arr[i].in_stock;
    }
    return sum;
}

int cheapest(struct Product arr[], int n) {
    if (n == 0) return -1;
    int min_price = arr[0].price;
    int min_id = arr[0].id;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i].price < min_price) {
            min_price = arr[i].price;
            min_id = arr[i].id;
        }
    }
    return min_id;
}

void print_products(struct Product arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("  [%d] price=%d cat=%d stock=%d\n",
               arr[i].id, arr[i].price, arr[i].category, arr[i].in_stock);
    }
}

int main(void) {
    struct Product products[8];
    products[0].id=1; products[0].price=50;  products[0].category=1; products[0].in_stock=10;
    products[1].id=2; products[1].price=150; products[1].category=2; products[1].in_stock=5;
    products[2].id=3; products[2].price=30;  products[2].category=1; products[2].in_stock=0;
    products[3].id=4; products[3].price=200; products[3].category=3; products[3].in_stock=3;
    products[4].id=5; products[4].price=75;  products[4].category=2; products[4].in_stock=8;
    products[5].id=6; products[5].price=25;  products[5].category=1; products[5].in_stock=20;
    products[6].id=7; products[6].price=100; products[6].category=3; products[6].in_stock=0;
    products[7].id=8; products[7].price=45;  products[7].category=2; products[7].in_stock=15;

    printf("all products: %d\n", 8);
    printf("total value: %d\n", total_value(products, 8));

    struct Product filtered[8];
    int cnt;

    cnt = filter_by_price(products, 8, filtered, 75);
    printf("price<=75: %d\n", cnt);
    print_products(filtered, cnt);

    cnt = filter_by_category(products, 8, filtered, 2);
    printf("category 2: %d\n", cnt);
    print_products(filtered, cnt);

    cnt = filter_in_stock(products, 8, filtered);
    printf("in stock: %d\n", cnt);
    printf("cheapest in stock: %d\n", cheapest(filtered, cnt));

    cnt = filter_by_price(products, 8, filtered, 100);
    int cnt2;
    struct Product filtered2[8];
    cnt2 = filter_in_stock(filtered, cnt, filtered2);
    printf("price<=100 and in stock: %d\n", cnt2);
    print_products(filtered2, cnt2);

    return 0;
}
