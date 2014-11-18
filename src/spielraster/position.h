/*
   tronbot

   Copyright (C) 2014 by Diether Knof

   This file is part of tronbot.

   Tronbot is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   Software Foundation, either version 3 of the License, or
   your option) any later version.

   Tronbot is distributed in the hope that it will be useful,
   but without any warranty; without even the implied warranty of
   merchantability or fitness for a particular purpose.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

   Diese Datei ist Teil von tronbot.

   Trobot ist Freie Software: Sie können es unter den Bedingungen
   der GNU General Public License, wie von der Free Software Foundation,
   Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
   veröffentlichten Version, weiterverbreiten und/oder modifizieren.

   Trobot wird in der Hoffnung, dass es nützlich sein wird, aber
   ohne jede Gewährleistung, bereitgestellt; sogar ohne die implizite
   Gewährleistung der Marktfähigkeit oder Eignung für einen bestimmten Zweck.
   Siehe die GNU General Public License für weitere Details.

   Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
   Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
   */

#ifndef POSITION_H
#define POSITION_H

#include "constants.h"
#include "richtung.h"

/** Die Position im Feld
 **
 ** @todo    kein default Konstruktor
 ** @todo    += → +
 **/
class Position {
  public:
    // Konstruktor
    Position(int x, int y);
    // Konstruktor
    Position(istream& istr);

    // Die x-Position
    int x() const;
    // Die y-Position
    int y() const;

    // ob dies eine gültige Position ist
    operator bool() const;

    // Ein Schritt in der Richtung bewegen
    Position& operator+=(Richtung richtung);

  private:
    // x-Position
    int x_ = 0;
    // y-Position
    int y_ = 0;
}; // class Position

// Position ausgeben
ostream& operator<<(ostream& ostr, Position const& position);
// Positionen vergleichen
bool operator==(Position const& pa, Position const& pb);
// Positionen vergleichen
bool operator<(Position const& pa, Position const& pb);
// Ein Schritt weiter in der Richtung
Position operator+(Position lhs, Richtung rhs);

#endif // #ifndef POSITION_H
