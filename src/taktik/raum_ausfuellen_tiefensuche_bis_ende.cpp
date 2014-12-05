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

#include "raum_ausfuellen_tiefensuche_bis_ende.h"

namespace TaktikNS {
  /**
   ** Standardkonstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-08
   **/
  RaumAusfuellenTiefensucheBisEnde::RaumAusfuellenTiefensucheBisEnde() :
    Taktik{"Raum ausfüllen (Tiefensuche bis Ende)", "Den Raum möglichst gut ausfüllen, rekursiv mit Tiefensuche, bis der Raum aufgefüllt ist, wird ein optimaler Weg bestimmt."}
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
   ** @version   2014-11-08
   **/
  Taktik::Ergebnis
    RaumAusfuellenTiefensucheBisEnde::ergebnis(Spielraster const& spielraster,
                                        int const bot_nummer)
    {
      auto const& bp = spielraster.position(bot_nummer);
      int const wegweite_vorwaerts
        = this->wegweite(spielraster, bp + Bewegungsrichtung::VORWAERTS);
      int const wegweite_links
        = this->wegweite(spielraster, bp + Bewegungsrichtung::LINKS);
      int const wegweite_rechts
        = this->wegweite(spielraster, bp + Bewegungsrichtung::RECHTS);
      CLOG << Bewegungsrichtung::VORWAERTS << ": " << wegweite_vorwaerts << endl;
      CLOG << Bewegungsrichtung::LINKS << ": " << wegweite_links << endl;
      CLOG << Bewegungsrichtung::RECHTS << ": " << wegweite_rechts << endl;

      if (wegweite_vorwaerts >= wegweite_links)
        if (wegweite_vorwaerts >= wegweite_rechts)
          return Bewegungsrichtung::VORWAERTS;
        else
          return Bewegungsrichtung::RECHTS;
      else 
        if (wegweite_links >= wegweite_rechts)
          return Bewegungsrichtung::LINKS;
        else
          return Bewegungsrichtung::RECHTS;
    } // Taktik::Ergebnis RaumAusfuellenTiefensucheBisEnde::ergebnis(Spielraster const& spielraster, int const bot_nummer)

  /**
   ** -> Rückgabe
   ** Suche den längsten Weg.
   ** 
   ** @param     raster   Spielraster
   ** @param     bp     Position auf dem Raster
   **
   ** @return    Längster Weg ab der Position
   **
   ** @version   2014-11-08
   **/
  int
    RaumAusfuellenTiefensucheBisEnde::wegweite(Raster raster,
                                        Position const bp) const
    {
      // zwischen verschiedenen Versionen wählen
      return this->wegweite_simpel(raster, bp); // ganz simpel
      //return this->wegweite_untere_grenze(raster, bp, 0); // mit Maxvergleich
      //std::set<std::pair<Raster, Position>> bekannte_raster;
      //return this->wegweite_ohne_duplikate(raster, bp, bekannte_raster); // mit Duplikatvergleich
    } // int RaumAusfuellenTiefensucheBisEnde::wegweite(Raster raster, Position const& bp) const

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche den längsten Weg.
   ** Sollte bis etwa 25 Rasterer verwendet werden.
   ** 28 Rasterer: 1,05 s
   ** 5x5 Rasterer: 0,255 s
   ** 
   ** @param     raster   Spielraster
   ** @param     bp     Position auf dem Raster
   **
   ** @return    Längster Weg ab der Position
   **
   ** @version   2014-11-08
   **/
  int
    RaumAusfuellenTiefensucheBisEnde::wegweite_simpel(Raster raster,
                                               Position const bp) const
    {
      if (raster(bp))
        return 0;
      raster.belege(bp);

      int wegweite_max = 1;
      for (auto& r : ::richtungen) {
        if (raster(bp + r))
          continue;
        wegweite_max = std::max(wegweite_max, this->wegweite_simpel(raster, bp + r) + 1);
      }
      return wegweite_max;
    } // int RaumAusfuellenTiefensucheBisEnde::wegweite_simpel(Raster raster, BotPosition const p)

} // namespace TaktikNS
