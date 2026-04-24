int printf(const char *fmt, ...);
// EXPECT: init:0|inserted:1,2,3,4,5|active=5|age>25: 3=2,4,5|age<30: 3=1,3,5|score>=90:2=1,3|sumscore=428|avgage=28|maxscore=95|minscore=70|find3=2|find99=-1|score(1)=90|age(2)=30|name(1)=Alex|name(3)=Carl|nmatch=1|nmatch2=0|del4=1|after_del:4,1|del99=0|age>25_after:2=2,5|sumafter=358|update=1,newscore1=100|more_ins=6,7,8|total_act=7|age_20_30:  4=1,2,3,5|score=92:1=7|sorted_by_scor=2,5,7,3,1|new_maxsc=100|new_avgag=28|cntbob=1|after_all=0,8|empty_sum=0|empty_avg=0|new_id=9|final_active=1|end

/* Simple in-memory database simulation */

/* Each row has: id, name (as 4 ints), age, score, active */
struct Row {
    int id;
    int name0;
    int name1;
    int name2;
    int name3;
    int age;
    int score;
    int active; /* 1 = exists, 0 = deleted */
};

struct Row table[64];
int table_size;
int next_id;

/* Query result buffer */
int result_ids[64];
int result_count;

void init_db(void) {
    int i;
    table_size = 0;
    next_id = 1;
    i = 0;
    while (i < 64) {
        table[i].id = 0;
        table[i].name0 = 0;
        table[i].name1 = 0;
        table[i].name2 = 0;
        table[i].name3 = 0;
        table[i].age = 0;
        table[i].score = 0;
        table[i].active = 0;
        i = i + 1;
    }
}

int insert_row(int n0, int n1, int n2, int n3, int age, int score) {
    int idx;
    idx = table_size;
    if (idx >= 64) return -1;
    table[idx].id = next_id;
    table[idx].name0 = n0;
    table[idx].name1 = n1;
    table[idx].name2 = n2;
    table[idx].name3 = n3;
    table[idx].age = age;
    table[idx].score = score;
    table[idx].active = 1;
    next_id = next_id + 1;
    table_size = table_size + 1;
    return table[idx].id;
}

int delete_by_id(int id) {
    int i;
    i = 0;
    while (i < table_size) {
        if (table[i].active && table[i].id == id) {
            table[i].active = 0;
            return 1;
        }
        i = i + 1;
    }
    return 0;
}

