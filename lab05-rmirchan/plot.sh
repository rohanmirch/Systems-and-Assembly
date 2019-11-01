#!/bin/bash

set -e

# Directory where measurement data is stored
DATADIR=./data

# Directory where gnuplot configuration is stored
GPLOTDIR=./gpconf

#==========================================
# MAKE SOME PICTURES!

mkdir -p images

PLOTS="$GPLOTDIR/plot-append.gp $GPLOTDIR/plot-insert.gp"

PLOTS="$PLOTS $GPLOTDIR/plot-al-append.gp $GPLOTDIR/plot-al-insert.gp"
PLOTS="$PLOTS $GPLOTDIR/plot-al-append-detail.gp"

PLOTS="$PLOTS $GPLOTDIR/plot-ll-append.gp $GPLOTDIR/plot-ll-insert.gp"

PLOTS="$PLOTS $GPLOTDIR/plot-iter.gp"

if [ -x `which gnuplot` ]
then
    echo ================================================
    echo GENERATING PERFORMANCE PLOTS
    echo

    for F in $PLOTS ; do
        echo gnuplot $F
        gnuplot $F
    done
else
    echo ================================================
    echo NO PERFORMANCE PLOTS FOR YOU!
    echo
    echo gnuplot is not on the path, skipping image generation
fi

git add images
