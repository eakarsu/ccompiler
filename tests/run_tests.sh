#!/bin/bash
# Integration test runner for mycc
# Usage: ./run_tests.sh <path-to-mycc>

set -e

MYCC="${1:-./build/debug/mycc}"
TESTDIR="$(cd "$(dirname "$0")" && pwd)"
TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

PASS=0
FAIL=0
SKIP=0

run_test() {
    local name="$1"
    local src="$2"
    local expected="$3"

    # Compile with mycc (30s timeout)
    if ! timeout 30 "$MYCC" "$src" -o "$TMPDIR/a.out" 2>"$TMPDIR/err.txt"; then
        echo "  FAIL  $name (compilation failed)"
        FAIL=$((FAIL + 1))
        return
    fi

    # Run (with 10s timeout to catch infinite loops)
    set +e
    actual=$(timeout 10 "$TMPDIR/a.out" 2>&1)
    run_exit=$?
    set -e
    if [ $run_exit -eq 124 ]; then
        echo "  FAIL  $name (timeout - infinite loop?)"
        FAIL=$((FAIL + 1))
        return
    fi
    if [ "$actual" = "$expected" ]; then
        echo "  PASS  $name"
        PASS=$((PASS + 1))
    else
        echo "  FAIL  $name"
        echo "    expected: '$expected'"
        echo "    actual:   '$actual'"
        FAIL=$((FAIL + 1))
    fi
}

run_exit_test() {
    local name="$1"
    local src="$2"
    local expected_exit="$3"

    if ! timeout 30 "$MYCC" "$src" -o "$TMPDIR/a.out" 2>"$TMPDIR/err.txt"; then
        echo "  FAIL  $name (compilation failed)"
        FAIL=$((FAIL + 1))
        return
    fi

    set +e
    timeout 10 "$TMPDIR/a.out" > /dev/null 2>&1
    actual_exit=$?
    set -e

    if [ "$actual_exit" -eq "$expected_exit" ]; then
        echo "  PASS  $name"
        PASS=$((PASS + 1))
    else
        echo "  FAIL  $name"
        echo "    expected exit: $expected_exit"
        echo "    actual exit:   $actual_exit"
        FAIL=$((FAIL + 1))
    fi
}

echo "=== Integration Tests ==="

# Run tests from integration directory
if [ -d "$TESTDIR/integration" ]; then
    for test_file in "$TESTDIR"/integration/*.c; do
        [ -f "$test_file" ] || continue
        name=$(basename "$test_file" .c)

        # Extract expected output from // EXPECT: comments
        # Two formats supported:
        #   Single-line: // EXPECT: line1\nline2\nline3  (literal \n)
        #   Multi-line:  // EXPECT: first line
        #                // second line
        #                // third line
        first_expect=$(grep '^// EXPECT:' "$test_file" | head -1 | sed 's|^// EXPECT: *||')
        expected=""

        if [ -n "$first_expect" ]; then
            # Check if single-line format (contains literal \n)
            if echo "$first_expect" | grep -q '\\n'; then
                # Single-line format: decode \n to real newlines
                expected=$(printf '%b' "$first_expect")
            else
                # Multi-line format: first EXPECT line + continuation // lines
                in_expect=0
                while IFS= read -r line; do
                    if [[ "$line" =~ ^//\ EXPECT:\ ?(.*) ]]; then
                        in_expect=1
                        if [ -z "$expected" ]; then
                            expected="${BASH_REMATCH[1]}"
                        else
                            expected="$expected
${BASH_REMATCH[1]}"
                        fi
                    elif [ $in_expect -eq 1 ] && [[ "$line" =~ ^//\ (.*) ]]; then
                        expected="$expected
${BASH_REMATCH[1]}"
                    elif [ $in_expect -eq 1 ]; then
                        break
                    fi
                done < "$test_file"
            fi
        fi

        # Read expected exit code: // EXIT: <code>
        exit_code=$(grep '^// EXIT:' "$test_file" | head -1 | sed 's|^// EXIT: *||')

        if [ -n "$exit_code" ]; then
            run_exit_test "$name" "$test_file" "$exit_code"
        elif [ -n "$expected" ]; then
            run_test "$name" "$test_file" "$expected"
        else
            # Default: expect exit code 0
            run_exit_test "$name" "$test_file" 0
        fi
    done
fi

echo "=== $PASS passed, $FAIL failed, $SKIP skipped ==="
test $FAIL -eq 0
