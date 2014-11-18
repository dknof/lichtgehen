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

#ifndef TAKTIK_RICHTUNG_H
#define TAKTIK_RICHTUNG_H

#include "taktik.h"

namespace TaktikNS {
/** Taktik Richtung
 ** Gehe in die übergebene Richtung, wenn möglich
 **/
class Richtung : public Taktik {
  public:
    // Konstruktor
    Richtung(Bewegungsrichtung richtung);
    // Konstruktor
    Richtung(Bewegungsrichtung richtung, double p);

    // gibt das Ergebnis der Taktik zurück (ob sie greift und die Richtung)
    Ergebnis ergebnis(Spielraster const& spielraster,
                      int bot_nummer) override;

  private:
    // Die Richtung, in die sich bewegt werden soll
    Bewegungsrichtung const richtung;
    // Wahrscheinlichkeit, in die Richtung zu gehen
    double const wahrscheinlichkeit;

}; // class Richtung : public Taktik

} // namespace TaktikNS

#endif // #ifndef TAKTIK_RICHTUNG_H