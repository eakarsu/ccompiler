int printf(const char *fmt, ...);

// Interrupt controller (priority-based) simulation

enum irq_state {
    IRQ_INACTIVE,
    IRQ_PENDING,
    IRQ_ACTIVE
};

enum irq_type {
    IRQ_LEVEL,
    IRQ_EDGE
};

struct irq_source {
    int id;
    int priority;
    int state;
    int type;
    int enabled;
    int trigger_count;
    int service_count;
};

struct irq_controller {
    struct irq_source sources[8];
    int num_sources;
    int active_irq;
    int active_priority;
    int nested_depth;
    int total_interrupts;
    int mask_register;
    int preemption_enabled;
};

void irq_source_init(struct irq_source *src, int id, int priority, int type) {
    src->id = id;
    src->priority = priority;
    src->state = IRQ_INACTIVE;
    src->type = type;
    src->enabled = 1;
    src->trigger_count = 0;
    src->service_count = 0;
}

void irqc_init(struct irq_controller *ctrl, int num_sources) {
    int i;
    ctrl->num_sources = num_sources;
    ctrl->active_irq = -1;
    ctrl->active_priority = 999;
    ctrl->nested_depth = 0;
    ctrl->total_interrupts = 0;
    ctrl->mask_register = 0xFF;
    ctrl->preemption_enabled = 0;
    for (i = 0; i < 8; i++) {
        irq_source_init(&ctrl->sources[i], i, 7, IRQ_LEVEL);
    }
}

void irqc_set_priority(struct irq_controller *ctrl, int irq_id, int priority) {
    if (irq_id >= 0 && irq_id < ctrl->num_sources) {
        ctrl->sources[irq_id].priority = priority;
    }
}

void irqc_enable_irq(struct irq_controller *ctrl, int irq_id) {
    if (irq_id >= 0 && irq_id < ctrl->num_sources) {
        ctrl->sources[irq_id].enabled = 1;
        ctrl->mask_register = ctrl->mask_register | (1 << irq_id);
    }
}

void irqc_disable_irq(struct irq_controller *ctrl, int irq_id) {
    if (irq_id >= 0 && irq_id < ctrl->num_sources) {
        ctrl->sources[irq_id].enabled = 0;
        ctrl->mask_register = ctrl->mask_register & ~(1 << irq_id);
    }
}

int irqc_trigger(struct irq_controller *ctrl, int irq_id) {
    if (irq_id < 0 || irq_id >= ctrl->num_sources) return -1;
    if (!ctrl->sources[irq_id].enabled) return -2;

    ctrl->sources[irq_id].state = IRQ_PENDING;
    ctrl->sources[irq_id].trigger_count++;
    return 0;
}

int irqc_find_highest_pending(struct irq_controller *ctrl) {
    int i;
    int best_id = -1;
    int best_priority = 999;

    for (i = 0; i < ctrl->num_sources; i++) {
        if (ctrl->sources[i].state == IRQ_PENDING && ctrl->sources[i].enabled) {
            if (ctrl->sources[i].priority < best_priority) {
                best_priority = ctrl->sources[i].priority;
                best_id = i;
            }
        }
    }
    return best_id;
}

int irqc_acknowledge(struct irq_controller *ctrl) {
    int irq_id = irqc_find_highest_pending(ctrl);
    if (irq_id < 0) return -1;

    if (ctrl->preemption_enabled) {
        if (ctrl->sources[irq_id].priority >= ctrl->active_priority) {
            return -1;
        }
    } else {
        if (ctrl->active_irq >= 0) {
            return -1;
        }
    }

    ctrl->sources[irq_id].state = IRQ_ACTIVE;
    ctrl->active_irq = irq_id;
    ctrl->active_priority = ctrl->sources[irq_id].priority;
    ctrl->nested_depth++;
    ctrl->total_interrupts++;
    return irq_id;
}

int irqc_complete(struct irq_controller *ctrl, int irq_id) {
    if (irq_id < 0 || irq_id >= ctrl->num_sources) return -1;
    if (ctrl->sources[irq_id].state != IRQ_ACTIVE) return -1;

    ctrl->sources[irq_id].state = IRQ_INACTIVE;
    ctrl->sources[irq_id].service_count++;
    ctrl->nested_depth--;

    if (ctrl->nested_depth <= 0) {
        ctrl->active_irq = -1;
        ctrl->active_priority = 999;
        ctrl->nested_depth = 0;
    }

    return 0;
}

int irqc_get_pending_count(struct irq_controller *ctrl) {
    int i;
    int count = 0;
    for (i = 0; i < ctrl->num_sources; i++) {
        if (ctrl->sources[i].state == IRQ_PENDING) count++;
    }
    return count;
}

int irqc_process_all(struct irq_controller *ctrl) {
    int processed = 0;
    int irq_id;

    while (1) {
        irq_id = irqc_acknowledge(ctrl);
        if (irq_id < 0) break;
        irqc_complete(ctrl, irq_id);
        processed++;
    }
    return processed;
}

