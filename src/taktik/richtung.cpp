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

#include "richtung.h"
#include <cstdlib>

namespace TaktikNS {
  /**
   ** Standardkonstruktor
   ** 
   ** @param     richtung   Richtung, in die sich bewegt werden soll
   **
   ** @return    -
   **
   ** @version   2014-10-30
   **/
  Richtung::Richtung(Bewegungsrichtung const richtung) :
    Taktik{"Richtung", "gehe " + string("")
           + ((richtung == Bewegungsrichtung::VORWAERTS) ? "vorwärts" :
              (richtung == Bewegungsrichtung::LINKS) ? "nach links" :
              (richtung == Bewegungsrichtung::RECHTS) ? "nach rechts" : "?")
           + " wenn möglich"},
      richtung{richtung},
    wahrscheinlichkeit{1}
      { }

  /**
   ** Standardkonstruktor
   ** 
   ** @param     richtung   Richtung, in die sich bewegt werden soll
   ** @param     p   Wahrscheinlichkeit, mit der in die Richtung gegangen wird.
   **
   ** @return    -
   **
   ** @version   2014-10-30
   **/
  Richtung::Richtung(Bewegungsrichtung const richtung,
                     double const p) :
    Taktik{"Richtung", "gehe " + string("")
           + ((richtung == Bewegungsrichtung::VORWAERTS) ? "vorwärts" :
              (richtung == Bewegungsrichtung::LINKS) ? "nach links" :
              (richtung == Bewegungsrichtung::RECHTS) ? "nach rechts" : "?")
           + " mit " + std::to_string(static_cast<int>(p * 100))
           + " % Wahrscheinlichkeit"},
    richtung{richtung},
    wahrscheinlichkeit{p}
    { }

  /**
   ** -> Rückgabe
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     spieler_nummer   die Nummer des Spielers
   **
   ** @return    falsch, wenn noch ein Weg frei ist, VERWAERTS, wenn kein Weg mehr frei ist
   **
   ** @version   2014-10-30
   **/
  Taktik::Ergebnis
    Richtung::ergebnis(Spielraster const& spielraster, int const spieler_nummer)
    {
      SpielerPosition const bp{spielraster.position(spieler_nummer)};
      if (!spielraster(bp + this->richtung)
          && (rand() <= this->wahrscheinlichkeit * RAND_MAX))
        return this->richtung;
      return false;
    } // Taktik::Ergebnis Richtung::ergebnis(Spielraster const& spielraster, int const spieler_nummer)
} // namespace TaktikNS
