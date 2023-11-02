#!/bin/bash

TEST_DATA_FOLDER="../../datasets/test_data"

# Переменные для подсчета пройденных и не пройденных тестов
passed_tests=0
failed_tests=0
failed_scenarios=()
passed_scenarios=()
my_pattern_search='a'

for file in $TEST_DATA_FOLDER/*; do
  for flag in -e -i -v -c -l -n; do
    echo "Testing with flag $flag on $my_pattern_search $file"

    # Вывод команды grep
    grep_command="grep $flag $my_pattern_search $file"
    echo " "
    echo "Command: $grep_command"
    grep_output=$(grep $flag $my_pattern_search $file)
    echo "grep output:"
    echo "$grep_output"

    # Вывод команды s21_grep
    s21_grep_command="./s21_grep $flag $my_pattern_search $file"
    echo " "
    echo "Command: $s21_grep_command"
    s21_grep_output=$(./s21_grep $flag $my_pattern_search $file)
    echo "s21_grep output:"
    echo "$s21_grep_output"

    # Сравнение выводов
    if [ "$grep_output" == "$s21_grep_output" ]; then
      echo " "
      echo "Test PASSED"
      passed_tests=$((passed_tests + 1))
      passed_scenarios+=("$flag on $file")
    else
      echo " "
      echo "Test FAILED"
      failed_tests=$((failed_tests + 1))
      failed_scenarios+=("$flag on $file")
    fi

    echo "-------------------------"
  done
done

# Вывод краткого результата
echo "SUMMARY:"
echo "Passed tests: $passed_tests"
echo "Failed tests: $failed_tests"
echo " "
echo "Failed scenarios:"
for scenario in "${failed_scenarios[@]}"; do
  echo "$scenario"
done
echo " "
echo "Passed scenarios:"
for scenario in "${passed_scenarios[@]}"; do
  echo "$scenario"
done
