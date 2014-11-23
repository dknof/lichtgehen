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

#include "groesstes_einflussgebiet.h"

namespace TaktikNS {
  /**
   ** Standardkonstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-23
   **/
  GroesstesEinflussgebiet::GroesstesEinflussgebiet() :
    Taktik{"größtes Einflussgebiet", "Auf das Feld mit dem größten Einflussgebiet gehen."}
  { }

  /**
   ** -> Rückgabe
   ** Gehe auf das Feld, von dem aus der größte Einflussbereich vorhanden ist.
   ** Greift nur, wenn es eine Richtung mit dem größten Einflussbereich gibt
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot           die Nummer des Bots
   **
   ** @return    Richtung für den größten Einflussbereich
   **
   ** @version   2014-11-23
   **/
  Taktik::Ergebnis
    GroesstesEinflussgebiet::ergebnis(Spielraster const& spielraster, int const bot)
    {
      auto const groesse_v
        = spielraster.einflussbereich_groesse(bot, Bewegungsrichtung::VORWAERTS);
      auto const groesse_l
        = spielraster.einflussbereich_groesse(bot, Bewegungsrichtung::LINKS);
      auto const groesse_r
        = spielraster.einflussbereich_groesse(bot, Bewegungsrichtung::RECHTS);

      if (  !spielraster(spielraster.position(bot) + Bewegungsrichtung::VORWAERTS)
          && (groesse_v > groesse_l)
          && (groesse_v > groesse_r))
        return Bewegungsrichtung::VORWAERTS;
      if (  !spielraster(spielraster.position(bot) + Bewegungsrichtung::LINKS)
          && (groesse_l > groesse_v)
          && (groesse_l > groesse_r))
        return Bewegungsrichtung::LINKS;
      if (  !spielraster(spielraster.position(bot) + Bewegungsrichtung::RECHTS)
          && (groesse_r > groesse_v)
          && (groesse_r > groesse_l))
        return Bewegungsrichtung::RECHTS;
      return false;
    } // Taktik::Ergebnis GroesstesEinflussgebiet::ergebnis(Spielraster const& spielraster, int bot)
} // namespace TaktikNS
