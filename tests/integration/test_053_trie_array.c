int printf(const char *fmt, ...);
// EXPECT: T1:cat=1 car=1 card=1 dog=1 bat=0 cnt=4 T2:ca=1 do=1 ba=0 c=1 caCnt=3 doCnt=1 T3:cnt=4 delCar=1 car=0 card=1 cnt=3 T4:cnt=6 apple=1 app=1 ape=1 ant=1 and=1 an=1 aCnt=6 apCnt=3 anCnt=3 T5:a=1 b=1 c=1 d=0 z=0 cnt=3 T6:test=1 testing=1 tested=1 tester=1 tes=0 te=0 cnt=4 tesCnt=4 teCnt=4 T7:empty=0 search=0 prefix=0 del=0 T8:cnt=1 cnt=1 cnt=1 T9:full=1 half=0 pre=1 cnt=1 T10:lcp=2 cnt=3 T11:s=1 s=0 s=1 cnt=1 T12:cnt=10 cnt=5 sum=25 T13:abc=1 ab=0 a=0 abP=1 aP=1 a2=1 cnt=2 DONE

/* =========================================================================
 * Test 053: Trie Structure Using 2D Arrays
 * =========================================================================
 * Implements a trie (prefix tree) using fixed 2D arrays.
 * Supports insert, search, prefix check, count words, delete.
 * Words are represented as int arrays (using values 0-25 for a-z).
 * ========================================================================= */

/* Trie node pool: up to 200 nodes, each with 26 children */
int trie_children[200][26];
int trie_is_end[200];
int trie_count;

void trie_init(void) {
    int i;
    int j;
    trie_count = 1; /* node 0 is root */
    for (i = 0; i < 200; i = i + 1) {
        trie_is_end[i] = 0;
        for (j = 0; j < 26; j = j + 1) {
            trie_children[i][j] = -1;
        }
    }
}

int trie_new_node(void) {
    int n;
    int j;
    n = trie_count;
    trie_count = trie_count + 1;
    trie_is_end[n] = 0;
    for (j = 0; j < 26; j = j + 1) {
        trie_children[n][j] = -1;
    }
    return n;
}

/* Insert a word represented as int array, len = length */
void trie_insert(int *word, int len) {
    int node;
    int i;
    int c;
    node = 0;
    for (i = 0; i < len; i = i + 1) {
        c = word[i];
        if (trie_children[node][c] == -1) {
            trie_children[node][c] = trie_new_node();
        }
        node = trie_children[node][c];
    }
    trie_is_end[node] = 1;
}

/* Search for exact word match */
int trie_search(int *word, int len) {
    int node;
    int i;
    int c;
    node = 0;
    for (i = 0; i < len; i = i + 1) {
        c = word[i];
        if (trie_children[node][c] == -1) {
            return 0;
        }
        node = trie_children[node][c];
    }
    return trie_is_end[node];
}

/* Check if any word starts with given prefix */
int trie_starts_with(int *prefix, int len) {
    int node;
    int i;
    int c;
    node = 0;
    for (i = 0; i < len; i = i + 1) {
        c = prefix[i];
        if (trie_children[node][c] == -1) {
            return 0;
        }
        node = trie_children[node][c];
    }
    return 1;
}

/* Count total words in trie (DFS from a node) */
int trie_count_from(int node) {
    int total;
    int j;
    total = 0;
    if (trie_is_end[node]) {
        total = 1;
    }
    for (j = 0; j < 26; j = j + 1) {
        if (trie_children[node][j] != -1) {
            total = total + trie_count_from(trie_children[node][j]);
        }
    }
    return total;
}

int trie_count_words(void) {
    return trie_count_from(0);
}

/* Count words with a given prefix */
int trie_count_prefix(int *prefix, int len) {
    int node;
    int i;
    int c;
    node = 0;
    for (i = 0; i < len; i = i + 1) {
        c = prefix[i];
        if (trie_children[node][c] == -1) {
            return 0;
        }
        node = trie_children[node][c];
    }
    return trie_count_from(node);
}

