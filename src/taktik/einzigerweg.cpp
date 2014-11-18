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

#include "einzigerweg.h"

namespace TaktikNS {
  /**
   ** Standardkonstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-10-25
   **/
  EinzigerWeg::EinzigerWeg() :
    Taktik{"Einziger Weg", "Nur ein Weg → diesen nehmen"}
  { }

  /**
   ** -> Rückgabe
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot_nummer   die Nummer des Bots
   **
   ** @return    false, wenn noch mehr als ein Weg frei ist, ansonsten den einzigen noch offenen Weg
   **
   ** @version   2014-10-25
   **/
  Taktik::Ergebnis
    EinzigerWeg::ergebnis(Spielraster const& spielraster, int const bot_nummer)
    {
      BotPosition const bp{spielraster.position(bot_nummer)};
      if (   spielraster(bp + Bewegungsrichtung::VORWAERTS)
          && spielraster(bp + Bewegungsrichtung::LINKS)
          && !spielraster(bp + Bewegungsrichtung::RECHTS) )
        return Bewegungsrichtung::RECHTS;
      if (   spielraster(bp + Bewegungsrichtung::VORWAERTS)
          && !spielraster(bp + Bewegungsrichtung::LINKS)
          && spielraster(bp + Bewegungsrichtung::RECHTS) )
        return Bewegungsrichtung::LINKS;
      if (   !spielraster(bp + Bewegungsrichtung::VORWAERTS)
          && spielraster(bp + Bewegungsrichtung::LINKS)
          && spielraster(bp + Bewegungsrichtung::RECHTS) )
        return Bewegungsrichtung::VORWAERTS;
      return false;
    } // Taktik::Ergebnis EinzigerWeg::ergebnis(Spielraster const& spielraster, int const bot_nummer)
} // namespace TaktikNS
