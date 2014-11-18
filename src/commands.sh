#!/bin/bash
if [ -e ./tron ]; then
  for f in $(find . -name "*.cpp"); do
    if [ $f -nt ./tron ]; then rm ./tron; break; fi
  done
fi
if [ $(grep -c processor /proc/cpuinfo) -gt 1 ]; then
  nice -n 5 make -s -k --jobs=$(expr 1 + $(grep -c processor /proc/cpuinfo) ) --load-average=$(expr 1 + $(grep -c processor /proc/cpuinfo) ) && sleep 1
else
  nice -n 5 make -s -k && sleep 1
fi
