#!/bin/bash
# Run stress test suite only
# Usage: bash tests/run_stress.sh build/debug/mycc

CC="${1:-build/debug/mycc}"
TESTDIR="tests/stress"
PASS=0
FAIL=0
SKIP=0
COMPILE_FAIL=0
LINK_FAIL=0
CRASH=0
WRONG=0

if [ ! -x "$CC" ]; then
    echo "Error: compiler '$CC' not found or not executable"
    exit 1
fi

for src in "$TESTDIR"/stress_*.c; do
    name=$(basename "$src" .c)
    asm="/tmp/${name}.s"
    bin="/tmp/${name}"

    # Extract expected output (strip leading whitespace before // EXPECT:)
    expected_normalized=$(grep '// EXPECT:' "$src" | sed 's|^[[:space:]]*// EXPECT: ||' | while IFS= read -r line; do
        printf '%s\n' "$line"
    done | sed 's/\\n/\n/g' | sed '/^$/d')

    if [ -z "$expected_normalized" ]; then
        printf "  SKIP  %s (no EXPECT)\n" "$name"
        SKIP=$((SKIP + 1))
        continue
    fi

    # Compile
    if ! "$CC" -S -o "$asm" "$src" 2>/tmp/${name}_err.txt; then
        printf "  \033[1;31mFAIL\033[0m  %s (compile error)\n" "$name"
        head -2 /tmp/${name}_err.txt | sed 's/^/        /'
        FAIL=$((FAIL + 1))
        COMPILE_FAIL=$((COMPILE_FAIL + 1))
        continue
    fi

    # Assemble and link
    if ! cc "$asm" -o "$bin" 2>/tmp/${name}_err.txt; then
        printf "  \033[1;31mFAIL\033[0m  %s (link error)\n" "$name"
        head -2 /tmp/${name}_err.txt | sed 's/^/        /'
        FAIL=$((FAIL + 1))
        LINK_FAIL=$((LINK_FAIL + 1))
        continue
    fi

    # Run with timeout
    actual=$(timeout 10 "$bin" 2>&1)
    exitcode=$?
    if [ $exitcode -eq 124 ]; then
        printf "  \033[1;31mFAIL\033[0m  %s (timeout - infinite loop?)\n" "$name"
        FAIL=$((FAIL + 1))
        CRASH=$((CRASH + 1))
        continue
    fi
    if [ $exitcode -gt 128 ]; then
        sig=$((exitcode - 128))
        printf "  \033[1;31mFAIL\033[0m  %s (signal %d - crash)\n" "$name" "$sig"
        FAIL=$((FAIL + 1))
        CRASH=$((CRASH + 1))
        continue
    fi

    # Compare output
    actual_normalized=$(printf '%s' "$actual" | sed '/^$/d')

    if [ "$actual_normalized" = "$expected_normalized" ]; then
        printf "  \033[1;32mPASS\033[0m  %s\n" "$name"
        PASS=$((PASS + 1))
    else
        printf "  \033[1;31mFAIL\033[0m  %s (wrong output)\n" "$name"
        printf "    expected: %s\n" "$(echo "$expected_normalized" | head -2)"
        printf "    actual:   %s\n" "$(echo "$actual_normalized" | head -2)"
        FAIL=$((FAIL + 1))
        WRONG=$((WRONG + 1))
    fi
done

echo ""
echo "=== Results: $PASS passed, $FAIL failed, $SKIP skipped ==="
if [ $FAIL -gt 0 ]; then
    echo "    Breakdown: $COMPILE_FAIL compile errors, $LINK_FAIL link errors, $CRASH crashes/timeouts, $WRONG wrong output"
fi
[ $FAIL -eq 0 ]
