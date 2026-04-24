int printf(const char *fmt, ...);
// EXPECT: === Hash table basic ===\nput/get 42: 100\nput/get 7: 200\ncontains 42: 1\ncontains 99: 0\ncount: 3\n=== Frequency analysis ===\nFreq of 3: 3\nFreq of 7: 2\nDistinct: 4\n=== Most frequent ===\nMost freq in [1,3,3,3,2,2,7,7,1]: 3\n=== Majority element ===\nMajority [2,2,1,1,2,2,1]: 2\n=== Unique elements ===\nUnique count: 1\n=== Anagram detection (sort) ===\n[1,2,3] & [3,2,1]: anagram=1\n[1,2,3] & [1,2,4]: anagram=0\n[1,1,2] & [2,1,1]: anagram=1\n=== Anagram detection (hash) ===\n[1,2,3] & [3,2,1]: anagram_h=1\n[1,2,3] & [1,2,4]: anagram_h=0\n=== Anagram groups ===\nGroups: 2\n=== Duplicate finding ===\nFirst dup [2,1,3,5,3,2]: 3\nHas dups [1,2,3,4,5]: 0\nHas dups [1,2,3,2,5]: 1\nDup count [1,2,3,2,5,1]: 2\nAll dups: 1 2\n=== Two sum ===\nTwoSum [2,7,11,15] target=9: idx 0,1\nTwoSum [3,2,4] target=6: idx 1,2\n=== Pair counts ===\nPairs sum=5 in [1,2,3,4]: 2\nPairs diff=2 in [1,3,5,7]: 3\n=== Set intersection ===\nIntersect: 3 4\n=== Set union ===\nUnion: 1 2 3 4 5 6\n=== Set difference ===\nDiff A-B: 1 2\n=== Symmetric diff ===\nSymDiff: 1 2 6 7\n=== Subset check ===\n[1,2] subset of [1,2,3,4]: 1\n[1,5] subset of [1,2,3,4]: 0\n=== Subarray with sum ===\nSubSum=7 in [1,4,2,1]: found [0,2]\n=== Count subarrays with sum ===\nCount subsum=3 in [1,2,3,0,3]: 5\n=== Longest zero sum ===\nLongZero [1,-1,3,-3,2]: 4\n=== Array hash ===\nHash equality: 1\nHash inequality: 0\nAll hash app tests passed!
// ============================================================
// test_077_hash_apps.c
// Hash-based counting, frequency analysis, anagram detection
// (integer sorting), duplicate finding via hashing.
// ============================================================

// ---- Simple hash function for integers ----
int hash_int(int val, int table_size) {
    int h = val % table_size;
    if (h < 0) h = h + table_size;
    return h;
}

// ---- Hash table using open addressing (linear probing) ----
// Each slot: key, value, occupied flag
struct HashTable {
    int keys[101];
    int values[101];
    int occupied[101];
    int size;
};

void ht_init(struct HashTable *ht, int size) {
    ht->size = size;
    int i;
    for (i = 0; i < size; i++) {
        ht->keys[i] = 0;
        ht->values[i] = 0;
        ht->occupied[i] = 0;
    }
}

// ---- Insert or update key-value pair ----
void ht_put(struct HashTable *ht, int key, int value) {
    int h = hash_int(key, ht->size);
    int i;
    for (i = 0; i < ht->size; i++) {
        int idx = (h + i) % ht->size;
        if (!ht->occupied[idx]) {
            ht->keys[idx] = key;
            ht->values[idx] = value;
            ht->occupied[idx] = 1;
            return;
        }
        if (ht->keys[idx] == key) {
            ht->values[idx] = value;
            return;
        }
    }
}

// ---- Get value for key, returns -1 if not found ----
int ht_get(struct HashTable *ht, int key) {
    int h = hash_int(key, ht->size);
    int i;
    for (i = 0; i < ht->size; i++) {
        int idx = (h + i) % ht->size;
        if (!ht->occupied[idx]) return -1;
        if (ht->keys[idx] == key) return ht->values[idx];
    }
    return -1;
}

// ---- Check if key exists ----
int ht_contains(struct HashTable *ht, int key) {
    int h = hash_int(key, ht->size);
    int i;
    for (i = 0; i < ht->size; i++) {
        int idx = (h + i) % ht->size;
        if (!ht->occupied[idx]) return 0;
        if (ht->keys[idx] == key) return 1;
    }
    return 0;
}

