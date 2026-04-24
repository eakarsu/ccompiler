int printf(const char *fmt, ...);

// Feistel Network - simplified DES-like round structure

// Simple S-box substitution (4-bit -> 4-bit)
int sbox(int input) {
    int table[16];
    table[0] = 14; table[1] = 4;  table[2] = 13; table[3] = 1;
    table[4] = 2;  table[5] = 15; table[6] = 11; table[7] = 8;
    table[8] = 3;  table[9] = 10; table[10] = 6; table[11] = 12;
    table[12] = 5; table[13] = 9;  table[14] = 0; table[15] = 7;
    return table[input & 15];
}

// Round function: combines subkey with half-block, applies S-boxes
int feistel_f(int half, int subkey) {
    int mixed = half ^ subkey;
    // Apply S-box to each nibble (4 bits at a time)
    int result = 0;
    int i;
    for (i = 0; i < 8; i++) {
        int nibble = (mixed >> (i * 4)) & 15;
        int substituted = sbox(nibble);
        result = result | (substituted << (i * 4));
    }
    return result;
}

// Generate round subkeys from master key
void generate_subkeys(int master_key, int num_rounds, int *subkeys) {
    int i;
    int state = master_key;
    for (i = 0; i < num_rounds; i++) {
        state = state * 1103515245 + 12345;
        subkeys[i] = state;
    }
}

// Feistel encrypt: splits block into L and R halves
struct Block {
    int left;
    int right;
};

void feistel_encrypt(struct Block *block, int *subkeys, int rounds) {
    int i;
    int left = block->left;
    int right = block->right;

    for (i = 0; i < rounds; i++) {
        int new_left = right;
        int f_out = feistel_f(right, subkeys[i]);
        int new_right = left ^ f_out;
        left = new_left;
        right = new_right;
    }
    // Final swap
    block->left = right;
    block->right = left;
}

void feistel_decrypt(struct Block *block, int *subkeys, int rounds) {
    int i;
    int left = block->left;
    int right = block->right;

    // Reverse the final swap
    int tmp = left;
    left = right;
    right = tmp;

    // Apply rounds in reverse
    for (i = rounds - 1; i >= 0; i--) {
        int prev_right = left;
        int f_out = feistel_f(left, subkeys[i]);
        int prev_left = right ^ f_out;
        left = prev_left;
        right = prev_right;
    }
    block->left = left;
    block->right = right;
}

// Avalanche effect: count differing bits between two values
int count_diff_bits(int a, int b) {
    int diff = a ^ b;
    int count = 0;
    while (diff != 0) {
        count = count + (diff & 1);
        diff = (diff >> 1) & 2147483647; // unsigned shift
    }
    return count;
}

int main() {
    int subkeys[16];
    int rounds = 8;

    // Test 1: S-box is a permutation (bijective)
    int seen[16];
    int i;
    for (i = 0; i < 16; i++) seen[i] = 0;
    int is_perm = 1;
    for (i = 0; i < 16; i++) {
        int out = sbox(i);
        if (out < 0 || out > 15 || seen[out]) is_perm = 0;
        seen[out] = 1;
    }
    printf("S-box permutation: %d\n", is_perm);
    // EXPECT: S-box permutation: 1

    // Test 2: generate subkeys
    generate_subkeys(12345, rounds, subkeys);
    printf("Subkeys generated: %d\n", rounds);
    // EXPECT: Subkeys generated: 8

    // Test 3: encrypt and decrypt round-trip
    struct Block blk;
    blk.left = 305419896;  // 0x12345678
    blk.right = 987654321;
    int orig_left = blk.left;
    int orig_right = blk.right;

    feistel_encrypt(&blk, subkeys, rounds);
    int enc_left = blk.left;
    int enc_right = blk.right;

    // Encrypted should differ from original
    int differs = (enc_left != orig_left) || (enc_right != orig_right);
    printf("Encryption changes block: %d\n", differs);
    // EXPECT: Encryption changes block: 1

    // Test 4: decrypt back
    feistel_decrypt(&blk, subkeys, rounds);
    int rt_ok = (blk.left == orig_left) && (blk.right == orig_right);
    printf("Round trip: %d\n", rt_ok);
    // EXPECT: Round trip: 1

    // Test 5: different keys produce different ciphertexts
    int subkeys2[16];
    generate_subkeys(99999, rounds, subkeys2);
    struct Block blk1;
    struct Block blk2;
    blk1.left = 100; blk1.right = 200;
    blk2.left = 100; blk2.right = 200;
    feistel_encrypt(&blk1, subkeys, rounds);
    feistel_encrypt(&blk2, subkeys2, rounds);
    int diff_keys = (blk1.left != blk2.left) || (blk1.right != blk2.right);
    printf("Different keys differ: %d\n", diff_keys);
    // EXPECT: Different keys differ: 1

    // Test 6: single round encrypt/decrypt
    struct Block single;
    single.left = 42;
    single.right = 99;
    int orig_l = single.left;
    int orig_r = single.right;
    feistel_encrypt(&single, subkeys, 1);
    feistel_decrypt(&single, subkeys, 1);
    int single_rt = (single.left == orig_l) && (single.right == orig_r);
    printf("Single round RT: %d\n", single_rt);
    // EXPECT: Single round RT: 1

    // Test 7: zero block encrypt/decrypt
    struct Block zero;
    zero.left = 0;
    zero.right = 0;
    feistel_encrypt(&zero, subkeys, rounds);
    feistel_decrypt(&zero, subkeys, rounds);
    int zero_rt = (zero.left == 0) && (zero.right == 0);
    printf("Zero block RT: %d\n", zero_rt);
    // EXPECT: Zero block RT: 1

    // Test 8: bit difference count
    int bd = count_diff_bits(0, 255);
    printf("Bits diff 0 vs 255: %d\n", bd);
    // EXPECT: Bits diff 0 vs 255: 8

    int bd2 = count_diff_bits(170, 85);
    printf("Bits diff 170 vs 85: %d\n", bd2);
    // EXPECT: Bits diff 170 vs 85: 8

    // Test 9: multiple blocks
    int all_ok = 1;
    int j;
    for (j = 0; j < 5; j++) {
        struct Block mb;
        mb.left = j * 1000 + 1;
        mb.right = j * 2000 + 7;
        int sl = mb.left;
        int sr = mb.right;
        feistel_encrypt(&mb, subkeys, rounds);
        feistel_decrypt(&mb, subkeys, rounds);
        if (mb.left != sl || mb.right != sr) all_ok = 0;
    }
    printf("Multi-block RT: %d\n", all_ok);
    // EXPECT: Multi-block RT: 1

    printf("Feistel network tests passed\n");
    // EXPECT: Feistel network tests passed

    return 0;
}
