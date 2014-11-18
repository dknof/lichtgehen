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

#include "tiefensuche.h"
#include <cstdlib>

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
  Tiefensuche::Tiefensuche() :
    Taktik{"Tiefensuche", "Bestimme alle möglichen möglichen Wege und beschreite den „besten“ (nur für zwei Bots)."}
  { }

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche den längsten Weg.
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot   die Nummer des Bots
   **
   ** @return    Richtung mit dem längsten Weg
   **
   ** @version   2014-11-08
   **/
  Taktik::Ergebnis
    Tiefensuche::ergebnis(Spielraster const& spielraster,
                          int const bot)
    {
      // nur für zwei Bots
      if (spielraster.rauminfo(bot).bot_anz != 1)
        return false;

      // bot 2 suchen
      int bot2;
      for (bot2 = 0; bot2 < spielraster.bot_anz(); ++bot2) {
        if (bot2 == bot)
          continue;
        if (spielraster.kuerzeste_entfernung(spielraster.position(bot),
                                             spielraster.position(bot2))
            >= 0)
          break;
      }
      if (bot2 == spielraster.bot_anz())
        return false;

      auto const bewertung_vv = this->wert(spielraster, bot, bot2,
                                             Bewegungsrichtung::VORWAERTS,
                                             Bewegungsrichtung::VORWAERTS);
      auto const bewertung_vl = this->wert(spielraster, bot, bot2,
                                             Bewegungsrichtung::VORWAERTS,
                                             Bewegungsrichtung::LINKS);
      auto const bewertung_vr = this->wert(spielraster, bot, bot2,
                                             Bewegungsrichtung::VORWAERTS,
                                             Bewegungsrichtung::RECHTS);
      auto const bewertung_lv = this->wert(spielraster, bot, bot2,
                                             Bewegungsrichtung::LINKS,
                                             Bewegungsrichtung::VORWAERTS);
      auto const bewertung_ll = this->wert(spielraster, bot, bot2,
                                             Bewegungsrichtung::LINKS,
                                             Bewegungsrichtung::LINKS);
      auto const bewertung_lr = this->wert(spielraster, bot, bot2,
                                             Bewegungsrichtung::LINKS,
                                             Bewegungsrichtung::RECHTS);
      auto const bewertung_rv = this->wert(spielraster, bot, bot2,
                                             Bewegungsrichtung::RECHTS,
                                             Bewegungsrichtung::VORWAERTS);
      auto const bewertung_rl = this->wert(spielraster, bot, bot2,
                                             Bewegungsrichtung::RECHTS,
                                             Bewegungsrichtung::LINKS);
      auto const bewertung_rr = this->wert(spielraster, bot, bot2,
                                             Bewegungsrichtung::RECHTS,
                                             Bewegungsrichtung::RECHTS);

      // Minimiere den Verlust
      auto const bewertung_v = std::min(std::min(bewertung_vv, bewertung_vl), bewertung_vr);
      auto const bewertung_l = std::min(std::min(bewertung_lv, bewertung_ll), bewertung_lr);
      auto const bewertung_r = std::min(std::min(bewertung_rv, bewertung_rl), bewertung_rr);

      if (   (bewertung_v == -1)
          && (bewertung_l == -1)
          && (bewertung_r == -1) )
        return false;

      cout << bot << ": "
        << "V = " << bewertung_v << ", "
        << "L = " << bewertung_l << ", "
        << "R = " << bewertung_r << "\n";

      if (bewertung_v > bewertung_l) {
        if (bewertung_v > bewertung_r)
          return Bewegungsrichtung::VORWAERTS;
        else if (bewertung_v < bewertung_r)
          return Bewegungsrichtung::RECHTS;
        else // (bewertung_v == bewertung_r)
          return ((rand() <= RAND_MAX / 2)
                  ? Bewegungsrichtung::VORWAERTS
                  : Bewegungsrichtung::RECHTS);
      } else if (bewertung_v < bewertung_l) {
        if (bewertung_l > bewertung_r)
          return Bewegungsrichtung::LINKS;
        else if (bewertung_l < bewertung_r)
          return Bewegungsrichtung::RECHTS;
        else // (bewertung_l == bewertung_r)
          return ((rand() <= RAND_MAX / 2)
                  ? Bewegungsrichtung::LINKS
                  : Bewegungsrichtung::RECHTS);
      } else { // (bewertung_v == bewertung_l)
        if (bewertung_v < bewertung_r)
          return Bewegungsrichtung::RECHTS;
        else if (bewertung_v > bewertung_r)
          return ((rand() <= RAND_MAX / 2)
                  ? Bewegungsrichtung::VORWAERTS
                  : Bewegungsrichtung::LINKS);
        else // (bewertung_v == bewertung_l == bewertung_r)
          return ((rand() <= RAND_MAX / 3)
                  ? Bewegungsrichtung::VORWAERTS
                  : (rand() <= RAND_MAX / 2)
                  ? Bewegungsrichtung::LINKS
                  : Bewegungsrichtung::RECHTS);
      }

      return false;
    } // Taktik::Ergebnis Tiefensuche::ergebnis(Spielraster const& spielraster, int const bot)

  /**
   ** -> Rückgabe
   ** bewertet das Spielfeld rekursiv
   ** Der Wert ist vom ersten Bot zu sehen. 
   ** 1: er gewinnt, -1: er verliert, 0: unentschieden
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot1   die Nummer des ersten Bots
   ** @param     bot2   die Nummer des zweiten Bots
   ** @param     r1     die Bewegungsrichtung des ersten Bots
   ** @param     r2     die Bewegungsrichtung des zweiten Bots
   **
   ** @return    Wert der Bewegungen
   **
   ** @version   2014-11-08
   **/
  double
    Tiefensuche::wert(Spielraster const& spielraster,
                      int const bot1, int const bot2,
                      Bewegungsrichtung const r1,
                      Bewegungsrichtung const r2) const
  {
    BotPosition const bp1 = spielraster.position(bot1);
    BotPosition const bp2 = spielraster.position(bot2);

    if (bp1 + r1 == bp2 + r2)
      return 0; // ineinander gelaufen
    if (spielraster(bp1 + r1)) {
      if (spielraster(bp2 + r2))
        return 0; // beide gegen die Wand gelaufen
      else
        return -1; // alleine gegen eine Wand gelaufen :-(
    } else if (spielraster(bp2 + r2))
      return 1; // Gegner gegen eine Wand gelaufen :-)

    // Beide Bots konnten einen Schritt machen

    // die Schritte durchführen
    Spielraster sr{spielraster};
    sr.bewege_bot(bot1, r1);
    sr.bewege_bot(bot2, r2);

    // rekursiver Aufruf (aufwendig)
    auto const bewertung_vv = this->wert(sr, bot1, bot2,
                                           Bewegungsrichtung::VORWAERTS,
                                           Bewegungsrichtung::VORWAERTS);
    auto const bewertung_vl = this->wert(sr, bot1, bot2,
                                           Bewegungsrichtung::VORWAERTS,
                                           Bewegungsrichtung::LINKS);
    auto const bewertung_vr = this->wert(sr, bot1, bot2,
                                           Bewegungsrichtung::VORWAERTS,
                                           Bewegungsrichtung::RECHTS);
    auto const bewertung_lv = this->wert(sr, bot1, bot2,
                                           Bewegungsrichtung::LINKS,
                                           Bewegungsrichtung::VORWAERTS);
    auto const bewertung_ll = this->wert(sr, bot1, bot2,
                                           Bewegungsrichtung::LINKS,
                                           Bewegungsrichtung::LINKS);
    auto const bewertung_lr = this->wert(sr, bot1, bot2,
                                           Bewegungsrichtung::LINKS,
                                           Bewegungsrichtung::RECHTS);
    auto const bewertung_rv = this->wert(sr, bot1, bot2,
                                           Bewegungsrichtung::RECHTS,
                                           Bewegungsrichtung::VORWAERTS);
    auto const bewertung_rl = this->wert(sr, bot1, bot2,
                                           Bewegungsrichtung::RECHTS,
                                           Bewegungsrichtung::LINKS);
    auto const bewertung_rr = this->wert(sr, bot1, bot2,
                                           Bewegungsrichtung::RECHTS,
                                           Bewegungsrichtung::RECHTS);

#if 1
    { // Variante A: Mein Gegner verhält sich möglichst schlecht für mich
      // (pessimistisch)
      // Minimiere den Verlust
      auto const bewertung_v = std::min(bewertung_vv, bewertung_vl, bewertung_vr);
      auto const bewertung_l = std::min(bewertung_lv, bewertung_ll, bewertung_lr);
      auto const bewertung_r = std::min(bewertung_rv, bewertung_rl, bewertung_rr);

      return std::max(bewertung_v, bewertung_l, bewertung_r);
    } // Variante A (Pessimist)
#endif


#if 1
    { // Variante B: Mein Gegner verhält sich möglichst gut für sich (er verfolgt Variante 1)
      // (optimistisch)
      // Minimiere seinen Verlust
      auto const bewertung_v = std::max(bewertung_vv, bewertung_lv, bewertung_rv);
      auto const bewertung_l = std::max(bewertung_vl, bewertung_ll, bewertung_rl);
      auto const bewertung_r = std::max(bewertung_vr, bewertung_lr, bewertung_rr);
      auto const m = std::min(bewertung_v, bewertung_l, bewertung_r);
      auto res = -1;
      if (m == bewertung_v)
        res = std::max(res, bewertung_vv, bewertung_lv, bewertung_rv);
      if (m == bewertung_l)
        res = std::max(res, bewertung_vl, bewertung_ll, bewertung_rl);
      if (m == bewertung_r)
        res = std::max(res, bewertung_vr, bewertung_lr, bewertung_rr);
      return res;
    } // Variante B (Optimist)
#endif

#if 1
    { // Variante C
      // 1) Gewinnerstrategie gleich verwenden
      if (   ( (bewertung_vv == 1) && (bewertung_vl == 1) && (bewertung_vr == 1) )
          || ( (bewertung_lv == 1) && (bewertung_ll == 1) && (bewertung_lr == 1) )
          || ( (bewertung_rv == 1) && (bewertung_rl == 1) && (bewertung_rr == 1) ) )
        return 1;
      if (   ( (bewertung_vv == -1) && (bewertung_lv == -1) && (bewertung_rv == -1) )
          || ( (bewertung_vl == -1) && (bewertung_ll == -1) && (bewertung_rl == -1) )
          || ( (bewertung_vr == -1) && (bewertung_lr == -1) && (bewertung_rr == -1) ) )
        return -1;
      // 2) die ungünstigen Richtungen für beide Spieler entfernen
      // 3) die verbleibenen Bewertungen irgendwie mitteln
    } // Variante C
#endif

    return 0;
  } // Taktik::Ergebnis Tiefensuche::ergebnis(Spielraster const& spielraster, int const bot) const

} // namespace TaktikNS