// ---- Increment value for key (or set to 1 if not found) ----
void ht_increment(struct HashTable *ht, int key) {
    int h = hash_int(key, ht->size);
    int i;
    for (i = 0; i < ht->size; i++) {
        int idx = (h + i) % ht->size;
        if (!ht->occupied[idx]) {
            ht->keys[idx] = key;
            ht->values[idx] = 1;
            ht->occupied[idx] = 1;
            return;
        }
        if (ht->keys[idx] == key) {
            ht->values[idx] = ht->values[idx] + 1;
            return;
        }
    }
}

// ---- Count occupied slots ----
int ht_count(struct HashTable *ht) {
    int count = 0;
    int i;
    for (i = 0; i < ht->size; i++) {
        if (ht->occupied[i]) count++;
    }
    return count;
}

// ---- Get all keys ----
int ht_get_keys(struct HashTable *ht, int keys_out[]) {
    int count = 0;
    int i;
    for (i = 0; i < ht->size; i++) {
        if (ht->occupied[i]) {
            keys_out[count] = ht->keys[i];
            count++;
        }
    }
    return count;
}

// ============================================================
// Frequency Analysis
// ============================================================

// ---- Count frequency of each element ----
void count_frequency(int arr[], int n, struct HashTable *freq) {
    ht_init(freq, 101);
    int i;
    for (i = 0; i < n; i++) {
        ht_increment(freq, arr[i]);
    }
}

// ---- Find most frequent element ----
int most_frequent(int arr[], int n) {
    struct HashTable freq;
    count_frequency(arr, n, &freq);

    int max_count = 0;
    int max_elem = arr[0];
    int i;
    for (i = 0; i < freq.size; i++) {
        if (freq.occupied[i] && freq.values[i] > max_count) {
            max_count = freq.values[i];
            max_elem = freq.keys[i];
        }
    }
    return max_elem;
}

// ---- Find frequency of specific element ----
int element_frequency(int arr[], int n, int elem) {
    struct HashTable freq;
    count_frequency(arr, n, &freq);
    int result = ht_get(&freq, elem);
    return (result == -1) ? 0 : result;
}

// ---- Count distinct elements ----
int count_distinct(int arr[], int n) {
    struct HashTable ht;
    ht_init(&ht, 101);
    int i;
    for (i = 0; i < n; i++) {
        if (!ht_contains(&ht, arr[i])) {
            ht_put(&ht, arr[i], 1);
        }
    }
    return ht_count(&ht);
}

// ---- Find elements that appear exactly once ----
int find_unique(int arr[], int n, int result[]) {
    struct HashTable freq;
    count_frequency(arr, n, &freq);

    int count = 0;
    int i;
    for (i = 0; i < freq.size; i++) {
        if (freq.occupied[i] && freq.values[i] == 1) {
            result[count] = freq.keys[i];
            count++;
        }
    }
    return count;
}

// ---- Find elements that appear more than n/2 times (majority element) ----
// Using Boyer-Moore Voting Algorithm then verify with hash
int majority_element(int arr[], int n) {
    // Boyer-Moore Voting Algorithm
    int candidate = arr[0];
    int count = 1;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] == candidate) {
            count++;
        } else {
            count--;
            if (count == 0) {
                candidate = arr[i];
                count = 1;
            }
        }
    }

    // Verify
    int actual_count = element_frequency(arr, n, candidate);
    if (actual_count > n / 2) return candidate;
    return -1;
}

// ============================================================
// Anagram Detection
// ============================================================

