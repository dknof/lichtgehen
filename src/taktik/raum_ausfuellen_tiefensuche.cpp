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
   ** @version   2014-11-08
   **/
  Taktik::Ergebnis
    RaumAusfuellenTiefensuche::ergebnis(Spielraster const& spielraster,
                                        int const bot_nummer)
    {
      BotPosition const& bp = spielraster.position(bot_nummer);
      int const wegweite_vorwaerts
        = this->wegweite(spielraster, bp + Bewegungsrichtung::VORWAERTS);
      int const wegweite_links
        = this->wegweite(spielraster, bp + Bewegungsrichtung::LINKS);
      int const wegweite_rechts
        = this->wegweite(spielraster, bp + Bewegungsrichtung::RECHTS);

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
    } // Taktik::Ergebnis RaumAusfuellenTiefensuche::ergebnis(Spielraster const& spielraster, int const bot_nummer)

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
    RaumAusfuellenTiefensuche::wegweite(Raster raster,
                                        Position const bp) const
    {
      // zwischen verschiedenen Versionen wählen
      return this->wegweite_simpel(raster, bp); // ganz simpel
      //return this->wegweite_untere_grenze(raster, bp, 0); // mit Maxvergleich
      //std::set<std::pair<Raster, Position>> bekannte_raster;
      //return this->wegweite_ohne_duplikate(raster, bp, bekannte_raster); // mit Duplikatvergleich
    } // int RaumAusfuellenTiefensuche::wegweite(Raster raster, Position const& bp) const

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
    RaumAusfuellenTiefensuche::wegweite_simpel(Raster raster,
                                               Position const bp) const
    {
      if (raster(bp))
        return 0;
      raster.belege(bp);

      int wegweite_max = 0;
      for (auto& r : ::richtungen) {
        if (raster(bp + r))
          continue;
        wegweite_max = std::max(wegweite_max, this->wegweite_simpel(raster, bp + r) + 1);
      }
      return wegweite_max;
    } // int RaumAusfuellenTiefensuche::wegweite_simpel(Raster raster, BotPosition const p)

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche den längsten Weg.
   ** Bei der Suche wird geprüft, ob die bislang gefundene Länge noch übertroffen werden kann.
   ** Sollte bis etwa 25 Rasterer verwendet werden.
   ** 5x5 Rasterer: 1 Sekunde
   ** 
   ** @param     raster            Spielraster
   ** @param     bp              Position auf dem Raster
   ** @param     untere_grenze   schon erreichte Weglänge
   **
   ** @return    Längster Weg ab der Position
   **
   ** @version   2014-11-08
   **/
  int
    RaumAusfuellenTiefensuche::wegweite_untere_grenze(Raster raster,
                                                      Position const& bp,
                                                      int untere_grenze) const
    {
      if (raster(bp))
        return 0;
      raster.belege(bp);

      int wegweite_max = 0;
      for (auto& r : ::richtungen) {
        if (raster(bp + r))
          continue;
        if (raster.raumgroesse(bp + r) < wegweite_max)
          continue;
        wegweite_max = std::max(wegweite_max,
                                this->wegweite_untere_grenze(raster, bp + r,
                                                             std::max(untere_grenze - 1,
                                                                      wegweite_max)) + 1);
      }
      return wegweite_max;
    } // int RaumAusfuellenTiefensuche::wegweite_untere_grenze(Raster raster, BotPosition const p)

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche den längsten Weg.
   ** Dublikate bei der Suche werden übersprungen.
   ** 5x5 Rasterer: 10 Sekunden
   ** 
   ** @param     raster              Spielraster
   ** @param     bp                Position auf dem Raster
   ** @param     bekannte_raster   Menge mit den bekannten Rastern
   **
   ** @return    Längster Weg ab der Position
   **
   ** @version   2014-11-09
   **/
  int
    RaumAusfuellenTiefensuche::wegweite_ohne_duplikate(Raster raster,
                                                       Position const& bp,
                                                       std::set<std::pair<Raster, Position>>& bekannte_raster) const
    {
      if (raster(bp))
        return 0;
      raster.belege(bp);
      auto fbp = std::pair<Raster, Position>{raster, bp};
      if (bekannte_raster.find(fbp) != bekannte_raster.end())
        return 0;
      bekannte_raster.insert(fbp);

      int wegweite_max = 0;
      for (auto r : ::richtungen) {
        if (raster(bp + r))
          continue;
        wegweite_max = std::max(wegweite_max,
                                this->wegweite_ohne_duplikate(raster, bp + r,
                                                              bekannte_raster)
                                + 1);
      }
      return wegweite_max;
    } // int RaumAusfuellenTiefensuche::wegweite_ohne_duplikate(Raster raster, Position const& bp, std::set<std::pair<Raster, Position>>& bekannte_raster) const
} // namespace TaktikNS
