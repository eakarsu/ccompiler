int printf(const char *fmt, ...);
// EXPECT: 1:100\n2:200\n3:300\n4:-1\n5:3\n6:3\n7:1\n8:-1\n9:2\n10:1\n11:250\n12:3\n13:10\n14:20\n15:30\n16:1\n17:2\n18:3\n19:50\n20:99\n21:1\n22:1\n23:0\n24:150\n25:50\n26:10\n27:30\n28:0\n29:25\n30:841\n31:0\n32:100\n33:-1\n34:300\n35:100\n36:420\n37:740\n38:-1\n39:3\n40:4\n41:3\n42:2\n43:1\n44:0\n45:4\n46:10\n47:10\n48:-1\n49:190\n50:1000\n51:11\n52:-1\n53:0\n54:66\n55:385
// =============================================================================
// TEST 021: Hash Table with Open Addressing
// =============================================================================
// Tests hash table implementation using arrays with open addressing (linear
// probing). Covers insert, lookup, delete, collision handling, rehashing
// simulation, load factor tracking, and various edge cases.
// =============================================================================

// -----------------------------------------------------------------------------
// Constants for hash table
// -----------------------------------------------------------------------------
int HT_SIZE;
int HT_EMPTY;
int HT_DELETED;

// -----------------------------------------------------------------------------
// Hash table storage: parallel arrays for keys and values
// -----------------------------------------------------------------------------
int ht_keys[64];
int ht_vals[64];
int ht_status[64]; // 0=empty, 1=occupied, 2=deleted
int ht_count;

// -----------------------------------------------------------------------------
// Initialize hash table
// -----------------------------------------------------------------------------
void ht_init(void) {
    int i;
    HT_SIZE = 64;
    HT_EMPTY = 0;
    HT_DELETED = 2;
    ht_count = 0;
    for (i = 0; i < 64; i = i + 1) {
        ht_keys[i] = 0;
        ht_vals[i] = 0;
        ht_status[i] = 0;
    }
}

// -----------------------------------------------------------------------------
// Hash function: simple modular hash
// -----------------------------------------------------------------------------
int ht_hash(int key) {
    int h;
    h = key;
    if (h < 0) h = -h;
    return h % 64;
}

// -----------------------------------------------------------------------------
// Secondary hash for double hashing
// -----------------------------------------------------------------------------
int ht_hash2(int key) {
    int h;
    h = key;
    if (h < 0) h = -h;
    h = 7 - (h % 7);
    if (h == 0) h = 1;
    return h;
}