// ---- Sort an integer array (for comparison) ----
void sort_array(int arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// ---- Check if two arrays are anagrams (same elements, same frequency) ----
int are_anagrams(int a[], int na, int b[], int nb) {
    if (na != nb) return 0;

    // Method 1: Sort and compare
    int sorted_a[100], sorted_b[100];
    int i;
    for (i = 0; i < na; i++) sorted_a[i] = a[i];
    for (i = 0; i < nb; i++) sorted_b[i] = b[i];
    sort_array(sorted_a, na);
    sort_array(sorted_b, nb);
    for (i = 0; i < na; i++) {
        if (sorted_a[i] != sorted_b[i]) return 0;
    }
    return 1;
}

// ---- Check if two arrays are anagrams using hash table ----
int are_anagrams_hash(int a[], int na, int b[], int nb) {
    if (na != nb) return 0;

    struct HashTable ht;
    ht_init(&ht, 101);
    int i;

    for (i = 0; i < na; i++) {
        ht_increment(&ht, a[i]);
    }

    // Decrement for b
    for (i = 0; i < nb; i++) {
        int val = ht_get(&ht, b[i]);
        if (val <= 0) return 0;
        ht_put(&ht, b[i], val - 1);
    }

    // Check all values are 0
    for (i = 0; i < ht.size; i++) {
        if (ht.occupied[i] && ht.values[i] != 0) return 0;
    }
    return 1;
}

// ---- Group anagrams from array of arrays ----
// Given multiple arrays, group them by anagram signature
// Returns number of groups
int count_anagram_groups(int arrays[][10], int lengths[], int n) {
    // Sort each array copy and use as key signature
    int signatures[20][10];
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < lengths[i]; j++) {
            signatures[i][j] = arrays[i][j];
        }
        sort_array(signatures[i], lengths[i]);
    }

    int visited[20];
    for (i = 0; i < n; i++) visited[i] = 0;

    int groups = 0;
    for (i = 0; i < n; i++) {
        if (visited[i]) continue;
        groups++;
        visited[i] = 1;
        for (j = i + 1; j < n; j++) {
            if (visited[j]) continue;
            if (lengths[i] != lengths[j]) continue;
            // Compare signatures
            int same = 1;
            int k;
            for (k = 0; k < lengths[i]; k++) {
                if (signatures[i][k] != signatures[j][k]) {
                    same = 0;
                    break;
                }
            }
            if (same) visited[j] = 1;
        }
    }
    return groups;
}

// ============================================================
// Duplicate Finding
// ============================================================

// ---- Find first duplicate ----
int first_duplicate(int arr[], int n) {
    struct HashTable ht;
    ht_init(&ht, 101);
    int i;
    for (i = 0; i < n; i++) {
        if (ht_contains(&ht, arr[i])) return arr[i];
        ht_put(&ht, arr[i], 1);
    }
    return -1;
}

// ---- Count duplicates ----
int count_duplicates(int arr[], int n) {
    struct HashTable freq;
    count_frequency(arr, n, &freq);
    int count = 0;
    int i;
    for (i = 0; i < freq.size; i++) {
        if (freq.occupied[i] && freq.values[i] > 1) count++;
    }
    return count;
}

// ---- Find all duplicates ----
int find_duplicates(int arr[], int n, int result[]) {
    struct HashTable freq;
    count_frequency(arr, n, &freq);
    int count = 0;
    int i;
    for (i = 0; i < freq.size; i++) {
        if (freq.occupied[i] && freq.values[i] > 1) {
            result[count] = freq.keys[i];
            count++;
        }
    }
    // Sort result for deterministic output
    sort_array(result, count);
    return count;
}

// ---- Check if array has duplicates ----
int has_duplicates(int arr[], int n) {
    struct HashTable ht;
    ht_init(&ht, 101);
    int i;
    for (i = 0; i < n; i++) {
        if (ht_contains(&ht, arr[i])) return 1;
        ht_put(&ht, arr[i], 1);
    }
    return 0;
}

// ============================================================
// Two Sum and related problems
// ============================================================

// ---- Two sum: find pair with given sum ----
// Returns 1 if found, sets *idx1 and *idx2
int two_sum(int arr[], int n, int target, int *idx1, int *idx2) {
    struct HashTable ht;
    ht_init(&ht, 101);
    int i;
    for (i = 0; i < n; i++) {
        int complement = target - arr[i];
        if (ht_contains(&ht, complement)) {
            *idx1 = ht_get(&ht, complement);
            *idx2 = i;
            return 1;
        }
        ht_put(&ht, arr[i], i);
    }
    return 0;
}

// ---- Count pairs with given sum ----
int count_pairs_sum(int arr[], int n, int target) {
    struct HashTable freq;
    ht_init(&freq, 101);
    int count = 0;
    int i;

    for (i = 0; i < n; i++) {
        int complement = target - arr[i];
        int comp_freq = ht_get(&freq, complement);
        if (comp_freq > 0) {
            count = count + comp_freq;
        }
        ht_increment(&freq, arr[i]);
    }
    return count;
}

