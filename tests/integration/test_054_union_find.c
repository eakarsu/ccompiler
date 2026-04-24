int printf(const char *fmt, ...);
// EXPECT: T1:comp=10 c01=0 u01 c01=1 comp=9 u23 c23=1 c02=0 u02 c02=1 c13=1 comp=7 T2:comp=1 c07=1 c36=1 sz=8 T3:comp=2 c04=1 c59=1 c09=0 szA=5 szB=5 merge c09=1 comp=1 sz=10 T4:comp=5 c00=1 c01=0 T5:comp=1 sz=20 c019=1 c515=1 lg=20 T6:f0=4 p0=4 comp=1 T7:comp=4 sz0=3 sz3=4 sz7=2 sz9=6 lg=6 merge comp=3 sz0=5 T8:comp=5 c01=1 c02=0 comp=2 c07=1 T9:comp=10 u01:9 u12:8 u23:7 u34:6 u45:5 u56:4 u67:3 u78:2 u89:1 T10:islands=2 c01=1 c05=1 c1011=1 c1115=1 c015=0 T11:mstW=18 mstE=5 comp=1 T12:comp=2 cEven=1 cOdd=1 cMix=0 comp=1 c049=1 DONE

/* =========================================================================
 * Test 054: Disjoint Set Union-Find with Path Compression and Union by Rank
 * =========================================================================
 * Implements union-find data structure using arrays.
 * Tests: find, union, connected, component counting, path compression.
 * ========================================================================= */

int uf_parent[100];
int uf_rank[100];
int uf_size[100];
int uf_n;

void uf_init(int n) {
    int i;
    uf_n = n;
    for (i = 0; i < n; i = i + 1) {
        uf_parent[i] = i;
        uf_rank[i] = 0;
        uf_size[i] = 1;
    }
}

int uf_find(int x) {
    /* Path compression */
    if (uf_parent[x] != x) {
        uf_parent[x] = uf_find(uf_parent[x]);
    }
    return uf_parent[x];
}

int uf_connected(int x, int y) {
    return uf_find(x) == uf_find(y);
}

void uf_union(int x, int y) {
    int rx;
    int ry;
    rx = uf_find(x);
    ry = uf_find(y);
    if (rx == ry) {
        return;
    }
    /* Union by rank */
    if (uf_rank[rx] < uf_rank[ry]) {
        uf_parent[rx] = ry;
        uf_size[ry] = uf_size[ry] + uf_size[rx];
    } else if (uf_rank[rx] > uf_rank[ry]) {
        uf_parent[ry] = rx;
        uf_size[rx] = uf_size[rx] + uf_size[ry];
    } else {
        uf_parent[ry] = rx;
        uf_size[rx] = uf_size[rx] + uf_size[ry];
        uf_rank[rx] = uf_rank[rx] + 1;
    }
}

int uf_component_count(void) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < uf_n; i = i + 1) {
        if (uf_find(i) == i) {
            count = count + 1;
        }
    }
    return count;
}

int uf_component_size(int x) {
    return uf_size[uf_find(x)];
}

int uf_largest_component(void) {
    int mx;
    int i;
    mx = 0;
    for (i = 0; i < uf_n; i = i + 1) {
        if (uf_find(i) == i) {
            if (uf_size[i] > mx) {
                mx = uf_size[i];
            }
        }
    }
    return mx;
}

/* =========================================================================
 * Second union-find for independent testing
 * ========================================================================= */
int uf2_parent[50];
int uf2_rank[50];
int uf2_n;

void uf2_init(int n) {
    int i;
    uf2_n = n;
    for (i = 0; i < n; i = i + 1) {
        uf2_parent[i] = i;
        uf2_rank[i] = 0;
    }
}

int uf2_find(int x) {
    if (uf2_parent[x] != x) {
        uf2_parent[x] = uf2_find(uf2_parent[x]);
    }
    return uf2_parent[x];
}

