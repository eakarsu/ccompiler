int printf(const char *fmt, ...);

// Memento Pattern: save and restore state snapshots

struct GameState {
    int player_x;
    int player_y;
    int health;
    int score;
    int level;
};

struct Memento {
    struct GameState state;
    int valid;
    int save_id;
};

struct MementoStack {
    struct Memento slots[8];
    int top;
    int next_id;
};

void memento_stack_init(struct MementoStack *ms) {
    ms->top = 0;
    ms->next_id = 1;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        ms->slots[i].valid = 0;
    }
}

int memento_save(struct MementoStack *ms, struct GameState *gs) {
    if (ms->top >= 8) {
        printf("Stack full, cannot save\n");
        return 0;
    }
    ms->slots[ms->top].state = *gs;
    ms->slots[ms->top].valid = 1;
    ms->slots[ms->top].save_id = ms->next_id;
    ms->next_id = ms->next_id + 1;
    ms->top = ms->top + 1;
    return ms->next_id - 1;
}

int memento_restore(struct MementoStack *ms, struct GameState *gs) {
    if (ms->top <= 0) {
        printf("Stack empty, cannot restore\n");
        return 0;
    }
    ms->top = ms->top - 1;
    *gs = ms->slots[ms->top].state;
    ms->slots[ms->top].valid = 0;
    return ms->slots[ms->top].save_id;
}

void game_state_init(struct GameState *gs) {
    gs->player_x = 0;
    gs->player_y = 0;
    gs->health = 100;
    gs->score = 0;
    gs->level = 1;
}

void game_move(struct GameState *gs, int dx, int dy) {
    gs->player_x = gs->player_x + dx;
    gs->player_y = gs->player_y + dy;
}

void game_take_damage(struct GameState *gs, int dmg) {
    gs->health = gs->health - dmg;
    if (gs->health < 0) gs->health = 0;
}

void game_add_score(struct GameState *gs, int pts) {
    gs->score = gs->score + pts;
}

void game_level_up(struct GameState *gs) {
    gs->level = gs->level + 1;
    gs->health = 100;
}

void print_state(struct GameState *gs) {
    printf("pos:(%d,%d) hp:%d score:%d lvl:%d\n",
        gs->player_x, gs->player_y,
        gs->health, gs->score, gs->level);
}

int main() {
    struct GameState game;
    struct MementoStack saves;

    game_state_init(&game);
    memento_stack_init(&saves);

    printf("Initial state:\n"); // EXPECT: Initial state:
    print_state(&game); // EXPECT: pos:(0,0) hp:100 score:0 lvl:1

    int sid;
    sid = memento_save(&saves, &game);
    printf("Saved checkpoint %d\n", sid); // EXPECT: Saved checkpoint 1

    game_move(&game, 5, 3);
    game_add_score(&game, 100);
    printf("After move and score:\n"); // EXPECT: After move and score:
    print_state(&game); // EXPECT: pos:(5,3) hp:100 score:100 lvl:1

    sid = memento_save(&saves, &game);
    printf("Saved checkpoint %d\n", sid); // EXPECT: Saved checkpoint 2

    game_take_damage(&game, 40);
    game_move(&game, 2, -1);
    printf("After damage and move:\n"); // EXPECT: After damage and move:
    print_state(&game); // EXPECT: pos:(7,2) hp:60 score:100 lvl:1

    sid = memento_save(&saves, &game);
    printf("Saved checkpoint %d\n", sid); // EXPECT: Saved checkpoint 3

    game_take_damage(&game, 80);
    printf("After heavy damage:\n"); // EXPECT: After heavy damage:
    print_state(&game); // EXPECT: pos:(7,2) hp:0 score:100 lvl:1

    printf("Player died! Restoring...\n"); // EXPECT: Player died! Restoring...
    int rid;
    rid = memento_restore(&saves, &game);
    printf("Restored checkpoint %d\n", rid); // EXPECT: Restored checkpoint 3
    print_state(&game); // EXPECT: pos:(7,2) hp:60 score:100 lvl:1

    game_add_score(&game, 200);
    game_level_up(&game);
    printf("After level up:\n"); // EXPECT: After level up:
    print_state(&game); // EXPECT: pos:(7,2) hp:100 score:300 lvl:2

    sid = memento_save(&saves, &game);
    printf("Saved checkpoint %d\n", sid); // EXPECT: Saved checkpoint 4

    game_take_damage(&game, 999);
    printf("Died again:\n"); // EXPECT: Died again:
    print_state(&game); // EXPECT: pos:(7,2) hp:0 score:300 lvl:2

    rid = memento_restore(&saves, &game);
    printf("Restored checkpoint %d\n", rid); // EXPECT: Restored checkpoint 4
    print_state(&game); // EXPECT: pos:(7,2) hp:100 score:300 lvl:2

    printf("Saves remaining: %d\n", saves.top); // EXPECT: Saves remaining: 2

    rid = memento_restore(&saves, &game);
    printf("Restored checkpoint %d\n", rid); // EXPECT: Restored checkpoint 2
    print_state(&game); // EXPECT: pos:(5,3) hp:100 score:100 lvl:1

    printf("Done\n"); // EXPECT: Done
    return 0;
}
