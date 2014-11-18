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

#include "groesster_raum.h"

namespace TaktikNS {
  /**
   ** Standardkonstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-01
   **/
  GroessterRaum::GroessterRaum() :
    Taktik{"größter Raum", "In den größten Raum wechseln"}
  { }

  /**
   ** -> Rückgabe
   ** Gehe in die Richtung von den Felden mit den wenigsten freien Nachbarn. Ignoriere aber Sackgassen.
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot_nummer   die Nummer des Bots
   **
   ** @return    Entlang dem Weg mit den meisten Wänden
   **
   ** @version   2014-11-01
   **/
  Taktik::Ergebnis
    GroessterRaum::ergebnis(Spielraster const& spielraster, int const bot_nummer)
    {
      BotPosition const bp{spielraster.position(bot_nummer)};
      int const groesse_v
        = {spielraster.raumgroesse(bp + Bewegungsrichtung::VORWAERTS)};
      int const groesse_l
        = {spielraster.raumgroesse(bp + Bewegungsrichtung::LINKS)};
      int const groesse_r
        = {spielraster.raumgroesse(bp + Bewegungsrichtung::RECHTS)};

      if (   (groesse_v > groesse_l)
          && (groesse_v > groesse_r))
        return Bewegungsrichtung::VORWAERTS;
      if (   (groesse_l > groesse_v)
          && (groesse_l > groesse_r))
        return Bewegungsrichtung::LINKS;
      if (   (groesse_r > groesse_v)
          && (groesse_r > groesse_l))
        return Bewegungsrichtung::RECHTS;
      return false;
    } // Taktik::Ergebnis GroessterRaum::ergebnis(Spielraster const& spielraster, int const bot_nummer)
} // namespace TaktikNS
