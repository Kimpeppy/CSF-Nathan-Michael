#! /usr/bin/env bash

set -e

make clean
make
mkdir -p /tmp/mkim225
./gen_rand_data 16M /tmp/mkim225/data_16M.in
echo "Test run with threshold 2097152"
cp /tmp/mkim225/data_16M.in /tmp/mkim225/test_16M.in
time ./parsort /tmp/mkim225/test_16M.in 2097152
echo "Test run with threshold 1048576"
cp /tmp/mkim225/data_16M.in /tmp/mkim225/test_16M.in
time ./parsort /tmp/mkim225/test_16M.in 1048576
echo "Test run with threshold 524288"
cp /tmp/mkim225/data_16M.in /tmp/mkim225/test_16M.in
time ./parsort /tmp/mkim225/test_16M.in 524288
echo "Test run with threshold 262144"
cp /tmp/mkim225/data_16M.in /tmp/mkim225/test_16M.in
time ./parsort /tmp/mkim225/test_16M.in 262144
echo "Test run with threshold 131072"
cp /tmp/mkim225/data_16M.in /tmp/mkim225/test_16M.in
time ./parsort /tmp/mkim225/test_16M.in 131072
echo "Test run with threshold 65536"
cp /tmp/mkim225/data_16M.in /tmp/mkim225/test_16M.in
time ./parsort /tmp/mkim225/test_16M.in 65536
echo "Test run with threshold 32768"
cp /tmp/mkim225/data_16M.in /tmp/mkim225/test_16M.in
time ./parsort /tmp/mkim225/test_16M.in 32768
echo "Test run with threshold 16384"
cp /tmp/mkim225/data_16M.in /tmp/mkim225/test_16M.in
time ./parsort /tmp/mkim225/test_16M.in 16384
rm -rf /tmp/mkim225
