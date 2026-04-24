int printf(const char *fmt, ...);
// EXPECT: Hash Functions\nKey=7: 7 11 7 0\nKey=17: 1 0 1 2\nKey=27: 11 5 11 7\nKey=37: 5 10 5 13\nKey=47: 15 15 15 6\nLinear Probing\nInsert 3 probes1\nInsert 10 probes1\nInsert 17 probes1\nInsert 24 probes1\nInsert 31 probes1\nInsert 129 probes1\nInsert 136 probes1\nSearch 3=0\nSearch 38=500\nSearch 999=-1\nCount=20\nCollisions0\nLongCluster3\nClusters9\nLoadFact=62\nAvgProbe=10\nMaxProbeD=0\nAfterDel  38=-1\nCountAftDel19\nChaining\nChSearch 3=0\nChSearch 38=500\nChainCnt=20\nMaxChain=2\nEmptyBkts=0\nChLoadPc=125\nVarx100=19\nQuadratic Probing\nQPInsert 3 probes1\nQPInsert 10 probes1\nQPInsert 17 probes1\nQPSearch 3=0\nQPSearch 73=1000\nUniversalH\nUHat 5 ->6\nUHat 15 ->5\nUHat 25 ->4\nUHat 35 ->3\nUHat 45 ->2\nDistScore2\nRehashMv647\nAll  passed=1

// Multiple hash function implementations, collision resolution, load factor analysis

// Hash table using open addressing (linear probing)
int ht_keys[64];
int ht_vals[64];
int ht_occupied[64]; // 0=empty, 1=occupied, 2=deleted
int ht_size;
int ht_count;

// Second hash table using chaining (simulated with arrays)
int chain_keys[64][8];  // each bucket holds up to 8 entries
int chain_vals[64][8];
int chain_len[64];       // number of entries in each bucket
int chain_size;
int chain_count;

void init_linear_ht(int size) {
    int i;
    ht_size = size;
    ht_count = 0;
    for (i = 0; i < 64; i = i + 1) {
        ht_keys[i] = 0;
        ht_vals[i] = 0;
        ht_occupied[i] = 0;
    }
}

void init_chain_ht(int size) {
    int i;
    int j;
    chain_size = size;
    chain_count = 0;
    for (i = 0; i < 64; i = i + 1) {
        chain_len[i] = 0;
        for (j = 0; j < 8; j = j + 1) {
            chain_keys[i][j] = 0;
            chain_vals[i][j] = 0;
        }
    }
}

// Hash function 1: simple modulo
int hash1(int key, int size) {
    int k;
    k = key;
    if (k < 0) {
        k = -k;
    }
    return k - (k / size) * size;
}

// Hash function 2: multiplicative (using integer approximation)
// h(k) = ((k * 2654435) >> 16) % size  (simplified)
int hash2(int key, int size) {
    int k;
    int h;
    k = key;
    if (k < 0) {
        k = -k;
    }
    h = (k * 2654435) / 65536;
    if (h < 0) {
        h = -h;
    }
    return h - (h / size) * size;
}

// Hash function 3: folding
int hash3(int key, int size) {
    int k;
    int h;
    k = key;
    if (k < 0) {
        k = -k;
    }
    h = (k / 1000) + (k - (k / 1000) * 1000);
    return h - (h / size) * size;
}

// Hash function 4: mid-square
int hash4(int key, int size) {
    int k;
    int sq;
    int mid;
    k = key;
    if (k < 0) {
        k = -k;
    }
    sq = k * k;
    if (sq < 0) {
        sq = -sq;
    }
    mid = (sq / 100) - ((sq / 100) / 100) * 100;
    if (mid < 0) {
        mid = -mid;
    }
    return mid - (mid / size) * size;
}