// -----------------------------------------------------------------------------
// Insert key-value pair (linear probing)
// Returns 1 on success, 0 if table full
// -----------------------------------------------------------------------------
int ht_insert(int key, int val) {
    int h;
    int i;
    int idx;
    if (ht_count >= 60) return 0; // keep load factor reasonable
    h = ht_hash(key);
    for (i = 0; i < 64; i = i + 1) {
        idx = (h + i) % 64;
        if (ht_status[idx] == 0 || ht_status[idx] == 2) {
            ht_keys[idx] = key;
            ht_vals[idx] = val;
            ht_status[idx] = 1;
            ht_count = ht_count + 1;
            return 1;
        }
        if (ht_status[idx] == 1 && ht_keys[idx] == key) {
            // Update existing key
            ht_vals[idx] = val;
            return 1;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Lookup key, returns value or -1 if not found
// -----------------------------------------------------------------------------
int ht_lookup(int key) {
    int h;
    int i;
    int idx;
    h = ht_hash(key);
    for (i = 0; i < 64; i = i + 1) {
        idx = (h + i) % 64;
        if (ht_status[idx] == 0) return -1;
        if (ht_status[idx] == 1 && ht_keys[idx] == key) {
            return ht_vals[idx];
        }
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Delete key, returns 1 if deleted, 0 if not found
// -----------------------------------------------------------------------------
int ht_delete(int key) {
    int h;
    int i;
    int idx;
    h = ht_hash(key);
    for (i = 0; i < 64; i = i + 1) {
        idx = (h + i) % 64;
        if (ht_status[idx] == 0) return 0;
        if (ht_status[idx] == 1 && ht_keys[idx] == key) {
            ht_status[idx] = 2;
            ht_count = ht_count - 1;
            return 1;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Count occupied slots
// -----------------------------------------------------------------------------
int ht_occupied(void) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < 64; i = i + 1) {
        if (ht_status[i] == 1) c = c + 1;
    }
    return c;
}

// -----------------------------------------------------------------------------
// Count deleted slots
// -----------------------------------------------------------------------------
int ht_deleted_count(void) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < 64; i = i + 1) {
        if (ht_status[i] == 2) c = c + 1;
    }
    return c;
}

// -----------------------------------------------------------------------------
// Sum all values in the table
// -----------------------------------------------------------------------------
int ht_sum_values(void) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < 64; i = i + 1) {
        if (ht_status[i] == 1) {
            s = s + ht_vals[i];
        }
    }
    return s;
}

// -----------------------------------------------------------------------------
// Find max value in table
// -----------------------------------------------------------------------------
int ht_max_value(void) {
    int i;
    int mx;
    int found;
    mx = 0;
    found = 0;
    for (i = 0; i < 64; i = i + 1) {
        if (ht_status[i] == 1) {
            if (found == 0 || ht_vals[i] > mx) {
                mx = ht_vals[i];
                found = 1;
            }
        }
    }
    return mx;
}

// -----------------------------------------------------------------------------
// Find min value in table
// -----------------------------------------------------------------------------
int ht_min_value(void) {
    int i;
    int mn;
    int found;
    mn = 0;
    found = 0;
    for (i = 0; i < 64; i = i + 1) {
        if (ht_status[i] == 1) {
            if (found == 0 || ht_vals[i] < mn) {
                mn = ht_vals[i];
                found = 1;
            }
        }
    }
    return mn;
}

// -----------------------------------------------------------------------------
// Check if key exists
// -----------------------------------------------------------------------------
int ht_contains(int key) {
    if (ht_lookup(key) != -1) return 1;
    return 0;
}

// -----------------------------------------------------------------------------
// Clear the entire table
// -----------------------------------------------------------------------------
void ht_clear(void) {
    int i;
    for (i = 0; i < 64; i = i + 1) {
        ht_keys[i] = 0;
        ht_vals[i] = 0;
        ht_status[i] = 0;
    }
    ht_count = 0;
}

// -----------------------------------------------------------------------------
// Count how many probes needed to find a key
// -----------------------------------------------------------------------------
int ht_probe_count(int key) {
    int h;
    int i;
    int idx;
    h = ht_hash(key);
    for (i = 0; i < 64; i = i + 1) {
        idx = (h + i) % 64;
        if (ht_status[idx] == 0) return i + 1;
        if (ht_status[idx] == 1 && ht_keys[idx] == key) return i + 1;
    }
    return 64;
}

// -----------------------------------------------------------------------------
// Second hash table for double hashing tests
// -----------------------------------------------------------------------------
int ht2_keys[32];
int ht2_vals[32];
int ht2_status[32];
int ht2_count;

void ht2_init(void) {
    int i;
    ht2_count = 0;
    for (i = 0; i < 32; i = i + 1) {
        ht2_keys[i] = 0;
        ht2_vals[i] = 0;
        ht2_status[i] = 0;
    }
}

int ht2_hash(int key) {
    int h;
    h = key;
    if (h < 0) h = -h;
    return h % 32;
}

int ht2_insert(int key, int val) {
    int h;
    int step;
    int i;
    int idx;
    if (ht2_count >= 28) return 0;
    h = ht2_hash(key);
    step = ht_hash2(key); // use double hashing step
    for (i = 0; i < 32; i = i + 1) {
        idx = (h + i * step) % 32;
        if (ht2_status[idx] == 0 || ht2_status[idx] == 2) {
            ht2_keys[idx] = key;
            ht2_vals[idx] = val;
            ht2_status[idx] = 1;
            ht2_count = ht2_count + 1;
            return 1;
        }
        if (ht2_status[idx] == 1 && ht2_keys[idx] == key) {
            ht2_vals[idx] = val;
            return 1;
        }
    }
    return 0;
}

int ht2_lookup(int key) {
    int h;
    int step;
    int i;
    int idx;
    h = ht2_hash(key);
    step = ht_hash2(key);
    for (i = 0; i < 32; i = i + 1) {
        idx = (h + i * step) % 32;
        if (ht2_status[idx] == 0) return -1;
        if (ht2_status[idx] == 1 && ht2_keys[idx] == key) {
            return ht2_vals[idx];
        }
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Frequency counter using hash table
// -----------------------------------------------------------------------------
int freq_keys[64];
int freq_vals[64];
int freq_status[64];
int freq_count;

void freq_init(void) {
    int i;
    freq_count = 0;
    for (i = 0; i < 64; i = i + 1) {
        freq_keys[i] = 0;
        freq_vals[i] = 0;
        freq_status[i] = 0;
    }
}

void freq_increment(int key) {
    int h;
    int i;
    int idx;
    h = key;
    if (h < 0) h = -h;
    h = h % 64;
    for (i = 0; i < 64; i = i + 1) {
        idx = (h + i) % 64;
        if (freq_status[idx] == 0) {
            freq_keys[idx] = key;
            freq_vals[idx] = 1;
            freq_status[idx] = 1;
            freq_count = freq_count + 1;
            return;
        }
        if (freq_status[idx] == 1 && freq_keys[idx] == key) {
            freq_vals[idx] = freq_vals[idx] + 1;
            return;
        }
    }
}

int freq_get(int key) {
    int h;
    int i;
    int idx;
    h = key;
    if (h < 0) h = -h;
    h = h % 64;
    for (i = 0; i < 64; i = i + 1) {
        idx = (h + i) % 64;
        if (freq_status[idx] == 0) return 0;
        if (freq_status[idx] == 1 && freq_keys[idx] == key) {
            return freq_vals[idx];
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Helper: absolute value
// -----------------------------------------------------------------------------
int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// =============================================================================
// Main test function
// =============================================================================
int main(void) {
    int i;
    int v;
    int result;
    int test_num;
    int data[20];

    test_num = 0;

    // ---- Test basic insert and lookup ----
    ht_init();
    ht_insert(10, 100);
    ht_insert(20, 200);
    ht_insert(30, 300);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(10)); // 100

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(20)); // 200

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(30)); // 300

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(40)); // -1 (not found)

    // ---- Test count ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_count); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_occupied()); // 3

    // ---- Test delete ----
    result = ht_delete(20);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, result); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(20)); // -1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_count); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_deleted_count()); // 1

    // ---- Test re-insert after delete ----
    ht_insert(20, 250);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(20)); // 250

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_count); // 3

    // ---- Test collision handling (keys that hash to same slot) ----
    ht_clear();
    // Keys 0, 64, 128 all hash to slot 0
    ht_insert(0, 10);
    ht_insert(64, 20);
    ht_insert(128, 30);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(0)); // 10

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(64)); // 20

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(128)); // 30

    // Probe count increases with collisions
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_probe_count(0)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_probe_count(64)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_probe_count(128)); // 3

    // ---- Test update existing key ----
    ht_clear();
    ht_insert(5, 50);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(5)); // 50

    ht_insert(5, 99);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(5)); // 99

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_count); // 1 (not 2)

    // ---- Test contains ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_contains(5)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_contains(999)); // 0

    // ---- Test sum, max, min ----
    ht_clear();
    ht_insert(1, 10);
    ht_insert(2, 20);
    ht_insert(3, 30);
    ht_insert(4, 40);
    ht_insert(5, 50);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_sum_values()); // 150

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_max_value()); // 50

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_min_value()); // 10

    // ---- Test many insertions ----
    ht_clear();
    for (i = 0; i < 30; i = i + 1) {
        ht_insert(i * 3, i * i);
    }
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_count); // 30

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(0)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(15)); // 25

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(87)); // 841

    // ---- Test delete non-existent ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_delete(999)); // 0

    // ---- Test delete and re-lookup neighbors ----
    ht_clear();
    ht_insert(0, 100);
    ht_insert(64, 200);
    ht_insert(128, 300);
    ht_delete(64);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(0)); // 100

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(64)); // -1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(128)); // 300

    // ---- Double hashing tests ----
    ht2_init();
    ht2_insert(10, 100);
    ht2_insert(42, 420);
    ht2_insert(74, 740); // 10, 42, 74 all hash to 10 mod 32
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht2_lookup(10)); // 100

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht2_lookup(42)); // 420

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht2_lookup(74)); // 740

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht2_lookup(99)); // -1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht2_count); // 3

    // ---- Frequency counter tests ----
    freq_init();
    data[0] = 5; data[1] = 3; data[2] = 5; data[3] = 7;
    data[4] = 3; data[5] = 5; data[6] = 9; data[7] = 3;
    data[8] = 7; data[9] = 5;

    for (i = 0; i < 10; i = i + 1) {
        freq_increment(data[i]);
    }

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq_get(5)); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq_get(3)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq_get(7)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq_get(9)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq_get(1)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, freq_count); // 4 unique

    // ---- Stress test: insert then delete half ----
    ht_clear();
    for (i = 0; i < 20; i = i + 1) {
        ht_insert(i, i * 10);
    }
    for (i = 0; i < 20; i = i + 2) {
        ht_delete(i); // delete even keys
    }
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_count); // 10

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(1)); // 10

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(2)); // -1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(19)); // 190

    // Now insert into deleted slots
    ht_insert(100, 1000);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(100)); // 1000

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_count); // 11

    // ---- Edge case: lookup on empty table ----
    ht_clear();
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_lookup(42)); // -1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, ht_count); // 0

    // ---- Test with negative-ish keys (using large positive) ----
    ht_clear();
    ht_insert(1, 11);
    ht_insert(2, 22);
    ht_insert(3, 33);
    v = ht_lookup(1) + ht_lookup(2) + ht_lookup(3);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v); // 66

    // ---- Chain of operations ----
    ht_clear();
    for (i = 1; i <= 10; i = i + 1) {
        ht_insert(i, i * i);
    }
    v = 0;
    for (i = 1; i <= 10; i = i + 1) {
        v = v + ht_lookup(i);
    }
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v); // 1+4+9+16+25+36+49+64+81+100=385

    return 0;
}