/* Delete a word (just unmark end) */
int trie_delete(int *word, int len) {
    int node;
    int i;
    int c;
    node = 0;
    for (i = 0; i < len; i = i + 1) {
        c = word[i];
        if (trie_children[node][c] == -1) {
            return 0;
        }
        node = trie_children[node][c];
    }
    if (trie_is_end[node]) {
        trie_is_end[node] = 0;
        return 1;
    }
    return 0;
}

/* Check if node has any children */
int trie_has_children(int node) {
    int j;
    for (j = 0; j < 26; j = j + 1) {
        if (trie_children[node][j] != -1) {
            return 1;
        }
    }
    return 0;
}

/* Find longest common prefix length among all words from root */
int trie_lcp_len(void) {
    int node;
    int len;
    int child_count;
    int next;
    int j;

    node = 0;
    len = 0;
    while (1) {
        if (trie_is_end[node]) {
            return len;
        }
        child_count = 0;
        next = -1;
        for (j = 0; j < 26; j = j + 1) {
            if (trie_children[node][j] != -1) {
                child_count = child_count + 1;
                next = trie_children[node][j];
            }
        }
        if (child_count != 1) {
            return len;
        }
        node = next;
        len = len + 1;
    }
    return len;
}

/* =========================================================================
 * Test functions
 * ========================================================================= */

void test_basic_insert_search(void) {
    /* Words: cat(2,0,19), car(2,0,17), card(2,0,17,3), dog(3,14,6) */
    int cat[3];
    int car[3];
    int card[4];
    int dog[3];
    int bat[3];

    printf("T1:");
    trie_init();

    cat[0] = 2; cat[1] = 0; cat[2] = 19;       /* c,a,t */
    car[0] = 2; car[1] = 0; car[2] = 17;       /* c,a,r */
    card[0] = 2; card[1] = 0; card[2] = 17; card[3] = 3; /* c,a,r,d */
    dog[0] = 3; dog[1] = 14; dog[2] = 6;       /* d,o,g */
    bat[0] = 1; bat[1] = 0; bat[2] = 19;       /* b,a,t */

    trie_insert(cat, 3);
    trie_insert(car, 3);
    trie_insert(card, 4);
    trie_insert(dog, 3);

    printf("cat=%d", trie_search(cat, 3));
    printf(" car=%d", trie_search(car, 3));
    printf(" card=%d", trie_search(card, 4));
    printf(" dog=%d", trie_search(dog, 3));
    printf(" bat=%d", trie_search(bat, 3));
    printf(" cnt=%d", trie_count_words());
}

void test_prefix_search(void) {
    /* Trie still has cat, car, card, dog from test_basic */
    int ca[2];
    int ca_prefix[2];
    int do_pre[2];
    int ba_pre[2];
    int c_pre[1];

    printf(" T2:");

    ca[0] = 2; ca[1] = 0;              /* c,a */
    do_pre[0] = 3; do_pre[1] = 14;     /* d,o */
    ba_pre[0] = 1; ba_pre[1] = 0;      /* b,a */
    c_pre[0] = 2;                       /* c */

    printf("ca=%d", trie_starts_with(ca, 2));
    printf(" do=%d", trie_starts_with(do_pre, 2));
    printf(" ba=%d", trie_starts_with(ba_pre, 2));
    printf(" c=%d", trie_starts_with(c_pre, 1));

    /* Count words with prefix ca: cat, car, card = 3 */
    printf(" caCnt=%d", trie_count_prefix(ca, 2));
    /* Count words with prefix do: dog = 1 */
    printf(" doCnt=%d", trie_count_prefix(do_pre, 2));
}

void test_delete(void) {
    int car[3];
    int card[4];

    printf(" T3:");

    car[0] = 2; car[1] = 0; car[2] = 17;
    card[0] = 2; card[1] = 0; card[2] = 17; card[3] = 3;

    printf("cnt=%d", trie_count_words());
    /* Delete car */
    printf(" delCar=%d", trie_delete(car, 3));
    printf(" car=%d", trie_search(car, 3));
    printf(" card=%d", trie_search(card, 4));
    printf(" cnt=%d", trie_count_words());
}