// Linear probing insert
int lp_insert(int key, int val) {
    int h;
    int i;
    int idx;
    int probes;
    if (ht_count >= ht_size) {
        return -1; // full
    }
    h = hash1(key, ht_size);
    probes = 0;
    for (i = 0; i < ht_size; i = i + 1) {
        idx = (h + i) - ((h + i) / ht_size) * ht_size;
        probes = probes + 1;
        if (ht_occupied[idx] == 0 || ht_occupied[idx] == 2) {
            ht_keys[idx] = key;
            ht_vals[idx] = val;
            ht_occupied[idx] = 1;
            ht_count = ht_count + 1;
            return probes;
        }
        if (ht_occupied[idx] == 1 && ht_keys[idx] == key) {
            ht_vals[idx] = val; // update
            return probes;
        }
    }
    return -1;
}

// Linear probing search
int lp_search(int key) {
    int h;
    int i;
    int idx;
    h = hash1(key, ht_size);
    for (i = 0; i < ht_size; i = i + 1) {
        idx = (h + i) - ((h + i) / ht_size) * ht_size;
        if (ht_occupied[idx] == 0) {
            return -1; // not found
        }
        if (ht_occupied[idx] == 1 && ht_keys[idx] == key) {
            return ht_vals[idx];
        }
    }
    return -1;
}

// Linear probing delete
int lp_delete(int key) {
    int h;
    int i;
    int idx;
    h = hash1(key, ht_size);
    for (i = 0; i < ht_size; i = i + 1) {
        idx = (h + i) - ((h + i) / ht_size) * ht_size;
        if (ht_occupied[idx] == 0) {
            return 0;
        }
        if (ht_occupied[idx] == 1 && ht_keys[idx] == key) {
            ht_occupied[idx] = 2; // tombstone
            ht_count = ht_count - 1;
            return 1;
        }
    }
    return 0;
}

// Quadratic probing insert
int qp_keys[64];
int qp_vals[64];
int qp_occupied[64];
int qp_size;
int qp_count;

void init_qp_ht(int size) {
    int i;
    qp_size = size;
    qp_count = 0;
    for (i = 0; i < 64; i = i + 1) {
        qp_keys[i] = 0;
        qp_vals[i] = 0;
        qp_occupied[i] = 0;
    }
}

int qp_insert(int key, int val) {
    int h;
    int i;
    int idx;
    int probes;
    if (qp_count >= qp_size) {
        return -1;
    }
    h = hash1(key, qp_size);
    probes = 0;
    for (i = 0; i < qp_size; i = i + 1) {
        idx = (h + i * i);
        idx = idx - (idx / qp_size) * qp_size;
        probes = probes + 1;
        if (qp_occupied[idx] == 0) {
            qp_keys[idx] = key;
            qp_vals[idx] = val;
            qp_occupied[idx] = 1;
            qp_count = qp_count + 1;
            return probes;
        }
        if (qp_occupied[idx] == 1 && qp_keys[idx] == key) {
            qp_vals[idx] = val;
            return probes;
        }
    }
    return -1;
}

int qp_search(int key) {
    int h;
    int i;
    int idx;
    h = hash1(key, qp_size);
    for (i = 0; i < qp_size; i = i + 1) {
        idx = (h + i * i);
        idx = idx - (idx / qp_size) * qp_size;
        if (qp_occupied[idx] == 0) {
            return -1;
        }
        if (qp_occupied[idx] == 1 && qp_keys[idx] == key) {
            return qp_vals[idx];
        }
    }
    return -1;
}

// Chaining insert
int ch_insert(int key, int val) {
    int h;
    int i;
    int bucket;
    h = hash1(key, chain_size);
    bucket = h;
    // Check if key already exists
    for (i = 0; i < chain_len[bucket]; i = i + 1) {
        if (chain_keys[bucket][i] == key) {
            chain_vals[bucket][i] = val;
            return chain_len[bucket];
        }
    }
    if (chain_len[bucket] < 8) {
        chain_keys[bucket][chain_len[bucket]] = key;
        chain_vals[bucket][chain_len[bucket]] = val;
        chain_len[bucket] = chain_len[bucket] + 1;
        chain_count = chain_count + 1;
    }
    return chain_len[bucket];
}

