#!/bin/zsh

# tronbot kompilieren und ausführen

rm ./tronbot

#export CXXFLAGS="-Wall -Werror -Wno-parentheses -std=c++14 -O3"
#export CXXFLAGS="-Wall -Werror -Wno-parentheses -std=c++14 -O0 -ggdb"

echo "Kompiliere tronbot…"
make -j1 || exit

mkdir ../Ergebnisse.pdf
for i in 1; do 
  for f in ../freiesmagazin-2014-10-contest/results_turn$i/*.txt; do
    echo "$i -- "$(basename "$f")
    ./tronbot <"$f" >/dev/null
    mv Spielraster.pdf ../Ergebnisse.pdf/$(basename "$f" .txt)-$i.pdf
  done
done