// ---- Count pairs with given difference ----
int count_pairs_diff(int arr[], int n, int diff) {
    struct HashTable ht;
    ht_init(&ht, 101);
    int i;
    for (i = 0; i < n; i++) {
        ht_put(&ht, arr[i], 1);
    }

    int count = 0;
    for (i = 0; i < n; i++) {
        if (ht_contains(&ht, arr[i] + diff)) count++;
    }
    return count;
}

// ============================================================
// Set Operations using Hash
// ============================================================

// ---- Intersection of two arrays ----
int array_intersection(int a[], int na, int b[], int nb, int result[]) {
    struct HashTable ht;
    ht_init(&ht, 101);
    int i;

    for (i = 0; i < na; i++) {
        ht_put(&ht, a[i], 1);
    }

    int count = 0;
    struct HashTable seen;
    ht_init(&seen, 101);
    for (i = 0; i < nb; i++) {
        if (ht_contains(&ht, b[i]) && !ht_contains(&seen, b[i])) {
            result[count] = b[i];
            count++;
            ht_put(&seen, b[i], 1);
        }
    }
    sort_array(result, count);
    return count;
}

// ---- Union of two arrays ----
int array_union(int a[], int na, int b[], int nb, int result[]) {
    struct HashTable ht;
    ht_init(&ht, 101);
    int count = 0;
    int i;

    for (i = 0; i < na; i++) {
        if (!ht_contains(&ht, a[i])) {
            ht_put(&ht, a[i], 1);
            result[count] = a[i];
            count++;
        }
    }
    for (i = 0; i < nb; i++) {
        if (!ht_contains(&ht, b[i])) {
            ht_put(&ht, b[i], 1);
            result[count] = b[i];
            count++;
        }
    }
    sort_array(result, count);
    return count;
}

// ---- Difference of two arrays (a - b) ----
int array_difference(int a[], int na, int b[], int nb, int result[]) {
    struct HashTable ht_b;
    ht_init(&ht_b, 101);
    int i;

    for (i = 0; i < nb; i++) {
        ht_put(&ht_b, b[i], 1);
    }

    int count = 0;
    struct HashTable seen;
    ht_init(&seen, 101);
    for (i = 0; i < na; i++) {
        if (!ht_contains(&ht_b, a[i]) && !ht_contains(&seen, a[i])) {
            result[count] = a[i];
            count++;
            ht_put(&seen, a[i], 1);
        }
    }
    sort_array(result, count);
    return count;
}

// ---- Symmetric difference ----
int array_sym_diff(int a[], int na, int b[], int nb, int result[]) {
    int diff_ab[100], diff_ba[100];
    int n1 = array_difference(a, na, b, nb, diff_ab);
    int n2 = array_difference(b, nb, a, na, diff_ba);

    int count = 0;
    int i;
    for (i = 0; i < n1; i++) result[count++] = diff_ab[i];
    for (i = 0; i < n2; i++) result[count++] = diff_ba[i];
    sort_array(result, count);
    return count;
}

// ---- Check if a is subset of b ----
int is_subset(int a[], int na, int b[], int nb) {
    struct HashTable ht;
    ht_init(&ht, 101);
    int i;
    for (i = 0; i < nb; i++) ht_put(&ht, b[i], 1);
    for (i = 0; i < na; i++) {
        if (!ht_contains(&ht, a[i])) return 0;
    }
    return 1;
}

// ============================================================
// Hash-based Histogram
// ============================================================

// ---- Build histogram and find mode ----
int histogram_mode(int arr[], int n) {
    struct HashTable freq;
    count_frequency(arr, n, &freq);

    int mode = arr[0];
    int max_freq = 0;
    int i;
    for (i = 0; i < freq.size; i++) {
        if (freq.occupied[i] && freq.values[i] > max_freq) {
            max_freq = freq.values[i];
            mode = freq.keys[i];
        }
    }
    return mode;
}

// ---- Compute a simple hash of an array (for comparing arrays) ----
int array_hash(int arr[], int n) {
    int h = 0;
    int i;
    for (i = 0; i < n; i++) {
        h = h * 31 + arr[i];
    }
    return h;
}

