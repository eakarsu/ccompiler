int printf(const char *fmt, ...);

int global_arr[10];

void fill_array(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = (i + 1) * 10;
    }
}

int main(void) {
    // Basic pointer dereference in expression
    int a = 42;
    int *p = &a;
    int b = *p + 8;
    // EXPECT: 50
    printf("%d\n", b);

    // *p++ : dereference then advance pointer
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    int *q = arr;
    int v1 = *q++;
    // v1 = arr[0] = 10, q now points to arr[1]
    // EXPECT: 10
    printf("%d\n", v1);
    // EXPECT: 20
    printf("%d\n", *q);

    // *q++ again
    int v2 = *q++;
    // v2 = arr[1] = 20, q now points to arr[2]
    // EXPECT: 20
    printf("%d\n", v2);
    // EXPECT: 30
    printf("%d\n", *q);

    // (*p)++ : increment value pointed to
    int val = 100;
    int *vp = &val;
    (*vp)++;
    // EXPECT: 101
    printf("%d\n", val);
    (*vp)++;
    (*vp)++;
    // EXPECT: 103
    printf("%d\n", val);

    // *++p : advance pointer then dereference
    int *r = arr;
    int v3 = *++r;
    // r advances to arr[1], then dereference = 20
    // EXPECT: 20
    printf("%d\n", v3);
    int v4 = *++r;
    // r advances to arr[2] = 30
    // EXPECT: 30
    printf("%d\n", v4);

    // ++*p : increment the value at pointer
    int incval = 50;
    int *ip = &incval;
    int v5 = ++*ip;
    // value becomes 51, v5 = 51
    // EXPECT: 51
    printf("%d\n", v5);
    // EXPECT: 51
    printf("%d\n", incval);

    // array[*p] : pointer value as index
    int idx = 3;
    int *idxp = &idx;
    int data[5];
    data[0] = 100;
    data[1] = 200;
    data[2] = 300;
    data[3] = 400;
    data[4] = 500;
    // EXPECT: 400
    printf("%d\n", data[*idxp]);

    *idxp = 1;
    // EXPECT: 200
    printf("%d\n", data[*idxp]);

    // **pp : double pointer dereference
    int dval = 77;
    int *dp = &dval;
    int **dpp = &dp;
    // EXPECT: 77
    printf("%d\n", **dpp);

    // **pp + 1
    int dval2 = **dpp + 1;
    // EXPECT: 78
    printf("%d\n", dval2);

    // Modify through double pointer
    **dpp = 99;
    // EXPECT: 99
    printf("%d\n", dval);

    // Pointer subtraction (distance between elements)
    int *start = &arr[0];
    int *end = &arr[4];
    int dist = (int)(end - start);
    // EXPECT: 4
    printf("%d\n", dist);

    // Pointer comparison
    int lt = (start < end);
    // EXPECT: 1
    printf("%d\n", lt);

    // Pointer arithmetic in loop
    fill_array(global_arr, 5);
    int *gp = global_arr;
    int gsum = 0;
    for (int i = 0; i < 5; i++) {
        gsum = gsum + *(gp + i);
    }
    // 10+20+30+40+50 = 150
    // EXPECT: 150
    printf("%d\n", gsum);

    // Pointer walk with post-increment
    int *wp = global_arr;
    int wsum = 0;
    for (int i = 0; i < 5; i++) {
        wsum = wsum + *wp++;
    }
    // EXPECT: 150
    printf("%d\n", wsum);

    // Negative pointer offset
    int *mid = &arr[3];
    int behind = *(mid - 2);
    // arr[1] = 20
    // EXPECT: 20
    printf("%d\n", behind);

    // Complex: arr[*(idx_ptr)]  where idx comes from computation
    int indices[3];
    indices[0] = 4;
    indices[1] = 2;
    indices[2] = 0;
    int *iip = indices;
    // EXPECT: 50
    printf("%d\n", arr[*iip]);
    iip++;
    // EXPECT: 30
    printf("%d\n", arr[*iip]);
    iip++;
    // EXPECT: 10
    printf("%d\n", arr[*iip]);

    // Pointer to struct member
    struct pair { int first; int second; };
    struct pair sp;
    sp.first = 11;
    sp.second = 22;
    int *fld = &sp.first;
    // EXPECT: 11
    printf("%d\n", *fld);

    // Pointer to array element, modify and read back
    int mod_arr[3];
    mod_arr[0] = 1;
    mod_arr[1] = 2;
    mod_arr[2] = 3;
    int *mp = &mod_arr[1];
    *mp = 99;
    // EXPECT: 99
    printf("%d\n", mod_arr[1]);

    return 0;
}
