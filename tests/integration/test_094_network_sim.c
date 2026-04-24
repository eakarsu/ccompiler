int printf(const char *fmt, ...);
// EXPECT: nodes:0|added=6|links=6|c0_3= 4|c0_5= 4|c4_3= 4|c0_0= 0|c3_5= 3|c4_2= 3|h0_3= 1|h0_5= 4|h4_3= 5|reach0=6|reach3=6|dia=4|avgnbr=2|pkt1:dlvr=1,hops=3,pth=0-1-2-3|pkt2:dlvr=1,hops=3,pth=4-5-2-3|pkt3:dlvr=1,hops=2,pth=5-4-0|pkt4:dlvr=0|pkt5:dlvr=1,hops=0|pkt6:dlvr=1,hops=3,pth=3-2-5-4|stat:thops= 12,dlv=5,drp=1|pktcnt=6|after_add6:nodes=7,links=8|c0_6=   6|c4_6= 4|new_dia=6|pkt7:hops= 4,pth=0-1-2-3-6|reach6=7|end

/* Simple network routing simulation */

/* Node structure: up to 8 nodes, each with up to 4 neighbors */
struct Node {
    int id;
    int active;
    int neighbors[4];
    int neighbor_count;
    int costs[4]; /* cost to reach each neighbor */
};

/* Routing table entry */
struct RouteEntry {
    int dest;
    int next_hop;
    int total_cost;
};

/* Packet structure */
struct Packet {
    int src;
    int dest;
    int ttl;
    int hops;
    int delivered;
    int path[16];
    int path_len;
};

struct Node nodes[8];
int node_count;

struct RouteEntry route_table[8][8]; /* route_table[src][dest] */

struct Packet packets[16];
int packet_count;

/* Statistics */
int total_hops;
int total_delivered;
int total_dropped;

void init_network(void) {
    int i;
    int j;
    node_count = 0;
    packet_count = 0;
    total_hops = 0;
    total_delivered = 0;
    total_dropped = 0;
    i = 0;
    while (i < 8) {
        nodes[i].id = i;
        nodes[i].active = 0;
        nodes[i].neighbor_count = 0;
        j = 0;
        while (j < 4) {
            nodes[i].neighbors[j] = -1;
            nodes[i].costs[j] = 0;
            j = j + 1;
        }
        j = 0;
        while (j < 8) {
            route_table[i][j].dest = j;
            route_table[i][j].next_hop = -1;
            route_table[i][j].total_cost = 999;
            j = j + 1;
        }
        i = i + 1;
    }
}

int add_node(int id) {
    if (id < 0 || id >= 8) return 0;
    nodes[id].id = id;
    nodes[id].active = 1;
    node_count = node_count + 1;
    /* Route to self */
    route_table[id][id].next_hop = id;
    route_table[id][id].total_cost = 0;
    return 1;
}

int add_link(int a, int b, int cost) {
    int idx;
    if (!nodes[a].active || !nodes[b].active) return 0;
    /* Add b as neighbor of a */
    idx = nodes[a].neighbor_count;
    if (idx >= 4) return 0;
    nodes[a].neighbors[idx] = b;
    nodes[a].costs[idx] = cost;
    nodes[a].neighbor_count = nodes[a].neighbor_count + 1;
    /* Add a as neighbor of b */
    idx = nodes[b].neighbor_count;
    if (idx >= 4) return 0;
    nodes[b].neighbors[idx] = a;
    nodes[b].costs[idx] = cost;
    nodes[b].neighbor_count = nodes[b].neighbor_count + 1;
    /* Direct route entries */
    route_table[a][b].next_hop = b;
    route_table[a][b].total_cost = cost;
    route_table[b][a].next_hop = a;
    route_table[b][a].total_cost = cost;
    return 1;
}

/* Build routing table using simple distance vector (Bellman-Ford style) */
void build_routes(void) {
    int changed;
    int iter;
    int u;
    int v;
    int d;
    int ni;
    int n;
    int new_cost;
    iter = 0;
    changed = 1;
    while (changed && iter < 10) {
        changed = 0;
        u = 0;
        while (u < 8) {
            if (!nodes[u].active) { u = u + 1; continue; }
            ni = 0;
            while (ni < nodes[u].neighbor_count) {
                n = nodes[u].neighbors[ni];
                if (!nodes[n].active) { ni = ni + 1; continue; }
                d = 0;
                while (d < 8) {
                    if (!nodes[d].active) { d = d + 1; continue; }
                    new_cost = nodes[u].costs[ni] + route_table[n][d].total_cost;
                    if (new_cost < route_table[u][d].total_cost) {
                        route_table[u][d].total_cost = new_cost;
                        route_table[u][d].next_hop = n;
                        changed = 1;
                    }
                    d = d + 1;
                }
                ni = ni + 1;
            }
            u = u + 1;
        }
        iter = iter + 1;
    }
}

