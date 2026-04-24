int printf(const char *fmt, ...);
// EXPECT: a: {1:10}, {3:30}, {5:50}, {7:70}\nb: {2:20}, {3:35}, {6:60}, {7:75}\nmerge sorted (8): {1:10}, {2:20}, {3:30}, {3:35}, {5:50}, {6:60}, {7:70}, {7:75}\nmerge unique (6): {1:10}, {2:20}, {3:65}, {5:50}, {6:60}, {7:145}\nintersect (2): {3:65}, {7:145}\nmerge c+d (6): {10:100}, {15:150}, {20:200}, {25:250}, {30:300}, {35:350}\nintersect c+d (0): 
// Test: merge records from two sorted arrays

struct Record {
    int key;
    int value;
};

int merge_sorted(struct Record a[], int na, struct Record b[], int nb,
                 struct Record out[]) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < na && j < nb) {
        if (a[i].key <= b[j].key) {
            out[k] = a[i];
            i = i + 1;
        } else {
            out[k] = b[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < na) {
        out[k] = a[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < nb) {
        out[k] = b[j];
        j = j + 1;
        k = k + 1;
    }
    return k;
}

int merge_unique(struct Record a[], int na, struct Record b[], int nb,
                 struct Record out[]) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < na && j < nb) {
        if (a[i].key < b[j].key) {
            out[k] = a[i]; i = i + 1; k = k + 1;
        } else if (a[i].key > b[j].key) {
            out[k] = b[j]; j = j + 1; k = k + 1;
        } else {
            out[k].key = a[i].key;
            out[k].value = a[i].value + b[j].value;
            i = i + 1; j = j + 1; k = k + 1;
        }
    }
    while (i < na) { out[k] = a[i]; i = i + 1; k = k + 1; }
    while (j < nb) { out[k] = b[j]; j = j + 1; k = k + 1; }
    return k;
}

int intersect(struct Record a[], int na, struct Record b[], int nb,
              struct Record out[]) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < na && j < nb) {
        if (a[i].key < b[j].key) { i = i + 1; }
        else if (a[i].key > b[j].key) { j = j + 1; }
        else {
            out[k].key = a[i].key;
            out[k].value = a[i].value + b[j].value;
            i = i + 1; j = j + 1; k = k + 1;
        }
    }
    return k;
}

void print_records(struct Record arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(", ");
        printf("{%d:%d}", arr[i].key, arr[i].value);
    }
    printf("\n");
}

int main(void) {
    struct Record a[4];
    a[0].key=1; a[0].value=10;
    a[1].key=3; a[1].value=30;
    a[2].key=5; a[2].value=50;
    a[3].key=7; a[3].value=70;

    struct Record b[4];
    b[0].key=2; b[0].value=20;
    b[1].key=3; b[1].value=35;
    b[2].key=6; b[2].value=60;
    b[3].key=7; b[3].value=75;

    printf("a: "); print_records(a, 4);
    printf("b: "); print_records(b, 4);

    struct Record merged[8];
    int n = merge_sorted(a, 4, b, 4, merged);
    printf("merge sorted (%d): ", n); print_records(merged, n);

    struct Record unique[8];
    n = merge_unique(a, 4, b, 4, unique);
    printf("merge unique (%d): ", n); print_records(unique, n);

    struct Record inter[4];
    n = intersect(a, 4, b, 4, inter);
    printf("intersect (%d): ", n); print_records(inter, n);

    struct Record c[3];
    c[0].key=10; c[0].value=100;
    c[1].key=20; c[1].value=200;
    c[2].key=30; c[2].value=300;

    struct Record d[3];
    d[0].key=15; d[0].value=150;
    d[1].key=25; d[1].value=250;
    d[2].key=35; d[2].value=350;

    struct Record merged2[6];
    n = merge_sorted(c, 3, d, 3, merged2);
    printf("merge c+d (%d): ", n); print_records(merged2, n);

    n = intersect(c, 3, d, 3, inter);
    printf("intersect c+d (%d): ", n); print_records(inter, n);

    return 0;
}
