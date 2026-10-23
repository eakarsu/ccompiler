#!/bin/bash
# Integration test runner for mycc
# Usage: ./run_tests.sh <path-to-mycc>
# Tests run in parallel (JOBS env, default: CPU count); expectations unchanged.

set -e

MYCC="${1:-./build/debug/mycc}"
TESTDIR="$(cd "$(dirname "$0")" && pwd)"
TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT
RESULTS="$TMPDIR/results"
mkdir -p "$RESULTS"
export MYCC TMPDIR RESULTS

run_test() {
    local name="$1" src="$2" expected="$3"
    local work="$TMPDIR/work-$name"
    mkdir -p "$work"
    if ! timeout 30 "$MYCC" "$src" -o "$work/a.out" 2>"$work/err.txt"; then
        echo "  FAIL  $name (compilation failed)" > "$RESULTS/$name.txt"
        return
    fi
    set +e
    actual=$(timeout 10 "$work/a.out" 2>&1)
    run_exit=$?
    set -e
    if [ $run_exit -eq 124 ]; then
        echo "  FAIL  $name (timeout - infinite loop?)" > "$RESULTS/$name.txt"
        return
    fi
    if [ "$actual" = "$expected" ]; then
        echo "  PASS  $name" > "$RESULTS/$name.txt"
    else
        { echo "  FAIL  $name"; echo "    expected: '$expected'"; echo "    actual:   '$actual'"; } > "$RESULTS/$name.txt"
    fi
}

run_exit_test() {
    local name="$1" src="$2" expected_exit="$3"
    local work="$TMPDIR/work-$name"
    mkdir -p "$work"
    if ! timeout 30 "$MYCC" "$src" -o "$work/a.out" 2>"$work/err.txt"; then
        echo "  FAIL  $name (compilation failed)" > "$RESULTS/$name.txt"
        return
    fi
    set +e
    timeout 10 "$work/a.out" > /dev/null 2>&1
    actual_exit=$?
    set -e
    if [ "$actual_exit" -eq "$expected_exit" ]; then
        echo "  PASS  $name" > "$RESULTS/$name.txt"
    else
        { echo "  FAIL  $name"; echo "    expected exit: $expected_exit"; echo "    actual exit:   $actual_exit"; } > "$RESULTS/$name.txt"
    fi
}

process_file() {
    local test_file="$1"
    [ -f "$test_file" ] || return 0
    local name
    name=$(basename "$test_file" .c)
    local first_expect="" expected="" exit_code="" in_expect=0 line
    while IFS= read -r line; do
        if [ -z "$exit_code" ] && [[ "$line" =~ ^//\ EXIT:\ ?(.*) ]]; then
            exit_code="${BASH_REMATCH[1]}"
        fi
        if [[ "$line" =~ ^//\ EXPECT:\ ?(.*) ]]; then
            in_expect=1
            if [ -z "$first_expect" ] && [ -z "$expected" ]; then first_expect="${BASH_REMATCH[1]}"; expected="$first_expect"; else expected="$expected
${BASH_REMATCH[1]}"; fi
        elif [ $in_expect -eq 1 ] && [[ "$line" =~ ^//\ (.*) ]]; then
            expected="$expected
${BASH_REMATCH[1]}"
        elif [ $in_expect -eq 1 ]; then
            in_expect=2
        fi
    done < "$test_file"
    if [ -n "$first_expect" ]; then
        case "$first_expect" in
            *\\n*) expected=$(printf '%b' "$first_expect") ;;
        esac
    fi
    if [ -n "$exit_code" ]; then
        run_exit_test "$name" "$test_file" "$exit_code"
    elif [ -n "$expected" ]; then
        run_test "$name" "$test_file" "$expected"
    else
        run_exit_test "$name" "$test_file" 0
    fi
}
export -f run_test run_exit_test process_file

echo "=== Integration Tests ==="

if [ -d "$TESTDIR/integration" ]; then
    JOBS="${JOBS:-$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)}"
    printf '%s\0' "$TESTDIR"/integration/*.c | xargs -0 -P "$JOBS" -n 1 bash -c 'process_file "$1"' _
fi

PASS=0; FAIL=0; SKIP=0
for f in "$RESULTS"/*.txt; do
    [ -f "$f" ] || continue
    cat "$f"
    if grep -q '^  PASS' "$f"; then PASS=$((PASS+1)); else FAIL=$((FAIL+1)); fi
done

echo "=== $PASS passed, $FAIL failed, $SKIP skipped ==="
test $FAIL -eq 0
