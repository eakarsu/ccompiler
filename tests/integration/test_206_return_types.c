int printf(const char *fmt, ...);
// EXPECT: return_int(5)=11\nreturn_int(-3)=-5\nreturn_char(0)=A\nreturn_char(25)=Z\nbool(5,3)=1\nbool(2,7)=0\nswitch(0)=10\nswitch(1)=20\nswitch(5)=-1\ncond(5)=5\ncond(-8)=8\nfind(9)=3\nfind(99)=-1\nptr[4]=12\ncomplex(3,7,5)=14\nchain(-250)=50\nchain(350)=50\nmulti(-5)=-1\nmulti(0)=0\nmulti(5)=1\nmulti(50)=2\nmulti(500)=3
// Test 206: Functions returning different types

int return_int(int x) {
    return x * 2 + 1;
}

char return_char(int x) {
    return (char)(x + 65);
}

int return_bool_as_int(int a, int b) {
    if (a > b) return 1;
    return 0;
}

int return_from_switch(int x) {
    if (x == 0) return 10;
    if (x == 1) return 20;
    if (x == 2) return 30;
    return -1;
}

int return_conditional(int x) {
    return (x > 0) ? x : -x;
}

int return_early(int arr[10], int target) {
    int i = 0;
    while (i < 10) {
        if (arr[i] == target) return i;
        i = i + 1;
    }
    return -1;
}

int *return_pointer(int *arr, int index) {
    return &arr[index];
}

int return_complex(int a, int b, int c) {
    int result = a;
    if (b > a && b > c) result = b;
    if (c > a && c > b) result = c;
    return result * 2;
}

int chain_return(int x) {
    if (x < 0) return chain_return(-x);
    if (x > 100) return chain_return(x - 100);
    return x;
}

int multi_return_path(int x) {
    if (x < 0) return -1;
    if (x == 0) return 0;
    if (x < 10) return 1;
    if (x < 100) return 2;
    return 3;
}

int main(void) {
    printf("return_int(5)=%d\n", return_int(5));
    printf("return_int(-3)=%d\n", return_int(-3));

    char c = return_char(0);
    printf("return_char(0)=%c\n", c);
    c = return_char(25);
    printf("return_char(25)=%c\n", c);

    printf("bool(5,3)=%d\n", return_bool_as_int(5, 3));
    printf("bool(2,7)=%d\n", return_bool_as_int(2, 7));

    printf("switch(0)=%d\n", return_from_switch(0));
    printf("switch(1)=%d\n", return_from_switch(1));
    printf("switch(5)=%d\n", return_from_switch(5));

    printf("cond(5)=%d\n", return_conditional(5));
    printf("cond(-8)=%d\n", return_conditional(-8));

    int arr[10];
    int i = 0;
    while (i < 10) { arr[i] = i * 3; i = i + 1; }
    printf("find(9)=%d\n", return_early(arr, 9));
    printf("find(99)=%d\n", return_early(arr, 99));

    int *p = return_pointer(arr, 4);
    printf("ptr[4]=%d\n", *p);

    printf("complex(3,7,5)=%d\n", return_complex(3, 7, 5));
    printf("chain(-250)=%d\n", chain_return(-250));
    printf("chain(350)=%d\n", chain_return(350));

    printf("multi(-5)=%d\n", multi_return_path(-5));
    printf("multi(0)=%d\n", multi_return_path(0));
    printf("multi(5)=%d\n", multi_return_path(5));
    printf("multi(50)=%d\n", multi_return_path(50));
    printf("multi(500)=%d\n", multi_return_path(500));

    return 0;
}