// ---- Find subarray with given sum using hash ----
// Returns 1 if found, sets *start, *end
int subarray_with_sum(int arr[], int n, int target, int *start, int *end) {
    struct HashTable prefix_ht;
    ht_init(&prefix_ht, 101);

    int prefix_sum = 0;
    ht_put(&prefix_ht, 0, -1); // prefix sum 0 at index -1

    int i;
    for (i = 0; i < n; i++) {
        prefix_sum = prefix_sum + arr[i];
        int needed = prefix_sum - target;
        if (ht_contains(&prefix_ht, needed)) {
            *start = ht_get(&prefix_ht, needed) + 1;
            *end = i;
            return 1;
        }
        ht_put(&prefix_ht, prefix_sum, i);
    }
    return 0;
}

// ---- Count subarrays with given sum ----
int count_subarrays_sum(int arr[], int n, int target) {
    struct HashTable prefix_count;
    ht_init(&prefix_count, 101);

    int prefix_sum = 0;
    int count = 0;
    ht_put(&prefix_count, 0, 1);

    int i;
    for (i = 0; i < n; i++) {
        prefix_sum = prefix_sum + arr[i];
        int needed = prefix_sum - target;
        int prev = ht_get(&prefix_count, needed);
        if (prev > 0) count = count + prev;

        int cur = ht_get(&prefix_count, prefix_sum);
        if (cur == -1) cur = 0;
        ht_put(&prefix_count, prefix_sum, cur + 1);
    }
    return count;
}

// ---- Longest subarray with sum 0 ----
int longest_zero_sum_subarray(int arr[], int n) {
    struct HashTable ht;
    ht_init(&ht, 101);

    int prefix_sum = 0;
    int max_len = 0;
    ht_put(&ht, 0, -1);

    int i;
    for (i = 0; i < n; i++) {
        prefix_sum = prefix_sum + arr[i];
        if (ht_contains(&ht, prefix_sum)) {
            int start = ht_get(&ht, prefix_sum);
            int len = i - start;
            if (len > max_len) max_len = len;
        } else {
            ht_put(&ht, prefix_sum, i);
        }
    }
    return max_len;
}


