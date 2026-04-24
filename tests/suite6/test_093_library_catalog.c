int printf(const char *fmt, ...);

struct Book {
    char title[24];
    char author[16];
    int year;
    int checked_out;
    int times_borrowed;
};

int str_copy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
    return i;
}

int str_eq(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int str_contains(char *haystack, char *needle) {
    int i;
    int j;
    int match;
    i = 0;
    while (haystack[i] != 0) {
        j = 0;
        match = 1;
        while (needle[j] != 0) {
            if (haystack[i + j] == 0 || haystack[i + j] != needle[j]) {
                match = 0;
                break;
            }
            j = j + 1;
        }
        if (match) return 1;
        i = i + 1;
    }
    return 0;
}

int add_book(struct Book *catalog, int *count, char *title, char *author, int year) {
    int idx;
    idx = *count;
    if (idx >= 20) return -1;
    str_copy(catalog[idx].title, title);
    str_copy(catalog[idx].author, author);
    catalog[idx].year = year;
    catalog[idx].checked_out = 0;
    catalog[idx].times_borrowed = 0;
    *count = *count + 1;
    return idx;
}

int find_by_title(struct Book *catalog, int count, char *title) {
    int i;
    i = 0;
    while (i < count) {
        if (str_eq(catalog[i].title, title)) return i;
        i = i + 1;
    }
    return -1;
}

int checkout_book(struct Book *catalog, int count, char *title) {
    int idx;
    idx = find_by_title(catalog, count, title);
    if (idx < 0) return -1;
    if (catalog[idx].checked_out) return -2;
    catalog[idx].checked_out = 1;
    catalog[idx].times_borrowed = catalog[idx].times_borrowed + 1;
    return 0;
}

int return_book(struct Book *catalog, int count, char *title) {
    int idx;
    idx = find_by_title(catalog, count, title);
    if (idx < 0) return -1;
    if (catalog[idx].checked_out == 0) return -2;
    catalog[idx].checked_out = 0;
    return 0;
}

int count_available(struct Book *catalog, int count) {
    int i;
    int c;
    c = 0;
    i = 0;
    while (i < count) {
        if (catalog[i].checked_out == 0) c = c + 1;
        i = i + 1;
    }
    return c;
}

int count_checked_out(struct Book *catalog, int count) {
    return count - count_available(catalog, count);
}

int search_by_author(struct Book *catalog, int count, char *author, int *results, int max_results) {
    int i;
    int r;
    r = 0;
    i = 0;
    while (i < count && r < max_results) {
        if (str_eq(catalog[i].author, author)) {
            results[r] = i;
            r = r + 1;
        }
        i = i + 1;
    }
    return r;
}

int search_by_keyword(struct Book *catalog, int count, char *keyword, int *results, int max_results) {
    int i;
    int r;
    r = 0;
    i = 0;
    while (i < count && r < max_results) {
        if (str_contains(catalog[i].title, keyword)) {
            results[r] = i;
            r = r + 1;
        }
        i = i + 1;
    }
    return r;
}

int most_popular(struct Book *catalog, int count) {
    int i;
    int best;
    int best_count;
    best = 0;
    best_count = catalog[0].times_borrowed;
    i = 1;
    while (i < count) {
        if (catalog[i].times_borrowed > best_count) {
            best_count = catalog[i].times_borrowed;
            best = i;
        }
        i = i + 1;
    }
    return best;
}

int books_in_year_range(struct Book *catalog, int count, int from, int to) {
    int i;
    int c;
    c = 0;
    i = 0;
    while (i < count) {
        if (catalog[i].year >= from && catalog[i].year <= to) c = c + 1;
        i = i + 1;
    }
    return c;
}

int main(void) {
    struct Book catalog[20];
    int count;
    int result;
    int results[10];
    int num_results;

    count = 0;

    add_book(catalog, &count, "The C Book", "Kernighan", 1978);
    add_book(catalog, &count, "Clean Code", "Martin", 2008);
    add_book(catalog, &count, "The C Answer", "Kernighan", 1988);
    add_book(catalog, &count, "Design Patterns", "Gamma", 1994);
    add_book(catalog, &count, "Code Complete", "McConnell", 2004);

    // EXPECT: 5
    printf("%d\n", count);

    // EXPECT: 5
    printf("%d\n", count_available(catalog, count));

    // EXPECT: 0
    printf("%d\n", count_checked_out(catalog, count));

    /* Checkout "Clean Code" */
    result = checkout_book(catalog, count, "Clean Code");
    // EXPECT: 0
    printf("%d\n", result);

    // EXPECT: 4
    printf("%d\n", count_available(catalog, count));

    // EXPECT: 1
    printf("%d\n", count_checked_out(catalog, count));

    /* Double checkout fails */
    result = checkout_book(catalog, count, "Clean Code");
    // EXPECT: -2
    printf("%d\n", result);

    /* Checkout non-existent */
    result = checkout_book(catalog, count, "Unknown");
    // EXPECT: -1
    printf("%d\n", result);

    /* Return "Clean Code" */
    result = return_book(catalog, count, "Clean Code");
    // EXPECT: 0
    printf("%d\n", result);

    /* Double return fails */
    result = return_book(catalog, count, "Clean Code");
    // EXPECT: -2
    printf("%d\n", result);

    /* Search by author "Kernighan" -> 2 books */
    num_results = search_by_author(catalog, count, "Kernighan", results, 10);
    // EXPECT: 2
    printf("%d\n", num_results);
    // EXPECT: 0
    printf("%d\n", results[0]);
    // EXPECT: 2
    printf("%d\n", results[1]);

    /* Keyword search "Code" -> "Clean Code" and "Code Complete" */
    num_results = search_by_keyword(catalog, count, "Code", results, 10);
    // EXPECT: 2
    printf("%d\n", num_results);

    /* Checkout same book multiple times to test popularity */
    checkout_book(catalog, count, "The C Book");
    return_book(catalog, count, "The C Book");
    checkout_book(catalog, count, "The C Book");
    return_book(catalog, count, "The C Book");
    checkout_book(catalog, count, "The C Book");
    return_book(catalog, count, "The C Book");
    /* "The C Book" borrowed 3 times, "Clean Code" 1 time */

    result = most_popular(catalog, count);
    // EXPECT: 0
    printf("%d\n", result);
    // EXPECT: 3
    printf("%d\n", catalog[result].times_borrowed);

    /* Books in year range 1990-2010 */
    // EXPECT: 3
    printf("%d\n", books_in_year_range(catalog, count, 1990, 2010));

    /* Books in year range 1970-1980 */
    // EXPECT: 1
    printf("%d\n", books_in_year_range(catalog, count, 1970, 1980));

    /* Find by title */
    result = find_by_title(catalog, count, "Design Patterns");
    // EXPECT: 3
    printf("%d\n", result);
    // EXPECT: 1994
    printf("%d\n", catalog[result].year);

    return 0;
}
