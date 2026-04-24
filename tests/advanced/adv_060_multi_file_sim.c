
// EXPECT: === Multi-File Simulation ===
// EXPECT: --- Math Subsystem ---
// EXPECT: gcd(48,18)=6
// EXPECT: lcm(12,8)=24
// EXPECT: pow(2,10)=1024
// EXPECT: isqrt(144)=12
// EXPECT: isqrt(50)=7
// EXPECT: factorial(10)=3628800
// EXPECT: fib(10)=55
// EXPECT: is_prime(17)=yes
// EXPECT: is_prime(15)=no
// EXPECT: --- String Subsystem ---
// EXPECT: strlen("hello")=5
// EXPECT: strcmp("abc","abd")=not_equal
// EXPECT: strcmp("test","test")=equal
// EXPECT: reverse("abcdef")=fedcba
// EXPECT: to_upper("hello")=HELLO
// EXPECT: to_lower("WORLD")=world
// EXPECT: is_palindrome("racecar")=yes
// EXPECT: is_palindrome("hello")=no
// EXPECT: count_char("mississippi",'s')=4
// EXPECT: --- Data Subsystem ---
// EXPECT: stack: push 10,20,30
// EXPECT: stack: pop=30 pop=20 pop=10
// EXPECT: stack: empty=yes
// EXPECT: queue: enq 1,2,3,4,5
// EXPECT: queue: deq=1 deq=2 deq=3
// EXPECT: queue: size=2
// EXPECT: ring: write 10,20,30,40 (cap 4)
// EXPECT: ring: read=10 read=20
// EXPECT: ring: write 50,60
// EXPECT: ring: read=30 read=40 read=50 read=60
// EXPECT: --- Integration ---
// EXPECT: gcd_str: 6
// EXPECT: all_primes_to_20: 2 3 5 7 11 13 17 19
// EXPECT: sorted_string: "dlorw"
int printf(const char *fmt, ...);

// ============================================================
// Math Subsystem
// ============================================================

static int math_abs(int x) {
    return x < 0 ? -x : x;
}

static int math_gcd(int a, int b) {
    a = math_abs(a);
    b = math_abs(b);
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static int math_lcm(int a, int b) {
    int g = math_gcd(a, b);
    if (g == 0) return 0;
    return (a / g) * b;
}

static int math_pow(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exp = exp / 2;
    }
    return result;
}