int main() {
    // === Hash table basic ===
    printf("=== Hash table basic ===\n");
    {
        struct HashTable ht;
        ht_init(&ht, 101);
        ht_put(&ht, 42, 100);
        ht_put(&ht, 7, 200);
        ht_put(&ht, 13, 300);
        printf("put/get 42: %d\n", ht_get(&ht, 42));
        printf("put/get 7: %d\n", ht_get(&ht, 7));
        printf("contains 42: %d\n", ht_contains(&ht, 42));
        printf("contains 99: %d\n", ht_contains(&ht, 99));
        printf("count: %d\n", ht_count(&ht));
    }

    // === Frequency analysis ===
    printf("=== Frequency analysis ===\n");
    {
        int arr[9];
        arr[0]=1; arr[1]=3; arr[2]=3; arr[3]=3; arr[4]=2;
        arr[5]=2; arr[6]=7; arr[7]=7; arr[8]=1;
        printf("Freq of 3: %d\n", element_frequency(arr, 9, 3));
        printf("Freq of 7: %d\n", element_frequency(arr, 9, 7));
        printf("Distinct: %d\n", count_distinct(arr, 9));
    }

    // === Most frequent ===
    printf("=== Most frequent ===\n");
    {
        int arr[9];
        arr[0]=1; arr[1]=3; arr[2]=3; arr[3]=3; arr[4]=2;
        arr[5]=2; arr[6]=7; arr[7]=7; arr[8]=1;
        printf("Most freq in [1,3,3,3,2,2,7,7,1]: %d\n", most_frequent(arr, 9));
    }

    // === Majority element ===
    printf("=== Majority element ===\n");
    {
        int arr[7];
        arr[0]=2; arr[1]=2; arr[2]=1; arr[3]=1;
        arr[4]=2; arr[5]=2; arr[6]=1;
        printf("Majority [2,2,1,1,2,2,1]: %d\n", majority_element(arr, 7));
    }

    // === Unique elements ===
    printf("=== Unique elements ===\n");
    {
        int arr[7];
        arr[0]=1; arr[1]=2; arr[2]=1; arr[3]=3;
        arr[4]=2; arr[5]=4; arr[6]=3;
        int uniq[20];
        int uc = find_unique(arr, 7, uniq);
        printf("Unique count: %d\n", uc);
    }

    // === Anagram detection (sort) ===
    printf("=== Anagram detection (sort) ===\n");
    {
        int a[3]; a[0]=1; a[1]=2; a[2]=3;
        int b[3]; b[0]=3; b[1]=2; b[2]=1;
        printf("[1,2,3] & [3,2,1]: anagram=%d\n", are_anagrams(a, 3, b, 3));
    }
    {
        int a[3]; a[0]=1; a[1]=2; a[2]=3;
        int b[3]; b[0]=1; b[1]=2; b[2]=4;
        printf("[1,2,3] & [1,2,4]: anagram=%d\n", are_anagrams(a, 3, b, 3));
    }
    {
        int a[3]; a[0]=1; a[1]=1; a[2]=2;
        int b[3]; b[0]=2; b[1]=1; b[2]=1;
        printf("[1,1,2] & [2,1,1]: anagram=%d\n", are_anagrams(a, 3, b, 3));
    }

    // === Anagram detection (hash) ===
    printf("=== Anagram detection (hash) ===\n");
    {
        int a[3]; a[0]=1; a[1]=2; a[2]=3;
        int b[3]; b[0]=3; b[1]=2; b[2]=1;
        printf("[1,2,3] & [3,2,1]: anagram_h=%d\n", are_anagrams_hash(a, 3, b, 3));
    }
    {
        int a[3]; a[0]=1; a[1]=2; a[2]=3;
        int b[3]; b[0]=1; b[1]=2; b[2]=4;
        printf("[1,2,3] & [1,2,4]: anagram_h=%d\n", are_anagrams_hash(a, 3, b, 3));
    }

    // === Anagram groups ===
    printf("=== Anagram groups ===\n");
    {
        int arrays[4][10];
        int lengths[4];
        // Group 1: [1,2,3], [3,2,1], [2,1,3]
        arrays[0][0]=1; arrays[0][1]=2; arrays[0][2]=3; lengths[0]=3;
        arrays[1][0]=3; arrays[1][1]=2; arrays[1][2]=1; lengths[1]=3;
        arrays[2][0]=2; arrays[2][1]=1; arrays[2][2]=3; lengths[2]=3;
        // Group 2: [4,5]
        arrays[3][0]=4; arrays[3][1]=5; lengths[3]=2;
        printf("Groups: %d\n", count_anagram_groups(arrays, lengths, 4));
    }

    // === Duplicate finding ===
    printf("=== Duplicate finding ===\n");
    {
        int arr[6]; arr[0]=2; arr[1]=1; arr[2]=3; arr[3]=5; arr[4]=3; arr[5]=2;
        printf("First dup [2,1,3,5,3,2]: %d\n", first_duplicate(arr, 6));
    }
    {
        int arr[5]; arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
        printf("Has dups [1,2,3,4,5]: %d\n", has_duplicates(arr, 5));
    }
    {
        int arr[5]; arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=2; arr[4]=5;
        printf("Has dups [1,2,3,2,5]: %d\n", has_duplicates(arr, 5));
    }
    {
        int arr[6]; arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=2; arr[4]=5; arr[5]=1;
        printf("Dup count [1,2,3,2,5,1]: %d\n", count_duplicates(arr, 6));
    }
    {
        int arr[6]; arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=2; arr[4]=5; arr[5]=1;
        int dups[10];
        int nd = find_duplicates(arr, 6, dups);
        printf("All dups:");
        int i;
        for (i = 0; i < nd; i++) printf(" %d", dups[i]);
        printf("\n");
    }

    // === Two sum ===
    printf("=== Two sum ===\n");
    {
        int arr[4]; arr[0]=2; arr[1]=7; arr[2]=11; arr[3]=15;
        int i1, i2;
        two_sum(arr, 4, 9, &i1, &i2);
        printf("TwoSum [2,7,11,15] target=9: idx %d,%d\n", i1, i2);
    }
    {
        int arr[3]; arr[0]=3; arr[1]=2; arr[2]=4;
        int i1, i2;
        two_sum(arr, 3, 6, &i1, &i2);
        printf("TwoSum [3,2,4] target=6: idx %d,%d\n", i1, i2);
    }

    // === Pair counts ===
    printf("=== Pair counts ===\n");
    {
        int arr[4]; arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4;
        printf("Pairs sum=5 in [1,2,3,4]: %d\n", count_pairs_sum(arr, 4, 5));
    }
    {
        int arr[4]; arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=7;
        printf("Pairs diff=2 in [1,3,5,7]: %d\n", count_pairs_diff(arr, 4, 2));
    }

    // === Set intersection ===
    printf("=== Set intersection ===\n");
    {
        int a[4]; a[0]=1; a[1]=2; a[2]=3; a[3]=4;
        int b[4]; b[0]=3; b[1]=4; b[2]=5; b[3]=6;
        int result[10];
        int n = array_intersection(a, 4, b, 4, result);
        printf("Intersect:");
        int i;
        for (i = 0; i < n; i++) printf(" %d", result[i]);
        printf("\n");
    }

    // === Set union ===
    printf("=== Set union ===\n");
    {
        int a[4]; a[0]=1; a[1]=2; a[2]=3; a[3]=4;
        int b[4]; b[0]=3; b[1]=4; b[2]=5; b[3]=6;
        int result[10];
        int n = array_union(a, 4, b, 4, result);
        printf("Union:");
        int i;
        for (i = 0; i < n; i++) printf(" %d", result[i]);
        printf("\n");
    }

    // === Set difference ===
    printf("=== Set difference ===\n");
    {
        int a[4]; a[0]=1; a[1]=2; a[2]=3; a[3]=4;
        int b[4]; b[0]=3; b[1]=4; b[2]=5; b[3]=6;
        int result[10];
        int n = array_difference(a, 4, b, 4, result);
        printf("Diff A-B:");
        int i;
        for (i = 0; i < n; i++) printf(" %d", result[i]);
        printf("\n");
    }

    // === Symmetric difference ===
    printf("=== Symmetric diff ===\n");
    {
        int a[4]; a[0]=1; a[1]=2; a[2]=3; a[3]=4;
        int b[4]; b[0]=3; b[1]=4; b[2]=6; b[3]=7;
        int result[10];
        int n = array_sym_diff(a, 4, b, 4, result);
        printf("SymDiff:");
        int i;
        for (i = 0; i < n; i++) printf(" %d", result[i]);
        printf("\n");
    }

    // === Subset check ===
    printf("=== Subset check ===\n");
    {
        int a[2]; a[0]=1; a[1]=2;
        int b[4]; b[0]=1; b[1]=2; b[2]=3; b[3]=4;
        printf("[1,2] subset of [1,2,3,4]: %d\n", is_subset(a, 2, b, 4));
    }
    {
        int a[2]; a[0]=1; a[1]=5;
        int b[4]; b[0]=1; b[1]=2; b[2]=3; b[3]=4;
        printf("[1,5] subset of [1,2,3,4]: %d\n", is_subset(a, 2, b, 4));
    }

    // === Subarray with sum ===
    printf("=== Subarray with sum ===\n");
    {
        int arr[4]; arr[0]=1; arr[1]=4; arr[2]=2; arr[3]=1;
        int s, e;
        subarray_with_sum(arr, 4, 7, &s, &e);
        printf("SubSum=7 in [1,4,2,1]: found [%d,%d]\n", s, e);
    }

    // === Count subarrays with sum ===
    printf("=== Count subarrays with sum ===\n");
    {
        int arr[5]; arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=0; arr[4]=3;
        printf("Count subsum=3 in [1,2,3,0,3]: %d\n", count_subarrays_sum(arr, 5, 3));
    }

    // === Longest zero sum subarray ===
    printf("=== Longest zero sum ===\n");
    {
        int arr[5]; arr[0]=1; arr[1]=-1; arr[2]=3; arr[3]=-3; arr[4]=2;
        printf("LongZero [1,-1,3,-3,2]: %d\n", longest_zero_sum_subarray(arr, 5));
    }

    // === Array hash ===
    printf("=== Array hash ===\n");
    {
        int a[3]; a[0]=1; a[1]=2; a[2]=3;
        int b[3]; b[0]=1; b[1]=2; b[2]=3;
        int c[3]; c[0]=3; c[1]=2; c[2]=1;
        printf("Hash equality: %d\n", (array_hash(a, 3) == array_hash(b, 3)) ? 1 : 0);
        printf("Hash inequality: %d\n", (array_hash(a, 3) == array_hash(c, 3)) ? 1 : 0);
    }

    printf("All hash app tests passed!\n");
    return 0;
}
