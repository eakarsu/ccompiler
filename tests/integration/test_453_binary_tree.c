int printf(const char *fmt, ...);
// EXPECT: Inorder: 20 30 40 50 60 70 80 \nPreorder: 50 30 20 40 70 60 80 \nPostorder: 20 40 30 60 80 70 50 \nSearch 40: 1\nSearch 55: 0\nHeight: 3\nLeaves: 4\nSize: 7
// Binary tree (array-based): insert, search, traversals

struct BTree {
    int data[31];
    int used[31];
    int size;
};

void bt_init(struct BTree *bt) {
    int i;
    for (i = 0; i < 31; i++) {
        bt->data[i] = 0;
        bt->used[i] = 0;
    }
    bt->size = 0;
}

int bt_left(int i) { return 2 * i + 1; }
int bt_right(int i) { return 2 * i + 2; }
int bt_parent(int i) { return (i - 1) / 2; }

void bt_insert(struct BTree *bt, int val) {
    int i = 0;
    while (i < 31) {
        if (!bt->used[i]) {
            bt->data[i] = val;
            bt->used[i] = 1;
            bt->size++;
            return;
        }
        if (val < bt->data[i]) {
            i = bt_left(i);
        } else {
            i = bt_right(i);
        }
    }
}

int bt_search(struct BTree *bt, int val) {
    int i = 0;
    while (i < 31 && bt->used[i]) {
        if (bt->data[i] == val) return 1;
        if (val < bt->data[i]) {
            i = bt_left(i);
        } else {
            i = bt_right(i);
        }
    }
    return 0;
}

void bt_inorder(struct BTree *bt, int i) {
    if (i >= 31 || !bt->used[i]) return;
    bt_inorder(bt, bt_left(i));
    printf("%d ", bt->data[i]);
    bt_inorder(bt, bt_right(i));
}

void bt_preorder(struct BTree *bt, int i) {
    if (i >= 31 || !bt->used[i]) return;
    printf("%d ", bt->data[i]);
    bt_preorder(bt, bt_left(i));
    bt_preorder(bt, bt_right(i));
}

void bt_postorder(struct BTree *bt, int i) {
    if (i >= 31 || !bt->used[i]) return;
    bt_postorder(bt, bt_left(i));
    bt_postorder(bt, bt_right(i));
    printf("%d ", bt->data[i]);
}

int bt_height(struct BTree *bt, int i) {
    int lh;
    int rh;
    if (i >= 31 || !bt->used[i]) return 0;
    lh = bt_height(bt, bt_left(i));
    rh = bt_height(bt, bt_right(i));
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int bt_count_leaves(struct BTree *bt, int i) {
    int l;
    int r;
    if (i >= 31 || !bt->used[i]) return 0;
    l = bt_left(i);
    r = bt_right(i);
    if ((l >= 31 || !bt->used[l]) && (r >= 31 || !bt->used[r]))
        return 1;
    return bt_count_leaves(bt, l) + bt_count_leaves(bt, r);
}

int main(void) {
    struct BTree bt;
    bt_init(&bt);

    bt_insert(&bt, 50);
    bt_insert(&bt, 30);
    bt_insert(&bt, 70);
    bt_insert(&bt, 20);
    bt_insert(&bt, 40);
    bt_insert(&bt, 60);
    bt_insert(&bt, 80);

    printf("Inorder: ");
    bt_inorder(&bt, 0);
    printf("\n");

    printf("Preorder: ");
    bt_preorder(&bt, 0);
    printf("\n");

    printf("Postorder: ");
    bt_postorder(&bt, 0);
    printf("\n");

    printf("Search 40: %d\n", bt_search(&bt, 40));
    printf("Search 55: %d\n", bt_search(&bt, 55));
    printf("Height: %d\n", bt_height(&bt, 0));
    printf("Leaves: %d\n", bt_count_leaves(&bt, 0));
    printf("Size: %d\n", bt.size);

    return 0;
}