// Chaining search
int ch_search(int key) {
    int h;
    int i;
    int bucket;
    h = hash1(key, chain_size);
    bucket = h;
    for (i = 0; i < chain_len[bucket]; i = i + 1) {
        if (chain_keys[bucket][i] == key) {
            return chain_vals[bucket][i];
        }
    }
    return -1;
}

// Count collisions in linear probing table
int count_collisions_lp(void) {
    int collisions;
    int i;
    int h;
    collisions = 0;
    for (i = 0; i < ht_size; i = i + 1) {
        if (ht_occupied[i] == 1) {
            h = hash1(ht_keys[i], ht_size);
            if (h != i) {
                collisions = collisions + 1;
            }
        }
    }
    return collisions;
}

// Max chain length
int max_chain_length(void) {
    int mx;
    int i;
    mx = 0;
    for (i = 0; i < chain_size; i = i + 1) {
        if (chain_len[i] > mx) {
            mx = chain_len[i];
        }
    }
    return mx;
}

// Count empty buckets in chain table
int count_empty_buckets(void) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < chain_size; i = i + 1) {
        if (chain_len[i] == 0) {
            count = count + 1;
        }
    }
    return count;
}

// Load factor as percentage (count * 100 / size)
int load_factor_pct(int count, int size) {
    return (count * 100) / size;
}

// Average probe length for successful search (approximation)
int avg_probes_success(void) {
    int total;
    int count;
    int i;
    int h;
    int j;
    int idx;
    total = 0;
    count = 0;
    for (i = 0; i < ht_size; i = i + 1) {
        if (ht_occupied[i] == 1) {
            h = hash1(ht_keys[i], ht_size);
            for (j = 0; j < ht_size; j = j + 1) {
                idx = (h + j) - ((h + j) / ht_size) * ht_size;
                total = total + 1;
                if (idx == i) {
                    break;
                }
            }
            count = count + 1;
        }
    }
    if (count == 0) {
        return 0;
    }
    return (total * 10) / count; // x10 for one decimal place
}

// Double hashing step
int double_hash_step(int key, int size) {
    int k;
    int step;
    k = key;
    if (k < 0) {
        k = -k;
    }
    step = 1 + (k - (k / (size - 1)) * (size - 1));
    return step;
}

// Cluster analysis: find longest cluster in linear probing
int longest_cluster(void) {
    int max_cl;
    int cur_cl;
    int i;
    max_cl = 0;
    cur_cl = 0;
    for (i = 0; i < ht_size; i = i + 1) {
        if (ht_occupied[i] == 1) {
            cur_cl = cur_cl + 1;
            if (cur_cl > max_cl) {
                max_cl = cur_cl;
            }
        } else {
            cur_cl = 0;
        }
    }
    return max_cl;
}

// Count clusters
int count_clusters(void) {
    int clusters;
    int in_cluster;
    int i;
    clusters = 0;
    in_cluster = 0;
    for (i = 0; i < ht_size; i = i + 1) {
        if (ht_occupied[i] == 1) {
            if (in_cluster == 0) {
                clusters = clusters + 1;
                in_cluster = 1;
            }
        } else {
            in_cluster = 0;
        }
    }
    return clusters;
}

// Variance of chain lengths (times 100 to avoid float)
int chain_length_variance_x100(void) {
    int sum;
    int sum_sq;
    int i;
    int mean_x10;
    int var_x100;
    sum = 0;
    sum_sq = 0;
    for (i = 0; i < chain_size; i = i + 1) {
        sum = sum + chain_len[i];
        sum_sq = sum_sq + chain_len[i] * chain_len[i];
    }
    // variance = E[X^2] - (E[X])^2
    // = sum_sq/n - (sum/n)^2
    // times 100: sum_sq*100/n - sum*sum*100/(n*n)
    var_x100 = (sum_sq * 100) / chain_size - (sum * sum * 100) / (chain_size * chain_size);
    return var_x100;
}

