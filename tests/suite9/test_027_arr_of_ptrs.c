int printf(const char *fmt, ...);

// Test: Array of pointers vs pointer to array
// Demonstrates the difference and usage patterns

struct Item {
    int id;
    int weight;
};

int sum_via_ptr_arr(int **ptrs, int n) {
    printf("summing via ptr array\n");
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + *ptrs[i];
    }
    return s;
}

int main() {
    // Array of pointers to int
    int a = 10;
    int b = 20;
    int c = 30;
    int d = 40;
    int e = 50;

    int *arr_of_ptrs[5];
    arr_of_ptrs[0] = &a;
    arr_of_ptrs[1] = &b;
    arr_of_ptrs[2] = &c;
    arr_of_ptrs[3] = &d;
    arr_of_ptrs[4] = &e;

    // Access through array of pointers
    // EXPECT: arr_of_ptrs[0] = 10
    printf("arr_of_ptrs[0] = %d\n", *arr_of_ptrs[0]);
    // EXPECT: arr_of_ptrs[2] = 30
    printf("arr_of_ptrs[2] = %d\n", *arr_of_ptrs[2]);
    // EXPECT: arr_of_ptrs[4] = 50
    printf("arr_of_ptrs[4] = %d\n", *arr_of_ptrs[4]);

    // Modify original through array of pointers
    *arr_of_ptrs[1] = 200;
    // EXPECT: b after mod: 200
    printf("b after mod: %d\n", b);

    // Sum via function taking pointer array
    // Reset b for clean sum
    *arr_of_ptrs[1] = 20;
    // EXPECT: summing via ptr array
    int s = sum_via_ptr_arr(arr_of_ptrs, 5);
    // EXPECT: sum: 150
    printf("sum: %d\n", s);

    // Pointer to array (int (*)[4])
    int matrix[3][4];
    int i;
    int j;
    int v = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            matrix[i][j] = v;
            v = v + 1;
        }
    }

    // ptr_to_row points to a row (array of 4 ints)
    int (*ptr_to_row)[4] = matrix;
    // EXPECT: ptr_to_row[0][0] = 1
    printf("ptr_to_row[0][0] = %d\n", ptr_to_row[0][0]);
    // EXPECT: ptr_to_row[1][2] = 7
    printf("ptr_to_row[1][2] = %d\n", ptr_to_row[1][2]);
    // EXPECT: ptr_to_row[2][3] = 12
    printf("ptr_to_row[2][3] = %d\n", ptr_to_row[2][3]);

    // Advance pointer to array
    ptr_to_row = ptr_to_row + 1;
    // EXPECT: after advance [0][0] = 5
    printf("after advance [0][0] = %d\n", ptr_to_row[0][0]);
    // EXPECT: after advance [1][0] = 9
    printf("after advance [1][0] = %d\n", ptr_to_row[1][0]);

    // Array of pointers to strings
    char *names[4];
    names[0] = "alpha";
    names[1] = "beta";
    names[2] = "gamma";
    names[3] = "delta";
    // EXPECT: name 0: alpha
    printf("name 0: %s\n", names[0]);
    // EXPECT: name 1: beta
    printf("name 1: %s\n", names[1]);
    // EXPECT: name 2: gamma
    printf("name 2: %s\n", names[2]);
    // EXPECT: name 3: delta
    printf("name 3: %s\n", names[3]);

    // Array of pointers to structs
    struct Item items[3];
    items[0].id = 1;
    items[0].weight = 10;
    items[1].id = 2;
    items[1].weight = 25;
    items[2].id = 3;
    items[2].weight = 15;

    struct Item *item_ptrs[3];
    item_ptrs[0] = &items[0];
    item_ptrs[1] = &items[1];
    item_ptrs[2] = &items[2];

    // Find heaviest via pointer array
    int max_w = 0;
    int max_id = 0;
    for (i = 0; i < 3; i++) {
        if (item_ptrs[i]->weight > max_w) {
            max_w = item_ptrs[i]->weight;
            max_id = item_ptrs[i]->id;
        }
    }
    // EXPECT: heaviest id: 2 weight: 25
    printf("heaviest id: %d weight: %d\n", max_id, max_w);

    // Swap pointers in array (sort by weight descending)
    struct Item *tmp;
    for (i = 0; i < 2; i++) {
        for (j = i + 1; j < 3; j++) {
            if (item_ptrs[j]->weight > item_ptrs[i]->weight) {
                tmp = item_ptrs[i];
                item_ptrs[i] = item_ptrs[j];
                item_ptrs[j] = tmp;
            }
        }
    }
    // EXPECT: sorted: 25 15 10
    printf("sorted: %d %d %d\n",
           item_ptrs[0]->weight,
           item_ptrs[1]->weight,
           item_ptrs[2]->weight);

    // EXPECT: arr_of_ptrs done
    printf("arr_of_ptrs done\n");

    return 0;
}