void uf2_union(int x, int y) {
    int rx;
    int ry;
    rx = uf2_find(x);
    ry = uf2_find(y);
    if (rx == ry) { return; }
    if (uf2_rank[rx] < uf2_rank[ry]) {
        uf2_parent[rx] = ry;
    } else if (uf2_rank[rx] > uf2_rank[ry]) {
        uf2_parent[ry] = rx;
    } else {
        uf2_parent[ry] = rx;
        uf2_rank[rx] = uf2_rank[rx] + 1;
    }
}

int uf2_connected(int x, int y) {
    return uf2_find(x) == uf2_find(y);
}

int uf2_count(void) {
    int c;
    int i;
    c = 0;
    for (i = 0; i < uf2_n; i = i + 1) {
        if (uf2_find(i) == i) { c = c + 1; }
    }
    return c;
}

/* =========================================================================
 * Test functions
 * ========================================================================= */

void test_basic(void) {
    printf("T1:");
    uf_init(10);

    printf("comp=%d", uf_component_count());
    printf(" c01=%d", uf_connected(0, 1));

    uf_union(0, 1);
    printf(" u01 c01=%d", uf_connected(0, 1));
    printf(" comp=%d", uf_component_count());

    uf_union(2, 3);
    printf(" u23 c23=%d", uf_connected(2, 3));
    printf(" c02=%d", uf_connected(0, 2));

    uf_union(0, 2);
    printf(" u02 c02=%d", uf_connected(0, 2));
    printf(" c13=%d", uf_connected(1, 3));
    printf(" comp=%d", uf_component_count());
}

void test_chain_union(void) {
    printf(" T2:");
    uf_init(8);

    /* Chain: 0-1-2-3-4-5-6-7 */
    uf_union(0, 1);
    uf_union(1, 2);
    uf_union(2, 3);
    uf_union(3, 4);
    uf_union(4, 5);
    uf_union(5, 6);
    uf_union(6, 7);

    printf("comp=%d", uf_component_count());
    printf(" c07=%d", uf_connected(0, 7));
    printf(" c36=%d", uf_connected(3, 6));
    printf(" sz=%d", uf_component_size(0));
}

void test_two_groups(void) {
    printf(" T3:");
    uf_init(10);

    /* Group A: 0,1,2,3,4 */
    uf_union(0, 1);
    uf_union(1, 2);
    uf_union(2, 3);
    uf_union(3, 4);

    /* Group B: 5,6,7,8,9 */
    uf_union(5, 6);
    uf_union(6, 7);
    uf_union(7, 8);
    uf_union(8, 9);

    printf("comp=%d", uf_component_count());
    printf(" c04=%d", uf_connected(0, 4));
    printf(" c59=%d", uf_connected(5, 9));
    printf(" c09=%d", uf_connected(0, 9));
    printf(" szA=%d", uf_component_size(0));
    printf(" szB=%d", uf_component_size(5));

    /* Merge the two groups */
    uf_union(4, 5);
    printf(" merge c09=%d", uf_connected(0, 9));
    printf(" comp=%d", uf_component_count());
    printf(" sz=%d", uf_component_size(0));
}

void test_self_union(void) {
    printf(" T4:");
    uf_init(5);

    uf_union(0, 0);
    uf_union(1, 1);
    printf("comp=%d", uf_component_count());
    printf(" c00=%d", uf_connected(0, 0));
    printf(" c01=%d", uf_connected(0, 1));
}

void test_star_topology(void) {
    int i;
    printf(" T5:");
    uf_init(20);

    /* Star: all connect to 0 */
    for (i = 1; i < 20; i = i + 1) {
        uf_union(0, i);
    }

    printf("comp=%d", uf_component_count());
    printf(" sz=%d", uf_component_size(0));
    printf(" c019=%d", uf_connected(0, 19));
    printf(" c515=%d", uf_connected(5, 15));
    printf(" lg=%d", uf_largest_component());
}

