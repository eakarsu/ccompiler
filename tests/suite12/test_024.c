int printf(const char *fmt, ...);

// Music Sequencer
// Step sequencer with 16 steps, 4 tracks
// Each step has a note (MIDI-like number 0-127) and velocity (0-127)

struct Step {
    int note;      // 0 = rest, 1-127 = note number
    int velocity;  // 0-127
};

struct Track {
    struct Step steps[16];
    int length;     // number of active steps (1-16)
    int instrument; // instrument ID
};

struct PlayEvent {
    int track_id;
    int step;
    int note;
    int velocity;
};

int is_rest(struct Step s) {
    return s.note == 0;
}

int transpose_note(int note, int semitones) {
    int result = note + semitones;
    if (result < 1) result = 1;
    if (result > 127) result = 127;
    return result;
}

int velocity_scale(int velocity, int percent) {
    int result = (velocity * percent) / 100;
    if (result > 127) result = 127;
    if (result < 0) result = 0;
    return result;
}

void init_track(struct Track *t, int instrument, int length) {
    int i;
    t->instrument = instrument;
    t->length = length;
    for (i = 0; i < 16; i = i + 1) {
        t->steps[i].note = 0;
        t->steps[i].velocity = 0;
    }
}

void set_step(struct Track *t, int idx, int note, int vel) {
    if (idx >= 0 && idx < 16) {
        t->steps[idx].note = note;
        t->steps[idx].velocity = vel;
    }
}

int count_active_steps(struct Track *t) {
    int count = 0;
    int i;
    for (i = 0; i < t->length; i = i + 1) {
        if (t->steps[i].note > 0) {
            count = count + 1;
        }
    }
    return count;
}

int get_highest_note(struct Track *t) {
    int highest = 0;
    int i;
    for (i = 0; i < t->length; i = i + 1) {
        if (t->steps[i].note > highest) {
            highest = t->steps[i].note;
        }
    }
    return highest;
}

int get_average_velocity(struct Track *t) {
    int sum = 0;
    int count = 0;
    int i;
    for (i = 0; i < t->length; i = i + 1) {
        if (t->steps[i].note > 0) {
            sum = sum + t->steps[i].velocity;
            count = count + 1;
        }
    }
    if (count == 0) return 0;
    return sum / count;
}

void play_sequence(struct Track *t, int num_steps) {
    int i;
    for (i = 0; i < num_steps; i = i + 1) {
        int idx = i % t->length;
        struct Step s = t->steps[idx];
        if (s.note > 0) {
            printf("  Step %d: note=%d vel=%d\n", i, s.note, s.velocity);
        } else {
            printf("  Step %d: rest\n", i);
        }
    }
}

int main() {
    struct Track melody;
    init_track(&melody, 1, 8);

    // C major scale fragment: C4=60, D4=62, E4=64, F4=65, G4=67
    set_step(&melody, 0, 60, 100);
    set_step(&melody, 1, 62, 90);
    set_step(&melody, 2, 64, 80);
    set_step(&melody, 3, 0, 0);    // rest
    set_step(&melody, 4, 65, 100);
    set_step(&melody, 5, 67, 110);
    set_step(&melody, 6, 64, 85);
    set_step(&melody, 7, 60, 95);

    printf("Melody sequence:\n");
    // EXPECT: Melody sequence:
    play_sequence(&melody, 8);
    // EXPECT:   Step 0: note=60 vel=100
    // EXPECT:   Step 1: note=62 vel=90
    // EXPECT:   Step 2: note=64 vel=80
    // EXPECT:   Step 3: rest
    // EXPECT:   Step 4: note=65 vel=100
    // EXPECT:   Step 5: note=67 vel=110
    // EXPECT:   Step 6: note=64 vel=85
    // EXPECT:   Step 7: note=60 vel=95

    int active = count_active_steps(&melody);
    printf("Active steps: %d\n", active);
    // EXPECT: Active steps: 7

    int highest = get_highest_note(&melody);
    printf("Highest note: %d\n", highest);
    // EXPECT: Highest note: 67

    int avg_vel = get_average_velocity(&melody);
    printf("Average velocity: %d\n", avg_vel);
    // EXPECT: Average velocity: 94

    // Transpose up 12 semitones (one octave)
    int n1 = transpose_note(60, 12);
    printf("C4 up octave: %d\n", n1);
    // EXPECT: C4 up octave: 72

    int n2 = transpose_note(120, 12);
    printf("Clamped high: %d\n", n2);
    // EXPECT: Clamped high: 127

    int n3 = transpose_note(5, -10);
    printf("Clamped low: %d\n", n3);
    // EXPECT: Clamped low: 1

    // Velocity scaling
    int v1 = velocity_scale(100, 50);
    printf("Vel 50%%: %d\n", v1);
    // EXPECT: Vel 50%: 50

    int v2 = velocity_scale(100, 150);
    printf("Vel 150%%: %d\n", v2);
    // EXPECT: Vel 150%: 127

    // Drum track
    struct Track drums;
    init_track(&drums, 10, 4);
    set_step(&drums, 0, 36, 127);  // kick
    set_step(&drums, 1, 0, 0);     // rest
    set_step(&drums, 2, 38, 100);  // snare
    set_step(&drums, 3, 0, 0);     // rest

    printf("Drum pattern (2 bars):\n");
    // EXPECT: Drum pattern (2 bars):
    play_sequence(&drums, 8);
    // EXPECT:   Step 0: note=36 vel=127
    // EXPECT:   Step 1: rest
    // EXPECT:   Step 2: note=38 vel=100
    // EXPECT:   Step 3: rest
    // EXPECT:   Step 4: note=36 vel=127
    // EXPECT:   Step 5: rest
    // EXPECT:   Step 6: note=38 vel=100
    // EXPECT:   Step 7: rest

    int drum_active = count_active_steps(&drums);
    printf("Drum active: %d\n", drum_active);
    // EXPECT: Drum active: 2

    return 0;
}
