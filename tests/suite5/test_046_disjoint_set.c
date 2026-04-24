int printf(const char *fmt, ...);

int parent[64];
int rank[64];
int ds_size;

void ds_init(int n) {
    int i;
    ds_size = n;
    i = 0;
    while (i < n) {
        parent[i] = i;
        rank[i] = 0;
        i = i + 1;
    }
}

int ds_find(int x) {
    if (parent[x] != x) {
        parent[x] = ds_find(parent[x]);
    }
    return parent[x];
}

void ds_union(int x, int y) {
    int rx;
    int ry;
    rx = ds_find(x);
    ry = ds_find(y);
    if (rx == ry) return;
    if (rank[rx] < rank[ry]) {
        parent[rx] = ry;
    } else if (rank[rx] > rank[ry]) {
        parent[ry] = rx;
    } else {
        parent[ry] = rx;
        rank[rx] = rank[rx] + 1;
    }
}

int ds_connected(int x, int y) {
    return ds_find(x) == ds_find(y);
}

int ds_count_components(int n) {
    int count;
    int i;
    count = 0;
    i = 0;
    while (i < n) {
        if (ds_find(i) == i) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int ds_component_size(int x) {
    int root;
    int count;
    int i;
    root = ds_find(x);
    count = 0;
    i = 0;
    while (i < ds_size) {
        if (ds_find(i) == root) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int main(void) {
    int comp;
    int ret;

    ds_init(10);

    comp = ds_count_components(10);
    // EXPECT: initial components: 10
    printf("initial components: %d\n", comp);

    ds_union(0, 1);
    ds_union(2, 3);
    ds_union(4, 5);

    comp = ds_count_components(10);
    // EXPECT: after 3 unions: 7
    printf("after 3 unions: %d\n", comp);

    ret = ds_connected(0, 1);
    // EXPECT: 0-1 connected: 1
    printf("0-1 connected: %d\n", ret);

    ret = ds_connected(0, 2);
    // EXPECT: 0-2 connected: 0
    printf("0-2 connected: %d\n", ret);

    ds_union(0, 2);
    ret = ds_connected(0, 3);
    // EXPECT: 0-3 after merge: 1
    printf("0-3 after merge: %d\n", ret);

    ret = ds_connected(1, 3);
    // EXPECT: 1-3 connected: 1
    printf("1-3 connected: %d\n", ret);

    comp = ds_count_components(10);
    // EXPECT: components: 6
    printf("components: %d\n", comp);

    // EXPECT: size of 0 comp: 4
    printf("size of 0 comp: %d\n", ds_component_size(0));

    // EXPECT: size of 4 comp: 2
    printf("size of 4 comp: %d\n", ds_component_size(4));

    // EXPECT: size of 9 comp: 1
    printf("size of 9 comp: %d\n", ds_component_size(9));

    ds_union(4, 6);
    ds_union(6, 7);
    ds_union(7, 8);
    ds_union(8, 9);

    comp = ds_count_components(10);
    // EXPECT: components: 2
    printf("components: %d\n", comp);

    // EXPECT: size of 4 comp: 6
    printf("size of 4 comp: %d\n", ds_component_size(4));

    ret = ds_connected(5, 9);
    // EXPECT: 5-9 connected: 1
    printf("5-9 connected: %d\n", ret);

    ret = ds_connected(0, 9);
    // EXPECT: 0-9 connected: 0
    printf("0-9 connected: %d\n", ret);

    ds_union(0, 9);
    comp = ds_count_components(10);
    // EXPECT: all one: 1
    printf("all one: %d\n", comp);

    // EXPECT: total size: 10
    printf("total size: %d\n", ds_component_size(0));

    return 0;
}
