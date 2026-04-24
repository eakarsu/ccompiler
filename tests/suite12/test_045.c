int printf(const char *fmt, ...);

// Beam Search: finding best sequence through a layered graph
// Each layer has N nodes, beam keeps top-K candidates

int scores[5][8];    // 5 layers, 8 nodes each
int trans[8][8];     // transition costs between nodes

// Beam candidates: (node, total_score)
int beam_nodes[8];
int beam_scores[8];
int beam_paths[8][5];  // store path for each beam candidate
int beam_size;

int new_nodes[64];
int new_scores[64];
int new_parents[64];
int new_count;

void init_beam(int start_node) {
    beam_nodes[0] = start_node;
    beam_scores[0] = 0;
    beam_paths[0][0] = start_node;
    beam_size = 1;
}

void expand_beam(int layer) {
    new_count = 0;
    int b = 0;
    while (b < beam_size) {
        int cur_node = beam_nodes[b];
        int cur_score = beam_scores[b];
        int n = 0;
        while (n < 8) {
            int total = cur_score + scores[layer][n] + trans[cur_node][n];
            new_nodes[new_count] = n;
            new_scores[new_count] = total;
            new_parents[new_count] = b;
            new_count = new_count + 1;
            n = n + 1;
        }
        b = b + 1;
    }
}

// Select top-K from new candidates (K = beam width)
void select_top_k(int k, int layer) {
    int selected[8];
    int sel_count = 0;
    int used[64];
    int i = 0;
    while (i < 64) {
        used[i] = 0;
        i = i + 1;
    }

    while (sel_count < k && sel_count < new_count) {
        int best = -1;
        int best_score = -99999;
        i = 0;
        while (i < new_count) {
            if (used[i] == 0 && new_scores[i] > best_score) {
                best_score = new_scores[i];
                best = i;
            }
            i = i + 1;
        }
        if (best < 0) break;
        used[best] = 1;
        selected[sel_count] = best;
        sel_count = sel_count + 1;
    }

    // Copy selected to beam, preserving paths
    int old_paths[8][5];
    i = 0;
    while (i < beam_size) {
        int j = 0;
        while (j < 5) {
            old_paths[i][j] = beam_paths[i][j];
            j = j + 1;
        }
        i = i + 1;
    }

    i = 0;
    while (i < sel_count) {
        int idx = selected[i];
        int parent = new_parents[idx];
        beam_nodes[i] = new_nodes[idx];
        beam_scores[i] = new_scores[idx];
        // Copy parent path
        int j = 0;
        while (j < layer) {
            beam_paths[i][j] = old_paths[parent][j];
            j = j + 1;
        }
        beam_paths[i][layer] = new_nodes[idx];
        i = i + 1;
    }
    beam_size = sel_count;
}

int main() {
    // Initialize node scores
    int layer = 0;
    while (layer < 5) {
        int n = 0;
        while (n < 8) {
            scores[layer][n] = (layer + 1) * (n + 1) % 7 - 3;
            n = n + 1;
        }
        layer = layer + 1;
    }

    // Initialize transition costs
    int i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            int diff = i - j;
            if (diff < 0) diff = -diff;
            trans[i][j] = -diff;  // penalty for jumping far
            j = j + 1;
        }
        i = i + 1;
    }

    printf("Layer 0 scores: %d %d %d %d %d %d %d %d\n",
           scores[0][0], scores[0][1], scores[0][2], scores[0][3],
           scores[0][4], scores[0][5], scores[0][6], scores[0][7]);
    // EXPECT: Layer 0 scores: -2 -1 0 1 2 3 -3 -2

    // Beam search with width 3, starting from node 0
    init_beam(0);
    layer = 0;
    while (layer < 5) {
        expand_beam(layer);
        select_top_k(3, layer);
        layer = layer + 1;
    }

    printf("Beam size: %d\n", beam_size);
    // EXPECT: Beam size: 3

    printf("Best score: %d\n", beam_scores[0]);
    // EXPECT: Best score: 6

    printf("Best path: %d %d %d %d %d\n",
           beam_paths[0][0], beam_paths[0][1], beam_paths[0][2],
           beam_paths[0][3], beam_paths[0][4]);
    // EXPECT: Best path: 2 2 1 2 3

    // Try beam width 1 (greedy)
    init_beam(0);
    layer = 0;
    while (layer < 5) {
        expand_beam(layer);
        select_top_k(1, layer);
        layer = layer + 1;
    }
    printf("Greedy score: %d\n", beam_scores[0]);
    // EXPECT: Greedy score: 4

    printf("Greedy path: %d %d %d %d %d\n",
           beam_paths[0][0], beam_paths[0][1], beam_paths[0][2],
           beam_paths[0][3], beam_paths[0][4]);
    // EXPECT: Greedy path: 0 2 1 2 3

    // Try starting from node 5
    init_beam(5);
    layer = 0;
    while (layer < 5) {
        expand_beam(layer);
        select_top_k(3, layer);
        layer = layer + 1;
    }
    printf("Start-5 best score: %d\n", beam_scores[0]);
    // EXPECT: Start-5 best score: 10

    printf("Beam search done\n");
    // EXPECT: Beam search done

    return 0;
}
