int printf(const char *fmt, ...);

// Interrupt Controller Simulation
// Priority-based interrupt handling with nesting, masking, pending bits

enum IrqState { IRQ_INACTIVE, IRQ_PENDING, IRQ_ACTIVE };

struct Interrupt {
    int irq_number;
    int priority;       // 0 = highest priority
    int state;
    int enabled;
    int pending;
    int active;
    int trigger_count;
};

struct InterruptController {
    struct Interrupt irqs[16];
    int num_irqs;
    int global_enable;
    int active_irq;         // currently servicing (-1 if none)
    int active_priority;    // priority of current IRQ
    int nested_depth;
    int total_handled;
    int handling_log[32];   // log of handled IRQ numbers
    int log_index;
    int preemption_count;
};

void nvic_init(struct InterruptController *nvic) {
    int i;
    nvic->num_irqs = 16;
    nvic->global_enable = 0;
    nvic->active_irq = -1;
    nvic->active_priority = 99;
    nvic->nested_depth = 0;
    nvic->total_handled = 0;
    nvic->log_index = 0;
    nvic->preemption_count = 0;
    for (i = 0; i < 16; i++) {
        nvic->irqs[i].irq_number = i;
        nvic->irqs[i].priority = 15;  // default lowest
        nvic->irqs[i].state = IRQ_INACTIVE;
        nvic->irqs[i].enabled = 0;
        nvic->irqs[i].pending = 0;
        nvic->irqs[i].active = 0;
        nvic->irqs[i].trigger_count = 0;
    }
    for (i = 0; i < 32; i++) {
        nvic->handling_log[i] = -1;
    }
}

void nvic_enable_global(struct InterruptController *nvic) {
    nvic->global_enable = 1;
}

void nvic_disable_global(struct InterruptController *nvic) {
    nvic->global_enable = 0;
}

void nvic_set_priority(struct InterruptController *nvic, int irq, int prio) {
    if (irq < 0 || irq >= 16) return;
    nvic->irqs[irq].priority = prio;
}

void nvic_enable_irq(struct InterruptController *nvic, int irq) {
    if (irq < 0 || irq >= 16) return;
    nvic->irqs[irq].enabled = 1;
}

void nvic_disable_irq(struct InterruptController *nvic, int irq) {
    if (irq < 0 || irq >= 16) return;
    nvic->irqs[irq].enabled = 0;
}

// Trigger an interrupt (set pending)
void nvic_trigger(struct InterruptController *nvic, int irq) {
    if (irq < 0 || irq >= 16) return;
    if (!nvic->irqs[irq].enabled) return;
    nvic->irqs[irq].pending = 1;
    nvic->irqs[irq].state = IRQ_PENDING;
    nvic->irqs[irq].trigger_count = nvic->irqs[irq].trigger_count + 1;
}

void nvic_clear_pending(struct InterruptController *nvic, int irq) {
    if (irq < 0 || irq >= 16) return;
    nvic->irqs[irq].pending = 0;
    if (nvic->irqs[irq].state == IRQ_PENDING) {
        nvic->irqs[irq].state = IRQ_INACTIVE;
    }
}

// Find highest priority pending interrupt
int nvic_get_highest_pending(struct InterruptController *nvic) {
    int best_irq = -1;
    int best_prio = 99;
    int i;
    for (i = 0; i < nvic->num_irqs; i++) {
        if (nvic->irqs[i].pending && nvic->irqs[i].enabled) {
            if (nvic->irqs[i].priority < best_prio) {
                best_prio = nvic->irqs[i].priority;
                best_irq = i;
            }
        }
    }
    return best_irq;
}

// Process one interrupt (non-preemptive)
int nvic_process_one(struct InterruptController *nvic) {
    int irq;
    if (!nvic->global_enable) return -1;

    irq = nvic_get_highest_pending(nvic);
    if (irq < 0) return -1;

    // Check if higher priority than current
    if (nvic->active_irq >= 0) {
        if (nvic->irqs[irq].priority >= nvic->active_priority) {
            return -1;  // cannot preempt
        }
        nvic->preemption_count = nvic->preemption_count + 1;
        nvic->nested_depth = nvic->nested_depth + 1;
    }

    // Service the interrupt
    nvic->irqs[irq].pending = 0;
    nvic->irqs[irq].active = 1;
    nvic->irqs[irq].state = IRQ_ACTIVE;
    nvic->active_irq = irq;
    nvic->active_priority = nvic->irqs[irq].priority;

    // Log it
    if (nvic->log_index < 32) {
        nvic->handling_log[nvic->log_index] = irq;
        nvic->log_index = nvic->log_index + 1;
    }
    nvic->total_handled = nvic->total_handled + 1;

    return irq;
}

// Complete current interrupt service
void nvic_complete(struct InterruptController *nvic) {
    if (nvic->active_irq < 0) return;

    nvic->irqs[nvic->active_irq].active = 0;
    nvic->irqs[nvic->active_irq].state = IRQ_INACTIVE;

    if (nvic->nested_depth > 0) {
        nvic->nested_depth = nvic->nested_depth - 1;
    }

    nvic->active_irq = -1;
    nvic->active_priority = 99;
}

// Count pending interrupts
int nvic_count_pending(struct InterruptController *nvic) {
    int count = 0;
    int i;
    for (i = 0; i < nvic->num_irqs; i++) {
        if (nvic->irqs[i].pending) {
            count = count + 1;
        }
    }
    return count;
}

