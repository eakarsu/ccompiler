int printf(const char *fmt, ...);

// Test: Pointer casting between types
// Cast int* to char*, char* to int*, void*, struct reinterpretation

struct Pair {
    int first;
    int second;
};

int main() {
    // Cast int pointer to char pointer to read individual bytes
    int x = 0x04030201;
    char *cp = (char *)&x;
    // Little-endian: first byte is least significant
    // EXPECT: byte0: 1
    printf("byte0: %d\n", (int)cp[0]);
    // EXPECT: byte1: 2
    printf("byte1: %d\n", (int)cp[1]);
    // EXPECT: byte2: 3
    printf("byte2: %d\n", (int)cp[2]);
    // EXPECT: byte3: 4
    printf("byte3: %d\n", (int)cp[3]);

    // Write bytes and read as int
    char bytes[4];
    bytes[0] = 10;
    bytes[1] = 0;
    bytes[2] = 0;
    bytes[3] = 0;
    int *ip = (int *)bytes;
    // EXPECT: bytes as int: 10
    printf("bytes as int: %d\n", *ip);

    // Write a larger value byte by byte
    char bytes2[4];
    bytes2[0] = 0;      // 256 = 0x00000100
    bytes2[1] = 1;
    bytes2[2] = 0;
    bytes2[3] = 0;
    int *ip2 = (int *)bytes2;
    // EXPECT: 256 from bytes: 256
    printf("256 from bytes: %d\n", *ip2);

    // Cast through void pointer
    int val = 12345;
    int *vp_orig = &val;
    char *vp_void = (char *)vp_orig;
    int *vp_back = (int *)vp_void;
    // EXPECT: roundtrip via void: 12345
    printf("roundtrip via void: %d\n", *vp_back);

    // Cast int array to char array and sum bytes
    int arr[2];
    arr[0] = 1;
    arr[1] = 2;
    char *byte_view = (char *)arr;
    int sum = 0;
    int i;
    // First int: value 1, so byte[0]=1, rest 0
    // Second int: value 2, so byte[4]=2, rest 0
    for (i = 0; i < 8; i++) {
        sum = sum + (int)byte_view[i];
    }
    // EXPECT: byte sum: 3
    printf("byte sum: %d\n", sum);

    // Interpret two ints as a Pair struct
    int two_ints[2];
    two_ints[0] = 42;
    two_ints[1] = 84;
    struct Pair *pp = (struct Pair *)two_ints;
    // EXPECT: pair.first: 42
    printf("pair.first: %d\n", pp->first);
    // EXPECT: pair.second: 84
    printf("pair.second: %d\n", pp->second);

    // Interpret Pair struct as int array
    struct Pair p;
    p.first = 100;
    p.second = 200;
    int *as_arr = (int *)&p;
    // EXPECT: as_arr[0]: 100
    printf("as_arr[0]: %d\n", as_arr[0]);
    // EXPECT: as_arr[1]: 200
    printf("as_arr[1]: %d\n", as_arr[1]);

    // Cast between struct pointer and int pointer
    struct Pair sp;
    sp.first = 777;
    sp.second = 888;
    int *sp_as_int = (int *)&sp;
    // EXPECT: struct as int[0]: 777
    printf("struct as int[0]: %d\n", sp_as_int[0]);
    // EXPECT: struct as int[1]: 888
    printf("struct as int[1]: %d\n", sp_as_int[1]);

    // Modify struct through int pointer
    sp_as_int[0] = 111;
    sp_as_int[1] = 222;
    // EXPECT: after int write: 111 222
    printf("after int write: %d %d\n", sp.first, sp.second);

    // Cast char array to int, modify, read back as chars
    char buf[4];
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    int *buf_as_int = (int *)buf;
    *buf_as_int = 5;
    // EXPECT: buf[0] after int write: 5
    printf("buf[0] after int write: %d\n", (int)buf[0]);
    // EXPECT: buf[1] after int write: 0
    printf("buf[1] after int write: %d\n", (int)buf[1]);

    // Array of ints viewed as chars: count non-zero bytes
    int data[3];
    data[0] = 1;
    data[1] = 256;
    data[2] = 65537;
    char *dcp = (char *)data;
    int nonzero = 0;
    for (i = 0; i < 12; i++) {
        if (dcp[i] != 0) {
            nonzero = nonzero + 1;
        }
    }
    // data[0]=1: byte0=1 (1 nonzero)
    // data[1]=256=0x100: byte4=0, byte5=1 (1 nonzero)
    // data[2]=65537=0x10001: byte8=1, byte9=0, byte10=1 (2 nonzero)
    // Total: 4
    // EXPECT: nonzero bytes: 4
    printf("nonzero bytes: %d\n", nonzero);

    // EXPECT: cast done
    printf("cast done\n");

    return 0;
}
