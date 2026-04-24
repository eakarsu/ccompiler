int printf(const char *fmt, ...);

// Test: sizeof in expressions

struct SmallStruct {
    int x;
    int y;
};

struct BigStruct {
    int a;
    int b;
    int c;
    int d;
    char name[16];
};

struct Nested {
    struct SmallStruct s;
    int z;
};

int main() {
    int i;
    char c;
    int arr10[10];
    char carr[20];
    int *p;
    struct SmallStruct ss;
    struct BigStruct bs;
    struct Nested ns;
    int result;

    // sizeof basic types
    printf("%d\n", (int)sizeof(int));    // EXPECT: 4
    printf("%d\n", (int)sizeof(char));   // EXPECT: 1

    // sizeof pointer (8 bytes on 64-bit)
    printf("%d\n", (int)sizeof(int *));  // EXPECT: 8
    printf("%d\n", (int)sizeof(char *)); // EXPECT: 8

    // sizeof arrays
    printf("%d\n", (int)sizeof(arr10)); // EXPECT: 40
    printf("%d\n", (int)sizeof(carr));  // EXPECT: 20

    // sizeof structs
    printf("%d\n", (int)sizeof(struct SmallStruct)); // EXPECT: 8
    printf("%d\n", (int)sizeof(struct BigStruct));   // EXPECT: 32

    // sizeof nested struct
    printf("%d\n", (int)sizeof(struct Nested)); // EXPECT: 12

    // sizeof variable
    printf("%d\n", (int)sizeof(i)); // EXPECT: 4
    printf("%d\n", (int)sizeof(c)); // EXPECT: 1
    printf("%d\n", (int)sizeof(p)); // EXPECT: 8

    // sizeof in arithmetic
    result = (int)sizeof(int) + (int)sizeof(char);
    printf("%d\n", result); // EXPECT: 5

    result = (int)sizeof(arr10) / (int)sizeof(int);
    printf("%d\n", result); // EXPECT: 10

    result = (int)sizeof(carr) / (int)sizeof(char);
    printf("%d\n", result); // EXPECT: 20

    // sizeof as array count
    int nums[5];
    int count;
    count = (int)sizeof(nums) / (int)sizeof(nums[0]);
    printf("%d\n", count); // EXPECT: 5

    // sizeof in conditionals
    if ((int)sizeof(int) == 4) {
        printf("%d\n", 1); // EXPECT: 1
    }

    if ((int)sizeof(char) < (int)sizeof(int)) {
        printf("%d\n", 1); // EXPECT: 1
    }

    // sizeof with pointer vs array distinction
    int local_arr[8];
    printf("%d\n", (int)sizeof(local_arr)); // EXPECT: 32

    // sizeof in loop bound
    int data[4];
    data[0] = 10;
    data[1] = 20;
    data[2] = 30;
    data[3] = 40;
    int sum;
    sum = 0;
    for (i = 0; i < (int)sizeof(data) / (int)sizeof(data[0]); i = i + 1) {
        sum = sum + data[i];
    }
    printf("%d\n", sum); // EXPECT: 100

    // sizeof expression (not evaluated)
    i = 10;
    result = (int)sizeof(i + 1);
    printf("%d\n", result); // EXPECT: 4
    printf("%d\n", i);      // EXPECT: 10

    // sizeof struct members
    printf("%d\n", (int)sizeof(ss.x)); // EXPECT: 4
    printf("%d\n", (int)sizeof(ss.y)); // EXPECT: 4

    // sizeof used in multiplication
    result = 3 * (int)sizeof(int);
    printf("%d\n", result); // EXPECT: 12

    // sizeof comparison
    if ((int)sizeof(struct BigStruct) > (int)sizeof(struct SmallStruct)) {
        printf("%d\n", 1); // EXPECT: 1
    }

    // sizeof char array element
    printf("%d\n", (int)sizeof(carr[0])); // EXPECT: 1

    // sizeof in ternary
    result = ((int)sizeof(int) == 4) ? 100 : 200;
    printf("%d\n", result); // EXPECT: 100

    // sizeof struct variable
    printf("%d\n", (int)sizeof(ss)); // EXPECT: 8
    printf("%d\n", (int)sizeof(bs)); // EXPECT: 32
    printf("%d\n", (int)sizeof(ns)); // EXPECT: 12

    return 0;
}
