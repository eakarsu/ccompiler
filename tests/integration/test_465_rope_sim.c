int printf(const char *fmt, ...);
// EXPECT: String: Hello World!\nLength: 12\nDepth: 3\nChar at 0: H\nChar at 4: o\nChar at 5:  \nChar at 6: W\nChar at 11: !\nExtended: Hello World! :)\nNew length: 15\nNodes used: 9\nDone

// Rope data structure simulation for string manipulation

struct RopeNode {
    char data[16];
    int len;
    int left;
    int right;
    int weight;
    int is_leaf;
};

struct Rope {
    struct RopeNode nodes[32];
    int root;
    int next_id;
};

void rope_init(struct Rope *r) {
    r->root = -1;
    r->next_id = 0;
    int i = 0;
    while (i < 32) {
        r->nodes[i].left = -1;
        r->nodes[i].right = -1;
        r->nodes[i].len = 0;
        r->nodes[i].weight = 0;
        r->nodes[i].is_leaf = 0;
        i = i + 1;
    }
}

int rope_new_leaf(struct Rope *r, char *str, int len) {
    int id = r->next_id;
    r->next_id = r->next_id + 1;
    int i = 0;
    while (i < len && i < 15) {
        r->nodes[id].data[i] = str[i];
        i = i + 1;
    }
    r->nodes[id].data[i] = 0;
    r->nodes[id].len = len;
    r->nodes[id].weight = len;
    r->nodes[id].is_leaf = 1;
    r->nodes[id].left = -1;
    r->nodes[id].right = -1;
    return id;
}

int rope_total_weight(struct Rope *r, int node) {
    if (node < 0) return 0;
    if (r->nodes[node].is_leaf) return r->nodes[node].len;
    return rope_total_weight(r, r->nodes[node].left) +
           rope_total_weight(r, r->nodes[node].right);
}

int rope_concat(struct Rope *r, int left, int right) {
    int id = r->next_id;
    r->next_id = r->next_id + 1;
    r->nodes[id].is_leaf = 0;
    r->nodes[id].left = left;
    r->nodes[id].right = right;
    r->nodes[id].weight = rope_total_weight(r, left);
    r->nodes[id].len = 0;
    return id;
}

char rope_char_at(struct Rope *r, int node, int idx) {
    if (r->nodes[node].is_leaf) {
        return r->nodes[node].data[idx];
    }
    if (idx < r->nodes[node].weight) {
        return rope_char_at(r, r->nodes[node].left, idx);
    }
    return rope_char_at(r, r->nodes[node].right, idx - r->nodes[node].weight);
}

void rope_collect(struct Rope *r, int node, char *buf, int *pos) {
    if (node < 0) return;
    if (r->nodes[node].is_leaf) {
        int i = 0;
        while (i < r->nodes[node].len) {
            buf[*pos] = r->nodes[node].data[i];
            *pos = *pos + 1;
            i = i + 1;
        }
        return;
    }
    rope_collect(r, r->nodes[node].left, buf, pos);
    rope_collect(r, r->nodes[node].right, buf, pos);
}

void rope_print(struct Rope *r, int node) {
    char buf[64];
    int pos = 0;
    rope_collect(r, node, buf, &pos);
    buf[pos] = 0;
    printf("%s", buf);
}

int rope_length(struct Rope *r, int node) {
    return rope_total_weight(r, node);
}

int rope_depth(struct Rope *r, int node) {
    if (node < 0) return 0;
    if (r->nodes[node].is_leaf) return 1;
    int ld = rope_depth(r, r->nodes[node].left);
    int rd = rope_depth(r, r->nodes[node].right);
    return 1 + (ld > rd ? ld : rd);
}

int main(void) {
    struct Rope rope;
    rope_init(&rope);

    // Build "Hello" + " " + "World" + "!"
    char s1[6]; s1[0]='H'; s1[1]='e'; s1[2]='l'; s1[3]='l'; s1[4]='o'; s1[5]=0;
    char s2[2]; s2[0]=' '; s2[1]=0;
    char s3[6]; s3[0]='W'; s3[1]='o'; s3[2]='r'; s3[3]='l'; s3[4]='d'; s3[5]=0;
    char s4[2]; s4[0]='!'; s4[1]=0;

    int n1 = rope_new_leaf(&rope, s1, 5);
    int n2 = rope_new_leaf(&rope, s2, 1);
    int n3 = rope_new_leaf(&rope, s3, 5);
    int n4 = rope_new_leaf(&rope, s4, 1);

    int c1 = rope_concat(&rope, n1, n2);
    int c2 = rope_concat(&rope, n3, n4);
    int root = rope_concat(&rope, c1, c2);

    printf("String: ");
    rope_print(&rope, root);
    printf("\n");

    printf("Length: %d\n", rope_length(&rope, root));
    printf("Depth: %d\n", rope_depth(&rope, root));

    printf("Char at 0: %c\n", rope_char_at(&rope, root, 0));
    printf("Char at 4: %c\n", rope_char_at(&rope, root, 4));
    printf("Char at 5: %c\n", rope_char_at(&rope, root, 5));
    printf("Char at 6: %c\n", rope_char_at(&rope, root, 6));
    printf("Char at 11: %c\n", rope_char_at(&rope, root, 11));

    // Append more
    char s5[5]; s5[0]=' '; s5[1]=':'; s5[2]=')'; s5[3]=0;
    int n5 = rope_new_leaf(&rope, s5, 3);
    int root2 = rope_concat(&rope, root, n5);

    printf("Extended: ");
    rope_print(&rope, root2);
    printf("\n");
    printf("New length: %d\n", rope_length(&rope, root2));

    printf("Nodes used: %d\n", rope.next_id);
    printf("Done\n");
    return 0;
}
