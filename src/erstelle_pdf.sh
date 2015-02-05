#!/bin/zsh

# tronbot kompilieren und ausführen

rm ./tronbot

#export CXXFLAGS="-Wall -Werror -Wno-parentheses -std=c++14 -O3"
#export CXXFLAGS="-Wall -Werror -Wno-parentheses -std=c++14 -O0 -ggdb"

echo "Kompiliere tronbot…"
make -j1 || exit

mkdir -p ../Ergebnisse.pdf
for f in ../freiesmagazin-2014-10-contest/results/*.txt; do
  echo $(basename "$f")
  ./tronbot <"$f" >/dev/null
  mv Spielraster.pdf ../Ergebnisse.pdf/$(basename "$f" .txt).pdf
done
