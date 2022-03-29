#!/usr/bin/env bash

if [ ! -f argh.h ]; then
  >&2 echo "Preparing for compilation..." 
  wget https://raw.githubusercontent.com/adishavit/argh/v1.3.2/argh.h
fi


function compile() {
  SCHEDULE="schedule($1)"
  
  g++ v2.cpp -fopenmp -std=c++11 -DSCHEDULE=$SCHEDULE -DSCHEDULE_STR="\"$SCHEDULE\""
}

function run() {
  for threads in {1..4}; do
    ./a.out --threads=$threads --size=$1 --repeat=$2
  done
}

SCHEDULES=(
  "static" 
  "static,1"
  "dynamic"
  "dynamic,size/omp_get_num_threads()*100"
  "guided"
)

echo "threads;size;schedule;time"
IFS="" # remove separation by spaces
for schedule in ${SCHEDULES[*]}; do
  compile $schedule
  run 1000000 30
done