int find_by_id(int id) {
    int i;
    i = 0;
    while (i < table_size) {
        if (table[i].active && table[i].id == id) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int count_active(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < table_size) {
        if (table[i].active) {
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

int count_deleted(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < table_size) {
        if (!table[i].active) {
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

/* Select where age > threshold */
int select_age_gt(int threshold) {
    int i;
    result_count = 0;
    i = 0;
    while (i < table_size) {
        if (table[i].active && table[i].age > threshold) {
            result_ids[result_count] = table[i].id;
            result_count = result_count + 1;
        }
        i = i + 1;
    }
    return result_count;
}

/* Select where age < threshold */
int select_age_lt(int threshold) {
    int i;
    result_count = 0;
    i = 0;
    while (i < table_size) {
        if (table[i].active && table[i].age < threshold) {
            result_ids[result_count] = table[i].id;
            result_count = result_count + 1;
        }
        i = i + 1;
    }
    return result_count;
}

/* Select where score >= threshold */
int select_score_gte(int threshold) {
    int i;
    result_count = 0;
    i = 0;
    while (i < table_size) {
        if (table[i].active && table[i].score >= threshold) {
            result_ids[result_count] = table[i].id;
            result_count = result_count + 1;
        }
        i = i + 1;
    }
    return result_count;
}

/* Select where score == value */
int select_score_eq(int value) {
    int i;
    result_count = 0;
    i = 0;
    while (i < table_size) {
        if (table[i].active && table[i].score == value) {
            result_ids[result_count] = table[i].id;
            result_count = result_count + 1;
        }
        i = i + 1;
    }
    return result_count;
}

/* Select where age between lo and hi inclusive */
int select_age_between(int lo, int hi) {
    int i;
    result_count = 0;
    i = 0;
    while (i < table_size) {
        if (table[i].active && table[i].age >= lo && table[i].age <= hi) {
            result_ids[result_count] = table[i].id;
            result_count = result_count + 1;
        }
        i = i + 1;
    }
    return result_count;
}

/* Aggregate: sum of scores of active rows */
int sum_scores(void) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < table_size) {
        if (table[i].active) {
            s = s + table[i].score;
        }
        i = i + 1;
    }
    return s;
}

/* Aggregate: average age (integer division) */
int avg_age(void) {
    int i;
    int s;
    int cnt;
    s = 0;
    cnt = 0;
    i = 0;
    while (i < table_size) {
        if (table[i].active) {
            s = s + table[i].age;
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    if (cnt == 0) return 0;
    return s / cnt;
}

/* Aggregate: max score */
int max_score(void) {
    int i;
    int m;
    m = -1;
    i = 0;
    while (i < table_size) {
        if (table[i].active) {
            if (m < 0 || table[i].score > m) {
                m = table[i].score;
            }
        }
        i = i + 1;
    }
    return m;
}

/* Aggregate: min score */
int min_score(void) {
    int i;
    int m;
    m = -1;
    i = 0;
    while (i < table_size) {
        if (table[i].active) {
            if (m < 0 || table[i].score < m) {
                m = table[i].score;
            }
        }
        i = i + 1;
    }
    return m;
}

/* Update score by id */
int update_score(int id, int new_score) {
    int idx;
    idx = find_by_id(id);
    if (idx < 0) return 0;
    table[idx].score = new_score;
    return 1;
}

/* Update age by id */
int update_age(int id, int new_age) {
    int idx;
    idx = find_by_id(id);
    if (idx < 0) return 0;
    table[idx].age = new_age;
    return 1;
}

/* Sort result_ids by score (ascending, bubble sort) */
void sort_results_by_score(void) {
    int i;
    int j;
    int tmp;
    int si;
    int sj;
    int idx_i;
    int idx_j;
    i = 0;
    while (i < result_count - 1) {
        j = 0;
        while (j < result_count - 1 - i) {
            idx_i = find_by_id(result_ids[j]);
            idx_j = find_by_id(result_ids[j + 1]);
            si = table[idx_i].score;
            sj = table[idx_j].score;
            if (si > sj) {
                tmp = result_ids[j];
                result_ids[j] = result_ids[j + 1];
                result_ids[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

/* Print result ids */
void print_results(void) {
    int i;
    i = 0;
    while (i < result_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", result_ids[i]);
        i = i + 1;
    }
}

/* Get score of row by id */
int get_score(int id) {
    int idx;
    idx = find_by_id(id);
    if (idx < 0) return -1;
    return table[idx].score;
}

/* Get age of row by id */
int get_age(int id) {
    int idx;
    idx = find_by_id(id);
    if (idx < 0) return -1;
    return table[idx].age;
}

/* Print name chars of a row by id */
void print_name(int id) {
    int idx;
    idx = find_by_id(id);
    if (idx < 0) return;
    printf("%c", table[idx].name0);
    printf("%c", table[idx].name1);
    printf("%c", table[idx].name2);
    printf("%c", table[idx].name3);
}

/* Check if name matches */
int name_matches(int id, int n0, int n1, int n2, int n3) {
    int idx;
    idx = find_by_id(id);
    if (idx < 0) return 0;
    if (table[idx].name0 != n0) return 0;
    if (table[idx].name1 != n1) return 0;
    if (table[idx].name2 != n2) return 0;
    if (table[idx].name3 != n3) return 0;
    return 1;
}

/* Count rows matching a name */
int count_name(int n0, int n1, int n2, int n3) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < table_size) {
        if (table[i].active) {
            if (table[i].name0 == n0 && table[i].name1 == n1 &&
                table[i].name2 == n2 && table[i].name3 == n3) {
                cnt = cnt + 1;
            }
        }
        i = i + 1;
    }
    return cnt;
}

int main(void) {
    int id1;
    int id2;
    int id3;
    int id4;
    int id5;
    int id6;
    int id7;
    int id8;
    int cnt;
    int val;

    /* Initialize */
    init_db();
    printf("%c%c%c%c%c", 'i', 'n', 'i', 't', ':');
    printf("%d", count_active());

    /* Insert rows: name as 4 chars, age, score */
    /* 'A','l','e','x' = 65,108,101,120 */
    id1 = insert_row(65, 108, 101, 120, 25, 90);
    /* 'B','o','b',' ' = 66,111,98,32 */
    id2 = insert_row(66, 111, 98, 32, 30, 85);
    /* 'C','a','r','l' = 67,97,114,108 */
    id3 = insert_row(67, 97, 114, 108, 22, 95);
    /* 'D','a','n','a' = 68,97,110,97 */
    id4 = insert_row(68, 97, 110, 97, 35, 70);
    /* 'E','v','a',' ' = 69,118,97,32 */
    id5 = insert_row(69, 118, 97, 32, 28, 88);

    printf("%c%c%c%c%c%c%c%c%c", '|', 'i', 'n', 's', 'e', 'r', 't', 'e', 'd');
    printf("%c", ':');
    printf("%d", id1);
    printf("%c", ',');
    printf("%d", id2);
    printf("%c", ',');
    printf("%d", id3);
    printf("%c", ',');
    printf("%d", id4);
    printf("%c", ',');
    printf("%d", id5);

    /* Count */
    printf("%c%c%c%c%c%c%c%c", '|', 'a', 'c', 't', 'i', 'v', 'e', '=');
    printf("%d", count_active());

    /* Select age > 25 */
    cnt = select_age_gt(25);
    printf("%c%c%c%c%c%c%c%c%c", '|', 'a', 'g', 'e', '>', '2', '5', ':', ' ');
    printf("%d", cnt);
    printf("%c", '=');
    print_results();

    /* Select age < 30 */
    cnt = select_age_lt(30);
    printf("%c%c%c%c%c%c%c%c%c", '|', 'a', 'g', 'e', '<', '3', '0', ':', ' ');
    printf("%d", cnt);
    printf("%c", '=');
    print_results();

    /* Select score >= 90 */
    cnt = select_score_gte(90);
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 's', 'c', 'o', 'r', 'e', '>', '=', '9', '0', ':');
    printf("%d", cnt);
    printf("%c", '=');
    print_results();

    /* Sum of scores */
    printf("%c%c%c%c%c%c%c%c%c", '|', 's', 'u', 'm', 's', 'c', 'o', 'r', 'e');
    printf("%c", '=');
    printf("%d", sum_scores());

    /* Average age */
    printf("%c%c%c%c%c%c%c", '|', 'a', 'v', 'g', 'a', 'g', 'e');
    printf("%c", '=');
    printf("%d", avg_age());

    /* Max score */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'm', 'a', 'x', 's', 'c', 'o', 'r', 'e');
    printf("%c", '=');
    printf("%d", max_score());

    /* Min score */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'm', 'i', 'n', 's', 'c', 'o', 'r', 'e');
    printf("%c", '=');
    printf("%d", min_score());

    /* Find by id */
    printf("%c%c%c%c%c%c", '|', 'f', 'i', 'n', 'd', '3');
    printf("%c", '=');
    printf("%d", find_by_id(3));

    printf("%c%c%c%c%c%c%c", '|', 'f', 'i', 'n', 'd', '9', '9');
    printf("%c", '=');
    printf("%d", find_by_id(99));

    /* Get score of id 1 */
    printf("%c%c%c%c%c%c%c%c%c", '|', 's', 'c', 'o', 'r', 'e', '(', '1', ')');
    printf("%c", '=');
    printf("%d", get_score(1));

    /* Get age of id 2 */
    printf("%c%c%c%c%c%c%c%c", '|', 'a', 'g', 'e', '(', '2', ')', '=');
    printf("%d", get_age(2));

    /* Print name of id 1 */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'n', 'a', 'm', 'e', '(', '1', ')', '=');
    print_name(1);

    /* Print name of id 3 */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'n', 'a', 'm', 'e', '(', '3', ')', '=');
    print_name(3);

    /* Name match */
    printf("%c%c%c%c%c%c%c%c", '|', 'n', 'm', 'a', 't', 'c', 'h', '=');
    printf("%d", name_matches(1, 65, 108, 101, 120));

    printf("%c%c%c%c%c%c%c%c%c", '|', 'n', 'm', 'a', 't', 'c', 'h', '2', '=');
    printf("%d", name_matches(1, 66, 111, 98, 32));

    /* Delete id 4 */
    printf("%c%c%c%c%c", '|', 'd', 'e', 'l', '4');
    printf("%c", '=');
    printf("%d", delete_by_id(4));

    printf("%c%c%c%c%c%c%c%c%c%c", '|', 'a', 'f', 't', 'e', 'r', '_', 'd', 'e', 'l');
    printf("%c", ':');
    printf("%d", count_active());
    printf("%c", ',');
    printf("%d", count_deleted());

    /* Delete non-existent */
    printf("%c%c%c%c%c%c", '|', 'd', 'e', 'l', '9', '9');
    printf("%c", '=');
    printf("%d", delete_by_id(99));

    /* Select age > 25 after deletion */
    cnt = select_age_gt(25);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", '|', 'a', 'g', 'e', '>', '2', '5', '_', 'a', 'f', 't', 'e', 'r');
    printf("%c", ':');
    printf("%d", cnt);
    printf("%c", '=');
    print_results();

    /* Sum scores after deletion */
    printf("%c%c%c%c%c%c%c%c%c", '|', 's', 'u', 'm', 'a', 'f', 't', 'e', 'r');
    printf("%c", '=');
    printf("%d", sum_scores());

    /* Update score of id 1 to 100 */
    printf("%c%c%c%c%c%c%c", '|', 'u', 'p', 'd', 'a', 't', 'e');
    printf("%c", '=');
    printf("%d", update_score(1, 100));
    printf("%c%c%c%c%c%c%c%c%c%c%c", ',', 'n', 'e', 'w', 's', 'c', 'o', 'r', 'e', '1', '=');
    printf("%d", get_score(1));

    /* Insert more rows */
    /* 'F','r','e','d' = 70,114,101,100 */
    id6 = insert_row(70, 114, 101, 100, 40, 60);
    /* 'G','a','i','l' = 71,97,105,108 */
    id7 = insert_row(71, 97, 105, 108, 19, 92);
    /* 'H','a','l',' ' = 72,97,108,32 */
    id8 = insert_row(72, 97, 108, 32, 33, 75);

    printf("%c%c%c%c%c%c%c%c%c%c", '|', 'm', 'o', 'r', 'e', '_', 'i', 'n', 's', '=');
    printf("%d", id6);
    printf("%c", ',');
    printf("%d", id7);
    printf("%c", ',');
    printf("%d", id8);

    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 't', 'o', 't', 'a', 'l', '_', 'a', 'c', 't', '=');
    printf("%d", count_active());

    /* Select age between 20 and 30 */
    cnt = select_age_between(20, 30);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", '|', 'a', 'g', 'e', '_', '2', '0', '_', '3', '0', ':', ' ', ' ');
    /* actually just print cnt= and ids */
    printf("%d", cnt);
    printf("%c", '=');
    print_results();

    /* Select score == 92 */
    cnt = select_score_eq(92);
    printf("%c%c%c%c%c%c%c%c%c%c", '|', 's', 'c', 'o', 'r', 'e', '=', '9', '2', ':');
    printf("%d", cnt);
    printf("%c", '=');
    print_results();

    /* Sort results by score */
    select_score_gte(80);
    sort_results_by_score();
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", '|', 's', 'o', 'r', 't', 'e', 'd', '_', 'b', 'y', '_', 's', 'c', 'o', 'r');
    printf("%c", '=');
    print_results();

    /* Max score now */
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'n', 'e', 'w', '_', 'm', 'a', 'x', 's', 'c', '=');
    printf("%d", max_score());

    /* New avg age */
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'n', 'e', 'w', '_', 'a', 'v', 'g', 'a', 'g', '=');
    printf("%d", avg_age());

    /* Count name 'Bob ' */
    printf("%c%c%c%c%c%c%c%c", '|', 'c', 'n', 't', 'b', 'o', 'b', '=');
    printf("%d", count_name(66, 111, 98, 32));

    /* Delete all: delete ids 1,2,3,5,6,7,8 */
    delete_by_id(1);
    delete_by_id(2);
    delete_by_id(3);
    delete_by_id(5);
    delete_by_id(6);
    delete_by_id(7);
    delete_by_id(8);
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'a', 'f', 't', 'e', 'r', '_', 'a', 'l', 'l', '=');
    printf("%d", count_active());
    printf("%c", ',');
    printf("%d", count_deleted());

    /* Sum of empty table */
    printf("%c%c%c%c%c%c%c%c%c%c", '|', 'e', 'm', 'p', 't', 'y', '_', 's', 'u', 'm');
    printf("%c", '=');
    printf("%d", sum_scores());

    /* Avg of empty */
    printf("%c%c%c%c%c%c%c%c%c%c", '|', 'e', 'm', 'p', 't', 'y', '_', 'a', 'v', 'g');
    printf("%c", '=');
    printf("%d", avg_age());

    /* Re-insert and verify ids continue */
    val = insert_row(65, 65, 65, 65, 50, 50);
    printf("%c%c%c%c%c%c%c", '|', 'n', 'e', 'w', '_', 'i', 'd');
    printf("%c", '=');
    printf("%d", val);

    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", '|', 'f', 'i', 'n', 'a', 'l', '_', 'a', 'c', 't', 'i', 'v', 'e');
    printf("%c", '=');
    printf("%d", count_active());

    printf("%c%c%c%c", '|', 'e', 'n', 'd');

    printf("%c", '\n');

    return 0;
}
