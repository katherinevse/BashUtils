#!/bin/bash

TEST_DATA_FOLDER="../test_data"

# Переменные для подсчета пройденных и не пройденных тестов
passed_tests=0
failed_tests=0
failed_scenarios=()
passed_scenarios=()

for file in $TEST_DATA_FOLDER/*.txt; do
  for flag in -b -e -n -s -t; do
    echo "Testing with flag $flag on file $file"

    # Вывод команды cat
    cat_command="cat $flag $file"
    echo " "
    echo "Command: $cat_command"
    cat_output=$(cat $flag "$file")
    echo "cat output:"
    echo "$cat_output"

    # Вывод команды s21_cat
    s21_cat_command="./s21_cat $flag $file"
    echo " "
    echo "Command: $s21_cat_command"
    s21_cat_output=$(./s21_cat $flag "$file")
    echo "s21_cat output:"
    echo "$s21_cat_output"

    # Сравнение выводов
    if [ "$cat_output" == "$s21_cat_output" ]; then
      echo " "
      echo "Test PASSED"
      passed_tests=$((passed_tests + 1))
      passed_scenarios+=("$flag on $file")
    else
      echo " "
      echo "Test FAILED"
      diff <(echo -e "$cat_output") <(echo -e "$s21_cat_output")
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