void test_path_compression(void) {
    printf(" T6:");
    uf_init(6);

    /* Build a chain without path compression visible at first:
       0->1->2->3->4->5 via sequential unions */
    uf_union(4, 5);
    uf_union(3, 4);
    uf_union(2, 3);
    uf_union(1, 2);
    uf_union(0, 1);

    /* After find(0), path should be compressed */
    printf("f0=%d", uf_find(0));
    /* Now check that parent of 0 points directly to root */
    printf(" p0=%d", uf_parent[0]);
    printf(" comp=%d", uf_component_count());
}

void test_multiple_components(void) {
    printf(" T7:");
    uf_init(15);

    /* Component 1: {0,1,2} */
    uf_union(0, 1);
    uf_union(1, 2);

    /* Component 2: {3,4,5,6} */
    uf_union(3, 4);
    uf_union(4, 5);
    uf_union(5, 6);

    /* Component 3: {7,8} */
    uf_union(7, 8);

    /* Component 4: {9,10,11,12,13,14} */
    uf_union(9, 10);
    uf_union(10, 11);
    uf_union(11, 12);
    uf_union(12, 13);
    uf_union(13, 14);

    printf("comp=%d", uf_component_count());
    printf(" sz0=%d", uf_component_size(0));
    printf(" sz3=%d", uf_component_size(3));
    printf(" sz7=%d", uf_component_size(7));
    printf(" sz9=%d", uf_component_size(9));
    printf(" lg=%d", uf_largest_component());

    /* Merge comp 1 and 3 */
    uf_union(2, 7);
    printf(" merge comp=%d", uf_component_count());
    printf(" sz0=%d", uf_component_size(0));
}

void test_second_uf(void) {
    printf(" T8:");
    uf2_init(10);

    uf2_union(0, 1);
    uf2_union(2, 3);
    uf2_union(4, 5);
    uf2_union(6, 7);
    uf2_union(8, 9);

    printf("comp=%d", uf2_count());
    printf(" c01=%d", uf2_connected(0, 1));
    printf(" c02=%d", uf2_connected(0, 2));

    uf2_union(0, 2);
    uf2_union(4, 6);
    uf2_union(0, 4);
    printf(" comp=%d", uf2_count());
    printf(" c07=%d", uf2_connected(0, 7));
}

void test_incremental_union(void) {
    int i;
    printf(" T9:");
    uf_init(10);

    printf("comp=%d", uf_component_count());
    for (i = 0; i < 9; i = i + 1) {
        uf_union(i, i + 1);
        printf(" u%d%d:%d", i, i + 1, uf_component_count());
    }
}

void test_island_counting(void) {
    /* Grid-based connected components.
       Grid 4x4, 1=land, 0=water.
       Map cells to indices 0..15 (row*4+col).
       Connect adjacent land cells. */
    int grid[16];
    int i;
    int r;
    int c;
    int idx;
    int ridx;
    int didx;

    printf(" T10:");

    /* Grid:
       1 1 0 0
       0 1 0 0
       0 0 1 1
       0 0 0 1
    */
    grid[0]=1; grid[1]=1; grid[2]=0; grid[3]=0;
    grid[4]=0; grid[5]=1; grid[6]=0; grid[7]=0;
    grid[8]=0; grid[9]=0; grid[10]=1; grid[11]=1;
    grid[12]=0; grid[13]=0; grid[14]=0; grid[15]=1;

    uf_init(16);

    /* Connect adjacent land cells */
    for (r = 0; r < 4; r = r + 1) {
        for (c = 0; c < 4; c = c + 1) {
            idx = r * 4 + c;
            if (grid[idx] == 1) {
                /* Check right neighbor */
                if (c + 1 < 4) {
                    ridx = r * 4 + (c + 1);
                    if (grid[ridx] == 1) {
                        uf_union(idx, ridx);
                    }
                }
                /* Check down neighbor */
                if (r + 1 < 4) {
                    didx = (r + 1) * 4 + c;
                    if (grid[didx] == 1) {
                        uf_union(idx, didx);
                    }
                }
            }
        }
    }

    /* Count land components */
    {
        int land_comp;
        int seen[16];
        int root;
        land_comp = 0;
        for (i = 0; i < 16; i = i + 1) {
            seen[i] = 0;
        }
        for (i = 0; i < 16; i = i + 1) {
            if (grid[i] == 1) {
                root = uf_find(i);
                if (seen[root] == 0) {
                    seen[root] = 1;
                    land_comp = land_comp + 1;
                }
            }
        }
        printf("islands=%d", land_comp);
    }

    printf(" c01=%d", uf_connected(0, 1));
    printf(" c05=%d", uf_connected(0, 5));
    printf(" c1011=%d", uf_connected(10, 11));
    printf(" c1115=%d", uf_connected(11, 15));
    printf(" c015=%d", uf_connected(0, 15));
}