void test_many_words(void) {
    /* Insert: apple(0,15,15,11,4), app(0,15,15), ape(0,15,4),
       ant(0,13,19), and(0,13,3), an(0,13) */
    int apple[5];
    int app[3];
    int ape[3];
    int ant[3];
    int andd[3];
    int an[2];
    int a_pre[1];
    int ap_pre[2];
    int an_pre[2];

    printf(" T4:");
    trie_init();

    apple[0]=0; apple[1]=15; apple[2]=15; apple[3]=11; apple[4]=4;
    app[0]=0; app[1]=15; app[2]=15;
    ape[0]=0; ape[1]=15; ape[2]=4;
    ant[0]=0; ant[1]=13; ant[2]=19;
    andd[0]=0; andd[1]=13; andd[2]=3;
    an[0]=0; an[1]=13;

    trie_insert(apple, 5);
    trie_insert(app, 3);
    trie_insert(ape, 3);
    trie_insert(ant, 3);
    trie_insert(andd, 3);
    trie_insert(an, 2);

    printf("cnt=%d", trie_count_words());
    printf(" apple=%d", trie_search(apple, 5));
    printf(" app=%d", trie_search(app, 3));
    printf(" ape=%d", trie_search(ape, 3));
    printf(" ant=%d", trie_search(ant, 3));
    printf(" and=%d", trie_search(andd, 3));
    printf(" an=%d", trie_search(an, 2));

    a_pre[0] = 0;
    ap_pre[0] = 0; ap_pre[1] = 15;
    an_pre[0] = 0; an_pre[1] = 13;

    printf(" aCnt=%d", trie_count_prefix(a_pre, 1));
    printf(" apCnt=%d", trie_count_prefix(ap_pre, 2));
    printf(" anCnt=%d", trie_count_prefix(an_pre, 2));
}

void test_single_char_words(void) {
    int a[1];
    int b[1];
    int c[1];
    int d[1];
    int z[1];

    printf(" T5:");
    trie_init();

    a[0] = 0; b[0] = 1; c[0] = 2; d[0] = 3; z[0] = 25;

    trie_insert(a, 1);
    trie_insert(b, 1);
    trie_insert(c, 1);

    printf("a=%d", trie_search(a, 1));
    printf(" b=%d", trie_search(b, 1));
    printf(" c=%d", trie_search(c, 1));
    printf(" d=%d", trie_search(d, 1));
    printf(" z=%d", trie_search(z, 1));
    printf(" cnt=%d", trie_count_words());
}

void test_overlapping_prefixes(void) {
    /* test, testing, tested, tester */
    int test_w[4];
    int testing[7];
    int tested[6];
    int tester[6];
    int tes[3];
    int te[2];

    printf(" T6:");
    trie_init();

    test_w[0]=19; test_w[1]=4; test_w[2]=18; test_w[3]=19;
    testing[0]=19; testing[1]=4; testing[2]=18; testing[3]=19;
    testing[4]=8; testing[5]=13; testing[6]=6;
    tested[0]=19; tested[1]=4; tested[2]=18; tested[3]=19;
    tested[4]=4; tested[5]=3;
    tester[0]=19; tester[1]=4; tester[2]=18; tester[3]=19;
    tester[4]=4; tester[5]=17;
    tes[0]=19; tes[1]=4; tes[2]=18;
    te[0]=19; te[1]=4;

    trie_insert(test_w, 4);
    trie_insert(testing, 7);
    trie_insert(tested, 6);
    trie_insert(tester, 6);

    printf("test=%d", trie_search(test_w, 4));
    printf(" testing=%d", trie_search(testing, 7));
    printf(" tested=%d", trie_search(tested, 6));
    printf(" tester=%d", trie_search(tester, 6));
    printf(" tes=%d", trie_search(tes, 3));
    printf(" te=%d", trie_search(te, 2));
    printf(" cnt=%d", trie_count_words());

    printf(" tesCnt=%d", trie_count_prefix(tes, 3));
    printf(" teCnt=%d", trie_count_prefix(te, 2));
}

void test_empty_trie(void) {
    int word[3];

    printf(" T7:");
    trie_init();

    word[0] = 0; word[1] = 1; word[2] = 2;

    printf("empty=%d", trie_count_words());
    printf(" search=%d", trie_search(word, 3));
    printf(" prefix=%d", trie_starts_with(word, 1));
    printf(" del=%d", trie_delete(word, 3));
}

