int printf(const char *fmt, ...);
// EXPECT: djb2 hashes:\n  hello = 261238937\n  world = 279393645\n  foo = 193491849\n  bar = 193487034\n  empty = 5381\nrotate hashes:\n  hello = 7223983\n  world = 7439524\n  foo = 24735\nfnv hashes:\n  hello = 1335831723\n  world = 933488787\n  foo = -1443660073\nconsistency: 1\ncollision check: 1\nbucket distribution (size=16):\n  hello -> bucket 9\n  world -> bucket 13\n  foo -> bucket 9\n  bar -> bucket 10\ninteger hashes:\n  hash(0) = 0x0\n  hash(1) = 0x9B66\n  hash(42) = 0xA8F2\n  hash(1000) = 0xF3CD
// Test: simple hash

// DJB2 hash
int djb2_hash(char *str) {
    int hash = 5381;
    int i = 0;
    while (str[i] != 0) {
        hash = ((hash << 5) + hash) + str[i];
        i++;
    }
    return hash;
}

// Simple rotate-XOR hash
int rotate_hash(char *str) {
    int hash = 0;
    int i = 0;
    while (str[i] != 0) {
        hash = ((hash << 4) | ((hash >> 28) & 0xF)) ^ str[i];
        i++;
    }
    return hash;
}

// FNV-1a inspired hash (simplified)
int fnv_hash(char *str) {
    int hash = 0x811C9DC5;
    int i = 0;
    while (str[i] != 0) {
        hash = hash ^ str[i];
        hash = hash * 16777619;
        i++;
    }
    return hash;
}

int main(void) {
    // Test strings
    char s1[6]; s1[0]='h'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]=0;
    char s2[6]; s2[0]='w'; s2[1]='o'; s2[2]='r'; s2[3]='l'; s2[4]='d'; s2[5]=0;
    char s3[4]; s3[0]='f'; s3[1]='o'; s3[2]='o'; s3[3]=0;
    char s4[4]; s4[0]='b'; s4[1]='a'; s4[2]='r'; s4[3]=0;
    char s5[1]; s5[0]=0;

    // DJB2 hash values
    printf("djb2 hashes:\n");
    printf("  hello = %d\n", djb2_hash(s1));
    printf("  world = %d\n", djb2_hash(s2));
    printf("  foo = %d\n", djb2_hash(s3));
    printf("  bar = %d\n", djb2_hash(s4));
    printf("  empty = %d\n", djb2_hash(s5));

    // Rotate hash values
    printf("rotate hashes:\n");
    printf("  hello = %d\n", rotate_hash(s1));
    printf("  world = %d\n", rotate_hash(s2));
    printf("  foo = %d\n", rotate_hash(s3));

    // FNV hash values
    printf("fnv hashes:\n");
    printf("  hello = %d\n", fnv_hash(s1));
    printf("  world = %d\n", fnv_hash(s2));
    printf("  foo = %d\n", fnv_hash(s3));

    // Same string gives same hash
    printf("consistency: %d\n", djb2_hash(s1) == djb2_hash(s1));

    // Different strings give different hashes (usually)
    printf("collision check: %d\n", djb2_hash(s1) != djb2_hash(s2));

    // Hash distribution into buckets
    int bucket_size = 16;
    int mask = bucket_size - 1;
    char *strings[4];
    strings[0] = s1; strings[1] = s2; strings[2] = s3; strings[3] = s4;

    printf("bucket distribution (size=%d):\n", bucket_size);
    int i;
    for (i = 0; i < 4; i++) {
        int h = djb2_hash(strings[i]);
        int bucket = h & mask;
        if (bucket < 0) bucket = -bucket;
        printf("  %s -> bucket %d\n", strings[i], bucket);
    }

    // Hash for integers (treating as bytes)
    printf("integer hashes:\n");
    int nums[4];
    nums[0] = 0; nums[1] = 1; nums[2] = 42; nums[3] = 1000;
    for (i = 0; i < 4; i++) {
        // Simple integer hash
        int h = nums[i];
        h = h ^ (h >> 16);
        h = h * 0x45D9F3B;
        h = h ^ (h >> 16);
        printf("  hash(%d) = 0x%X\n", nums[i], h & 0xFFFF);
    }

    return 0;
}
