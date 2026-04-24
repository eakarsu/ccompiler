// EXPECT: === Serialization Tests ===
// EXPECT: === Serialize Int ===
// EXPECT: Serialize 305419896: bytes=78 56 34 12
// EXPECT: Deserialize: 305419896 (match=1)
// EXPECT: === Serialize Negative ===
// EXPECT: Serialize -1: bytes=FF FF FF FF
// EXPECT: Deserialize: -1 (match=1)
// EXPECT: Serialize -12345: bytes=C7 CF FF FF
// EXPECT: Deserialize: -12345 (match=1)
// EXPECT: === Serialize Point ===
// EXPECT: Original: Point(100, 200)
// EXPECT: Serialized 8 bytes
// EXPECT: Deserialized: Point(100, 200) (match=1)
// EXPECT: === Serialize Record ===
// EXPECT: Original: Record(id=42, x=10, y=20, z=30, flags=255)
// EXPECT: Serialized 20 bytes
// EXPECT: Deserialized: Record(id=42, x=10, y=20, z=30, flags=255) (match=1)
// EXPECT: === Endian Swap ===
// EXPECT: LE 0x12345678 -> BE 0x78563412
// EXPECT: Round-trip: 0x12345678 (match=1)
// EXPECT: === Array Serialization ===
// EXPECT: Serialized 5 ints (20 bytes)
// EXPECT: Deserialized: 10 20 30 40 50
// EXPECT: Array match: 1
// EXPECT: === Multiple Records ===
// EXPECT: Record 0: id=1, x=10, y=20, z=30, flags=1
// EXPECT: Record 1: id=2, x=20, y=30, z=40, flags=2
// EXPECT: Record 2: id=3, x=30, y=40, z=50, flags=3
// EXPECT: All records match: 1
// EXPECT: All serialization tests passed!

int printf(const char *fmt, ...);

// Serialize a 32-bit int to little-endian bytes
void serialize_int(int value, char *buf) {
    buf[0] = (char)(value & 0xFF);
    buf[1] = (char)((value >> 8) & 0xFF);
    buf[2] = (char)((value >> 16) & 0xFF);
    buf[3] = (char)((value >> 24) & 0xFF);
}

// Deserialize a 32-bit int from little-endian bytes
int deserialize_int(const char *buf) {
    int v = (buf[0] & 0xFF);
    v = v | ((buf[1] & 0xFF) << 8);
    v = v | ((buf[2] & 0xFF) << 16);
    v = v | ((buf[3] & 0xFF) << 24);
    return v;
}

// Byte-swap for endian conversion
int swap_endian(int value) {
    int b0 = (value & 0xFF) << 24;
    int b1 = ((value >> 8) & 0xFF) << 16;
    int b2 = ((value >> 16) & 0xFF) << 8;
    int b3 = (value >> 24) & 0xFF;
    return b0 | b1 | b2 | b3;
}

struct Point {
    int x;
    int y;
};

struct Record {
    int id;
    int x;
    int y;
    int z;
    int flags;
};

// Serialize a Point to bytes, return number of bytes written
int serialize_point(struct Point *p, char *buf) {
    serialize_int(p->x, buf);
    serialize_int(p->y, buf + 4);
    return 8;
}

// Deserialize a Point from bytes
int deserialize_point(const char *buf, struct Point *p) {
    p->x = deserialize_int(buf);
    p->y = deserialize_int(buf + 4);
    return 8;
}

// Serialize a Record
int serialize_record(struct Record *r, char *buf) {
    serialize_int(r->id, buf);
    serialize_int(r->x, buf + 4);
    serialize_int(r->y, buf + 8);
    serialize_int(r->z, buf + 12);
    serialize_int(r->flags, buf + 16);
    return 20;
}

// Deserialize a Record
int deserialize_record(const char *buf, struct Record *r) {
    r->id = deserialize_int(buf);
    r->x = deserialize_int(buf + 4);
    r->y = deserialize_int(buf + 8);
    r->z = deserialize_int(buf + 12);
    r->flags = deserialize_int(buf + 16);
    return 20;
}

// Serialize an array of ints
int serialize_int_array(int *arr, int count, char *buf) {
    int i;
    for (i = 0; i < count; i++) {
        serialize_int(arr[i], buf + i * 4);
    }
    return count * 4;
}

// Deserialize an array of ints
int deserialize_int_array(const char *buf, int *arr, int count) {
    int i;
    for (i = 0; i < count; i++) {
        arr[i] = deserialize_int(buf + i * 4);
    }
    return count * 4;
}

