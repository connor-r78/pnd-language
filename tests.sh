#!/bin/bash

# Build the project
make

# Test cases
read -r -d '' test_cases << EOM
(
)
hello
123
"hello world"
(+ 1 2)
(define x 10)
("hello" "world")
 ( ) 
	(
)
"unterminated
12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
a_symbol_that_is_way_too_long_to_fit_in_the_buffer_and_should_cause_an_error_or_at_least_be_truncated_properly_and_not_cause_a_crash_or_buffer_overflow_or_anything_like_that
?_.

 
+
*
/
-
(define (x 3)) (func f(a b) (+ x (* a 2) (- 10 (g b))))
EOM

# Run tests
i=1
while IFS= read -r test_case; do
  echo "Running test $i: $test_case"
  output=$(./slang "$test_case")
  echo "Output:"
  echo "$output"
  echo "------------------------"
  i=$((i+1))
done <<< "$test_cases"
