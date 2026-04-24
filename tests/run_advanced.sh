#!/bin/bash
# Run advanced test suite only
# Usage: bash tests/run_advanced.sh build/debug/mycc

CC="${1:-build/debug/mycc}"
TESTDIR="tests/advanced"
PASS=0
FAIL=0
SKIP=0

if [ ! -x "$CC" ]; then
    echo "Error: compiler '$CC' not found or not executable"
    exit 1
fi

for src in "$TESTDIR"/adv_*.c; do
    name=$(basename "$src" .c)
    asm="/tmp/${name}.s"
    bin="/tmp/${name}"

    # Extract expected output
    expected=$(grep '// EXPECT:' "$src" | sed 's|// EXPECT: ||' | tr '\n' '\n')
    if [ -z "$expected" ]; then
        printf "  SKIP  %s (no EXPECT)\n" "$name"
        SKIP=$((SKIP + 1))
        continue
    fi

    # Compile
    if ! "$CC" -S -o "$asm" "$src" 2>/tmp/${name}_err.txt; then
        printf "  FAIL  %s (compile error)\n" "$name"
        head -3 /tmp/${name}_err.txt | sed 's/^/        /'
        FAIL=$((FAIL + 1))
        continue
    fi

    # Assemble and link
    if ! cc "$asm" -o "$bin" 2>/tmp/${name}_err.txt; then
        printf "  FAIL  %s (link error)\n" "$name"
        head -3 /tmp/${name}_err.txt | sed 's/^/        /'
        FAIL=$((FAIL + 1))
        continue
    fi

    # Run with timeout
    actual=$(timeout 10 "$bin" 2>&1)
    exitcode=$?
    if [ $exitcode -eq 124 ]; then
        printf "  FAIL  %s (timeout - infinite loop?)\n" "$name"
        FAIL=$((FAIL + 1))
        continue
    fi
    if [ $exitcode -ne 0 ] && [ $exitcode -ne 137 ]; then
        # Allow non-zero exit for signal kills but check for crashes
        if [ $exitcode -gt 128 ]; then
            sig=$((exitcode - 128))
            printf "  FAIL  %s (signal %d)\n" "$name" "$sig"
            FAIL=$((FAIL + 1))
            continue
        fi
    fi

    # Compare output
    # Normalize: convert EXPECT lines (separated by \n in EXPECT comment) to actual newlines
    expected_normalized=$(grep '// EXPECT:' "$src" | sed 's|// EXPECT: ||' | while IFS= read -r line; do
        printf '%s\n' "$line"
    done | sed 's/\\n/\n/g' | sed '/^$/d')

    actual_normalized=$(printf '%s' "$actual" | sed '/^$/d')

    if [ "$actual_normalized" = "$expected_normalized" ]; then
        printf "  PASS  %s\n" "$name"
        PASS=$((PASS + 1))
    else
        printf "  FAIL  %s\n" "$name"
        printf "    expected: %s\n" "$(echo "$expected_normalized" | head -3)"
        printf "    actual:   %s\n" "$(echo "$actual_normalized" | head -3)"
        FAIL=$((FAIL + 1))
    fi
done

echo "=== $PASS passed, $FAIL failed, $SKIP skipped ==="
[ $FAIL -eq 0 ]
