#!/bin/bash
# Gabriele Venturato (125512)
# Advanced Scheduling Systems - Run MiniZinc model benchmarks
# @param $1 (optional) : time in milliseconds
# @param $2 (optional) : solver name

## Print spinner while waiting a process to finish
# @param $1: pid of the process
function wait_with_spinner {
    stepsize=1; frames='\|/-'

    # If this script is killed, kill the `cp'.
    trap "kill $1 2> /dev/null" EXIT

    # While copy is running...
    while kill -0 $1 2> /dev/null; do
        s=$(( (s+$stepsize) %${#frames} )); 
        printf "\r${frames:$s:$stepsize}"
        sleep 0.8
    done

    # Disable the trap on a normal exit.
    trap - EXIT
}

## ---------------------------------- MAIN ---------------------------------- ##
OUT_FOLDER="out/" # IMPORTANT: keep the slash at the end of the folder name
MODEL="../../flowshop.mzn"

if [ $# -lt 1 ]; then
    TIME=$((1000*60*3))
else
    TIME=$1
fi

if [ $# -lt 2 ]; then
    SOLVER=OSICBC
else
    SOLVER=$2
fi

## loop through all instances
for infile in ../../Instances/*; do
    # prepare output filename
    outfile=$(basename $infile)
    outfile=${outfile%.*}".txt"
    outfile=$OUT_FOLDER$SOLVER"_"$outfile
    # start testing
    echo "-> TESTING: $infile"
    date >> $outfile
    echo "Timeout: "$TIME >> $outfile
    case $SOLVER in
        Gecode)
            minizinc -s --fzn-flags "-time $TIME -p 0"\
                --solver $SOLVER $MODEL $infile >> $outfile &
            wait_with_spinner $!
        ;;
        OSICBC|Chuffed)
            minizinc -s --solver-time-limit $TIME\
                --solver $SOLVER $MODEL $infile >> $outfile &
            wait_with_spinner $!
        ;;
        *)
            echo "Solver not found."
            exit 1
        ;;
    esac
    echo
    echo >> $outfile
done