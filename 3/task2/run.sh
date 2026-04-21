#!/bin/bash

echo "=== RUN TESTS ==="

echo "Running sin test..."
./test/build/sin_test

echo "Running sqrt test..."
./test/build/sqrt_test

echo "Running pow test..."
./test/build/pow_test

echo "=== DONE ==="