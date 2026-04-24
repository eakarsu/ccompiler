#!/bin/bash
MYCC="${1:-build/debug/mycc}"
PASS=0; FAIL=0; SKIP=0
for src in tests/suite16/test_*.c; do
    name=$(basename "$src" .c)
    asm="/tmp/${name}_s16.s"
    bin="/tmp/${name}_s16"
    expected=$(grep '// EXPECT:' "$src" | sed 's/.*\/\/ EXPECT: //')
    if [ -z "$expected" ]; then SKIP=$((SKIP+1)); echo "  SKIP  $name (no EXPECT)"; continue; fi
    "$MYCC" -S -o "$asm" "$src" 2>/tmp/${name}_s16_err.txt
    if [ $? -ne 0 ]; then FAIL=$((FAIL+1)); echo -e "  \033[1;31mFAIL\033[0m  $name (compile error)"; continue; fi
    cc -o "$bin" "$asm" -lc 2>/tmp/${name}_s16_link_err.txt
    if [ $? -ne 0 ]; then FAIL=$((FAIL+1)); echo -e "  \033[1;31mFAIL\033[0m  $name (link error)"; continue; fi
    actual=$("$bin" 2>&1)
    if [ "$actual" = "$expected" ]; then
        PASS=$((PASS+1)); echo -e "  \033[1;32mPASS\033[0m  $name"
    else
        FAIL=$((FAIL+1)); echo -e "  \033[1;31mFAIL\033[0m  $name"
        diff <(echo "$expected") <(echo "$actual") | head -20
    fi
done
echo ""; echo "=== Suite16 Results: $PASS passed, $FAIL failed, $SKIP skipped ==="
