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

#include "raum_ausfuellen.h"

namespace TaktikNS {
  /**
   ** Standardkonstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-10-30
   **/
  RaumAusfuellen::RaumAusfuellen() :
    Taktik{"Raum ausfüllen", "Den Raum möglichst gut ausfüllen"}
  { }

  /**
   ** -> Rückgabe
   ** Gehe in die Richtung von den Felden mit den wenigsten freien Nachbarn. Ignoriere aber Sackgassen.
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     spieler_nummer   die Nummer des Spielers
   **
   ** @return    Entlang dem Weg mit den meisten Wänden
   **
   ** @version   2014-10-30
   **/
  Taktik::Ergebnis
    RaumAusfuellen::ergebnis(Spielraster const& spielraster, int const spieler_nummer)
    {
      int min_frei = 5; // minimale Anzahl an freien Nachbarrasterern
      SpielerPosition const bp{spielraster.position(spieler_nummer)};
      Bewegungsrichtung richtung = Bewegungsrichtung::VORWAERTS;
      if (!spielraster(bp + Bewegungsrichtung::VORWAERTS)) {
        int const frei = spielraster.nachbarn_frei(bp + Bewegungsrichtung::VORWAERTS);
        if (frei > 0) // keine Sackgasse
          min_frei = frei;
      }
      if (!spielraster(bp + Bewegungsrichtung::LINKS)) {
        int const frei = spielraster.nachbarn_frei(bp + Bewegungsrichtung::LINKS);
        if (   (frei > 0) // keine Sackgasse
            && (frei < min_frei) ) {
          min_frei = frei;
          richtung = Bewegungsrichtung::LINKS;
        }
      }
      if (!spielraster(bp + Bewegungsrichtung::RECHTS)) {
        int const frei = spielraster.nachbarn_frei(bp + Bewegungsrichtung::RECHTS);
        if (   (frei > 0) // keine Sackgasse
            && (frei < min_frei) ) {
          min_frei = frei;
          richtung = Bewegungsrichtung::RECHTS;
        }
      }
      if (min_frei == 5) // nur Sackgassen gefunden
        return false;
      return richtung;
    } // Taktik::Ergebnis RaumAusfuellen::ergebnis(Spielraster const& spielraster, int const spieler_nummer)
} // namespace TaktikNS
