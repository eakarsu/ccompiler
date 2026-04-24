int printf(const char *fmt, ...);
// EXPECT: === Genetic Algorithm Simulation ===\nTarget: 10110101\nInitial population:\n  0: 11001010 (fit=1)\n  1: 00110101 (fit=7)\n  2: 10011010 (fit=3)\n  3: 01100101 (fit=5)\n  4: 10101010 (fit=3)\n  5: 01010101 (fit=5)\nAvg fitness: 4\nGen 1: best_fit=7 avg=4 new_child=01100101 (fit=5)\nGen 2: best_fit=7 avg=5 new_child=00010101 (fit=6)\nGen 3: best_fit=7 avg=5 new_child=00100101 (fit=6)\nGen 4: best_fit=7 avg=5 new_child=00111101 (fit=6)\nFinal population:\n  0: 00111101 (fit=6)\n  1: 00110101 (fit=7)\n  2: 00010101 (fit=6)\n  3: 01100101 (fit=5)\n  4: 00100101 (fit=6)\n  5: 01010101 (fit=5)\nBest fitness: 7

struct Individual {
    int genes[8];
    int fitness;
};

struct Population {
    struct Individual members[6];
    int size;
    int generation;
};

int target[8];

void set_target(void) {
    target[0] = 1; target[1] = 0; target[2] = 1; target[3] = 1;
    target[4] = 0; target[5] = 1; target[6] = 0; target[7] = 1;
}

int evaluate_fitness(struct Individual *ind) {
    int fit = 0;
    int i;
    for (i = 0; i < 8; i++) {
        if (ind->genes[i] == target[i]) fit++;
    }
    ind->fitness = fit;
    return fit;
}

void init_population(struct Population *pop) {
    pop->size = 6;
    pop->generation = 0;
    int patterns[6][8];
    int i, j;

    patterns[0][0]=1; patterns[0][1]=1; patterns[0][2]=0; patterns[0][3]=0;
    patterns[0][4]=1; patterns[0][5]=0; patterns[0][6]=1; patterns[0][7]=0;
    patterns[1][0]=0; patterns[1][1]=0; patterns[1][2]=1; patterns[1][3]=1;
    patterns[1][4]=0; patterns[1][5]=1; patterns[1][6]=0; patterns[1][7]=1;
    patterns[2][0]=1; patterns[2][1]=0; patterns[2][2]=0; patterns[2][3]=1;
    patterns[2][4]=1; patterns[2][5]=0; patterns[2][6]=1; patterns[2][7]=0;
    patterns[3][0]=0; patterns[3][1]=1; patterns[3][2]=1; patterns[3][3]=0;
    patterns[3][4]=0; patterns[3][5]=1; patterns[3][6]=0; patterns[3][7]=1;
    patterns[4][0]=1; patterns[4][1]=0; patterns[4][2]=1; patterns[4][3]=0;
    patterns[4][4]=1; patterns[4][5]=0; patterns[4][6]=1; patterns[4][7]=0;
    patterns[5][0]=0; patterns[5][1]=1; patterns[5][2]=0; patterns[5][3]=1;
    patterns[5][4]=0; patterns[5][5]=1; patterns[5][6]=0; patterns[5][7]=1;

    for (i = 0; i < 6; i++) {
        for (j = 0; j < 8; j++) {
            pop->members[i].genes[j] = patterns[i][j];
        }
        evaluate_fitness(&pop->members[i]);
    }
}

int find_best(struct Population *pop) {
    int best = 0;
    int i;
    for (i = 1; i < pop->size; i++) {
        if (pop->members[i].fitness > pop->members[best].fitness) {
            best = i;
        }
    }
    return best;
}

int find_worst(struct Population *pop) {
    int worst = 0;
    int i;
    for (i = 1; i < pop->size; i++) {
        if (pop->members[i].fitness < pop->members[worst].fitness) {
            worst = i;
        }
    }
    return worst;
}

void crossover(struct Individual *p1, struct Individual *p2, struct Individual *child, int point) {
    int i;
    for (i = 0; i < point; i++) {
        child->genes[i] = p1->genes[i];
    }
    for (i = point; i < 8; i++) {
        child->genes[i] = p2->genes[i];
    }
}

void mutate(struct Individual *ind, int pos) {
    if (pos >= 0 && pos < 8) {
        ind->genes[pos] = 1 - ind->genes[pos];
    }
}

void print_individual(struct Individual *ind) {
    int i;
    for (i = 0; i < 8; i++) {
        printf("%d", ind->genes[i]);
    }
    printf(" (fit=%d)", ind->fitness);
}

int avg_fitness(struct Population *pop) {
    int sum = 0;
    int i;
    for (i = 0; i < pop->size; i++) {
        sum = sum + pop->members[i].fitness;
    }
    return sum / pop->size;
}

int main(void) {
    set_target();

    struct Population pop;
    init_population(&pop);

    printf("=== Genetic Algorithm Simulation ===\n");
    printf("Target: ");
    int i;
    for (i = 0; i < 8; i++) printf("%d", target[i]);
    printf("\n");

    printf("Initial population:\n");
    for (i = 0; i < pop.size; i++) {
        printf("  %d: ", i);
        print_individual(&pop.members[i]);
        printf("\n");
    }
    printf("Avg fitness: %d\n", avg_fitness(&pop));

    int gen;
    for (gen = 1; gen <= 4; gen++) {
        pop.generation = gen;
        int best = find_best(&pop);
        int worst = find_worst(&pop);

        int second_best = (best == 0) ? 1 : 0;
        for (i = 0; i < pop.size; i++) {
            if (i != best && pop.members[i].fitness > pop.members[second_best].fitness) {
                second_best = i;
            }
        }

        struct Individual child;
        crossover(&pop.members[best], &pop.members[second_best], &child, gen + 2);

        mutate(&child, gen % 8);

        evaluate_fitness(&child);
        pop.members[worst] = child;

        printf("Gen %d: best_fit=%d avg=%d new_child=",
               gen, pop.members[find_best(&pop)].fitness, avg_fitness(&pop));
        print_individual(&child);
        printf("\n");
    }

    printf("Final population:\n");
    for (i = 0; i < pop.size; i++) {
        printf("  %d: ", i);
        print_individual(&pop.members[i]);
        printf("\n");
    }
    printf("Best fitness: %d\n", pop.members[find_best(&pop)].fitness);

    return 0;
}
