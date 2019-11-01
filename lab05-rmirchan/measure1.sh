#!/bin/bash

set -e

ENABLE_ITER=0
ENABLE_PERF=0

PERF_REPS=3
ITER_REPS=3

while getopts 'p:i:' flag; do
  case "${flag}" in
    p) ENABLE_PERF=1; PERF_REPS=${OPTARG} ;;
    i) ENABLE_ITER=1; ITER_REPS=${OPTARG} ;;
    *) print_usage
       exit 1 ;;
  esac
done

# Directory where measurement data is stored
DATADIR=./data

# Directory where gnuplot configuration is stored
GPLOTDIR=./gpconf

OPTS="-v -r $PERF_REPS"

ITER_OPTS="-r $ITER_REPS -m 2048 -n 1048576 -s 4096"

if [ $ENABLE_PERF -eq 0 ] && [ $ENABLE_ITER -eq 0 ]; then
    echo "At least one of PERF and ITER needs to be enabled for this script to do anything."
    exit 1;
fi

# If there isn't a data directory, create it.
mkdir -p $DATADIR


echo ================================================
echo LINKED LIST IMPROVEMENTS PERFORMANCE TESTS
echo

# Node-pool optimization?
if grep -q LL_NODEPOOL linkedlist.c; then
    # Build with the nodepool optimization enabled.
    CPPFLAGS="-DLL_NODEPOOL" make clean listperf 

    if [ $ENABLE_PERF -ne 0 ]; then
        ./listperf $OPTS -t append -l linkedlist > $DATADIR/append-ll-nodepool.txt
        ./listperf $OPTS -t insert -l linkedlist > $DATADIR/insert-ll-nodepool.txt
    fi

    if [ $ENABLE_ITER -ne 0 ]; then
        ./listperf $ITER_OPTS -t iter -l linkedlist > $DATADIR/iter-ll-nodepool.txt
    fi
else
    echo "Linked list node-pool optimization not found, skipping."
fi

if [ $ENABLE_PERF -ne 0 ]; then
    # Node-pool optimization without cache improvement?
    if grep -q LL_NODEPOOL_BADCACHE linkedlist.c; then
        # Build with the nodepool optimization enabled.
        CPPFLAGS="-DLL_NODEPOOL -DLL_NODEPOOL_BADCACHE" make clean listperf 

        ./listperf $OPTS -t append -l linkedlist > $DATADIR/append-ll-nodepool-badcache.txt
        ./listperf $OPTS -t insert -l linkedlist > $DATADIR/insert-ll-nodepool-badcache.txt
    else
        echo "Linked list node-pool bad-cache tweak not found, skipping."
    fi
fi

echo ================================================
echo FULL OPTIMIZATION PERFORMANCE TESTS
echo

# Build the baseline code, and collect data from it.
CFLAGS="-Ofast" CPPFLAGS="-DAL_RESIZE_OPT -DAL_INSERT_OPT -DLL_NODEPOOL" make clean listperf 

if [ $ENABLE_PERF -ne 0 ]; then
    ./listperf $OPTS -t append -l arraylist > $DATADIR/append-al-fastest.txt
    ./listperf $OPTS -t insert -l arraylist > $DATADIR/insert-al-fastest.txt

    ./listperf $OPTS -t append -l linkedlist > $DATADIR/append-ll-fastest.txt
    ./listperf $OPTS -t insert -l linkedlist > $DATADIR/insert-ll-fastest.txt
fi

if [ $ENABLE_ITER -ne 0 ]; then
./listperf $ITER_OPTS -t iter   -l arraylist > $DATADIR/iter-al-fastest.txt
./listperf $ITER_OPTS -t iter   -l linkedlist > $DATADIR/iter-ll-fastest.txt
fi

git add data