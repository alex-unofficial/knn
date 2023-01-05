#!/bin/bash

bin_dir=./bin

tests="sedm qselect knn"

for test in $tests; do
    $bin_dir/test_$test || exit 1;
done