// Process all pending interrupts in priority order
int nvic_process_all(struct InterruptController *nvic) {
    int count = 0;
    int irq;
    while (1) {
        irq = nvic_process_one(nvic);
        if (irq < 0) break;
        nvic_complete(nvic);
        count = count + 1;
    }
    return count;
}

// Get pending mask as integer (bit per IRQ)
int nvic_pending_mask(struct InterruptController *nvic) {
    int mask = 0;
    int i;
    for (i = 0; i < 16; i++) {
        if (nvic->irqs[i].pending) {
            mask = mask | (1 << i);
        }
    }
    return mask;
}

int main() {
    struct InterruptController nvic;
    int irq;
    int count;
    int i;

    nvic_init(&nvic);
    printf("NVIC initialized, %d IRQs\n", nvic.num_irqs);
    // EXPECT: NVIC initialized, 16 IRQs

    nvic_enable_global(&nvic);

    // Configure interrupts
    nvic_enable_irq(&nvic, 0);  // Timer
    nvic_enable_irq(&nvic, 1);  // UART
    nvic_enable_irq(&nvic, 2);  // SPI
    nvic_enable_irq(&nvic, 3);  // I2C
    nvic_enable_irq(&nvic, 4);  // ADC

    nvic_set_priority(&nvic, 0, 0);   // Timer: highest
    nvic_set_priority(&nvic, 1, 2);   // UART: medium
    nvic_set_priority(&nvic, 2, 3);   // SPI: low
    nvic_set_priority(&nvic, 3, 3);   // I2C: low (same as SPI)
    nvic_set_priority(&nvic, 4, 1);   // ADC: high

    printf("Priorities set\n");
    // EXPECT: Priorities set

    // Trigger single interrupt
    nvic_trigger(&nvic, 1);
    printf("Pending count: %d\n", nvic_count_pending(&nvic));
    // EXPECT: Pending count: 1

    irq = nvic_process_one(&nvic);
    printf("Serviced IRQ: %d\n", irq);
    // EXPECT: Serviced IRQ: 1
    nvic_complete(&nvic);

    // Trigger multiple - should process highest priority first
    nvic_trigger(&nvic, 2);  // prio 3
    nvic_trigger(&nvic, 4);  // prio 1
    nvic_trigger(&nvic, 1);  // prio 2
    printf("Pending: %d\n", nvic_count_pending(&nvic));
    // EXPECT: Pending: 3

    irq = nvic_process_one(&nvic);
    printf("First serviced: IRQ %d (prio %d)\n", irq, nvic.irqs[irq].priority);
    // EXPECT: First serviced: IRQ 4 (prio 1)
    nvic_complete(&nvic);

    irq = nvic_process_one(&nvic);
    printf("Second serviced: IRQ %d (prio %d)\n", irq, nvic.irqs[irq].priority);
    // EXPECT: Second serviced: IRQ 1 (prio 2)
    nvic_complete(&nvic);

    irq = nvic_process_one(&nvic);
    printf("Third serviced: IRQ %d (prio %d)\n", irq, nvic.irqs[irq].priority);
    // EXPECT: Third serviced: IRQ 2 (prio 3)
    nvic_complete(&nvic);

    // Process all at once
    nvic_trigger(&nvic, 0);
    nvic_trigger(&nvic, 3);
    nvic_trigger(&nvic, 4);
    count = nvic_process_all(&nvic);
    printf("Processed all: %d\n", count);
    // EXPECT: Processed all: 3

    printf("Total handled: %d\n", nvic.total_handled);
    // EXPECT: Total handled: 7

    // Check handling log
    printf("Log:");
    for (i = 0; i < nvic.log_index; i++) {
        printf(" %d", nvic.handling_log[i]);
    }
    printf("\n");
    // EXPECT: Log: 1 4 1 2 0 4 3

    // Test disabled interrupt
    nvic_trigger(&nvic, 5);  // not enabled
    printf("Disabled pending: %d\n", nvic_count_pending(&nvic));
    // EXPECT: Disabled pending: 0

    // Test global disable
    nvic_trigger(&nvic, 0);
    nvic_disable_global(&nvic);
    irq = nvic_process_one(&nvic);
    printf("Global disabled result: %d\n", irq);
    // EXPECT: Global disabled result: -1

    nvic_enable_global(&nvic);
    irq = nvic_process_one(&nvic);
    printf("After re-enable: IRQ %d\n", irq);
    // EXPECT: After re-enable: IRQ 0
    nvic_complete(&nvic);

    // Test pending mask
    nvic_trigger(&nvic, 0);
    nvic_trigger(&nvic, 2);
    nvic_trigger(&nvic, 4);
    int mask = nvic_pending_mask(&nvic);
    printf("Pending mask: %d\n", mask);
    // EXPECT: Pending mask: 21

    nvic_process_all(&nvic);

    // Trigger counts
    printf("Timer triggers: %d\n", nvic.irqs[0].trigger_count);
    // EXPECT: Timer triggers: 3
    printf("ADC triggers: %d\n", nvic.irqs[4].trigger_count);
    // EXPECT: ADC triggers: 3

    // Clear pending test
    nvic_trigger(&nvic, 1);
    nvic_clear_pending(&nvic, 1);
    printf("After clear, pending: %d\n", nvic_count_pending(&nvic));
    // EXPECT: After clear, pending: 0

    printf("Interrupt controller test complete\n");
    // EXPECT: Interrupt controller test complete

    return 0;
}
