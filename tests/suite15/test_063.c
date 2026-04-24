int printf(const char *fmt, ...);

// INI file parser: parse [sections] and key=value pairs

struct INIEntry {
    char key[32];
    char value[32];
    int section_id;
};

struct INISection {
    char name[32];
    int entry_start;
    int entry_count;
};

struct INIFile {
    struct INISection sections[8];
    int section_count;
    struct INIEntry entries[32];
    int entry_count;
};

int str_len(char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

void str_copy(char *dst, char *src, int n) {
    int i;
    for (i = 0; i < n && i < 31; i++) dst[i] = src[i];
    dst[i] = 0;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

void skip_spaces(char *line, int *pos) {
    while (line[*pos] == ' ' || line[*pos] == '\t') (*pos)++;
}

int parse_ini_line(char *line, struct INIFile *ini) {
    int pos = 0;
    skip_spaces(line, &pos);

    // Empty or comment
    if (line[pos] == 0 || line[pos] == ';' || line[pos] == '#') {
        return 0;
    }

    // Section header
    if (line[pos] == '[') {
        pos++;
        int start = pos;
        while (line[pos] && line[pos] != ']') pos++;
        int sidx = ini->section_count;
        str_copy(ini->sections[sidx].name, line + start, pos - start);
        ini->sections[sidx].entry_start = ini->entry_count;
        ini->sections[sidx].entry_count = 0;
        ini->section_count++;
        return 1;
    }

    // Key=value
    int kstart = pos;
    while (line[pos] && line[pos] != '=' && line[pos] != ' ') pos++;
    int kend = pos;
    skip_spaces(line, &pos);
    if (line[pos] == '=') pos++;
    skip_spaces(line, &pos);
    int vstart = pos;
    int vend = str_len(line);
    // Trim trailing spaces
    while (vend > vstart && (line[vend - 1] == ' ' || line[vend - 1] == '\t' ||
           line[vend - 1] == '\n' || line[vend - 1] == '\r')) vend--;

    int eidx = ini->entry_count;
    str_copy(ini->entries[eidx].key, line + kstart, kend - kstart);
    str_copy(ini->entries[eidx].value, line + vstart, vend - vstart);
    ini->entries[eidx].section_id = ini->section_count - 1;
    ini->entry_count++;

    if (ini->section_count > 0) {
        ini->sections[ini->section_count - 1].entry_count++;
    }
    return 2;
}

int find_section(struct INIFile *ini, char *name) {
    int i;
    for (i = 0; i < ini->section_count; i++) {
        if (str_eq(ini->sections[i].name, name)) return i;
    }
    return -1;
}

char *get_value(struct INIFile *ini, int section, char *key) {
    int start = ini->sections[section].entry_start;
    int count = ini->sections[section].entry_count;
    int i;
    for (i = start; i < start + count; i++) {
        if (str_eq(ini->entries[i].key, key)) return ini->entries[i].value;
    }
    return "";
}

int main() {
    struct INIFile ini;
    ini.section_count = 0;
    ini.entry_count = 0;

    // Parse lines one by one
    parse_ini_line("[database]", &ini);
    parse_ini_line("host = localhost", &ini);
    parse_ini_line("port = 5432", &ini);
    parse_ini_line("name = mydb", &ini);
    parse_ini_line("", &ini);
    parse_ini_line("; this is a comment", &ini);
    parse_ini_line("[server]", &ini);
    parse_ini_line("bind = 0.0.0.0", &ini);
    parse_ini_line("workers = 4", &ini);
    parse_ini_line("[logging]", &ini);
    parse_ini_line("level = debug", &ini);
    parse_ini_line("file = app.log", &ini);

    printf("%d\n", ini.section_count);
    // EXPECT: 3
    printf("%d\n", ini.entry_count);
    // EXPECT: 7

    printf("%s\n", ini.sections[0].name);
    // EXPECT: database
    printf("%d\n", ini.sections[0].entry_count);
    // EXPECT: 3

    printf("%s\n", ini.sections[1].name);
    // EXPECT: server
    printf("%d\n", ini.sections[1].entry_count);
    // EXPECT: 2

    printf("%s\n", ini.sections[2].name);
    // EXPECT: logging
    printf("%d\n", ini.sections[2].entry_count);
    // EXPECT: 2

    // Find section
    int db = find_section(&ini, "database");
    printf("%d\n", db);
    // EXPECT: 0

    // Get values
    printf("%s\n", get_value(&ini, db, "host"));
    // EXPECT: localhost
    printf("%s\n", get_value(&ini, db, "port"));
    // EXPECT: 5432
    printf("%s\n", get_value(&ini, db, "name"));
    // EXPECT: mydb

    int srv = find_section(&ini, "server");
    printf("%s\n", get_value(&ini, srv, "bind"));
    // EXPECT: 0.0.0.0
    printf("%s\n", get_value(&ini, srv, "workers"));
    // EXPECT: 4

    int log_sec = find_section(&ini, "logging");
    printf("%s\n", get_value(&ini, log_sec, "level"));
    // EXPECT: debug
    printf("%s\n", get_value(&ini, log_sec, "file"));
    // EXPECT: app.log

    // Missing section
    int ms = find_section(&ini, "cache");
    printf("%d\n", ms);
    // EXPECT: -1

    // Missing key returns empty string
    char *mv = get_value(&ini, db, "password");
    printf("%d\n", str_len(mv));
    // EXPECT: 0

    // Comments and blank lines should not add entries
    struct INIFile ini2;
    ini2.section_count = 0;
    ini2.entry_count = 0;
    parse_ini_line("# comment", &ini2);
    parse_ini_line("", &ini2);
    parse_ini_line("; another comment", &ini2);
    printf("%d\n", ini2.section_count);
    // EXPECT: 0
    printf("%d\n", ini2.entry_count);
    // EXPECT: 0

    return 0;
}