static int math_isqrt(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

static int math_factorial(int n) {
    int result = 1;
    int i;
    for (i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

static int math_fib(int n) {
    if (n <= 1) return n;
    int a = 0;
    int b = 1;
    int i;
    for (i = 2; i <= n; i++) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

static int math_is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    int i;
    for (i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// ============================================================
// String Subsystem
// ============================================================

static int str_len(const char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

static int str_cmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) {
        a++;
        b++;
    }
    return *a - *b;
}

static void str_copy(char *dst, const char *src) {
    while (*src) {
        *dst++ = *src++;
    }
    *dst = 0;
}

static void str_reverse(char *s) {
    int len = str_len(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

static void str_to_upper(char *s) {
    while (*s) {
        if (*s >= 'a' && *s <= 'z') {
            *s = *s - 32;
        }
        s++;
    }
}

static void str_to_lower(char *s) {
    while (*s) {
        if (*s >= 'A' && *s <= 'Z') {
            *s = *s + 32;
        }
        s++;
    }
}

static int str_is_palindrome(const char *s) {
    int len = str_len(s);
    int i = 0;
    int j = len - 1;
    while (i < j) {
        if (s[i] != s[j]) return 0;
        i++;
        j--;
    }
    return 1;
}

static int str_count_char(const char *s, char c) {
    int count = 0;
    while (*s) {
        if (*s == c) count++;
        s++;
    }
    return count;
}

// Simple int-to-string for integration test
static void int_to_str(int n, char *buf) {
    if (n == 0) {
        buf[0] = '0';
        buf[1] = 0;
        return;
    }
    int neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    int len = 0;
    int tmp = n;
    while (tmp > 0) {
        len++;
        tmp /= 10;
    }
    if (neg) {
        buf[0] = '-';
        buf[len + 1] = 0;
        int i;
        for (i = len; i >= 1; i--) {
            buf[i] = '0' + (n % 10);
            n /= 10;
        }
    } else {
        buf[len] = 0;
        int i;
        for (i = len - 1; i >= 0; i--) {
            buf[i] = '0' + (n % 10);
            n /= 10;
        }
    }
}

// ============================================================
// Data Subsystem - Stack
// ============================================================

struct Stack {
    int data[32];
    int top;
};

static void stack_init(struct Stack *s) {
    s->top = 0;
}

static int stack_empty(struct Stack *s) {
    return s->top == 0;
}

static void stack_push(struct Stack *s, int val) {
    s->data[s->top++] = val;
}

static int stack_pop(struct Stack *s) {
    return s->data[--s->top];
}

// ============================================================
// Data Subsystem - Queue
// ============================================================

struct Queue {
    int data[32];
    int head;
    int tail;
    int count;
};

static void queue_init(struct Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

static void queue_enqueue(struct Queue *q, int val) {
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % 32;
    q->count++;
}

static int queue_dequeue(struct Queue *q) {
    int val = q->data[q->head];
    q->head = (q->head + 1) % 32;
    q->count--;
    return val;
}

static int queue_size(struct Queue *q) {
    return q->count;
}

// ============================================================
// Data Subsystem - Ring Buffer
// ============================================================

struct Ring {
    int data[16];
    int capacity;
    int head;
    int tail;
    int count;
};

static void ring_init(struct Ring *r, int cap) {
    r->capacity = cap;
    r->head = 0;
    r->tail = 0;
    r->count = 0;
}

static int ring_write(struct Ring *r, int val) {
    if (r->count >= r->capacity) return 0;
    r->data[r->tail] = val;
    r->tail = (r->tail + 1) % r->capacity;
    r->count++;
    return 1;
}

static int ring_read(struct Ring *r, int *val) {
    if (r->count == 0) return 0;
    *val = r->data[r->head];
    r->head = (r->head + 1) % r->capacity;
    r->count--;
    return 1;
}

// ============================================================
// Bubble sort for chars (used in integration)
// ============================================================

static void sort_chars(char *s) {
    int len = str_len(s);
    int i;
    int j;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (s[j] > s[j + 1]) {
                char tmp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = tmp;
            }
        }
    }
}

// ============================================================
// Main - exercises all subsystems
// ============================================================

int main(void) {
    printf("=== Multi-File Simulation ===\n");

    // --- Math ---
    printf("--- Math Subsystem ---\n");
    printf("gcd(48,18)=%d\n", math_gcd(48, 18));
    printf("lcm(12,8)=%d\n", math_lcm(12, 8));
    printf("pow(2,10)=%d\n", math_pow(2, 10));
    printf("isqrt(144)=%d\n", math_isqrt(144));
    printf("isqrt(50)=%d\n", math_isqrt(50));
    printf("factorial(10)=%d\n", math_factorial(10));
    printf("fib(10)=%d\n", math_fib(10));
    printf("is_prime(17)=%s\n", math_is_prime(17) ? "yes" : "no");
    printf("is_prime(15)=%s\n", math_is_prime(15) ? "yes" : "no");

    // --- String ---
    printf("--- String Subsystem ---\n");
    printf("strlen(\"hello\")=%d\n", str_len("hello"));

    printf("strcmp(\"abc\",\"abd\")=%s\n",
           str_cmp("abc", "abd") == 0 ? "equal" : "not_equal");
    printf("strcmp(\"test\",\"test\")=%s\n",
           str_cmp("test", "test") == 0 ? "equal" : "not_equal");

    {
        char buf[20];
        str_copy(buf, "abcdef");
        str_reverse(buf);
        printf("reverse(\"abcdef\")=%s\n", buf);
    }

    {
        char buf[20];
        str_copy(buf, "hello");
        str_to_upper(buf);
        printf("to_upper(\"hello\")=%s\n", buf);
    }

    {
        char buf[20];
        str_copy(buf, "WORLD");
        str_to_lower(buf);
        printf("to_lower(\"WORLD\")=%s\n", buf);
    }

    printf("is_palindrome(\"racecar\")=%s\n",
           str_is_palindrome("racecar") ? "yes" : "no");
    printf("is_palindrome(\"hello\")=%s\n",
           str_is_palindrome("hello") ? "yes" : "no");

    printf("count_char(\"mississippi\",'s')=%d\n",
           str_count_char("mississippi", 's'));

    // --- Data ---
    printf("--- Data Subsystem ---\n");

    // Stack
    {
        struct Stack s;
        stack_init(&s);
        stack_push(&s, 10);
        stack_push(&s, 20);
        stack_push(&s, 30);
        printf("stack: push 10,20,30\n");
        printf("stack: pop=%d pop=%d pop=%d\n",
               stack_pop(&s), stack_pop(&s), stack_pop(&s));
        printf("stack: empty=%s\n", stack_empty(&s) ? "yes" : "no");
    }

    // Queue
    {
        struct Queue q;
        queue_init(&q);
        queue_enqueue(&q, 1);
        queue_enqueue(&q, 2);
        queue_enqueue(&q, 3);
        queue_enqueue(&q, 4);
        queue_enqueue(&q, 5);
        printf("queue: enq 1,2,3,4,5\n");
        printf("queue: deq=%d deq=%d deq=%d\n",
               queue_dequeue(&q), queue_dequeue(&q), queue_dequeue(&q));
        printf("queue: size=%d\n", queue_size(&q));
    }

    // Ring buffer
    {
        struct Ring r;
        ring_init(&r, 4);
        ring_write(&r, 10);
        ring_write(&r, 20);
        ring_write(&r, 30);
        ring_write(&r, 40);
        printf("ring: write 10,20,30,40 (cap 4)\n");

        int v;
        ring_read(&r, &v);
        printf("ring: read=%d", v);
        ring_read(&r, &v);
        printf(" read=%d\n", v);

        ring_write(&r, 50);
        ring_write(&r, 60);
        printf("ring: write 50,60\n");

        printf("ring:");
        while (r.count > 0) {
            ring_read(&r, &v);
            printf(" read=%d", v);
        }
        printf("\n");
    }

    // --- Integration: combine subsystems ---
    printf("--- Integration ---\n");

    // Use math + string: compute GCD and convert to string
    {
        int g = math_gcd(48, 18);
        char buf[20];
        int_to_str(g, buf);
        printf("gcd_str: %s\n", buf);
    }

    // All primes up to 20 using math subsystem
    {
        printf("all_primes_to_20:");
        int i;
        for (i = 2; i <= 20; i++) {
            if (math_is_prime(i)) {
                printf(" %d", i);
            }
        }
        printf("\n");
    }

    // Sort characters of a string
    {
        char word[10];
        str_copy(word, "world");
        sort_chars(word);
        printf("sorted_string: \"%s\"\n", word);
    }

    return 0;
}
