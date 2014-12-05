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

#include "raum_ausfuellen_tiefensuche.h"

#define TIEFE_MAX 6

namespace TaktikNS {
  /**
   ** Standardkonstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-12-05
   **/
  RaumAusfuellenTiefensuche::RaumAusfuellenTiefensuche() :
    Taktik{"Raum ausfüllen (Tiefensuche)", "Den Raum möglichst gut ausfüllen, rekursiv mit Tiefensuche wird ein optimaler Weg bestimmt."}
  { }

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche den längsten Weg.
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot_nummer   die Nummer des Bots
   **
   ** @return    Richtung mit dem längsten Weg
   **
   ** @version   2014-12-05
   **/
  Taktik::Ergebnis
    RaumAusfuellenTiefensuche::ergebnis(Spielraster const& spielraster,
                                        int const bot_nummer)
    {
      auto const& bp = spielraster.position(bot_nummer);
      int const wegweite_vorwaerts
        = this->wegweite(spielraster, bp + Bewegungsrichtung::VORWAERTS, TIEFE_MAX);
      int const wegweite_links
        = this->wegweite(spielraster, bp + Bewegungsrichtung::LINKS, TIEFE_MAX);
      int const wegweite_rechts
        = this->wegweite(spielraster, bp + Bewegungsrichtung::RECHTS, TIEFE_MAX);
      CLOG << Bewegungsrichtung::VORWAERTS << ": " << wegweite_vorwaerts << endl;
      CLOG << Bewegungsrichtung::LINKS << ": " << wegweite_links << endl;
      CLOG << Bewegungsrichtung::RECHTS << ": " << wegweite_rechts << endl;

      if (wegweite_links >= wegweite_vorwaerts)
        if (wegweite_links >= wegweite_rechts)
          return Bewegungsrichtung::LINKS;
        else
          return Bewegungsrichtung::RECHTS;
      else 
        if (wegweite_vorwaerts >= wegweite_rechts)
          return Bewegungsrichtung::VORWAERTS;
        else
          return Bewegungsrichtung::RECHTS;
    } // Taktik::Ergebnis RaumAusfuellenTiefensuche::ergebnis(Spielraster const& spielraster, int const bot_nummer)

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche den längsten Weg.
   ** Sollte bis etwa 25 Rasterer verwendet werden.
   ** 28 Rasterer: 1,05 s
   ** 5x5 Rasterer: 0,255 s
   ** 
   ** @param     raster              Spielraster
   ** @param     p                   Position auf dem Raster
   ** @param     tiefe_verbleibend   verbleibende Tiefe zu prüfen
   **
   ** @return    Längster Weg ab der Position
   **
   ** @version   2014-12-05
   **/
  int
    RaumAusfuellenTiefensuche::wegweite(Raster raster,
                                        Position p,
                                        int tiefe_verbleibend) const
    {
      if (raster(p))
        return 0;
      raster.belege(p);
      int n = 1;
      while (raster.nachbarn_frei(p) == 1) {
        p = raster.freier_nachbar(p);
        raster.belege(p);
        n += 1;
      }
      if (tiefe_verbleibend == 0)
        return (n + raster.raumgroesse_erreichbar(p));

      tiefe_verbleibend -= 1;
      int wegweite_max = 0;
      for (auto& r : ::richtungen) {
        wegweite_max = std::max(wegweite_max,
                                this->wegweite(raster, p + r, tiefe_verbleibend));
      }
      return n + wegweite_max;
    } // int RaumAusfuellenTiefensuche::wegweite(Raster raster, BotPosition const p, int tiefe_verbleibend)

} // namespace TaktikNS