void test_kruskal_like(void) {
    /* Simulate Kruskal's: edges sorted by weight, union if not connected */
    /* 6 nodes, edges: (0,1,1),(1,2,2),(0,2,3),(3,4,4),(2,3,5),(4,5,6),(3,5,7) */
    int eu[7];
    int ev[7];
    int ew[7];
    int mst_weight;
    int mst_edges;
    int i;

    printf(" T11:");
    uf_init(6);

    eu[0]=0; ev[0]=1; ew[0]=1;
    eu[1]=1; ev[1]=2; ew[1]=2;
    eu[2]=0; ev[2]=2; ew[2]=3;
    eu[3]=3; ev[3]=4; ew[3]=4;
    eu[4]=2; ev[4]=3; ew[4]=5;
    eu[5]=4; ev[5]=5; ew[5]=6;
    eu[6]=3; ev[6]=5; ew[6]=7;

    mst_weight = 0;
    mst_edges = 0;

    for (i = 0; i < 7; i = i + 1) {
        if (uf_connected(eu[i], ev[i]) == 0) {
            uf_union(eu[i], ev[i]);
            mst_weight = mst_weight + ew[i];
            mst_edges = mst_edges + 1;
        }
    }

    printf("mstW=%d", mst_weight);
    printf(" mstE=%d", mst_edges);
    printf(" comp=%d", uf_component_count());
}

void test_large_set(void) {
    int i;
    printf(" T12:");
    uf_init(50);

    /* Union all even numbers together */
    for (i = 0; i < 48; i = i + 2) {
        uf_union(i, i + 2);
    }

    /* Union all odd numbers together */
    for (i = 1; i < 48; i = i + 2) {
        uf_union(i, i + 2);
    }

    printf("comp=%d", uf_component_count());
    printf(" cEven=%d", uf_connected(0, 48));
    printf(" cOdd=%d", uf_connected(1, 49));
    printf(" cMix=%d", uf_connected(0, 1));

    /* Merge even and odd */
    uf_union(0, 1);
    printf(" comp=%d", uf_component_count());
    printf(" c049=%d", uf_connected(0, 49));
    printf(" DONE\n");
}

/* =========================================================================
 * Helper: verify that all elements in same component have same root
 * Used internally for correctness checking
 * ========================================================================= */
int verify_consistency(int n) {
    int i;
    int root;
    int ok;
    ok = 1;
    for (i = 0; i < n; i = i + 1) {
        root = uf_find(i);
        if (root < 0 || root >= n) {
            ok = 0;
        }
        /* After find, parent should point directly to root */
        if (uf_parent[i] != root) {
            ok = 0;
        }
    }
    return ok;
}

int main(void) {
    test_basic();
    test_chain_union();
    test_two_groups();
    test_self_union();
    test_star_topology();
    test_path_compression();
    test_multiple_components();
    test_second_uf();
    test_incremental_union();
    test_island_counting();
    test_kruskal_like();
    test_large_set();
    return 0;
}