int irqc_simulate_latency(int priority, int base_latency) {
    return base_latency + priority * 2;
}

int irqc_count_active_sources(struct irq_controller *ctrl) {
    int i;
    int count = 0;
    for (i = 0; i < ctrl->num_sources; i++) {
        if (ctrl->sources[i].enabled) count++;
    }
    return count;
}

int main(void) {
    struct irq_controller ctrl;
    int result;

    irqc_init(&ctrl, 8);
    printf("%d\n", ctrl.num_sources);
    // EXPECT: 8
    printf("%d\n", ctrl.active_irq);
    // EXPECT: -1
    printf("%d\n", ctrl.total_interrupts);
    // EXPECT: 0

    irqc_set_priority(&ctrl, 0, 0);
    irqc_set_priority(&ctrl, 1, 1);
    irqc_set_priority(&ctrl, 2, 2);
    irqc_set_priority(&ctrl, 3, 3);

    printf("%d\n", ctrl.sources[0].priority);
    // EXPECT: 0
    printf("%d\n", ctrl.sources[3].priority);
    // EXPECT: 3

    result = irqc_trigger(&ctrl, 2);
    printf("%d\n", result);
    // EXPECT: 0
    printf("%d\n", ctrl.sources[2].state);
    // EXPECT: 1

    int highest = irqc_find_highest_pending(&ctrl);
    printf("%d\n", highest);
    // EXPECT: 2

    irqc_trigger(&ctrl, 0);
    highest = irqc_find_highest_pending(&ctrl);
    printf("%d\n", highest);
    // EXPECT: 0

    int acked = irqc_acknowledge(&ctrl);
    printf("%d\n", acked);
    // EXPECT: 0
    printf("%d\n", ctrl.active_irq);
    // EXPECT: 0
    printf("%d\n", ctrl.nested_depth);
    // EXPECT: 1

    int next_ack = irqc_acknowledge(&ctrl);
    printf("%d\n", next_ack);
    // EXPECT: -1

    result = irqc_complete(&ctrl, 0);
    printf("%d\n", result);
    // EXPECT: 0
    printf("%d\n", ctrl.active_irq);
    // EXPECT: -1
    printf("%d\n", ctrl.nested_depth);
    // EXPECT: 0

    acked = irqc_acknowledge(&ctrl);
    printf("%d\n", acked);
    // EXPECT: 2
    irqc_complete(&ctrl, 2);

    printf("%d\n", ctrl.total_interrupts);
    // EXPECT: 2

    printf("%d\n", ctrl.sources[0].service_count);
    // EXPECT: 1
    printf("%d\n", ctrl.sources[2].service_count);
    // EXPECT: 1

    irqc_disable_irq(&ctrl, 5);
    result = irqc_trigger(&ctrl, 5);
    printf("%d\n", result);
    // EXPECT: -2

    int active_count = irqc_count_active_sources(&ctrl);
    printf("%d\n", active_count);
    // EXPECT: 7

    irqc_trigger(&ctrl, 1);
    irqc_trigger(&ctrl, 3);
    irqc_trigger(&ctrl, 4);
    printf("%d\n", irqc_get_pending_count(&ctrl));
    // EXPECT: 3

    int processed = irqc_process_all(&ctrl);
    printf("%d\n", processed);
    // EXPECT: 3
    printf("%d\n", irqc_get_pending_count(&ctrl));
    // EXPECT: 0
    printf("%d\n", ctrl.total_interrupts);
    // EXPECT: 5

    ctrl.preemption_enabled = 1;
    irqc_trigger(&ctrl, 3);
    acked = irqc_acknowledge(&ctrl);
    printf("%d\n", acked);
    // EXPECT: 3
    printf("%d\n", ctrl.active_priority);
    // EXPECT: 3

    irqc_trigger(&ctrl, 0);
    acked = irqc_acknowledge(&ctrl);
    printf("%d\n", acked);
    // EXPECT: 0
    printf("%d\n", ctrl.nested_depth);
    // EXPECT: 2

    irqc_trigger(&ctrl, 6);
    acked = irqc_acknowledge(&ctrl);
    printf("%d\n", acked);
    // EXPECT: -1

    irqc_complete(&ctrl, 0);
    irqc_complete(&ctrl, 3);
    printf("%d\n", ctrl.nested_depth);
    // EXPECT: 0

    int latency = irqc_simulate_latency(0, 10);
    printf("%d\n", latency);
    // EXPECT: 10
    latency = irqc_simulate_latency(3, 10);
    printf("%d\n", latency);
    // EXPECT: 16

    printf("%d\n", ctrl.sources[0].trigger_count);
    // EXPECT: 2
    printf("%d\n", ctrl.sources[0].service_count);
    // EXPECT: 2

    printf("%s\n", "irqc_done");
    // EXPECT: irqc_done

    return 0;
}
