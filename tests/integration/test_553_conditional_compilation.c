int printf(const char *fmt, ...);
// EXPECT: Version 2.5\nLOG: Application started\nDBG: Debug mode active\nLOG: Items initialized\nCount: 10\nTotal: 550\nAverage: 55\nDBG: Processing complete\nMin: 10\nMax: 100\nRange: 90\nExtended features enabled\nEvenSum: 550\nOddSum: 0\nLOG: Application finished
#define FEATURE_LOGGING 1
#define FEATURE_STATS 1
#define DEBUG_LEVEL 2
#define VERSION_MAJOR 2
#define VERSION_MINOR 5
#define MAX_ITEMS 10

#if FEATURE_LOGGING
#define LOG(msg) printf("LOG: %s\n", msg)
#else
#define LOG(msg)
#endif

#if DEBUG_LEVEL >= 2
#define DBG(msg) printf("DBG: %s\n", msg)
#else
#define DBG(msg)
#endif

int main(void) {
    printf("Version %d.%d\n", VERSION_MAJOR, VERSION_MINOR);

    LOG("Application started");
    DBG("Debug mode active");

    int items[MAX_ITEMS];
    int count = 0;
    int i;

    for (i = 0; i < MAX_ITEMS; i++) {
        items[i] = (i + 1) * 10;
        count++;
    }
    LOG("Items initialized");

    int total = 0;
    for (i = 0; i < count; i++) {
        total = total + items[i];
    }

#if FEATURE_STATS
    printf("Count: %d\n", count);
    printf("Total: %d\n", total);
    printf("Average: %d\n", total / count);
#endif

    DBG("Processing complete");

    int min_val = items[0];
    int max_val = items[0];
    for (i = 1; i < count; i++) {
        if (items[i] < min_val) min_val = items[i];
        if (items[i] > max_val) max_val = items[i];
    }

#if FEATURE_STATS
    printf("Min: %d\n", min_val);
    printf("Max: %d\n", max_val);
    printf("Range: %d\n", max_val - min_val);
#endif

#if VERSION_MAJOR >= 2
    printf("Extended features enabled\n");
    int even_sum = 0;
    int odd_sum = 0;
    for (i = 0; i < count; i++) {
        if (items[i] % 2 == 0) {
            even_sum = even_sum + items[i];
        } else {
            odd_sum = odd_sum + items[i];
        }
    }
    printf("EvenSum: %d\n", even_sum);
    printf("OddSum: %d\n", odd_sum);
#endif

    LOG("Application finished");
    return 0;
}
