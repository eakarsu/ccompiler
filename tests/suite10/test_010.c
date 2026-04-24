int printf(const char *fmt, ...);

// Grammar Production Rules Simulator
// Simulates a simple context-free grammar (CFG) derivation
// Grammar: S -> aSb | ab | SS
// This grammar generates the Dyck language (matched parentheses using a/b)
// We test membership by parsing with a recursive descent approach

// Check if string is in the Dyck language (balanced a's and b's where
// at every prefix, count(a) >= count(b))
int is_dyck(char *s, int start, int end) {
    if (start >= end) return 1; // empty string is valid
    if (s[start] != 'a') return 0; // must start with 'a'

    // Find matching 'b' for this 'a'
    int depth = 0;
    int i;
    for (i = start; i < end; i++) {
        if (s[i] == 'a') depth = depth + 1;
        else if (s[i] == 'b') depth = depth - 1;
        if (depth == 0) {
            // Found matching 'b' at position i
            // Check: inner part (start+1..i-1) must be valid
            // and rest (i+1..end) must be valid
            if (is_dyck(s, start + 1, i) && is_dyck(s, i + 1, end)) {
                return 1;
            }
            return 0;
        }
        if (depth < 0) return 0;
    }
    return 0; // unmatched
}

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len = len + 1;
    return len;
}

int check_dyck(char *s) {
    return is_dyck(s, 0, str_len(s));
}

// Derivation tree representation
struct DerivNode {
    int rule_used;    // 0=aSb, 1=ab, 2=SS, -1=leaf
    int start;
    int end;
    int split;        // for SS rule, where to split
};

// Try to build a derivation for the string
// Returns 1 if derivation found, 0 otherwise
int derive(char *s, int start, int end, struct DerivNode *nodes, int *node_count) {
    if (start >= end) return 1;
    if (end - start < 2) return 0;

    // Try rule: ab (exactly two chars)
    if (end - start == 2 && s[start] == 'a' && s[start + 1] == 'b') {
        int idx = *node_count;
        nodes[idx].rule_used = 1;
        nodes[idx].start = start;
        nodes[idx].end = end;
        nodes[idx].split = -1;
        *node_count = idx + 1;
        return 1;
    }

    // Try rule: aSb
    if (s[start] == 'a' && s[end - 1] == 'b') {
        if (derive(s, start + 1, end - 1, nodes, node_count)) {
            int idx = *node_count;
            nodes[idx].rule_used = 0;
            nodes[idx].start = start;
            nodes[idx].end = end;
            nodes[idx].split = -1;
            *node_count = idx + 1;
            return 1;
        }
    }

    // Try rule: SS (split into two parts)
    int mid;
    for (mid = start + 2; mid <= end - 2; mid = mid + 2) {
        int saved = *node_count;
        if (derive(s, start, mid, nodes, node_count)) {
            if (derive(s, mid, end, nodes, node_count)) {
                int idx = *node_count;
                nodes[idx].rule_used = 2;
                nodes[idx].start = start;
                nodes[idx].end = end;
                nodes[idx].split = mid;
                *node_count = idx + 1;
                return 1;
            }
        }
        *node_count = saved;
    }

    return 0;
}

void test_membership(char *s, int expected) {
    int result = check_dyck(s);
    if (result == expected) {
        printf("PASS: '%s' in L = %d\n", s, result);
    } else {
        printf("FAIL: '%s' in L = %d (expected %d)\n", s, result, expected);
    }
}

int main() {
    // Test Dyck language membership
    // EXPECT: PASS: 'ab' in L = 1
    test_membership("ab", 1);
    // EXPECT: PASS: 'aabb' in L = 1
    test_membership("aabb", 1);
    // EXPECT: PASS: 'abab' in L = 1
    test_membership("abab", 1);
    // EXPECT: PASS: 'aaabbb' in L = 1
    test_membership("aaabbb", 1);
    // EXPECT: PASS: 'aababb' in L = 1
    test_membership("aababb", 1);
    // EXPECT: PASS: 'ababab' in L = 1
    test_membership("ababab", 1);

    // Rejections
    // EXPECT: PASS: 'a' in L = 0
    test_membership("a", 0);
    // EXPECT: PASS: 'b' in L = 0
    test_membership("b", 0);
    // EXPECT: PASS: 'ba' in L = 0
    test_membership("ba", 0);
    // EXPECT: PASS: 'aab' in L = 0
    test_membership("aab", 0);
    // EXPECT: PASS: 'abb' in L = 0
    test_membership("abb", 0);
    // EXPECT: PASS: 'abba' in L = 0
    test_membership("abba", 0);

    // Test derivation
    struct DerivNode nodes[30];
    int node_count = 0;
    char *test_str = "aabb";
    int has_deriv = derive(test_str, 0, 4, nodes, &node_count);
    // EXPECT: Derivation of 'aabb': found=1 nodes=2
    printf("Derivation of 'aabb': found=%d nodes=%d\n", has_deriv, node_count);

    // Show rules used
    int i;
    for (i = 0; i < node_count; i++) {
        char *rule;
        if (nodes[i].rule_used == 0) rule = "aSb";
        else if (nodes[i].rule_used == 1) rule = "ab";
        else rule = "SS";
        // EXPECT: Rule: ab [1,3)
        // EXPECT: Rule: aSb [0,4)
        printf("Rule: %s [%d,%d)\n", rule, nodes[i].start, nodes[i].end);
    }

    // Derive "abab"
    node_count = 0;
    test_str = "abab";
    has_deriv = derive(test_str, 0, 4, nodes, &node_count);
    // EXPECT: Derivation of 'abab': found=1 nodes=3
    printf("Derivation of 'abab': found=%d nodes=%d\n", has_deriv, node_count);

    // Count Dyck strings of length 6
    int count = 0;
    char buf[7];
    int a; int b; int c; int d; int e; int f;
    char syms[2];
    syms[0] = 'a'; syms[1] = 'b';
    for (a = 0; a < 2; a++) {
      for (b = 0; b < 2; b++) {
        for (c = 0; c < 2; c++) {
          for (d = 0; d < 2; d++) {
            for (e = 0; e < 2; e++) {
              for (f = 0; f < 2; f++) {
                buf[0] = syms[a]; buf[1] = syms[b]; buf[2] = syms[c];
                buf[3] = syms[d]; buf[4] = syms[e]; buf[5] = syms[f];
                buf[6] = 0;
                if (check_dyck(buf)) count = count + 1;
              }
            }
          }
        }
      }
    }
    // Catalan number C(3) = 5 Dyck strings of length 6
    // EXPECT: Dyck strings of length 6: 5
    printf("Dyck strings of length 6: %d\n", count);

    // EXPECT: Grammar tests complete
    printf("Grammar tests complete\n");

    return 0;
}