int main(void) {
    char buf[256];
    int val;
    int i;

    printf("=== Serialization Tests ===\n");

    // Basic int serialization
    printf("=== Serialize Int ===\n");
    val = 0x12345678;
    serialize_int(val, buf);
    printf("Serialize %d: bytes=%02X %02X %02X %02X\n",
           val, buf[0] & 0xFF, buf[1] & 0xFF, buf[2] & 0xFF, buf[3] & 0xFF);
    int result = deserialize_int(buf);
    printf("Deserialize: %d (match=%d)\n", result, result == val);

    // Negative numbers
    printf("=== Serialize Negative ===\n");
    val = -1;
    serialize_int(val, buf);
    printf("Serialize %d: bytes=%02X %02X %02X %02X\n",
           val, buf[0] & 0xFF, buf[1] & 0xFF, buf[2] & 0xFF, buf[3] & 0xFF);
    result = deserialize_int(buf);
    printf("Deserialize: %d (match=%d)\n", result, result == val);

    val = -12345;
    serialize_int(val, buf);
    printf("Serialize %d: bytes=%02X %02X %02X %02X\n",
           val, buf[0] & 0xFF, buf[1] & 0xFF, buf[2] & 0xFF, buf[3] & 0xFF);
    result = deserialize_int(buf);
    printf("Deserialize: %d (match=%d)\n", result, result == val);

    // Point serialization
    printf("=== Serialize Point ===\n");
    struct Point p1;
    struct Point p2;
    p1.x = 100;
    p1.y = 200;
    printf("Original: Point(%d, %d)\n", p1.x, p1.y);
    int nbytes = serialize_point(&p1, buf);
    printf("Serialized %d bytes\n", nbytes);
    deserialize_point(buf, &p2);
    int match = (p1.x == p2.x && p1.y == p2.y);
    printf("Deserialized: Point(%d, %d) (match=%d)\n", p2.x, p2.y, match);

    // Record serialization
    printf("=== Serialize Record ===\n");
    struct Record r1;
    struct Record r2;
    r1.id = 42;
    r1.x = 10;
    r1.y = 20;
    r1.z = 30;
    r1.flags = 255;
    printf("Original: Record(id=%d, x=%d, y=%d, z=%d, flags=%d)\n",
           r1.id, r1.x, r1.y, r1.z, r1.flags);
    nbytes = serialize_record(&r1, buf);
    printf("Serialized %d bytes\n", nbytes);
    deserialize_record(buf, &r2);
    match = (r1.id == r2.id && r1.x == r2.x && r1.y == r2.y &&
             r1.z == r2.z && r1.flags == r2.flags);
    printf("Deserialized: Record(id=%d, x=%d, y=%d, z=%d, flags=%d) (match=%d)\n",
           r2.id, r2.x, r2.y, r2.z, r2.flags, match);

    // Endian swap
    printf("=== Endian Swap ===\n");
    val = 0x12345678;
    int swapped = swap_endian(val);
    printf("LE 0x%X -> BE 0x%X\n", val, swapped);
    int back = swap_endian(swapped);
    printf("Round-trip: 0x%X (match=%d)\n", back, back == val);

    // Array serialization
    printf("=== Array Serialization ===\n");
    int arr_in[5];
    int arr_out[5];
    arr_in[0] = 10;
    arr_in[1] = 20;
    arr_in[2] = 30;
    arr_in[3] = 40;
    arr_in[4] = 50;
    nbytes = serialize_int_array(arr_in, 5, buf);
    printf("Serialized 5 ints (%d bytes)\n", nbytes);
    deserialize_int_array(buf, arr_out, 5);
    printf("Deserialized:");
    for (i = 0; i < 5; i++) {
        printf(" %d", arr_out[i]);
    }
    printf("\n");
    match = 1;
    for (i = 0; i < 5; i++) {
        if (arr_in[i] != arr_out[i]) match = 0;
    }
    printf("Array match: %d\n", match);

    // Multiple records
    printf("=== Multiple Records ===\n");
    struct Record recs_in[3];
    struct Record recs_out[3];
    for (i = 0; i < 3; i++) {
        recs_in[i].id = i + 1;
        recs_in[i].x = (i + 1) * 10;
        recs_in[i].y = (i + 1) * 10 + 10;
        recs_in[i].z = (i + 1) * 10 + 20;
        recs_in[i].flags = i + 1;
    }
    for (i = 0; i < 3; i++) {
        serialize_record(&recs_in[i], buf + i * 20);
    }
    for (i = 0; i < 3; i++) {
        deserialize_record(buf + i * 20, &recs_out[i]);
    }
    match = 1;
    for (i = 0; i < 3; i++) {
        printf("Record %d: id=%d, x=%d, y=%d, z=%d, flags=%d\n",
               i, recs_out[i].id, recs_out[i].x, recs_out[i].y,
               recs_out[i].z, recs_out[i].flags);
        if (recs_in[i].id != recs_out[i].id) match = 0;
        if (recs_in[i].x != recs_out[i].x) match = 0;
        if (recs_in[i].y != recs_out[i].y) match = 0;
        if (recs_in[i].z != recs_out[i].z) match = 0;
        if (recs_in[i].flags != recs_out[i].flags) match = 0;
    }
    printf("All records match: %d\n", match);

    printf("All serialization tests passed!\n");
    return 0;
}
