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

#ifndef SPIELERPOSITION_H
#define SPIELERPOSITION_H

#include "constants.h"
#include "position.h"
#include "richtung.h"

/** Die Position eines Spielers
 ** 
 ** @todo    kein default Konstruktor
 ** @todo    += → +
 **/
class SpielerPosition : public Position {
  public:
    // Konstruktor
    SpielerPosition();
    // Konstruktor
    SpielerPosition(Position const& position, Richtung const richtung);
    // Konstruktor
    SpielerPosition(istream& istr);

    // Die Richtung
    Richtung richtung() const;

    // Text
    operator string() const;

    // Ein Schritt in der Bewegungsrichtung bewegen
    SpielerPosition& operator+=(Bewegungsrichtung bewegungsrichtung);

  private:
    // Blickrichtung
    Richtung richtung_ = Richtung::NORDEN;
}; // class SpielerPosition

// der Weg eines Spielers
typedef vector<SpielerPosition> SpielerWeg;

// SpielerPosition ausgeben
ostream& operator<<(ostream& ostr, SpielerPosition const& spielerposition);
// Ein Schritt in der Bewegungsrichtung weiter
SpielerPosition operator+(SpielerPosition lhs, Bewegungsrichtung rhs);

namespace std {
  inline string to_string(SpielerPosition const bp)
  { return static_cast<string>(bp); }
}

#endif // #ifndef SPIELERPOSITION_H
