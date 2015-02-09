/*
   lichtgehen

   Copyright (C) 2014 by Diether Knof

   This file is part of lichtgehen.

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

   Diese Datei ist Teil von lichtgehen.

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

#include "gegner_verfolgen.h"

namespace TaktikNS {
  /**
   ** Standardkonstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-12-08
   **/
  GegnerVerfolgen::GegnerVerfolgen() :
    Taktik{"gegner verfolgen", "Gehe zum dichtesten Gegner"}
  { }

  /**
   ** -> Rückgabe
   ** Gehe zum dichtesten Gegner
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     spieler_nummer    die Nummer des Spielers
   **
   ** @return    Richtung zum Gegner
   **
   ** @version   2014-12-08
   **/
  Taktik::Ergebnis
    GegnerVerfolgen::ergebnis(Spielraster const& spielraster, int const spieler_nummer)
    {
      int entfernung_min = spielraster.groesse();
      auto richtung_min = Bewegungsrichtung::VORWAERTS;
      for (auto r : bewegungsrichtungen) {
        for (int b = 0; b < spielraster.spieler_anz(); ++b) {
          if (b == spieler_nummer)
            continue;
          auto const e = spielraster.kuerzeste_entfernung(spielraster.position(b), spielraster.position(spieler_nummer) + r);
          if ((e > 0) && (e < entfernung_min)) {
            entfernung_min = e;
            richtung_min = r;
          }
        } // for (b)
      } // for (r : richtungen)

      if (entfernung_min == spielraster.groesse())
        return false;
      return richtung_min;
    } // Taktik::Ergebnis GegnerVerfolgen::ergebnis(Spielraster const& spielraster, int const spieler_nummer)
} // namespace TaktikNS