// Helper: absolute value
int abs_val(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

// Compare hash distributions
int distribution_score(int size) {
    int counts[16];
    int i;
    int h;
    int score;
    for (i = 0; i < 16; i = i + 1) {
        counts[i] = 0;
    }
    // Hash keys 1..50 into 16 buckets
    for (i = 1; i <= 50; i = i + 1) {
        h = hash1(i, 16);
        counts[h] = counts[h] + 1;
    }
    // Score = sum of absolute deviations from ideal (50/16 ~ 3)
    score = 0;
    for (i = 0; i < 16; i = i + 1) {
        score = score + abs_val(counts[i] - 3);
    }
    return score;
}

// Universal hash family simulation
int universal_hash(int key, int a, int b, int p, int m) {
    int h;
    h = ((a * key + b) - ((a * key + b) / p) * p);
    if (h < 0) {
        h = h + p;
    }
    return h - (h / m) * m;
}

// Robin Hood hashing: probe distance
int probe_distance(int idx, int key) {
    int h;
    int d;
    h = hash1(key, ht_size);
    d = idx - h;
    if (d < 0) {
        d = d + ht_size;
    }
    return d;
}

// Max probe distance in current table
int max_probe_distance(void) {
    int mx;
    int i;
    int d;
    mx = 0;
    for (i = 0; i < ht_size; i = i + 1) {
        if (ht_occupied[i] == 1) {
            d = probe_distance(i, ht_keys[i]);
            if (d > mx) {
                mx = d;
            }
        }
    }
    return mx;
}

// Rehash simulation: count how many would move
int count_moves_on_rehash(int new_size) {
    int moves;
    int i;
    int old_h;
    int new_h;
    moves = 0;
    for (i = 0; i < ht_size; i = i + 1) {
        if (ht_occupied[i] == 1) {
            old_h = hash1(ht_keys[i], ht_size);
            new_h = hash1(ht_keys[i], new_size);
            if (old_h != new_h) {
                moves = moves + 1;
            }
        }
    }
    return moves;
}

int main(void) {
    int i;
    int probes;
    int val;
    int h1;
    int h2;
    int h3v;
    int h4v;

    // Test hash functions on sample keys
    printf("%c%c%c%c %c%c%c%c%c%c%c%c%c", 'H','a','s','h', 'F','u','n','c','t','i','o','n','s');
    printf("%c", 10);

    for (i = 0; i < 5; i = i + 1) {
        h1 = hash1(i * 10 + 7, 16);
        h2 = hash2(i * 10 + 7, 16);
        h3v = hash3(i * 10 + 7, 16);
        h4v = hash4(i * 10 + 7, 16);
        printf("%c%c%c%c%d%c %d %d %d %d", 'K','e','y','=', i * 10 + 7, ':', h1, h2, h3v, h4v);
        printf("%c", 10);
    }

    // Linear probing tests
    printf("%c%c%c%c%c%c %c%c%c%c%c%c%c", 'L','i','n','e','a','r', 'P','r','o','b','i','n','g');
    printf("%c", 10);

    init_linear_ht(32);
    // Insert 20 keys
    for (i = 0; i < 20; i = i + 1) {
        probes = lp_insert(i * 7 + 3, i * 100);
        if (i < 5 || i >= 18) {
            printf("%c%c%c%c%c%c %d %c%c%c%c%c%c%d", 'I','n','s','e','r','t', i * 7 + 3, 'p','r','o','b','e','s', probes);
            printf("%c", 10);
        }
    }

    // Search tests
    val = lp_search(3);
    printf("%c%c%c%c%c%c %d%c%d", 'S','e','a','r','c','h', 3, '=', val);
    printf("%c", 10);

    val = lp_search(38);
    printf("%c%c%c%c%c%c %d%c%d", 'S','e','a','r','c','h', 38, '=', val);
    printf("%c", 10);

    val = lp_search(999);
    printf("%c%c%c%c%c%c %d%c%d", 'S','e','a','r','c','h', 999, '=', val);
    printf("%c", 10);

    // Stats
    printf("%c%c%c%c%c%c%d", 'C','o','u','n','t','=', ht_count);
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%c%d", 'C','o','l','l','i','s','i','o','n','s', count_collisions_lp());
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'L','o','n','g','C','l','u','s','t','e','r', longest_cluster());
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%d", 'C','l','u','s','t','e','r','s', count_clusters());
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%d", 'L','o','a','d','F','a','c','t','=', load_factor_pct(ht_count, ht_size));
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%d", 'A','v','g','P','r','o','b','e','=', avg_probes_success());
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%c%d", 'M','a','x','P','r','o','b','e','D','=', max_probe_distance());
    printf("%c", 10);

    // Delete test
    lp_delete(38);
    val = lp_search(38);
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'A','f','t','e','r','D','e','l',' ', 38, '=', val);
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'C','o','u','n','t','A','f','t','D','e','l', ht_count);
    printf("%c", 10);

    // Chaining tests
    printf("%c%c%c%c%c%c%c%c", 'C','h','a','i','n','i','n','g');
    printf("%c", 10);

    init_chain_ht(16);
    for (i = 0; i < 20; i = i + 1) {
        ch_insert(i * 7 + 3, i * 100);
    }

    val = ch_search(3);
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'C','h','S','e','a','r','c','h', 3, '=', val);
    printf("%c", 10);

    val = ch_search(38);
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'C','h','S','e','a','r','c','h', 38, '=', val);
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%d", 'C','h','a','i','n','C','n','t','=', chain_count);
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%d", 'M','a','x','C','h','a','i','n','=', max_chain_length());
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%c%d", 'E','m','p','t','y','B','k','t','s','=', count_empty_buckets());
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%d", 'C','h','L','o','a','d','P','c','=', load_factor_pct(chain_count, chain_size));
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%d", 'V','a','r','x','1','0','0','=', chain_length_variance_x100());
    printf("%c", 10);

    // Quadratic probing
    printf("%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c", 'Q','u','a','d','r','a','t','i','c', 'P','r','o','b','i','n','g');
    printf("%c", 10);

    init_qp_ht(32);
    for (i = 0; i < 15; i = i + 1) {
        probes = qp_insert(i * 7 + 3, i * 100);
        if (i < 3) {
            printf("%c%c%c%c%c%c%c%c %d %c%c%c%c%c%c%d", 'Q','P','I','n','s','e','r','t', i * 7 + 3, 'p','r','o','b','e','s', probes);
            printf("%c", 10);
        }
    }

    val = qp_search(3);
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'Q','P','S','e','a','r','c','h', 3, '=', val);
    printf("%c", 10);

    val = qp_search(73);
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'Q','P','S','e','a','r','c','h', 73, '=', val);
    printf("%c", 10);

    // Universal hash tests
    printf("%c%c%c%c%c%c%c%c%c%c", 'U','n','i','v','e','r','s','a','l','H');
    printf("%c", 10);
    for (i = 0; i < 5; i = i + 1) {
        h1 = universal_hash(i * 10 + 5, 3, 7, 31, 16);
        printf("%c%c%c%c %d %c%c%d", 'U','H','a','t', i * 10 + 5, '-', '>', h1);
        printf("%c", 10);
    }

    // Distribution score
    printf("%c%c%c%c%c%c%c%c%c%d", 'D','i','s','t','S','c','o','r','e', distribution_score(16));
    printf("%c", 10);

    // Rehash move count
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'R','e','h','a','s','h','M','v','6','4', count_moves_on_rehash(64));
    printf("%c", 10);

    printf("%c%c%c%c %c%c%c%c%c%c%c%d", 'A','l','l',' ', 'p','a','s','s','e','d','=', 1);
    printf("%c", 10);

    return 0;
}
