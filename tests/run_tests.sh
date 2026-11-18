#!/bin/bash
# run_tests.sh — runs all .m tests, compares output, reports pass/fail
MEANS="${MEANS:-../means}"
PASS=0
FAIL=0
for t in tests/*.m; do
    [ -f "$t" ] || continue
    expected=$(grep "^#" "$t" | sed 's/^#//')
    actual=$("$MEANS" -f "$t" 2>&1)
    if [ "$expected" == "$actual" ]; then
        printf "PASS  %s\n" "$(basename "$t")"
        PASS=$((PASS+1))
    else
        printf "FAIL  %s\n" "$(basename "$t")"
        echo "  --- expected ---"
        echo "$expected" | sed 's/^/  /'
        echo "  --- actual ---"
        echo "$actual" | sed 's/^/  /'
        FAIL=$((FAIL+1))
    fi
done
echo "─────────────────────────"
echo "  $PASS passed, $FAIL failed"
[ "$FAIL" -eq 0 ]
