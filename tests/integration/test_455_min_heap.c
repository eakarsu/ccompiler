int printf(const char *fmt, ...);
// EXPECT: Heap: [10,20,40,50,30,70,60]\nPeek: 10\nSize: 7\nPopping: 10 20 30 40 50 60 70 \nHeap sorted: 11 12 22 25 37 45 64 90 
// Min-heap operations with array

struct MinHeap {
    int data[32];
    int size;
};

void heap_init(struct MinHeap *h) {
    int i;
    for (i = 0; i < 32; i++) h->data[i] = 0;
    h->size = 0;
}

void heap_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void heap_sift_up(struct MinHeap *h, int i) {
    int parent;
    while (i > 0) {
        parent = (i - 1) / 2;
        if (h->data[parent] > h->data[i]) {
            heap_swap(&h->data[parent], &h->data[i]);
            i = parent;
        } else {
            break;
        }
    }
}

void heap_sift_down(struct MinHeap *h, int i) {
    int left;
    int right;
    int smallest;
    while (1) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;
        if (left < h->size && h->data[left] < h->data[smallest])
            smallest = left;
        if (right < h->size && h->data[right] < h->data[smallest])
            smallest = right;
        if (smallest != i) {
            heap_swap(&h->data[i], &h->data[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
}

void heap_push(struct MinHeap *h, int val) {
    if (h->size >= 32) return;
    h->data[h->size] = val;
    heap_sift_up(h, h->size);
    h->size++;
}

int heap_pop(struct MinHeap *h) {
    int val;
    if (h->size == 0) return -1;
    val = h->data[0];
    h->size--;
    h->data[0] = h->data[h->size];
    heap_sift_down(h, 0);
    return val;
}

int heap_peek(struct MinHeap *h) {
    if (h->size == 0) return -1;
    return h->data[0];
}

void heap_print(struct MinHeap *h) {
    int i;
    printf("[");
    for (i = 0; i < h->size; i++) {
        printf("%d", h->data[i]);
        if (i < h->size - 1) printf(",");
    }
    printf("]\n");
}

void heap_sort(int *arr, int n) {
    struct MinHeap h;
    int i;
    heap_init(&h);
    for (i = 0; i < n; i++) {
        heap_push(&h, arr[i]);
    }
    for (i = 0; i < n; i++) {
        arr[i] = heap_pop(&h);
    }
}

int main(void) {
    struct MinHeap h;
    int arr[8];
    int i;
    heap_init(&h);

    heap_push(&h, 50);
    heap_push(&h, 30);
    heap_push(&h, 70);
    heap_push(&h, 10);
    heap_push(&h, 20);
    heap_push(&h, 60);
    heap_push(&h, 40);

    printf("Heap: ");
    heap_print(&h);
    printf("Peek: %d\n", heap_peek(&h));
    printf("Size: %d\n", h.size);

    printf("Popping: ");
    while (h.size > 0) {
        printf("%d ", heap_pop(&h));
    }
    printf("\n");

    arr[0] = 64; arr[1] = 25; arr[2] = 12; arr[3] = 22;
    arr[4] = 11; arr[5] = 90; arr[6] = 45; arr[7] = 37;
    heap_sort(arr, 8);
    printf("Heap sorted: ");
    for (i = 0; i < 8; i++) printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