int get_route_cost(int src, int dest) {
    return route_table[src][dest].total_cost;
}

int get_next_hop(int src, int dest) {
    return route_table[src][dest].next_hop;
}

int create_packet(int src, int dest, int ttl) {
    int idx;
    idx = packet_count;
    if (idx >= 16) return -1;
    packets[idx].src = src;
    packets[idx].dest = dest;
    packets[idx].ttl = ttl;
    packets[idx].hops = 0;
    packets[idx].delivered = 0;
    packets[idx].path[0] = src;
    packets[idx].path_len = 1;
    packet_count = packet_count + 1;
    return idx;
}

/* Forward a single packet one hop */
int forward_packet(int pkt_idx) {
    int current;
    int dest;
    int next;
    if (packets[pkt_idx].delivered) return 0;
    if (packets[pkt_idx].ttl <= 0) {
        total_dropped = total_dropped + 1;
        return -1;
    }
    current = packets[pkt_idx].path[packets[pkt_idx].path_len - 1];
    dest = packets[pkt_idx].dest;
    if (current == dest) {
        packets[pkt_idx].delivered = 1;
        total_delivered = total_delivered + 1;
        return 1;
    }
    next = get_next_hop(current, dest);
    if (next < 0) {
        total_dropped = total_dropped + 1;
        return -1;
    }
    packets[pkt_idx].path[packets[pkt_idx].path_len] = next;
    packets[pkt_idx].path_len = packets[pkt_idx].path_len + 1;
    packets[pkt_idx].hops = packets[pkt_idx].hops + 1;
    packets[pkt_idx].ttl = packets[pkt_idx].ttl - 1;
    total_hops = total_hops + 1;
    if (next == dest) {
        packets[pkt_idx].delivered = 1;
        total_delivered = total_delivered + 1;
        return 1;
    }
    return 0;
}

/* Route packet to completion */
int route_packet(int pkt_idx) {
    int result;
    int safety;
    safety = 0;
    while (safety < 20) {
        result = forward_packet(pkt_idx);
        if (result == 1) return 1;  /* delivered */
        if (result == -1) return 0; /* dropped */
        safety = safety + 1;
    }
    total_dropped = total_dropped + 1;
    return 0;
}

void print_path(int pkt_idx) {
    int i;
    i = 0;
    while (i < packets[pkt_idx].path_len) {
        if (i > 0) printf("%c", '-');
        printf("%d", packets[pkt_idx].path[i]);
        i = i + 1;
    }
}

/* Count reachable nodes from src */
int count_reachable(int src) {
    int cnt;
    int d;
    cnt = 0;
    d = 0;
    while (d < 8) {
        if (nodes[d].active && route_table[src][d].total_cost < 999) {
            cnt = cnt + 1;
        }
        d = d + 1;
    }
    return cnt;
}

/* Find diameter (max shortest path among all pairs) */
int network_diameter(void) {
    int maxc;
    int u;
    int v;
    maxc = 0;
    u = 0;
    while (u < 8) {
        if (!nodes[u].active) { u = u + 1; continue; }
        v = 0;
        while (v < 8) {
            if (!nodes[v].active) { v = v + 1; continue; }
            if (route_table[u][v].total_cost < 999) {
                if (route_table[u][v].total_cost > maxc) {
                    maxc = route_table[u][v].total_cost;
                }
            }
            v = v + 1;
        }
        u = u + 1;
    }
    return maxc;
}

/* Count total links (edges) */
int count_links(void) {
    int cnt;
    int i;
    cnt = 0;
    i = 0;
    while (i < 8) {
        if (nodes[i].active) {
            cnt = cnt + nodes[i].neighbor_count;
        }
        i = i + 1;
    }
    /* Each link counted twice */
    return cnt / 2;
}