void test_duplicate_insert(void) {
    int word[3];

    printf(" T8:");
    trie_init();

    word[0] = 5; word[1] = 14; word[2] = 23; /* f,o,x */

    trie_insert(word, 3);
    printf("cnt=%d", trie_count_words());
    trie_insert(word, 3);
    printf(" cnt=%d", trie_count_words());
    trie_insert(word, 3);
    printf(" cnt=%d", trie_count_words());
}

void test_long_word(void) {
    int word[10];
    int pre[5];
    int i;

    printf(" T9:");
    trie_init();

    /* Word: abcdefghij => 0,1,2,3,4,5,6,7,8,9 */
    for (i = 0; i < 10; i = i + 1) {
        word[i] = i;
    }
    for (i = 0; i < 5; i = i + 1) {
        pre[i] = i;
    }

    trie_insert(word, 10);
    printf("full=%d", trie_search(word, 10));
    printf(" half=%d", trie_search(pre, 5));
    printf(" pre=%d", trie_starts_with(pre, 5));
    printf(" cnt=%d", trie_count_words());
}

void test_lcp(void) {
    /* Words sharing common prefix */
    int w1[4];
    int w2[4];
    int w3[5];

    printf(" T10:");
    trie_init();

    /* abc, abd, abef */
    w1[0]=0; w1[1]=1; w1[2]=2;
    w2[0]=0; w2[1]=1; w2[2]=3;
    w3[0]=0; w3[1]=1; w3[2]=4; w3[3]=5;

    trie_insert(w1, 3);
    trie_insert(w2, 3);
    trie_insert(w3, 4);

    printf("lcp=%d", trie_lcp_len());
    printf(" cnt=%d", trie_count_words());
}

void test_delete_and_reinsert(void) {
    int word[3];

    printf(" T11:");
    trie_init();

    word[0] = 7; word[1] = 8; word[2] = 9; /* h,i,j */

    trie_insert(word, 3);
    printf("s=%d", trie_search(word, 3));
    trie_delete(word, 3);
    printf(" s=%d", trie_search(word, 3));
    trie_insert(word, 3);
    printf(" s=%d", trie_search(word, 3));
    printf(" cnt=%d", trie_count_words());
}

void test_many_single_chars(void) {
    int w[1];
    int i;
    int total;

    printf(" T12:");
    trie_init();

    /* Insert a through j (0-9) */
    for (i = 0; i < 10; i = i + 1) {
        w[0] = i;
        trie_insert(w, 1);
    }
    printf("cnt=%d", trie_count_words());

    /* Delete every other */
    for (i = 0; i < 10; i = i + 2) {
        w[0] = i;
        trie_delete(w, 1);
    }
    printf(" cnt=%d", trie_count_words());

    /* Check which remain */
    total = 0;
    for (i = 0; i < 10; i = i + 1) {
        w[0] = i;
        if (trie_search(w, 1)) {
            total = total + i;
        }
    }
    printf(" sum=%d", total);
}

void test_prefix_only(void) {
    /* Insert "abc" but check that "ab" is not found as a word */
    int abc[3];
    int ab[2];
    int a[1];

    printf(" T13:");
    trie_init();

    abc[0]=0; abc[1]=1; abc[2]=2;
    ab[0]=0; ab[1]=1;
    a[0]=0;

    trie_insert(abc, 3);
    printf("abc=%d", trie_search(abc, 3));
    printf(" ab=%d", trie_search(ab, 2));
    printf(" a=%d", trie_search(a, 1));
    printf(" abP=%d", trie_starts_with(ab, 2));
    printf(" aP=%d", trie_starts_with(a, 1));

    /* Now insert "a" */
    trie_insert(a, 1);
    printf(" a2=%d", trie_search(a, 1));
    printf(" cnt=%d", trie_count_words());
    printf(" DONE\n");
}

int main(void) {
    test_basic_insert_search();
    test_prefix_search();
    test_delete();
    test_many_words();
    test_single_char_words();
    test_overlapping_prefixes();
    test_empty_trie();
    test_duplicate_insert();
    test_long_word();
    test_lcp();
    test_delete_and_reinsert();
    test_many_single_chars();
    test_prefix_only();
    return 0;
}
