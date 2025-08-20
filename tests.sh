#!/bin/bash

# Build the project
make

lex_test_cases=(
  "("
  ")"
  "()"
  "hello"
  "123"
  '"hello world"'
  "(+ 1 2)"
  "(define x 10)"
  '("hello" "world")'
  " ( ) "
  "	("
)

# Expected outputs for each test case.
# Please fill these in with the correct expected output for each test case.
# For multi-line output, use newline characters (

lex_expected_outputs=(
  'TOKEN_LPAREN ( '
  'TOKEN_RPAREN ) '
  'TOKEN_LPAREN ( TOKEN_RPAREN ) '
  'TOKEN_SYMBOL hello '
  'TOKEN_NUMBER 123 '
  "TOKEN_STRING hello world "
  'TOKEN_LPAREN ( TOKEN_SYMBOL + TOKEN_NUMBER 1 TOKEN_NUMBER 2 TOKEN_RPAREN ) '
  'TOKEN_LPAREN ( TOKEN_SYMBOL define TOKEN_SYMBOL x TOKEN_NUMBER 10 TOKEN_RPAREN ) '
  'TOKEN_LPAREN ( TOKEN_STRING hello TOKEN_STRING world TOKEN_RPAREN ) '
  'TOKEN_LPAREN ( TOKEN_RPAREN ) '
  'TOKEN_LPAREN ( '

)

# Run tests
i=0
total_tests=${#lex_test_cases[@]}
passed_tests=0
failed_tests=()

for test_case in "${lex_test_cases[@]}"; do
  output=$(./build/pnd -c "$test_case")
  expected_output="${lex_expected_outputs[$i]}"

  if [ "$output" == "$expected_output" ]; then
    passed_tests=$((passed_tests + 1))
  else
    failed_tests+=($((i+1)))
  fi

  i=$((i+1))
done

echo "Summary: $passed_tests / $total_tests tests passed."

if [ ${#failed_tests[@]} -ne 0 ]; then
    echo "Failed tests: ${failed_tests[*]}"
    echo "Details of failed tests:"
    for test_num in "${failed_tests[@]}"; do
        index=$((test_num - 1))
        echo "------------------------"
        echo "Test $test_num: ${lex_test_cases[$index]}"
        echo "Expected:"
        echo -e "${lex_expected_outputs[$index]}"
        echo "Got:"
        echo -e "$(./build/pnd -c "${lex_test_cases[$index]}")"
    done
    exit 1
fi

exit 0
