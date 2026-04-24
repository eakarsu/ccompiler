int printf(const char *fmt, ...);
// EXPECT: Process creation:\n  PID 1 (init): NEW -> RUNNING\n  PID 2 (shell): NEW -> READY\n  PID 3 (editor): NEW -> READY\n  PID 4 (compiler): NEW -> READY\n  PID 5 (daemon): NEW -> READY\n  PID 1 (init): RUNNING -> READY\n  PID 2 (shell): READY -> RUNNING\n  PID 4 (compiler): READY -> RUNNING\n  PID 4 (compiler): RUNNING -> TERMINATED\n\nProcess table:\nPID  Name       State        Prio  CPU    Mem Parent\n1    init       READY           0    2     64      0\n2    shell      RUNNING         1    1    128      1\n3    editor     READY           2    0    256      2\n4    compiler   TERMINATED      3    3    512      2\n5    daemon     READY           1    0     32      1\nTotal memory in use: 480\nActive processes: 4
#define MAX_PROCS 8

enum PState { P_NEW = 0, P_READY = 1, P_RUNNING = 2, P_BLOCKED = 3, P_TERMINATED = 4 };

struct PCB {
    int pid;
    char *name;
    int state;
    int priority;
    int cpu_used;
    int memory;
    int parent_pid;
};

struct ProcTable {
    struct PCB procs[MAX_PROCS];
    int count;
    int next_pid;
};

char *state_str(int s) {
    if (s == P_NEW) return "NEW";
    if (s == P_READY) return "READY";
    if (s == P_RUNNING) return "RUNNING";
    if (s == P_BLOCKED) return "BLOCKED";
    if (s == P_TERMINATED) return "TERMINATED";
    return "UNKNOWN";
}

void pt_init(struct ProcTable *pt) {
    pt->count = 0;
    pt->next_pid = 1;
}

int pt_create(struct ProcTable *pt, char *name, int prio, int mem, int parent) {
    if (pt->count >= MAX_PROCS) return -1;
    int i = pt->count;
    pt->procs[i].pid = pt->next_pid;
    pt->next_pid++;
    pt->procs[i].name = name;
    pt->procs[i].state = P_NEW;
    pt->procs[i].priority = prio;
    pt->procs[i].cpu_used = 0;
    pt->procs[i].memory = mem;
    pt->procs[i].parent_pid = parent;
    pt->count++;
    return pt->procs[i].pid;
}

int pt_find(struct ProcTable *pt, int pid) {
    int i;
    for (i = 0; i < pt->count; i++) {
        if (pt->procs[i].pid == pid) return i;
    }
    return -1;
}

void pt_set_state(struct ProcTable *pt, int pid, int new_state) {
    int i = pt_find(pt, pid);
    if (i >= 0) {
        printf("  PID %d (%s): %s -> %s\n", pid, pt->procs[i].name,
               state_str(pt->procs[i].state), state_str(new_state));
        pt->procs[i].state = new_state;
    }
}

void pt_run_tick(struct ProcTable *pt, int pid) {
    int i = pt_find(pt, pid);
    if (i >= 0 && pt->procs[i].state == P_RUNNING) {
        pt->procs[i].cpu_used++;
    }
}

void pt_print(struct ProcTable *pt) {
    printf("%-4s %-10s %-12s %4s %4s %6s %6s\n",
           "PID", "Name", "State", "Prio", "CPU", "Mem", "Parent");
    int i;
    for (i = 0; i < pt->count; i++) {
        printf("%-4d %-10s %-12s %4d %4d %6d %6d\n",
               pt->procs[i].pid, pt->procs[i].name,
               state_str(pt->procs[i].state),
               pt->procs[i].priority, pt->procs[i].cpu_used,
               pt->procs[i].memory, pt->procs[i].parent_pid);
    }
}

int pt_total_memory(struct ProcTable *pt) {
    int total = 0;
    int i;
    for (i = 0; i < pt->count; i++) {
        if (pt->procs[i].state != P_TERMINATED) {
            total = total + pt->procs[i].memory;
        }
    }
    return total;
}

int main(void) {
    struct ProcTable pt;
    pt_init(&pt);

    int init = pt_create(&pt, "init", 0, 64, 0);
    int shell = pt_create(&pt, "shell", 1, 128, init);
    int editor = pt_create(&pt, "editor", 2, 256, shell);
    int compiler = pt_create(&pt, "compiler", 3, 512, shell);
    int daemon = pt_create(&pt, "daemon", 1, 32, init);

    printf("Process creation:\n");
    pt_set_state(&pt, init, P_RUNNING);
    pt_set_state(&pt, shell, P_READY);
    pt_set_state(&pt, editor, P_READY);
    pt_set_state(&pt, compiler, P_READY);
    pt_set_state(&pt, daemon, P_READY);

    pt_run_tick(&pt, init);
    pt_run_tick(&pt, init);
    pt_set_state(&pt, init, P_READY);
    pt_set_state(&pt, shell, P_RUNNING);
    pt_run_tick(&pt, shell);
    pt_set_state(&pt, compiler, P_RUNNING);
    pt_run_tick(&pt, compiler);
    pt_run_tick(&pt, compiler);
    pt_run_tick(&pt, compiler);
    pt_set_state(&pt, compiler, P_TERMINATED);

    printf("\nProcess table:\n");
    pt_print(&pt);
    printf("Total memory in use: %d\n", pt_total_memory(&pt));
    printf("Active processes: ");
    int active = 0;
    int i;
    for (i = 0; i < pt.count; i++) {
        if (pt.procs[i].state != P_TERMINATED) active++;
    }
    printf("%d\n", active);

    return 0;
}
