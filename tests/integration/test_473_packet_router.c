int printf(const char *fmt, ...);
// EXPECT: Routing tables:\nRouter 0: [0->0] [1->1] [2->2] [3->1] [4->1] [5->1] \nRouter 1: [0->0] [1->1] [2->0] [3->3] [4->3] [5->3] \nRouter 2: [0->0] [1->0] [2->2] [3->3] [4->3] [5->3] \nRouter 3: [0->1] [1->1] [2->2] [3->3] [4->4] [5->4] \nRouter 4: [0->3] [1->3] [2->3] [3->3] [4->4] [5->5] \nRouter 5: [0->4] [1->4] [2->4] [3->4] [4->4] [5->5] \n\nRouting packets:\nPacket 0->5 (data=100): 0->1->3->4->5 (hops=4)\nPacket 5->0 (data=200): 5->4->3->1->0 (hops=4)\nPacket 1->4 (data=300): 1->3->4 (hops=2)\nDelivered: 3, Dropped: 0
// Network packet router simulation

struct Packet {
    int src;
    int dst;
    int ttl;
    int data;
    int hops;
};

struct Router {
    int id;
    int routing_table[8];
    int neighbors[8];
    int num_neighbors;
    int packets_forwarded;
};

struct Network {
    struct Router routers[8];
    int num_routers;
    struct Packet packets[16];
    int num_packets;
    int delivered;
    int dropped;
};

void net_init(struct Network *net, int n) {
    int i;
    int j;
    net->num_routers = n;
    net->num_packets = 0;
    net->delivered = 0;
    net->dropped = 0;
    for (i = 0; i < n; i++) {
        net->routers[i].id = i;
        net->routers[i].num_neighbors = 0;
        net->routers[i].packets_forwarded = 0;
        for (j = 0; j < 8; j++) {
            net->routers[i].routing_table[j] = -1;
            net->routers[i].neighbors[j] = -1;
        }
    }
}

void net_add_link(struct Network *net, int a, int b) {
    int na = net->routers[a].num_neighbors;
    int nb = net->routers[b].num_neighbors;
    net->routers[a].neighbors[na] = b;
    net->routers[a].num_neighbors++;
    net->routers[b].neighbors[nb] = a;
    net->routers[b].num_neighbors++;
}

void net_compute_routes(struct Network *net) {
    int i;
    int j;
    int k;
    int visited[8];
    int queue[8];
    int prev[8];
    int front;
    int back;
    int cur;
    int n = net->num_routers;

    for (i = 0; i < n; i++) {
        /* BFS from router i to find next-hop for each destination */
        for (j = 0; j < 8; j++) { visited[j] = 0; prev[j] = -1; }
        front = 0; back = 0;
        queue[back] = i; back++;
        visited[i] = 1;
        while (front < back) {
            cur = queue[front]; front++;
            for (k = 0; k < net->routers[cur].num_neighbors; k++) {
                int nb = net->routers[cur].neighbors[k];
                if (!visited[nb]) {
                    visited[nb] = 1;
                    prev[nb] = cur;
                    queue[back] = nb; back++;
                }
            }
        }
        for (j = 0; j < n; j++) {
            if (j == i) { net->routers[i].routing_table[j] = i; continue; }
            /* trace back from j to i to find first hop */
            cur = j;
            while (prev[cur] != i && prev[cur] != -1) cur = prev[cur];
            net->routers[i].routing_table[j] = cur;
        }
    }
}

void net_send_packet(struct Network *net, int src, int dst, int data) {
    int idx = net->num_packets;
    net->packets[idx].src = src;
    net->packets[idx].dst = dst;
    net->packets[idx].ttl = 10;
    net->packets[idx].data = data;
    net->packets[idx].hops = 0;
    net->num_packets++;
}

void net_route_all(struct Network *net) {
    int i;
    int cur;
    int next;
    for (i = 0; i < net->num_packets; i++) {
        cur = net->packets[i].src;
        printf("Packet %d->%d (data=%d): ", net->packets[i].src, net->packets[i].dst, net->packets[i].data);
        while (cur != net->packets[i].dst && net->packets[i].ttl > 0) {
            next = net->routers[cur].routing_table[net->packets[i].dst];
            net->routers[cur].packets_forwarded++;
            net->packets[i].hops++;
            net->packets[i].ttl--;
            printf("%d->", cur);
            cur = next;
        }
        if (cur == net->packets[i].dst) {
            printf("%d (hops=%d)\n", cur, net->packets[i].hops);
            net->delivered++;
        } else {
            printf("DROPPED\n");
            net->dropped++;
        }
    }
}

int main(void) {
    struct Network net;
    int i;
    net_init(&net, 6);

    net_add_link(&net, 0, 1);
    net_add_link(&net, 0, 2);
    net_add_link(&net, 1, 3);
    net_add_link(&net, 2, 3);
    net_add_link(&net, 3, 4);
    net_add_link(&net, 4, 5);

    net_compute_routes(&net);

    printf("Routing tables:\n");
    for (i = 0; i < 6; i++) {
        printf("Router %d: ", i);
        int j;
        for (j = 0; j < 6; j++) {
            printf("[%d->%d] ", j, net.routers[i].routing_table[j]);
        }
        printf("\n");
    }

    net_send_packet(&net, 0, 5, 100);
    net_send_packet(&net, 5, 0, 200);
    net_send_packet(&net, 1, 4, 300);

    printf("\nRouting packets:\n");
    net_route_all(&net);
    printf("Delivered: %d, Dropped: %d\n", net.delivered, net.dropped);

    return 0;
}
