int printf(const char *fmt, ...);

// Note Frequency Table
// Maps MIDI note numbers to integer frequencies (Hz)
// Uses a precomputed table for one octave, then shifts

// Base frequencies for C4(60) through B4(71) in Hz (integer approximation)
// C4=262, C#4=277, D4=294, D#4=311, E4=330, F4=349,
// F#4=370, G4=392, G#4=415, A4=440, A#4=466, B4=494

int base_freq[12];

void init_freq_table() {
    base_freq[0] = 262;   // C
    base_freq[1] = 277;   // C#
    base_freq[2] = 294;   // D
    base_freq[3] = 311;   // D#
    base_freq[4] = 330;   // E
    base_freq[5] = 349;   // F
    base_freq[6] = 370;   // F#
    base_freq[7] = 392;   // G
    base_freq[8] = 415;   // G#
    base_freq[9] = 440;   // A
    base_freq[10] = 466;  // A#
    base_freq[11] = 494;  // B
}

int note_to_freq(int midi_note) {
    if (midi_note < 0 || midi_note > 127) return 0;

    int note_in_octave = midi_note % 12;
    int octave = midi_note / 12;
    int base = base_freq[note_in_octave];

    // Octave 4 is our reference (MIDI notes 60-71)
    // Each octave up doubles frequency, each down halves
    int ref_octave = 5;  // MIDI octave (60/12 = 5)
    int diff = octave - ref_octave;

    if (diff > 0) {
        int i;
        for (i = 0; i < diff; i = i + 1) {
            base = base * 2;
        }
    } else if (diff < 0) {
        int i;
        for (i = 0; i < -diff; i = i + 1) {
            base = base / 2;
        }
    }
    return base;
}

int note_name_idx(int midi_note) {
    return midi_note % 12;
}

int note_octave(int midi_note) {
    return (midi_note / 12) - 1;
}

void print_note_name(int midi_note) {
    int idx = note_name_idx(midi_note);
    int oct = note_octave(midi_note);
    if (idx == 0) printf("C%d", oct);
    else if (idx == 1) printf("C#%d", oct);
    else if (idx == 2) printf("D%d", oct);
    else if (idx == 3) printf("D#%d", oct);
    else if (idx == 4) printf("E%d", oct);
    else if (idx == 5) printf("F%d", oct);
    else if (idx == 6) printf("F#%d", oct);
    else if (idx == 7) printf("G%d", oct);
    else if (idx == 8) printf("G#%d", oct);
    else if (idx == 9) printf("A%d", oct);
    else if (idx == 10) printf("A#%d", oct);
    else if (idx == 11) printf("B%d", oct);
}

int freq_to_period(int freq, int sample_rate) {
    if (freq == 0) return 0;
    return sample_rate / freq;
}

int interval_ratio(int note1, int note2) {
    // Return frequency ratio * 1000
    int f1 = note_to_freq(note1);
    int f2 = note_to_freq(note2);
    if (f1 == 0) return 0;
    return (f2 * 1000) / f1;
}

int is_consonant(int note1, int note2) {
    int diff = note2 - note1;
    if (diff < 0) diff = -diff;
    diff = diff % 12;
    // Perfect consonances: unison(0), fifth(7), fourth(5), octave(0)
    if (diff == 0 || diff == 5 || diff == 7) return 1;
    // Major/minor thirds
    if (diff == 3 || diff == 4) return 1;
    return 0;
}

int main() {
    init_freq_table();

    // Print A4 frequency
    int a4 = note_to_freq(69);
    printf("A4 freq: %d\n", a4);
    // EXPECT: A4 freq: 440

    // Print C4 (middle C)
    int c4 = note_to_freq(60);
    printf("C4 freq: %d\n", c4);
    // EXPECT: C4 freq: 262

    // Print A3 (one octave below A4)
    int a3 = note_to_freq(57);
    printf("A3 freq: %d\n", a3);
    // EXPECT: A3 freq: 220

    // Print A5 (one octave above A4)
    int a5 = note_to_freq(81);
    printf("A5 freq: %d\n", a5);
    // EXPECT: A5 freq: 880

    // Note names
    printf("Note 60: ");
    print_note_name(60);
    printf("\n");
    // EXPECT: Note 60: C4

    printf("Note 69: ");
    print_note_name(69);
    printf("\n");
    // EXPECT: Note 69: A4

    printf("Note 72: ");
    print_note_name(72);
    printf("\n");
    // EXPECT: Note 72: C5

    // C major scale frequencies
    int scale_notes[8];
    scale_notes[0] = 60;  // C
    scale_notes[1] = 62;  // D
    scale_notes[2] = 64;  // E
    scale_notes[3] = 65;  // F
    scale_notes[4] = 67;  // G
    scale_notes[5] = 69;  // A
    scale_notes[6] = 71;  // B
    scale_notes[7] = 72;  // C5

    printf("C major scale: ");
    int i;
    for (i = 0; i < 8; i = i + 1) {
        printf("%d", note_to_freq(scale_notes[i]));
        if (i < 7) printf(" ");
    }
    printf("\n");
    // EXPECT: C major scale: 262 294 330 349 392 440 494 524

    // Period at 44100 Hz sample rate
    int period_a4 = freq_to_period(440, 44100);
    printf("A4 period at 44100: %d\n", period_a4);
    // EXPECT: A4 period at 44100: 100

    int period_c4 = freq_to_period(262, 44100);
    printf("C4 period at 44100: %d\n", period_c4);
    // EXPECT: C4 period at 44100: 168

    // Interval ratios (times 1000)
    int fifth = interval_ratio(60, 67);  // C to G
    printf("C-G ratio x1000: %d\n", fifth);
    // EXPECT: C-G ratio x1000: 1496

    int octave = interval_ratio(60, 72);  // C4 to C5
    printf("Octave ratio x1000: %d\n", octave);
    // EXPECT: Octave ratio x1000: 2000

    // Consonance checks
    int cons1 = is_consonant(60, 67);  // C-G (perfect fifth)
    printf("C-G consonant: %d\n", cons1);
    // EXPECT: C-G consonant: 1

    int cons2 = is_consonant(60, 66);  // C-F# (tritone)
    printf("C-F# consonant: %d\n", cons2);
    // EXPECT: C-F# consonant: 0

    int cons3 = is_consonant(60, 64);  // C-E (major third)
    printf("C-E consonant: %d\n", cons3);
    // EXPECT: C-E consonant: 1

    return 0;
}