/* Average neighbor count */
int avg_neighbors(void) {
    int total;
    int cnt;
    int i;
    total = 0;
    cnt = 0;
    i = 0;
    while (i < 8) {
        if (nodes[i].active) {
            total = total + nodes[i].neighbor_count;
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    if (cnt == 0) return 0;
    return total / cnt;
}

int main(void) {
    int pkt;
    int result;

    init_network();
    printf("%c%c%c%c%c", 'n', 'o', 'd', 'e', 's');
    printf("%c", ':');
    printf("%d", node_count);

    /* Create network: 0 -- 1 -- 2 -- 3
     *                 |         |
     *                 4 ------- 5
     *                 costs: 0-1:1, 1-2:2, 2-3:1, 0-4:3, 2-5:2, 4-5:1 */
    add_node(0);
    add_node(1);
    add_node(2);
    add_node(3);
    add_node(4);
    add_node(5);

    printf("%c%c%c%c%c%c%c", '|', 'a', 'd', 'd', 'e', 'd', '=');
    printf("%d", node_count);

    add_link(0, 1, 1);
    add_link(1, 2, 2);
    add_link(2, 3, 1);
    add_link(0, 4, 3);
    add_link(2, 5, 2);
    add_link(4, 5, 1);

    printf("%c%c%c%c%c%c%c", '|', 'l', 'i', 'n', 'k', 's', '=');
    printf("%d", count_links());

    /* Build routing table */
    build_routes();

    /* Test route costs */
    printf("%c%c%c%c%c%c%c", '|', 'c', '0', '_', '3', '=', ' ');
    printf("%d", get_route_cost(0, 3));

    printf("%c%c%c%c%c%c%c", '|', 'c', '0', '_', '5', '=', ' ');
    printf("%d", get_route_cost(0, 5));

    printf("%c%c%c%c%c%c%c", '|', 'c', '4', '_', '3', '=', ' ');
    printf("%d", get_route_cost(4, 3));

    printf("%c%c%c%c%c%c%c", '|', 'c', '0', '_', '0', '=', ' ');
    printf("%d", get_route_cost(0, 0));

    printf("%c%c%c%c%c%c%c", '|', 'c', '3', '_', '5', '=', ' ');
    printf("%d", get_route_cost(3, 5));

    printf("%c%c%c%c%c%c%c", '|', 'c', '4', '_', '2', '=', ' ');
    printf("%d", get_route_cost(4, 2));

    /* Next hops */
    printf("%c%c%c%c%c%c%c", '|', 'h', '0', '_', '3', '=', ' ');
    printf("%d", get_next_hop(0, 3));

    printf("%c%c%c%c%c%c%c", '|', 'h', '0', '_', '5', '=', ' ');
    printf("%d", get_next_hop(0, 5));

    printf("%c%c%c%c%c%c%c", '|', 'h', '4', '_', '3', '=', ' ');
    printf("%d", get_next_hop(4, 3));

    /* Reachability */
    printf("%c%c%c%c%c%c%c", '|', 'r', 'e', 'a', 'c', 'h', '0');
    printf("%c", '=');
    printf("%d", count_reachable(0));

    printf("%c%c%c%c%c%c%c", '|', 'r', 'e', 'a', 'c', 'h', '3');
    printf("%c", '=');
    printf("%d", count_reachable(3));

    /* Diameter */
    printf("%c%c%c%c", '|', 'd', 'i', 'a');
    printf("%c", '=');
    printf("%d", network_diameter());

    /* Average neighbors */
    printf("%c%c%c%c%c%c%c%c", '|', 'a', 'v', 'g', 'n', 'b', 'r', '=');
    printf("%d", avg_neighbors());

    /* Packet 1: 0 -> 3 with TTL=10 */
    pkt = create_packet(0, 3, 10);
    result = route_packet(pkt);
    printf("%c%c%c%c%c%c", '|', 'p', 'k', 't', '1', ':');
    printf("%c%c%c%c%c", 'd', 'l', 'v', 'r', '=');
    printf("%d", result);
    printf("%c%c%c%c%c%c", ',', 'h', 'o', 'p', 's', '=');
    printf("%d", packets[pkt].hops);
    printf("%c%c%c%c%c", ',', 'p', 't', 'h', '=');
    print_path(pkt);

    /* Packet 2: 4 -> 3 */
    pkt = create_packet(4, 3, 10);
    result = route_packet(pkt);
    printf("%c%c%c%c%c%c", '|', 'p', 'k', 't', '2', ':');
    printf("%c%c%c%c%c", 'd', 'l', 'v', 'r', '=');
    printf("%d", result);
    printf("%c%c%c%c%c%c", ',', 'h', 'o', 'p', 's', '=');
    printf("%d", packets[pkt].hops);
    printf("%c%c%c%c%c", ',', 'p', 't', 'h', '=');
    print_path(pkt);

    /* Packet 3: 5 -> 0 */
    pkt = create_packet(5, 0, 10);
    result = route_packet(pkt);
    printf("%c%c%c%c%c%c", '|', 'p', 'k', 't', '3', ':');
    printf("%c%c%c%c%c", 'd', 'l', 'v', 'r', '=');
    printf("%d", result);
    printf("%c%c%c%c%c%c", ',', 'h', 'o', 'p', 's', '=');
    printf("%d", packets[pkt].hops);
    printf("%c%c%c%c%c", ',', 'p', 't', 'h', '=');
    print_path(pkt);

    /* Packet 4: 0 -> 5 with TTL=1 (should drop because needs more hops) */
    pkt = create_packet(0, 5, 1);
    result = route_packet(pkt);
    printf("%c%c%c%c%c%c", '|', 'p', 'k', 't', '4', ':');
    printf("%c%c%c%c%c", 'd', 'l', 'v', 'r', '=');
    printf("%d", result);

    /* Packet 5: 0 -> 0 (same node) */
    pkt = create_packet(0, 0, 10);
    result = route_packet(pkt);
    printf("%c%c%c%c%c%c", '|', 'p', 'k', 't', '5', ':');
    printf("%c%c%c%c%c", 'd', 'l', 'v', 'r', '=');
    printf("%d", result);
    printf("%c%c%c%c%c%c", ',', 'h', 'o', 'p', 's', '=');
    printf("%d", packets[pkt].hops);

    /* Packet 6: 3 -> 4 */
    pkt = create_packet(3, 4, 10);
    result = route_packet(pkt);
    printf("%c%c%c%c%c%c", '|', 'p', 'k', 't', '6', ':');
    printf("%c%c%c%c%c", 'd', 'l', 'v', 'r', '=');
    printf("%d", result);
    printf("%c%c%c%c%c%c", ',', 'h', 'o', 'p', 's', '=');
    printf("%d", packets[pkt].hops);
    printf("%c%c%c%c%c", ',', 'p', 't', 'h', '=');
    print_path(pkt);

    /* Statistics */
    printf("%c%c%c%c%c%c", '|', 's', 't', 'a', 't', ':');
    printf("%c%c%c%c%c%c%c", 't', 'h', 'o', 'p', 's', '=', ' ');
    printf("%d", total_hops);
    printf("%c%c%c%c%c", ',', 'd', 'l', 'v', '=');
    printf("%d", total_delivered);
    printf("%c%c%c%c%c", ',', 'd', 'r', 'p', '=');
    printf("%d", total_dropped);

    /* Packet count */
    printf("%c%c%c%c%c%c%c", '|', 'p', 'k', 't', 'c', 'n', 't');
    printf("%c", '=');
    printf("%d", packet_count);

    /* Add node 6 connected to 3 and 5 */
    add_node(6);
    add_link(3, 6, 2);
    add_link(5, 6, 3);
    build_routes();

    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'a', 'f', 't', 'e', 'r', '_', 'a', 'd', 'd', '6');
    printf("%c%c%c%c%c%c%c", ':', 'n', 'o', 'd', 'e', 's', '=');
    printf("%d", node_count);
    printf("%c%c%c%c%c%c%c", ',', 'l', 'i', 'n', 'k', 's', '=');
    printf("%d", count_links());

    /* Route 0 to 6 */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'c', '0', '_', '6', '=', ' ', ' ', ' ');
    /* just print cost */
    printf("%d", get_route_cost(0, 6));

    /* Route 4 to 6 */
    printf("%c%c%c%c%c%c%c", '|', 'c', '4', '_', '6', '=', ' ');
    printf("%d", get_route_cost(4, 6));

    /* New diameter */
    printf("%c%c%c%c%c%c%c%c", '|', 'n', 'e', 'w', '_', 'd', 'i', 'a');
    printf("%c", '=');
    printf("%d", network_diameter());

    /* Packet 0->6 */
    pkt = create_packet(0, 6, 10);
    result = route_packet(pkt);
    printf("%c%c%c%c%c%c", '|', 'p', 'k', 't', '7', ':');
    printf("%c%c%c%c%c%c", 'h', 'o', 'p', 's', '=', ' ');
    printf("%d", packets[pkt].hops);
    printf("%c%c%c%c%c", ',', 'p', 't', 'h', '=');
    print_path(pkt);

    /* Reachability from 6 */
    printf("%c%c%c%c%c%c%c", '|', 'r', 'e', 'a', 'c', 'h', '6');
    printf("%c", '=');
    printf("%d", count_reachable(6));

    printf("%c%c%c%c", '|', 'e', 'n', 'd');

    printf("%c", '\n');

    return 0;
}
