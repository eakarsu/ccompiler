int printf(const char *fmt, ...);

// Variable-Length Integer Encoding (Varint) - similar to protobuf varint

// Encode an integer using varint encoding into a byte buffer.
// Each byte uses 7 bits of data and 1 continuation bit (high bit).
// Returns number of bytes written.
int varint_encode(int value, int *buf) {
    int count = 0;
    int v = value;
    // Handle negative by converting to positive for simplicity
    // We only handle non-negative values here
    if (v == 0) {
        buf[0] = 0;
        return 1;
    }
    while (v > 0) {
        int byte_val = v & 127; // low 7 bits
        v = v >> 7;
        if (v > 0) {
            byte_val = byte_val | 128; // set continuation bit
        }
        buf[count] = byte_val;
        count = count + 1;
    }
    return count;
}

// Decode a varint from buffer. Returns the decoded value.
// Sets *bytes_read to number of bytes consumed.
int varint_decode(int *buf, int *bytes_read) {
    int result = 0;
    int shift = 0;
    int i = 0;
    while (1) {
        int byte_val = buf[i];
        result = result | ((byte_val & 127) << shift);
        i = i + 1;
        if ((byte_val & 128) == 0) break;
        shift = shift + 7;
    }
    *bytes_read = i;
    return result;
}

// Encode a zigzag value (maps signed to unsigned for more efficient varint)
int zigzag_encode(int n) {
    return (n << 1) ^ (n >> 31);
}

int zigzag_decode(int n) {
    return (n >> 1) ^ (-(n & 1));
}

// Encode multiple values into a buffer, returns total bytes
int encode_array(int *values, int count, int *buf) {
    int pos = 0;
    int i = 0;
    while (i < count) {
        int written = varint_encode(values[i], buf + pos);
        pos = pos + written;
        i = i + 1;
    }
    return pos;
}

// Decode multiple values from a buffer
int decode_array(int *buf, int total_bytes, int *values) {
    int pos = 0;
    int count = 0;
    while (pos < total_bytes) {
        int bytes_read = 0;
        values[count] = varint_decode(buf + pos, &bytes_read);
        pos = pos + bytes_read;
        count = count + 1;
    }
    return count;
}

int main() {
    int buf[32];

    // Test 1: Small value (fits in 1 byte)
    int n1 = varint_encode(42, buf);
    printf("42 needs %d bytes\n", n1); // EXPECT: 42 needs 1 bytes
    printf("42 encoded: %d\n", buf[0]); // EXPECT: 42 encoded: 42

    int bytes_read = 0;
    int val = varint_decode(buf, &bytes_read);
    printf("Decoded: %d read: %d\n", val, bytes_read); // EXPECT: Decoded: 42 read: 1

    // Test 2: Value needing 2 bytes (128-16383)
    int n2 = varint_encode(300, buf);
    printf("300 needs %d bytes\n", n2); // EXPECT: 300 needs 2 bytes
    val = varint_decode(buf, &bytes_read);
    printf("300 decoded: %d\n", val); // EXPECT: 300 decoded: 300

    // Test 3: Larger value
    int n3 = varint_encode(16384, buf);
    printf("16384 needs %d bytes\n", n3); // EXPECT: 16384 needs 3 bytes
    val = varint_decode(buf, &bytes_read);
    printf("16384 decoded: %d\n", val); // EXPECT: 16384 decoded: 16384

    // Test 4: Zero
    int n4 = varint_encode(0, buf);
    printf("0 needs %d bytes\n", n4); // EXPECT: 0 needs 1 bytes
    val = varint_decode(buf, &bytes_read);
    printf("0 decoded: %d\n", val); // EXPECT: 0 decoded: 0

    // Test 5: Value 127 (max single byte)
    int n5 = varint_encode(127, buf);
    printf("127 needs %d bytes\n", n5); // EXPECT: 127 needs 1 bytes
    val = varint_decode(buf, &bytes_read);
    printf("127 decoded: %d\n", val); // EXPECT: 127 decoded: 127

    // Test 6: Value 128 (needs 2 bytes)
    int n6 = varint_encode(128, buf);
    printf("128 needs %d bytes\n", n6); // EXPECT: 128 needs 2 bytes
    val = varint_decode(buf, &bytes_read);
    printf("128 decoded: %d\n", val); // EXPECT: 128 decoded: 128

    // Test 7: Zigzag encoding for signed values
    int zz1 = zigzag_encode(0);
    int zz2 = zigzag_encode(-1);
    int zz3 = zigzag_encode(1);
    int zz4 = zigzag_encode(-2);
    int zz5 = zigzag_encode(2);
    printf("Zigzag 0: %d\n", zz1); // EXPECT: Zigzag 0: 0
    printf("Zigzag -1: %d\n", zz2); // EXPECT: Zigzag -1: 1
    printf("Zigzag 1: %d\n", zz3); // EXPECT: Zigzag 1: 2
    printf("Zigzag -2: %d\n", zz4); // EXPECT: Zigzag -2: 3
    printf("Zigzag 2: %d\n", zz5); // EXPECT: Zigzag 2: 4

    // Test 8: Zigzag roundtrip
    int zd1 = zigzag_decode(zz2);
    int zd2 = zigzag_decode(zz4);
    printf("Zigzag decode -1: %d\n", zd1); // EXPECT: Zigzag decode -1: -1
    printf("Zigzag decode -2: %d\n", zd2); // EXPECT: Zigzag decode -2: -2

    // Test 9: Array encoding
    int values[5];
    values[0] = 1; values[1] = 127; values[2] = 300; values[3] = 0; values[4] = 16384;
    int total_bytes = encode_array(values, 5, buf);
    printf("Array total bytes: %d\n", total_bytes); // EXPECT: Array total bytes: 8

    int decoded_vals[5];
    int decoded_count = decode_array(buf, total_bytes, decoded_vals);
    printf("Decoded count: %d\n", decoded_count); // EXPECT: Decoded count: 5
    printf("Decoded vals: %d %d %d %d %d\n", decoded_vals[0], decoded_vals[1], decoded_vals[2], decoded_vals[3], decoded_vals[4]);
    // EXPECT: Decoded vals: 1 127 300 0 16384

    printf("Varint done\n"); // EXPECT: Varint done
    return 0;
}
